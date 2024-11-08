#include "wxModularCore.h"
#include <cassert>
#include <memory>                        // unique_ptr
#include <mutex>                         // call_once
#include <wx/listimpl.cpp>
#include <wx/init.h>                     // wxUninitialize
#include <wx/stdpaths.h>                 // wxStandardPaths
#include "wxModularCoreSettings.h"
#include "wxGuiPluginBase.h"             // wxEVT_GUI_PLUGIN_INTEROP
#include "host_interaction.hpp"
#include "Auto.h"

static void const *ForPlugins_GetHostAPI( unsigned version, void (*addr_of_wxuninit)(void) ); // defined lower down in this file
static wxGuiPluginBase *ForHost_Process_ActiveX_Plugin(wxDynamicLibrary *const dll, wxString const &fileName);  // defined lower down in this file
static wxGuiPluginBase* ForHost_Process_NativeHandle_Plugin   (wxDynamicLibrary *const dll, wxString const &fileName);  // defined lower down in this file
static wxGuiPluginBase* ForHost_Process_DotNet_Plugin (wxDynamicLibrary* const dll, wxString const &fileName);  // defined lower down in this file
static wxGuiPluginBase* ForHost_Process_ScreenCoord_Plugin (wxDynamicLibrary* const dll, wxString const &fileName);  // defined lower down in this file
static wxGuiPluginBase *ForHost_Process_Pixels_Plugin (wxDynamicLibrary* const dll, wxString const &fileName);  // defined lower down in this file

wxModularCore::wxModularCore()
	: m_Settings(new wxModularCoreSettings), m_Handler(new wxEvtHandler)
{

}

wxModularCore::~wxModularCore()
{
	this->Clear();
	wxDELETE(this->m_Handler);
	wxDELETE(this->m_Settings);
}

bool wxModularCore::UnloadPlugin(wxString const &filename)
{
    auto const it = this->mymap.find(filename);
    if ( this->mymap.end() == it ) return false;
    Auto( this->mymap.erase(it) );

    wxDynamicLibrary  &dll    = it->second.first ;
    wxGuiPluginBase  *&plugin = it->second.second;

    if ( false == dll.IsLoaded() )
    {
        assert( nullptr == plugin );
        return false;
    }

    if ( nullptr == plugin ) return false;

    auto const pfnDeletePlugin = (DeletePlugin_function) dll.RawGetSymbol(wxT("DeletePlugin"));

    if ( pfnDeletePlugin ) pfnDeletePlugin(plugin);
    else wxDELETE(plugin);

    return true;
}

bool wxModularCore::UnloadAllPlugins(void)
{
    bool result = true;
    while ( false == this->mymap.empty() )
    {
        wxString const &filename = this->mymap.cbegin()->first;
        assert( false == filename.IsEmpty() );
        result &= UnloadPlugin( filename );
    }
    return result;
}

wxGuiPluginBase *wxModularCore::LoadPlugin(wxString const &fileName)
{
    auto const it = this->mymap.find(fileName);
    if ( this->mymap.end() == it ) return nullptr;

    wxDynamicLibrary  &dll    = it->second.first ;
    wxGuiPluginBase  *&plugin = it->second.second;

    if ( false == dll.IsLoaded() )
    {
        assert( nullptr == plugin );
        dll.Attach( wxDynamicLibrary::RawLoad(fileName) );
    }

    if ( false == dll.IsLoaded() ) return nullptr;

    if ( nullptr != plugin ) return plugin;

    auto const pfnCreatePlugin = (CreatePlugin_function) dll.RawGetSymbol( wxT("CreatePlugin") );

    if ( pfnCreatePlugin )
    {
        plugin = pfnCreatePlugin(&ForPlugins_GetHostAPI);
    }
    else if ( plugin = ForHost_Process_ActiveX_Plugin(&dll, fileName) )
    {
        /* nothing to do in here */
    }
    else if ( plugin = ForHost_Process_DotNet_Plugin(&dll, fileName) )
    {
        /* nothing to do in here */
    }
    else if ( plugin = ForHost_Process_NativeHandle_Plugin(&dll, fileName) )
    {
        /* nothing to do in here */
    }
    else if ( plugin = ForHost_Process_Pixels_Plugin(&dll, fileName) )
    {
        /* nothing to do in here */
    }
    else if ( plugin = ForHost_Process_ScreenCoord_Plugin(&dll, fileName) )
    {
        /* nothing to do in here */
    }

    return plugin;
}

