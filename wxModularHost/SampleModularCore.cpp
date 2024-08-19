#include "stdwx.h"
#include "SampleModularCore.h"
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(wxGuiPluginBaseList);

SampleModularCore::~SampleModularCore()
{
	Clear();
}

bool SampleModularCore::LoadAllPlugins(bool forceProgramPath)
{
	wxString pluginsRootDir = GetPluginsPath(forceProgramPath);
	bool result = true;
	result &= LoadPlugins<
		wxGuiPluginBaseList,
		wxGuiPluginToDllDictionary,
		CreatePlugin_function>(pluginsRootDir,
		m_GuiPlugins, 
		m_MapGuiPluginsDll);
	// You can implement other logic which takes in account
	// the result of LoadPlugins() calls
	for(wxGuiPluginBaseList::Node * node = m_GuiPlugins.GetFirst(); 
		node; node = node->GetNext())
	{
		wxGuiPluginBase * plugin = node->GetData();
		plugin->SetEventHandler(m_Handler);
	}
	return true;
}

bool SampleModularCore::UnloadAllPlugins()
{
	return 
		UnloadPlugins<
			wxGuiPluginBaseList,
			wxGuiPluginToDllDictionary,
			DeletePlugin_function>(m_GuiPlugins, 
			m_MapGuiPluginsDll);
}

const wxGuiPluginBaseList & SampleModularCore::GetGuiPlugins() const
{
	return m_GuiPlugins;
}