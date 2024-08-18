#include "stdwx.h"
#include <wx/init.h>             // wxUninitialize
#include <wxPluginBase.h>
#include "SampleNonGuiPlugin.h"

PLUGIN_EXPORTED_API wxPluginBase * CreatePlugin()
{
	return new SampleNonGuiPlugin;
}

PLUGIN_EXPORTED_API void DeletePlugin(wxPluginBase * plugin)
{
	wxDELETE(plugin);
}

PLUGIN_EXPORTED_API void *Get_Address_Of_wxUninitialze(void)
{
	return (void*)&wxUninitialize;
}
