#include "stdwx.h"
#include "wxModularCore.h"
#include "wxModularCoreSettings.h"
#include <wx/listimpl.cpp>
#include <wx/init.h>                     // wxUninitialize
#include "wxGuiPluginBase.h"             // wxEVT_GUI_PLUGIN_INTEROP
#include "host_interaction.hpp"
#include "Auto.h"

WX_DEFINE_LIST(wxDynamicLibraryList);

wxModularCore::wxModularCore()
	: m_Settings(new wxModularCoreSettings), m_Handler(new wxEvtHandler)
{
	// This will allow to delete all objects from this list automatically
	m_DllList.DeleteContents(true);
}

wxModularCore::~wxModularCore()
{
	wxDELETE(m_Handler);
	wxDELETE(m_Settings);
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

extern "C" {
DEMO_API void const *ForPlugins_GetHostAPI( unsigned const version, void (*const addr_of_wxuninit)(void) )
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
}  // extern "C"

#ifdef __WXMSW__

#include <guiddef.h>                     // CLSID, CLSID_NULL, REFCLSID, REFIID
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

DEMO_API wxGuiPluginBase *ForHost_Process_ActiveX_Plugin(wxDynamicLibrary *const dll)
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

#else

// For all platforms other than MS-Windows
DEMO_API wxGuiPluginBase *ForHost_Process_ActiveX_Plugin(wxDynamicLibrary *dll) { return nullptr; }

#endif // ifdef __WXMSW__
