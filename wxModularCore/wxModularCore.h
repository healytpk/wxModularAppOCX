#pragma once

// We need to keep the list of loaded DLLs
WX_DECLARE_LIST(wxDynamicLibrary, wxDynamicLibraryList);

class wxModularCoreSettings;

class wxModularCore
{
public:
	wxModularCore();
	virtual ~wxModularCore();

	virtual wxString GetPluginsPath(bool forceProgramPath) const;
	virtual wxString GetPluginExt();

	virtual bool LoadAllPlugins(bool forceProgramPath) = 0;
	virtual bool UnloadAllPlugins() = 0;
	virtual void Clear();
protected:

	wxDynamicLibraryList m_DllList;

	wxModularCoreSettings * m_Settings;
	wxEvtHandler * m_Handler;

	template<typename PluginType,
		typename PluginListType>
		bool RegisterPlugin(PluginType * plugin, 
		PluginListType & list)
	{
		list.Append(plugin);
		return true;
	}

	template<typename PluginType, 
		typename PluginListType,
		typename PluginToDllDictionaryType, 
		typename DeletePluginFunctionType> 
		bool UnRegisterPlugin(
			PluginType * plugin, 
			PluginListType & container, 
			PluginToDllDictionaryType & pluginMap)
	{
		typename PluginListType::compatibility_iterator it = 
			container.Find(plugin);
		if (it == NULL)
			return false;

		do 
		{
			wxDynamicLibrary * dll = (wxDynamicLibrary *)pluginMap[plugin];
			if (!dll) // Probably plugin was not loaded from dll
				break;

			auto const pfnDeletePlugin = (DeletePluginFunctionType) dll->RawGetSymbol(wxT("DeletePlugin"));
			if (pfnDeletePlugin)
			{
				pfnDeletePlugin(plugin);
				container.Erase(it);
				pluginMap.erase(plugin);
				return true;
			}
		} while (false);

		// If plugin is not loaded from DLL (e.g. embedded into executable)
		wxDELETE(plugin);
		container.Erase(it);

		return true;
	}

	template<typename PluginType, 
		typename PluginListType,
		typename PluginToDllDictionaryType, 
		typename DeletePluginFunctionType> 
	bool UnloadPlugins(PluginListType & list, 
		PluginToDllDictionaryType & pluginDictoonary)
	{
		bool result = true;
		PluginType * plugin = NULL;
		while (list.GetFirst() && (plugin =  
			list.GetFirst()->GetData()))
		{
			result &= UnRegisterPlugin<PluginType, 
				PluginListType,
				PluginToDllDictionaryType, 
				DeletePluginFunctionType>(plugin, 
					list, pluginDictoonary);
		}
		return result;
	}

	template <typename PluginType, 
		typename PluginListType,
		typename PluginToDllDictionaryType, 
		typename CreatePluginFunctionType> 
	bool LoadPlugins(const wxString & pluginsDirectory, 
		PluginListType & list,
		PluginToDllDictionaryType & pluginDictionary,
		const wxString & subFolder)
	{
		wxFileName fn;
		fn.AssignDir(pluginsDirectory);
		wxLogDebug(wxT("%s"), fn.GetFullPath().data());
		fn.AppendDir(subFolder);
		wxLogDebug(wxT("%s"), fn.GetFullPath().data());
		if (!fn.DirExists())
			return false;

		if(!wxDirExists(fn.GetFullPath())) return false;
		wxString wildcard = wxString::Format(wxT("*.%s"), 
			GetPluginExt().GetData());
		wxArrayString pluginPaths;
		wxDir::GetAllFiles(fn.GetFullPath(), 
			&pluginPaths, wildcard);
		wxDir::GetAllFiles(fn.GetFullPath(),
			&pluginPaths, "*.ocx");

		for(size_t i = 0; i < pluginPaths.GetCount(); ++i)
		{
			wxString fileName = pluginPaths[i];
			wxDynamicLibrary * dll = new wxDynamicLibrary(fileName);
			if (dll->IsLoaded())
			{
				auto const pfnCreatePlugin = (CreatePluginFunctionType)dll->RawGetSymbol( wxT("CreatePlugin") );

				if (pfnCreatePlugin)
				{
					PluginType * plugin = pfnCreatePlugin();
					RegisterPlugin(plugin, list);
					m_DllList.Append(dll);
					pluginDictionary[plugin] = dll;
				}
				else
				{
					PluginType *const plugin = Process_ActiveX_Plugin<PluginType>::Do(dll);
					if ( plugin )
					{
						RegisterPlugin(plugin, list);
						m_DllList.Append(dll);
						pluginDictionary[plugin] = dll;
					}
					else
						wxDELETE(dll);
				}
			}
		}
		return true;
	}

};

#include <wx/dynlib.h>    // wxDynamicLibrary

template<class PluginType, bool is_gui_plugin = PluginType::is_gui_plugin>
struct Process_ActiveX_Plugin {
	static PluginType *Do(wxDynamicLibrary*)
	{
		// ActiveX controls are not non-GUI plugins.
		// See template specialisation lower down in
		// this file for GUI plugins.
		return nullptr;
	}
};

#ifdef __WXMSW__

#include <cassert>        // assert
#include <new>            // new(nothrow)
#include <wx/string.h>    // wxString
#include <wx/window.h>    // wxWindow

extern "C" {
	void OCX_Release_IOleObject(void *p_IOleObject);                  // defined in wxModularCoreOCX.cpp
	wxWindow *OCX_CreatePanel(void *p_IOleObject, wxWindow *parent);  // defined in wxModularCoreOCX.cpp
	void *OCX_Process_ActiveX_Plugin(wxDynamicLibrary *dll);          // defined in wxModularCoreOCX.cpp
}

template<class PluginType>
class wxGuiPluginOCX : public PluginType {
protected:
	void *const p_IOleObject;

public:
	using PluginType::GetName;
	using PluginType::GetId;
	using PluginType::CreatePanel;

	virtual wxString GetName(void) const override { return "Name OCX Plugin 0"; }
	virtual wxString GetId  (void) const override { return "ID   OCX Plugin 0"; }

	virtual wxWindow *CreatePanel(wxWindow *const parent) override
	{
		assert( nullptr != this->p_IOleObject );
		assert( nullptr != parent             );
		return OCX_CreatePanel(this->p_IOleObject, parent);
	}

	wxGuiPluginOCX(void *const arg_p_IOleObject) : PluginType( nullptr /* event handler */ ), p_IOleObject(arg_p_IOleObject)
	{
		assert( nullptr != arg_p_IOleObject );
	}

	~wxGuiPluginOCX(void) override
	{
		assert( nullptr != this->p_IOleObject );
		OCX_Release_IOleObject(this->p_IOleObject);
	}
};

template<class PluginType>
struct Process_ActiveX_Plugin<PluginType,true> {
	static PluginType *Do(wxDynamicLibrary *const dll)
	{
		void *const pole = OCX_Process_ActiveX_Plugin(dll);

		if ( nullptr == pole ) return nullptr;

		auto *const plugin = new(std::nothrow) wxGuiPluginOCX<PluginType>(pole);
		if ( nullptr == plugin )
		{
			OCX_Release_IOleObject(pole);
			return nullptr;
		}

		return plugin;
	}
};

#endif