bool wxModularCore::DiscoverPlugins(wxString const &pluginsDirectory)
{
    using std::regex_match;
    wxFileName fn;
    fn.AssignDir(pluginsDirectory);
    wxLogDebug(wxT("%s"), fn.GetFullPath().data());
    if ( false == fn.DirExists() ) return false;
    if ( false == wxDirExists(fn.GetFullPath()) ) return false;
    wxArrayString pluginPaths;
    wxDir::GetAllFiles(fn.GetFullPath(), &pluginPaths, wxEmptyString, wxDIR_FILES /*|wxDIR_DIRS*/ |wxDIR_HIDDEN);

    // ==== The 'pluginPaths' now contains all of the
    // ==== filenames in the directory, so we iterate
    // ==== through them all to remove the ones
    // ==== that don't match the regex for a plugin
    for ( unsigned i = 0u; i < pluginPaths.GetCount(); ++i )
    {
        // Hopefully converting the wxString to an std::string
        // will leave us with a string that we can do regex on
        wxString const &filename = pluginPaths[i];
        if ( !regex_match( filename.ToStdString(), this->GetPluginRegex() ) ) continue;
        this->mymap[filename];
    }

    return false == this->mymap.empty();
}

void wxModularCore::Clear()
{
	UnloadAllPlugins();
	// TODO: Add the code which resets the object to initial state
}

wxString wxModularCore::GetPluginsPath(bool forceProgramPath) const
{
#if defined(__WXMAC__)
	return wxStandardPaths::Get().GetPluginsDir();
#else
	wxString path;
	if (m_Settings->GetStoreInAppData() && !forceProgramPath)
		path = wxStandardPaths::Get().GetConfigDir();
	else
		path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
	wxFileName fn;
	fn.AssignDir(path);
	return fn.GetFullPath();
#endif
}

std::regex wxModularCore::GetPluginRegex() const
{
#if defined(__WXMSW__)
	return std::regex("(.*\\\\)?(((?!wx)[^\\\\]*\\.dll)|([^\\\\]*\\.ocx))", std::regex::icase);
#elif defined(__WXGTK__)
	return std::regex("(.*/)?((?!libwx)[^/]+)\\.so(\\.[0-9]+)*");
#elif defined(__WXMAC__)
	return std::regex("(.*/)?((?!libwx)[^/]+)\\.dylib(\\.[0-9]+)*");
#else
	return std::regex("no_known_plugin");
#endif
}

bool wxModularCore::DiscoverAllPlugins(bool const forceProgramPath)
{
	wxString pluginsRootDir = GetPluginsPath(forceProgramPath);
	bool result = true;
	result &= DiscoverPlugins(pluginsRootDir);
	return result;
}

std::vector<wxString> wxModularCore::GetGuiPluginFilenames(void) const
{
	std::vector<wxString> retval;
	for ( auto const &e : this->mymap ) retval.emplace_back(e.first);
	return retval;
}

