#include "wxGuiPluginBase.h"

DEFINE_EVENT_TYPE(wxEVT_GUI_PLUGIN_INTEROP)

wxGuiPluginScreenCoord::wxGuiPluginScreenCoord(FuncPtr_t const arg) : pfnPopulate(arg)
{
    assert( nullptr != arg );
}

wxGuiPluginScreenCoord::~wxGuiPluginScreenCoord(void)
{
}

wxWindow *wxGuiPluginScreenCoord::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent );
    wxPanel *const mypanel = new(std::nothrow) wxPanel(parent, wxID_ANY);
    if ( nullptr == mypanel ) return nullptr;
    bool retval = false;
    try
    {
        auto const rect = parent->GetScreenRect();
        retval = pfnPopulate( rect.x, rect.y, rect.width, rect.height );
    }
    catch(...){ retval = false; }
    if ( retval ) return mypanel;
    delete mypanel;
    return nullptr;
}

#ifdef __WXMSW__

#include <cassert>       // assert
#include <cstdint>       // int32_t, uintptr_t
#include <string>        // to_wstring
#include <new>           // new(nothrow)
#include <metahost.h>    // ICLRMetaHost, ICLRRuntimeInfo, ICLRRuntimeHost
#include <wx/dynlib.h>   // wxDynamicLibrary
#include <wx/panel.h>    // wxPanel
#include <wx/string.h>   // wxString
#include <wx/window.h>   // wxWindow
#include "Auto.h"        // Auto

extern void OCX_Release_IOleObject(void *const p_IOleObject);

// I don't know why the Microsoft VC++ compiler requires
// the following function to have C linkage
extern "C" std::int32_t DynamicallyLoaded_AtlAxAttachControl(void *arg0, void *arg1);

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
    std::int32_t retval = 666;
    try { retval = DynamicallyLoaded_AtlAxAttachControl( this->p_IOleObject, mypanel->GetHandle() ); }
    catch(...){ retval = 666; }
    if ( S_OK == retval ) return mypanel;
    delete mypanel;
    return nullptr;
}

wxGuiPluginHWND::wxGuiPluginHWND(FuncPtr_t const arg) : pfnPopulate(arg)
{
    assert( nullptr != arg );
}

wxGuiPluginHWND::~wxGuiPluginHWND(void)
{
}

wxWindow *wxGuiPluginHWND::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent );
    wxPanel *const mypanel = new(std::nothrow) wxPanel(parent, wxID_ANY);
    if ( nullptr == mypanel ) return nullptr;
    bool retval = false;
    try { retval = pfnPopulate( mypanel->GetHandle() ); }
    catch(...){ retval = false; }
    if ( retval ) return mypanel;
    delete mypanel;
    return nullptr;
}

wxGuiPluginDotNet::wxGuiPluginDotNet(std::wstring_view const arg_name, ICLRRuntimeHost *const arg_runtimeHost)
  : name(arg_name), runtimeHost(arg_runtimeHost)
{
    assert( false   == name.empty());
    assert( nullptr != runtimeHost );
}

wxGuiPluginDotNet::~wxGuiPluginDotNet(void)
{
    if ( runtimeHost ) runtimeHost->Release();
}

wxWindow *wxGuiPluginDotNet::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent );
    wxPanel *const mypanel = new(std::nothrow) wxPanel(parent, wxID_ANY);
    if ( nullptr == mypanel ) return nullptr;
    bool success = false;
    try
    {
        DWORD retval_from_DotNet_method = 0u;

        HRESULT const res = runtimeHost->ExecuteInDefaultAppDomain(
            (name + L".dll"   ).c_str(),
            (name + L".Plugin").c_str(),
            L"PopulatePanelHWND",
            std::to_wstring( (std::uintptr_t)mypanel->GetHandle() ).c_str(),
            &retval_from_DotNet_method);

        if ( (S_OK == res) && (667 == retval_from_DotNet_method) ) success = true;
    }
    catch(...){ success = false; }
    if ( success ) return mypanel;
    delete mypanel;
    return nullptr;
}

#endif  // ifdef __WXMSW__
