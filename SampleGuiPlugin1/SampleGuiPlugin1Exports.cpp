#include "stdwx.h"
#include <wx/init.h>             // wxUninitialize
#include <wxGuiPluginBase.h>
#include "SampleGuiPlugin1.h"
#include "host_interaction.hpp"

HostAPIv1 const *hostapi = nullptr;

PLUGIN_EXPORTED_API wxGuiPluginBase *CreatePlugin()
{
    hostapi = static_cast<HostAPIv1 const*>(  GetHostAPI(1u)  );
    if ( nullptr == hostapi ) return nullptr;
    return new SampleGuiPlugin1;
}

PLUGIN_EXPORTED_API void DeletePlugin(wxGuiPluginBase *plugin)
{
    wxDELETE(plugin);
}

PLUGIN_EXPORTED_API void *Get_Address_Of_wxUninitialze(void)
{
    return (void*)&wxUninitialize;
}
