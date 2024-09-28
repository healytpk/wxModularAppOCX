#ifndef INCLUSION_GUARD_12946374654987334987151337180421369
#define INCLUSION_GUARD_12946374654987334987151337180421369

#include <wx/dynlib.h>                          // wxDynamicLibrary
#include <wx/window.h>                          // wxWindow

class wxGuiPluginBase;
class wxGuiPluginWindowBase;

struct HostAPIv1 {
    // Standalone functions
    wxEventType const& (*GetEventTypeObject)();

    // Static member functions
    bool (*wxGuiPluginWindowBase_ShowToolTips)();

    // Constructors
    void (*wxGuiPluginWindowBase_Constructor_void)(wxGuiPluginWindowBase&);
    void (*wxGuiPluginWindowBase_Constructor_ManyArguments)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long);

    // Member functions
    bool (*wxGuiPluginWindowBase_Create)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long);
    void (*wxGuiPluginWindowBase_Init)(wxGuiPluginWindowBase&);
    void (*wxGuiPluginWindowBase_CreateControls)(wxGuiPluginWindowBase&);
    wxBitmap (*wxGuiPluginWindowBase_GetBitmapResource)(wxGuiPluginWindowBase&, const wxString&);
    wxIcon (*wxGuiPluginWindowBase_GetIconResource)(wxGuiPluginWindowBase&, const wxString&);
};

extern HostAPIv1 const *hostfuncs;

inline void const *GetHostAPI(unsigned const version)
{
    auto const prog = wxDynamicLibrary::GetProgramHandle();
    if ( !prog ) return nullptr;
    auto const pf = (void const *(*)(unsigned))wxDynamicLibrary::RawGetSymbol(prog, "ForPlugins_GetHostAPI");
    if ( !pf ) return nullptr;
    return pf(version);
}

#endif  // INCLUSION_GUARD
