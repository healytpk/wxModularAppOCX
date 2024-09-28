#include "stdwx.h"
#include "wxGuiPluginBase.h"

DEFINE_EVENT_TYPE(wxEVT_GUI_PLUGIN_INTEROP)

wxEventType const &ForPlugins_GetEventTypeObject(void)
{
    return wxEVT_GUI_PLUGIN_INTEROP;
}
