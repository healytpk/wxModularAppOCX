#include "stdwx.h"
#include <wx/init.h>             // wxUninitialize
#include <wxNonGuiPluginBase.h>
#include "SampleNonGuiPlugin.h"

PLUGIN_EXPORTED_API wxNonGuiPluginBase * CreatePlugin()
{
	return new SampleNonGuiPlugin;
}

PLUGIN_EXPORTED_API void DeletePlugin(wxNonGuiPluginBase * plugin)
{
	wxDELETE(plugin);
}

PLUGIN_EXPORTED_API void *Get_Address_Of_wxUninitialze(void)
{
	return (void*)&wxUninitialize;
}
