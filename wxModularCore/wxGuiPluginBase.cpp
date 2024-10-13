#include "wxGuiPluginBase.h"

DEFINE_EVENT_TYPE(wxEVT_GUI_PLUGIN_INTEROP)

#ifdef __WXMSW__

#include <cassert>       // assert
#include <cstdint>       // int32_t
#include <new>           // new(nothrow)
#include <wx/dynlib.h>   // wxDynamicLibrary
#include <wx/panel.h>    // wxPanel
#include <wx/string.h>   // wxString
#include <wx/window.h>   // wxWindow
#include "Auto.h"        // Auto

extern void OCX_Release_IOleObject(void *const p_IOleObject);
extern std::int32_t DynamicallyLoaded_AtlAxAttachControl(void *arg0, void *arg1);

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

#endif  // ifdef __WXMSW__