void TabWindowForPlugin::ShowPluginWidgets(void)
{
    auto const Arrange  =
      [this](int const flags)
      {
        assert( nullptr != this->child  );
        bool const spacers = (nullptr == this->plugin) || (this->plugin && this->plugin->ShouldInsertSpacers());
        auto *const bsizer = new wxBoxSizer(wxVERTICAL);
        if ( spacers ) bsizer->AddStretchSpacer(-1);
        bsizer->Add(this->child, 1, flags);
        if ( spacers ) bsizer->AddStretchSpacer(-1);
        this->SetSizer(bsizer);
        bsizer->Fit(this->child);
        this->Layout();
      };

    Auto(
        if ( nullptr == this->child )
        {
            this->child = new wxStaticText(this, wxID_STATIC, _("Failed to load plugin."));
            Arrange(wxALIGN_CENTRE_HORIZONTAL);
            return;
        }
        auto *const phandler = this->child->GetEventHandler();
        if ( nullptr == phandler ) return;
        wxShowEvent e( this->child->GetId(), true );
        e.SetEventObject( this->child );
        phandler->AddPendingEvent(e);
    );
    if ( nullptr != this->child ) return;
    if ( nullptr == this->plugin )
    {
        assert( nullptr != this->pluginManager );
        this->plugin = this->pluginManager->LoadPlugin(this->filename);
        if ( nullptr == plugin ) return;
        auto *const phandler = this->pluginManager->GetEventHandler();
        assert( nullptr != phandler );
        plugin->SetEventHandler(phandler);
    }
    this->child = this->plugin->CreatePanel(this);
    if ( nullptr == this->child ) return;
    Arrange(wxEXPAND);
}

static wxGuiPluginBase *ForHost_Process_NativeHandle_Plugin(wxDynamicLibrary *const dll, wxString const &fileName)
{
	auto const pfnPopulatePanelNativeHandle =
		(bool(*)(void*)) dll->RawGetSymbol("PopulatePanelNativeHandle");

	if ( nullptr == pfnPopulatePanelNativeHandle ) return nullptr;

	auto *const plugin = new(std::nothrow) wxGuiPluginNativeHandle(pfnPopulatePanelNativeHandle);
	if ( nullptr != plugin ) return plugin;

	return nullptr;
}

static wxGuiPluginBase *ForHost_Process_Pixels_Plugin(wxDynamicLibrary *const dll, wxString const &fileName)
{
	auto const pfnCreate =
		(bool(*)(void)) dll->RawGetSymbol("CreatePluginPixels");

	auto const pfnRender =
		(void(*)(PixelContainer&,int,int)) dll->RawGetSymbol("RenderWidgets");

	if ( (nullptr == pfnCreate) || (nullptr == pfnRender) ) return nullptr;

	auto *const plugin = new(std::nothrow) wxGuiPluginPixels(pfnCreate,pfnRender);
	if ( nullptr != plugin ) return plugin;

	return nullptr;
}

static wxGuiPluginBase *ForHost_Process_ScreenCoord_Plugin(wxDynamicLibrary *const dll, wxString const &fileName)
{
	auto const pfnPopulate =
		(bool(*)(int,int,int,int)) dll->RawGetSymbol("PopulateScreenCoord");

	if ( nullptr == pfnPopulate) return nullptr;

	auto *const plugin = new(std::nothrow) wxGuiPluginScreenCoord(pfnPopulate);
	if ( nullptr != plugin ) return plugin;

	return nullptr;
}

// Global objects
// Standalone functions
// Class static member functions
extern bool ForPlugins_wxGuiPluginWindowBase_ShowToolTips();
// Class constructors
extern void ForPlugins_wxGuiPluginWindowBase_Constructor_void(wxGuiPluginWindowBase & obj);
extern void ForPlugins_wxGuiPluginWindowBase_Constructor_ManyArguments(wxGuiPluginWindowBase & obj, wxGuiPluginBase * plugin, wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style);
// Class member functions
extern void ForPlugins_wxGuiPluginWindowBase_Init(wxGuiPluginWindowBase & obj);
extern void ForPlugins_wxGuiPluginWindowBase_CreateControls(wxGuiPluginWindowBase & obj);
extern bool ForPlugins_wxGuiPluginWindowBase_Create(wxGuiPluginWindowBase & obj, wxGuiPluginBase * plugin, wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style);
extern wxBitmap ForPlugins_wxGuiPluginWindowBase_GetBitmapResource(wxGuiPluginWindowBase & obj, const wxString& /*name*/);
extern wxIcon ForPlugins_wxGuiPluginWindowBase_GetIconResource(wxGuiPluginWindowBase & obj, const wxString& /*name*/);

