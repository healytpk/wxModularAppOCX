#include "stdwx.h"
#include <wx/init.h>             // wxUninitialize
#include <wxGuiPluginBase.h>
#include "SampleGuiPlugin2.h"

PLUGIN_EXPORTED_API wxGuiPluginBase * CreatePlugin()
{
	return new SampleGuiPlugin2;
}

PLUGIN_EXPORTED_API void DeletePlugin(wxGuiPluginBase * plugin)
{
	wxDELETE(plugin);
}

PLUGIN_EXPORTED_API void *Get_Address_Of_wxUninitialze(void)
{
	return (void*)&wxUninitialize;
}
