#include "stdwx.h"
#include <wx/init.h>             // wxUninitialize
#include <wxGuiPluginBase.h>
#include "SampleGuiPlugin1.h"

PLUGIN_EXPORTED_API wxGuiPluginBase * CreatePlugin()
{
	return new SampleGuiPlugin1;
}

PLUGIN_EXPORTED_API void DeletePlugin(wxGuiPluginBase * plugin)
{
	wxDELETE(plugin);
}

PLUGIN_EXPORTED_API void *Get_Address_Of_wxUninitialze(void)
{
	return (void*)&wxUninitialize;
}