static void const *ForPlugins_GetHostAPI( unsigned const version, void (*const addr_of_wxuninit)(void) )
{
	// The next line ensures that the host program and
	// the plugin are both using the same wxWidgets
	// library. This is necessary, as neither the host
	// nor the plugin can link statically with wxWidgets,
	// and both the host and the plugin must link dynamically
	// with the exact same wxWidgets library files
	// -- specifically there can only be one instance of the
	// wxWidgets global variables inside the address space
	// of the current process.
	if ( &wxUninitialize != addr_of_wxuninit ) return nullptr;

	static constexpr HostAPIv1 api1 = {
		wxEVT_GUI_PLUGIN_INTEROP,
		ForPlugins_wxGuiPluginWindowBase_ShowToolTips,
		ForPlugins_wxGuiPluginWindowBase_Constructor_void,
		ForPlugins_wxGuiPluginWindowBase_Constructor_ManyArguments,
		ForPlugins_wxGuiPluginWindowBase_Create,
		ForPlugins_wxGuiPluginWindowBase_Init,
		ForPlugins_wxGuiPluginWindowBase_CreateControls,
		ForPlugins_wxGuiPluginWindowBase_GetBitmapResource,
		ForPlugins_wxGuiPluginWindowBase_GetIconResource,
	};

	/*
	static constexpr HostAPIv2 api2 = {
		. . .
		. . .   // 2nd version of API goes here
		. . .
	};
	*/

	switch ( version )
	{
	case 1u:   return &api1;
	case 2u: //return &api2;
	default:
		;  // fall through
	}

	return nullptr;
}

#ifdef __WXMSW__

#include <guiddef.h>                     // CLSID, CLSID_NULL, REFCLSID, REFIID
#include <metahost.h>                    // ICLRMetaHost, ICLRRuntimeInfo, ICLRRuntimeHost
#include <oaidl.h>                       // ITypeLib
#include <objbase.h>                     // CoInitializeEx
#include <oleauto.h>                     // LoadTypeLib
#include <oleidl.h>                      // IOleObject
#include <unknwn.h>                      // IClassFactory

void OCX_Release_IOleObject(void *const p_IOleObject)
{
    assert( nullptr != p_IOleObject );
    static_cast<IOleObject*>(p_IOleObject)->Release();
}

extern "C" std::int32_t DynamicallyLoaded_AtlAxAttachControl(void *const arg0, void *const arg1)
{
	static wxDynamicLibrary dll( wxT("atl.dll") );  // must keep loaded -- don't unload it
	if ( false == dll.IsLoaded() ) return 666;
	auto const pfnAttach = (std::int32_t(__stdcall*)(void*, void*, void*)) dll.RawGetSymbol("AtlAxAttachControl");
	if ( nullptr == pfnAttach ) return 666;
	return pfnAttach(arg0, arg1, nullptr);
}

