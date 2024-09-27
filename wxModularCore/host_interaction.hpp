#ifndef INCLUSION_GUARD_12946374654987334987151337180421369
#define INCLUSION_GUARD_12946374654987334987151337180421369

#include <wx/dynlib.h>                          // wxDynamicLibrary
#include <wx/window.h>                          // wxWindow

class wxGuiPluginBase;
class wxGuiPluginWindowBase;

struct HostFunctions {
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

inline HostFunctions hostfuncs;

inline bool ResolveAllHostFuncs(void)
{
    auto const prog = wxDynamicLibrary::GetProgramHandle();
    if ( !prog ) return false;
    constexpr void *(*sym)(wxDllType,wxString const &) = &wxDynamicLibrary::RawGetSymbol;
    hostfuncs.GetEventTypeObject =
     (wxEventType const& (*)())sym(prog, "ForPlugins_GetEventTypeObject");
    if ( nullptr == hostfuncs.GetEventTypeObject ) return false;
    hostfuncs.wxGuiPluginWindowBase_Constructor_void =
     (void (*)(wxGuiPluginWindowBase&))sym(prog, "ForPlugins_wxGuiPluginWindowBase_Constructor_void");
    if ( nullptr == hostfuncs.wxGuiPluginWindowBase_Constructor_void ) return false;
    hostfuncs.wxGuiPluginWindowBase_Constructor_ManyArguments =
     (void (*)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long))sym(prog, "ForPlugins_wxGuiPluginWindowBase_Constructor_ManyArguments");
    if ( nullptr == hostfuncs.wxGuiPluginWindowBase_Constructor_ManyArguments ) return false;
    hostfuncs.wxGuiPluginWindowBase_Create =
     (bool (*)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long))sym(prog, "ForPlugins_wxGuiPluginWindowBase_Create");
    if ( nullptr == hostfuncs.wxGuiPluginWindowBase_Create ) return false;
    hostfuncs.wxGuiPluginWindowBase_Init =
     (void (*)(wxGuiPluginWindowBase&))sym(prog, "ForPlugins_wxGuiPluginWindowBase_Init");
    if ( nullptr == hostfuncs.wxGuiPluginWindowBase_Init ) return false;
    hostfuncs.wxGuiPluginWindowBase_CreateControls =
     (void (*)(wxGuiPluginWindowBase&))sym(prog, "ForPlugins_wxGuiPluginWindowBase_CreateControls");
    if ( nullptr == hostfuncs.wxGuiPluginWindowBase_CreateControls ) return false;
    hostfuncs.wxGuiPluginWindowBase_GetBitmapResource =
     (wxBitmap (*)(wxGuiPluginWindowBase&, const wxString&))sym(prog, "ForPlugins_wxGuiPluginWindowBase_GetBitmapResource");
    if ( nullptr == hostfuncs.wxGuiPluginWindowBase_GetBitmapResource ) return false;
    hostfuncs.wxGuiPluginWindowBase_GetIconResource =
     (wxIcon (*)(wxGuiPluginWindowBase&, const wxString&))sym(prog, "ForPlugins_wxGuiPluginWindowBase_GetIconResource");
    if ( nullptr == hostfuncs.wxGuiPluginWindowBase_GetIconResource ) return false;
    hostfuncs.wxGuiPluginWindowBase_ShowToolTips =
     (bool (*)())sym(prog, "ForPlugins_wxGuiPluginWindowBase_ShowToolTips");
    if ( nullptr == hostfuncs.wxGuiPluginWindowBase_ShowToolTips ) return false;
    return true; // All functions resolved successfully
}

#endif  // INCLUSION_GUARD
