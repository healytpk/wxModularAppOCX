#ifndef INCLUSION_GUARD_12946374654987334987151337180421369
#define INCLUSION_GUARD_12946374654987334987151337180421369

#include <dlfcn.h>                              // dlopen, dlsym

#include <wx/window.h>

class wxGuiPluginBase;
class wxGuiPluginWindowBase;

struct HostFunctions {
    wxEventType const& (*GetEventTypeObject)();
    wxIcon (*wxGuiPluginWindowBase_Constructor_void)(wxGuiPluginWindowBase&);
    bool (*wxGuiPluginWindowBase_Constructor_ManyArguments)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long);
    bool (*wxGuiPluginWindowBase_Create)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long);
    void (*wxGuiPluginWindowBase_Init)(wxGuiPluginWindowBase&);
    void (*wxGuiPluginWindowBase_CreateControls)(wxGuiPluginWindowBase&);
    wxBitmap (*wxGuiPluginWindowBase_GetBitmapResource)(wxGuiPluginWindowBase&, const wxString&);
    wxIcon (*wxGuiPluginWindowBase_GetIconResource)(wxGuiPluginWindowBase&, const wxString&);
    bool (*wxGuiPluginWindowBase_ShowToolTips)();
};

inline HostFunctions hostfuncs;

inline bool ResolveAllHostFuncs(void)
{
    auto const prog = dlopen(nullptr, RTLD_LAZY);
    hostfuncs.GetEventTypeObject = (wxEventType const& (*)())dlsym(prog, "ForPlugins_GetEventTypeObject");
    if (!hostfuncs.GetEventTypeObject) return false;
    hostfuncs.wxGuiPluginWindowBase_Constructor_void = (wxIcon (*)(wxGuiPluginWindowBase&))dlsym(prog, "ForPlugins_wxGuiPluginWindowBase_Constructor_void");
    if (!hostfuncs.wxGuiPluginWindowBase_Constructor_void) return false;
    hostfuncs.wxGuiPluginWindowBase_Constructor_ManyArguments = (bool (*)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long))dlsym(prog, "ForPlugins_wxGuiPluginWindowBase_Constructor_ManyArguments");
    if (!hostfuncs.wxGuiPluginWindowBase_Constructor_ManyArguments) return false;
    hostfuncs.wxGuiPluginWindowBase_Create = (bool (*)(wxGuiPluginWindowBase&, wxGuiPluginBase*, wxWindow*, wxWindowID, const wxPoint&, const wxSize&, long))dlsym(prog, "ForPlugins_wxGuiPluginWindowBase_Create");
    if (!hostfuncs.wxGuiPluginWindowBase_Create) return false;
    hostfuncs.wxGuiPluginWindowBase_Init = (void (*)(wxGuiPluginWindowBase&))dlsym(prog, "ForPlugins_wxGuiPluginWindowBase_Init");
    if (!hostfuncs.wxGuiPluginWindowBase_Init) return false;
    hostfuncs.wxGuiPluginWindowBase_CreateControls = (void (*)(wxGuiPluginWindowBase&))dlsym(prog, "ForPlugins_wxGuiPluginWindowBase_CreateControls");
    if (!hostfuncs.wxGuiPluginWindowBase_CreateControls) return false;
    hostfuncs.wxGuiPluginWindowBase_GetBitmapResource = (wxBitmap (*)(wxGuiPluginWindowBase&, const wxString&))dlsym(prog, "ForPlugins_wxGuiPluginWindowBase_GetBitmapResource");
    if (!hostfuncs.wxGuiPluginWindowBase_GetBitmapResource) return false;
    hostfuncs.wxGuiPluginWindowBase_GetIconResource = (wxIcon (*)(wxGuiPluginWindowBase&, const wxString&))dlsym(prog, "ForPlugins_wxGuiPluginWindowBase_GetIconResource");
    if (!hostfuncs.wxGuiPluginWindowBase_GetIconResource) return false;
    hostfuncs.wxGuiPluginWindowBase_ShowToolTips = (bool (*)())dlsym(prog, "ForPlugins_wxGuiPluginWindowBase_ShowToolTips");
    if (!hostfuncs.wxGuiPluginWindowBase_ShowToolTips) return false;
    return true; // All functions resolved successfully
}

#endif  // INCLUSION_GUARD
