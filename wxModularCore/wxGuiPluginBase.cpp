#include "stdwx.h"
#include "wxGuiPluginBase.h"

DEFINE_EVENT_TYPE(wxEVT_GUI_PLUGIN_INTEROP)

#ifdef __WXMSW__

#include <cassert>       // assert
#include <cstdint>       // int32_t
#include <new>           // new(nothrow)
#include <guiddef.h>     // CLSID, CLSID_NULL, REFCLSID, REFIID
#include <oaidl.h>       // ITypeLib
#include <objbase.h>     // CoInitializeEx
#include <oleauto.h>     // LoadTypeLib
#include <oleidl.h>      // IOleObject
#include <unknwn.h>      // IClassFactory
#include <wx/dynlib.h>   // wxDynamicLibrary
#include <wx/panel.h>    // wxPanel
#include <wx/string.h>   // wxString
#include <wx/window.h>   // wxWindow
#include "Auto.h"        // Auto

static std::int32_t DynamicallyLoaded_AtlAxAttachControl(void *const arg0, void *const arg1)
{
	static wxDynamicLibrary dll( wxT("atl.dll") );  // must keep loaded -- don't unload it
	if ( false == dll.IsLoaded() ) return 666;
	auto const pfnAttach = (std::int32_t(__stdcall*)(void*, void*, void*)) dll.RawGetSymbol("AtlAxAttachControl");
	if ( nullptr == pfnAttach ) return 666;
	return pfnAttach(arg0, arg1, nullptr);
}

static CLSID Get_First_ActiveX_Control(TCHAR const *lib)
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

static void OCX_Release_IOleObject(void *const p_IOleObject)
{
    assert( nullptr != p_IOleObject );
    static_cast<IOleObject*>(p_IOleObject)->Release();
}

wxGuiPluginOCX::wxGuiPluginOCX(void *const arg_p_IOleObject) : p_IOleObject(arg_p_IOleObject)
{
    assert( nullptr != arg_p_IOleObject );
}

wxGuiPluginOCX::~wxGuiPluginOCX(void)
{
    assert( nullptr != this->p_IOleObject );
    OCX_Release_IOleObject(this->p_IOleObject);
}

wxWindow *wxGuiPluginOCX::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent             );
    assert( nullptr != this->p_IOleObject );
    wxPanel *const mypanel = new(std::nothrow) wxPanel(parent, wxID_ANY);
    if ( nullptr == mypanel ) return nullptr;
    auto const retval = DynamicallyLoaded_AtlAxAttachControl( this->p_IOleObject, mypanel->GetHandle() );
    if ( S_OK == retval ) return mypanel;
    delete mypanel;
    return nullptr;
}

wxGuiPluginBase *Process_ActiveX_Plugin(wxDynamicLibrary *const dll)
{
    // If control reaches here, we couldn't find 'CreatePlugin'
    // inside the dynamically-loaded library. Now let's check
    // instead for 'DllGetClassObject' to find an ActiveX control

    auto const pfnDllGetClassObject =
        (HRESULT(__stdcall*)(REFCLSID,REFIID,void**)) dll->RawGetSymbol("DllGetClassObject");

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
        retval = ::GetModuleFileName( dll->GetLibHandle(), buffer, MAX_PATH );
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

    IClassFactory *pfactory = nullptr;
    retval = pfnDllGetClassObject(clsid, IID_IClassFactory, (void**)&pfactory);
    if ( S_OK    != retval   ) return nullptr;
    if ( nullptr == pfactory ) return nullptr;

    IOleObject *pole = nullptr;
    retval = pfactory->CreateInstance(nullptr, IID_IOleObject, (void**)&pole);
    pfactory->Release();
    if ( (S_OK == retval) && (nullptr != pole) )
    {
        auto *const plugin = new(std::nothrow) wxGuiPluginOCX(pole);
        if ( nullptr != plugin ) return plugin;
    }
    if ( nullptr != pole ) pole->Release();
    return nullptr;
}

#endif  // ifdef __WXMSW__
