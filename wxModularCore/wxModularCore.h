#pragma once

#include <cstddef>        // size_t
#include <memory>         // unique_ptr
#include <regex>          // regex, regex_match
#include <wx/dynlib.h>    // wxDynamicLibrary
#include "wxGuiPluginBase.h"

// We need to keep the list of loaded DLLs
WX_DECLARE_LIST(wxDynamicLibrary, wxDynamicLibraryList);

class wxModularCoreSettings;

struct Process_ActiveX_Plugin {
	static wxGuiPluginBase *Do(wxDynamicLibrary*);
};

class wxModularCore
{
public:
	wxModularCore();
	virtual ~wxModularCore();

	virtual wxString GetPluginsPath(bool forceProgramPath) const;
	virtual std::regex GetPluginRegex() const;

	virtual bool LoadAllPlugins(bool forceProgramPath) = 0;
	virtual bool UnloadAllPlugins() = 0;
	virtual void Clear();
protected:

	wxDynamicLibraryList m_DllList;

	wxModularCoreSettings * m_Settings;
	wxEvtHandler * m_Handler;

	template<typename PluginListType>
		bool RegisterPlugin(wxGuiPluginBase * plugin, 
		PluginListType & list)
	{
		list.Append(plugin);
		return true;
	}

	template<typename PluginListType,
		typename PluginToDllDictionaryType, 
		typename DeletePluginFunctionType> 
		bool UnRegisterPlugin(
			wxGuiPluginBase * plugin, 
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

	template<typename PluginListType,
		typename PluginToDllDictionaryType, 
		typename DeletePluginFunctionType> 
	bool UnloadPlugins(PluginListType & list, 
		PluginToDllDictionaryType & pluginDictoonary)
	{
		bool result = true;
		wxGuiPluginBase * plugin = NULL;
		while (list.GetFirst() && (plugin =  
			list.GetFirst()->GetData()))
		{
			result &= UnRegisterPlugin<
				PluginListType,
				PluginToDllDictionaryType, 
				DeletePluginFunctionType>(plugin, 
					list, pluginDictoonary);
		}
		return result;
	}

	template <typename PluginListType,
		typename PluginToDllDictionaryType, 
		typename CreatePluginFunctionType> 
	bool LoadPlugins(const wxString & pluginsDirectory, 
		PluginListType & list,
		PluginToDllDictionaryType & pluginDictionary)
	{
		wxFileName fn;
		fn.AssignDir(pluginsDirectory);
		wxLogDebug(wxT("%s"), fn.GetFullPath().data());
		if (!fn.DirExists())
			return false;

		if(!wxDirExists(fn.GetFullPath())) return false;
		wxArrayString pluginPaths;
		wxDir::GetAllFiles(fn.GetFullPath(), 
			&pluginPaths, wxEmptyString, wxDIR_FILES|wxDIR_DIRS|wxDIR_HIDDEN);

		// ==== The 'pluginPaths' now contains all of the
		// ==== filenames in the directory, so we iterate
		// ==== through them all to remove the ones
		// ==== that don't match the regex for a plugin
		for ( std::size_t i = 0u; i < pluginPaths.GetCount(); ++i )
		{
			// Hopefully converting the wxString to an std::string
			// will leave us with a string that we can do regex on
			if ( false == std::regex_match( pluginPaths[i].ToStdString(), this->GetPluginRegex() ) )
			{
				pluginPaths.RemoveAt( i-- );
			}
		}

		auto const yes = [this,&list,&pluginDictionary]( std::unique_ptr<wxDynamicLibrary> &pL, wxGuiPluginBase *const pP )
			{
					this->RegisterPlugin(pP, list);
					this->m_DllList.Append( pL.get() );
					auto const p = pL.release();
					pluginDictionary[pP] = p;
			};

		for(size_t i = 0; i < pluginPaths.GetCount(); ++i)
		{
			wxGuiPluginBase *plugin = nullptr;

			wxString fileName = pluginPaths[i];
			std::unique_ptr<wxDynamicLibrary> dll( new wxDynamicLibrary(fileName) );
			if ( !dll || !dll->IsLoaded() ) continue;

			auto const pfnCreatePlugin = (CreatePluginFunctionType)dll->RawGetSymbol( wxT("CreatePlugin") );

			if ( pfnCreatePlugin )
			{
				// ==================================================
				//   The next 9 lines check that the plugin's
				//   wxWidgets library is the same as the wxWidgets
				//   library used by the main program.
				auto const pfnGetAddrUninit = (void*(*)(void))dll->RawGetSymbol( wxT("Get_Address_Of_wxUninitialze") );
				if (pfnGetAddrUninit)
				{
					if ( (void*)&wxUninitialize != pfnGetAddrUninit() ) continue;
				}
				// ==================================================
				if ( !(plugin = pfnCreatePlugin()) ) continue;  // deliberate assignment
				yes(dll, plugin);
			}
			else if ( plugin = Process_ActiveX_Plugin::Do( dll.get() ) )  // deliberate assignment
			{
				yes(dll, plugin);
			}
		}

		return true;
	}

};

#ifndef __WXMSW__

inline wxGuiPluginBase *Process_ActiveX_Plugin::Do(wxDynamicLibrary*)
{
    // ActiveX controls are not non-GUI plugins.
    // See template specialisation lower down in
    // this file for GUI plugins.
    return nullptr;
}

#else

#include <cassert>        // assert
#include <new>            // new(nothrow)
#include <wx/string.h>    // wxString
#include <wx/window.h>    // wxWindow

extern "C" {
	void OCX_Release_IOleObject(void *p_IOleObject);                  // defined in wxModularCoreOCX.cpp
	wxWindow *OCX_CreatePanel(void *p_IOleObject, wxWindow *parent);  // defined in wxModularCoreOCX.cpp
	void *OCX_Process_ActiveX_Plugin(wxDynamicLibrary *dll);          // defined in wxModularCoreOCX.cpp
}

class wxGuiPluginOCX : public wxGuiPluginBase {
protected:
	void *const p_IOleObject;

public:
	virtual wxString GetName(void) const override { return "Name OCX Plugin 0"; }
	virtual wxString GetId  (void) const override { return "ID   OCX Plugin 0"; }

	virtual wxWindow *CreatePanel(wxWindow *const parent) override
	{
		assert( nullptr != this->p_IOleObject );
		assert( nullptr != parent             );
		return OCX_CreatePanel(this->p_IOleObject, parent);
	}

	wxGuiPluginOCX(void *const arg_p_IOleObject) : wxGuiPluginBase( nullptr /* event handler */ ), p_IOleObject(arg_p_IOleObject)
	{
		assert( nullptr != arg_p_IOleObject );
	}

	~wxGuiPluginOCX(void) override
	{
		assert( nullptr != this->p_IOleObject );
		OCX_Release_IOleObject(this->p_IOleObject);
	}
};

inline wxGuiPluginBase *Process_ActiveX_Plugin::Do(wxDynamicLibrary*)
{
    void *const pole = OCX_Process_ActiveX_Plugin(dll);

    if ( nullptr == pole ) return nullptr;

    auto *const plugin = new(std::nothrow) wxGuiPluginOCX(pole);
    if ( nullptr == plugin )
    {
        OCX_Release_IOleObject(pole);
        return nullptr;
    }

    return plugin;
}

#endif
