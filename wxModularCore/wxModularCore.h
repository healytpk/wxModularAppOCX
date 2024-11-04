#pragma once
#include <cassert>        // assert
#include <cstddef>        // size_t
#include <map>            // map
#include <memory>         // unique_ptr
#include <regex>          // regex, regex_match
#include <utility>        // pair
#include <vector>         // vector
#include <wx/dir.h>       // wxDir
#include <wx/dynlib.h>    // wxDynamicLibrary
#include <wx/filename.h>  // wxFileName
#include <wx/log.h>       // wxLogDebug
#include "wxGuiPluginBase.h"
#include "host_interaction.hpp"

class wxModularCoreSettings;

class wxModularCore {
protected:
	std::map< wxString, std::pair<wxDynamicLibrary, wxGuiPluginBase*> > mymap;
	wxModularCoreSettings * m_Settings;
	wxEvtHandler * m_Handler;

	bool UnloadPlugin(wxString const &filename);
	bool UnloadAllPlugins(void);
	bool DiscoverPlugins(wxString const &pluginsDirectory);

public:
	wxModularCore();
	~wxModularCore();

	wxString GetPluginsPath(bool forceProgramPath) const;
	std::regex GetPluginRegex() const;

	wxGuiPluginBase *LoadPlugin(wxString const &);

	bool DiscoverAllPlugins(bool forceProgramPath);
	void Clear();

	std::vector<wxString> GetGuiPluginFilenames(void) const;
	wxEvtHandler *GetEventHandler(void) { return this->m_Handler; }
};

class TabWindowForPlugin : public wxWindow {
protected:
    wxModularCore *pluginManager = nullptr;
    wxWindow *child = nullptr;
    wxGuiPluginBase *plugin = nullptr;
    wxString const filename;
public:
    TabWindowForPlugin(wxModularCore *const arg_plugman, wxWindow *const parent, wxString const &arg_filename)
      : wxWindow(parent, wxID_ANY), pluginManager(arg_plugman), filename(arg_filename)
    {
        assert( nullptr != pluginManager      );
        assert( false   == filename.IsEmpty() );
    }
    void ShowPluginWidgets(void);
};
