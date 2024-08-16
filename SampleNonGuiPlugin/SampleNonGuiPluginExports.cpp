#include "stdwx.h"
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
	extern void wxUninitialize(void);   // defined in wxWidgets/src/common/init.cpp
	return (void*)&wxUninitialize;
}