static CLSID Get_First_ActiveX_Control(TCHAR const *const lib)
{
	ITypeLib *pTypeLib = nullptr;

	// Load the type library from the OCX file
	HRESULT hr = LoadTypeLib(lib, &pTypeLib);
	if ( FAILED(hr) || (nullptr == pTypeLib) ) return CLSID_NULL;
	Auto( pTypeLib->Release() );

	UINT const typeInfoCount = pTypeLib->GetTypeInfoCount();

	// Iterate through each TypeInfo in the TypeLib
	for ( UINT i = 0u; i < typeInfoCount; ++i )
	{
		ITypeInfo *pTypeInfo = nullptr;
		hr = pTypeLib->GetTypeInfo(i, &pTypeInfo);
		if ( FAILED(hr) || (nullptr == pTypeInfo) ) continue;
		Auto( pTypeInfo->Release() );

		TYPEATTR *pTypeAttr = nullptr;
		hr = pTypeInfo->GetTypeAttr(&pTypeAttr);
		if ( FAILED(hr) || (nullptr == pTypeAttr) ) continue;
		Auto( pTypeInfo->ReleaseTypeAttr(pTypeAttr) );

		if ( TKIND_COCLASS != pTypeAttr->typekind ) continue;

		// Check implemented interfaces
		auto const count = pTypeAttr->cImplTypes;
		for ( UINT j = 0u; j < count; ++j )
		{
			HREFTYPE hrefType;
			hr = pTypeInfo->GetRefTypeOfImplType(j, &hrefType);
			if ( FAILED(hr) ) continue;

			ITypeInfo *pRefTypeInfo = nullptr;
			hr = pTypeInfo->GetRefTypeInfo(hrefType, &pRefTypeInfo);
			if ( FAILED(hr) || (nullptr == pRefTypeInfo) ) continue;
			Auto( pRefTypeInfo->Release() );

			TYPEATTR *pRefTypeAttr = nullptr;
			hr = pRefTypeInfo->GetTypeAttr(&pRefTypeAttr);
			if ( FAILED(hr) || (nullptr == pRefTypeAttr) ) continue;
			Auto( pRefTypeInfo->ReleaseTypeAttr(pRefTypeAttr) );

			// Check if the interface is dispatchable
			// and if so, return its CLSID
			if ( pRefTypeAttr->wTypeFlags & TYPEFLAG_FDISPATCHABLE ) return pTypeAttr->guid;
		}
	}

	return CLSID_NULL;
}

static wxGuiPluginBase *ForHost_Process_ActiveX_Plugin(wxDynamicLibrary *const dll, wxString const &fileName)
{
	// If control reaches here, we couldn't find 'CreatePlugin'
	// inside the dynamically-loaded library. Now let's check
	// instead for 'DllGetClassObject' to find an ActiveX control

	auto const pfnDllGetClassObject =
		(HRESULT(__stdcall*)(REFCLSID, REFIID, void**)) dll->RawGetSymbol("DllGetClassObject");

	if ( nullptr == pfnDllGetClassObject ) return nullptr;

	// If control reaches here, we're dealing with a library
	// that exports 'DllGetClassObject' -- possibly an OCX
	// file containing ActiveX controls

	HRESULT retval = 0;

#if 1
	CLSID clsid = CLSID_NULL;
	{
		TCHAR buffer[MAX_PATH];
		buffer[0] = '\0';
		retval = ::GetModuleFileName(dll->GetLibHandle(), buffer, MAX_PATH);
		if ( (ERROR_SUCCESS != ::GetLastError()) || (0 == retval) || ('\0' == buffer[0]) ) return nullptr;
		clsid = Get_First_ActiveX_Control(buffer);
	}
	if ( CLSID_NULL == clsid ) return nullptr;
#else
    // You can hardcode your own CLSID here for testing:
    constexpr CLSID clsid = { 0x6BF52A52, 0x394A, 0x11d3, {0xB1, 0x53, 0x00, 0xC0, 0x4F, 0x79, 0xFA, 0xA6 } };  // Windows Media Player
  //constexpr CLSID clsid = { 0x48E96FFE, 0x1D86, 0x4BE5, {0x9E, 0xE0, 0xF2, 0x4D, 0x44, 0x93, 0x2E, 0x34 } };  // MFCActiveXControl1
#endif

	retval = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	if ( (S_OK != retval) && (S_FALSE != retval) ) return nullptr;

	IClassFactory* pfactory = nullptr;
	retval = pfnDllGetClassObject(clsid, IID_IClassFactory, (void**)&pfactory);
	if ( S_OK != retval ) return nullptr;
	if ( nullptr == pfactory ) return nullptr;

	IOleObject* pole = nullptr;
	retval = pfactory->CreateInstance(nullptr, IID_IOleObject, (void**)&pole);
	pfactory->Release();
	if ( (S_OK == retval) && (nullptr != pole) )
	{
		auto* const plugin = new(std::nothrow) wxGuiPluginOCX(pole);
		if ( nullptr != plugin ) return plugin;
	}
	if ( nullptr != pole ) pole->Release();
	return nullptr;
}

