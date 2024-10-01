#ifndef INCLUSION_GUARD_12946374654987334987151337180421369
#define INCLUSION_GUARD_12946374654987334987151337180421369

#include <wx/dynlib.h>                          // wxDynamicLibrary
#include <wx/window.h>                          // wxWindow
#include "wxGuiPluginBase.h"                    // wxEVT_GUI_PLUGIN_INTEROP

class wxGuiPluginWindowBase;

struct HostAPIv1 {
    // Global objects
    wxEventType const &wxEVT_GUI_PLUGIN_INTEROP;

    // Standalone functions

    // Class static member functions
    bool (&wxGuiPluginWindowBase_ShowToolTips)(void);

    // Class constructors
    void (&wxGuiPluginWindowBase_Constructor_void)(wxGuiPluginWindowBase&);
    void (&wxGuiPluginWindowBase_Constructor_ManyArguments)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long);

    // Class member functions
    bool (&wxGuiPluginWindowBase_Create)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long);
    void (&wxGuiPluginWindowBase_Init)(wxGuiPluginWindowBase&);
    void (&wxGuiPluginWindowBase_CreateControls)(wxGuiPluginWindowBase&);
    wxBitmap (&wxGuiPluginWindowBase_GetBitmapResource)(wxGuiPluginWindowBase&, const wxString&);
    wxIcon (&wxGuiPluginWindowBase_GetIconResource)(wxGuiPluginWindowBase&, const wxString&);
};

extern HostAPIv1 const *hostapi;

inline void const *GetHostAPI( unsigned const version, void (*const addr_of_wxuninit)(void) )
{
    auto const prog = wxDynamicLibrary::GetProgramHandle();
    if ( !prog ) return nullptr;
    auto const pf = (void const *(*)(unsigned,void(*)(void)))wxDynamicLibrary::RawGetSymbol(prog, "ForPlugins_GetHostAPI");
    if ( !pf ) return nullptr;
    return pf(version, addr_of_wxuninit);
}

#endif  // INCLUSION_GUARD
