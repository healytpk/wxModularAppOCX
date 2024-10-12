#pragma once

#include <cstddef>        // size_t
#include <memory>         // unique_ptr
#include <regex>          // regex, regex_match
#include <wx/dir.h>       // wxDir
#include <wx/dynlib.h>    // wxDynamicLibrary
#include <wx/filename.h>  // wxFileName
#include <wx/log.h>       // wxLogDebug
#include "wxGuiPluginBase.h"
#include "host_interaction.hpp"

DEMO_API wxGuiPluginBase *ForHost_Process_ActiveX_Plugin(wxDynamicLibrary *dll);  // defined in wxModularCore.cpp

// We need to keep the list of loaded DLLs
WX_DECLARE_LIST(wxDynamicLibrary, wxDynamicLibraryList);
// We need to know which DLL produced the specific plugin object.
WX_DECLARE_HASH_MAP(wxGuiPluginBase*, wxDynamicLibrary*, \
					wxPointerHash, wxPointerEqual, \
					wxGuiPluginToDllDictionary);
// And separate list of loaded plugins for faster access.
WX_DECLARE_LIST(wxGuiPluginBase, wxGuiPluginBaseList);

class wxModularCoreSettings;

class wxModularCore {
protected:
	wxGuiPluginToDllDictionary m_MapGuiPluginsDll;
	wxGuiPluginBaseList m_GuiPlugins;
	wxDynamicLibraryList m_DllList;
	wxModularCoreSettings * m_Settings;
	wxEvtHandler * m_Handler;

	bool RegisterPlugin(wxGuiPluginBase *plugin, wxGuiPluginBaseList &list);
	bool UnRegisterPlugin(wxGuiPluginBase *plugin, wxGuiPluginBaseList &container, wxGuiPluginToDllDictionary &pluginMap);
	bool UnloadPlugins(wxGuiPluginBaseList & list, wxGuiPluginToDllDictionary &pluginDictionary);
	bool LoadPlugins(wxString const &pluginsDirectory, wxGuiPluginBaseList &list, wxGuiPluginToDllDictionary &pluginDictionary);

public:
	wxModularCore();
	~wxModularCore();

	wxString GetPluginsPath(bool forceProgramPath) const;
	std::regex GetPluginRegex() const;

	bool LoadAllPlugins(bool forceProgramPath);
	bool UnloadAllPlugins();
	void Clear();

	wxGuiPluginBaseList const &GetGuiPlugins(void) const;
};