template<typename T>
class unique_releaser : public std::unique_ptr< T, void(*)(T*) noexcept > {
protected:
	static void unique_releaser_deleter(T *const arg) noexcept { try { arg->Release(); }catch(...){} }
public:
	unique_releaser(T *const arg = nullptr) noexcept
	  : std::unique_ptr< T, void(*)(T*) noexcept >(arg, &unique_releaser_deleter) {}
};

static ICLRRuntimeHost *GetDotNet(void) noexcept  // thread-safe
{
	static unique_releaser<ICLRRuntimeHost> runtimeHost;
	static unique_releaser<ICLRRuntimeInfo> runtimeInfo;
	static unique_releaser<ICLRMetaHost   > metaHost   ;
	static wxDynamicLibrary dllcoree;
	static std::once_flag myflag;

	std::call_once(myflag,[&]
	{
		bool success = false;
		[&]()
		{
			assert( nullptr == runtimeHost         );
			assert( nullptr == runtimeInfo         );
			assert( nullptr == metaHost            );
			assert( false   == dllcoree.IsLoaded() );
			typedef HRESULT (__stdcall *FuncPtr_t)(REFCLSID, REFIID, LPVOID*);
			dllcoree.Attach( wxDynamicLibrary::RawLoad("mscoree.dll") );
			if ( false == dllcoree.IsLoaded() ) return;
			auto const pfnCLRCreateInstance = (FuncPtr_t)dllcoree.RawGetSymbol("CLRCreateInstance");
			if ( nullptr == pfnCLRCreateInstance ) return;

			ICLRMetaHost    *VmetaHost    = nullptr;
			ICLRRuntimeInfo *VruntimeInfo = nullptr;
			ICLRRuntimeHost *VruntimeHost = nullptr;
			pfnCLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&VmetaHost);
			if ( nullptr == VmetaHost ) return;
			metaHost.reset(VmetaHost);
			metaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&VruntimeInfo);
			if ( nullptr == VruntimeInfo ) return;
			runtimeInfo.reset(VruntimeInfo);
			runtimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&VruntimeHost);
			if ( nullptr == VruntimeHost ) return;
			runtimeHost.reset(VruntimeHost);
			if ( S_OK != runtimeHost->Start() ) return;
			success = true;
		}();
		if ( false == success )
		{
			runtimeHost.reset();
			runtimeInfo.reset();
			metaHost   .reset();
			try { dllcoree.Unload(); } catch(...){}
		}
	});

	return runtimeHost.get();
}


static wxGuiPluginBase *ForHost_Process_DotNet_Plugin(wxDynamicLibrary *const dll, wxString const &fileName)
{
	assert( wxIsMainThread() );

	ICLRRuntimeHost *const runtimeHost = GetDotNet();
	if ( nullptr == runtimeHost ) return nullptr;

	std::wstring const name = wxFileName(fileName).GetName().ToStdWstring();
	if ( name.empty() ) return nullptr;

	DWORD retval_from_DotNet_method = 0;

	HRESULT const res = runtimeHost->ExecuteInDefaultAppDomain(
		(name + L".dll"   ).c_str(),
		(name + L".Plugin").c_str(),
		L"QueryPlugin",
		L"",
		&retval_from_DotNet_method);

	if ( (S_OK != res) || (666 != retval_from_DotNet_method) ) return nullptr;

	auto *const p =  new wxGuiPluginDotNet(name,runtimeHost);
	if ( nullptr == p ) return nullptr;

	return p;
}

#else

// For all platforms other than MS-Windows
static wxGuiPluginBase *ForHost_Process_ActiveX_Plugin(wxDynamicLibrary *dll, wxString const &fileName) { return nullptr; }
static wxGuiPluginBase *ForHost_Process_DotNet_Plugin (wxDynamicLibrary *dll, wxString const &fileName) { return nullptr; }

#endif // ifdef __WXMSW__
