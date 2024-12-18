#include <wx/init.h>             // wxUninitialize
#include <wxGuiPluginBase.h>
#include "SampleGuiPlugin1.h"
#include "host_interaction.hpp"

HostAPIv1 const *hostapi = nullptr;

PLUGIN_EXPORTED_API wxGuiPluginBase *CreatePlugin( void const *(*const ForPlugins_GetHostAPI)( unsigned version, void (*addr_of_wxuninit)(void) ) )
{
    hostapi = static_cast<HostAPIv1 const*>(  ForPlugins_GetHostAPI(1u, &wxUninitialize)  );
    if ( nullptr == hostapi ) return nullptr;
    return new SampleGuiPlugin1;
}

PLUGIN_EXPORTED_API void DeletePlugin(wxGuiPluginBase *plugin)
{
    wxDELETE(plugin);
}
