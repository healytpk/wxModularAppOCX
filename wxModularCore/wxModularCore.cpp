#include "stdwx.h"
#include "wxModularCore.h"
#include "wxModularCoreSettings.h"
#include <wx/listimpl.cpp>
#include <wx/init.h>                     // wxUninitialize
#include "wxGuiPluginBase.h"             // wxEVT_GUI_PLUGIN_INTEROP
#include "host_interaction.hpp"

WX_DEFINE_LIST(wxDynamicLibraryList);

wxModularCore::wxModularCore()
	: m_Settings(new wxModularCoreSettings), m_Handler(new wxEvtHandler)
{
	// This will allow to delete all objects from this list automatically
	m_DllList.DeleteContents(true);
}

wxModularCore::~wxModularCore()
{
	wxDELETE(m_Handler);
	wxDELETE(m_Settings);
}

void wxModularCore::Clear()
{
	UnloadAllPlugins();
	// TODO: Add the code which resets the object to initial state
}

wxString wxModularCore::GetPluginsPath(bool forceProgramPath) const
{
#if defined(__WXMAC__)
	return wxStandardPaths::Get().GetPluginsDir();
#else
	wxString path;
	if (m_Settings->GetStoreInAppData() && !forceProgramPath)
		path = wxStandardPaths::Get().GetConfigDir();
	else
		path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
	wxFileName fn;
	fn.AssignDir(path);
	return fn.GetFullPath();
#endif
}

std::regex wxModularCore::GetPluginRegex() const
{
#if defined(__WXMSW__)
	return std::regex( "(.*\\\\)?(((?!wx)[^\\\\]*\\.dll)|([^\\\\]*\\.ocx))", std::regex::icase );
#elif defined(__WXGTK__)
	return std::regex("(.*/)?((?!libwx)[^/]+)\\.so(\\.[0-9]+)*");
#elif defined(__WXMAC__)
	return std::regex("(.*/)?((?!libwx)[^/]+)\\.dylib(\\.[0-9]+)*");
#else
	return std::regex("no_known_plugin");
#endif
}

// Global objects
// Standalone functions
// Class static member functions
extern bool ForPlugins_wxGuiPluginWindowBase_ShowToolTips();
// Class constructors
extern void ForPlugins_wxGuiPluginWindowBase_Constructor_void(wxGuiPluginWindowBase & obj);
extern void ForPlugins_wxGuiPluginWindowBase_Constructor_ManyArguments(wxGuiPluginWindowBase & obj, wxGuiPluginBase * plugin, wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style);
// Class member functions
extern void ForPlugins_wxGuiPluginWindowBase_Init(wxGuiPluginWindowBase & obj);
extern void ForPlugins_wxGuiPluginWindowBase_CreateControls(wxGuiPluginWindowBase & obj);
extern bool ForPlugins_wxGuiPluginWindowBase_Create(wxGuiPluginWindowBase & obj, wxGuiPluginBase * plugin, wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style);
extern wxBitmap ForPlugins_wxGuiPluginWindowBase_GetBitmapResource(wxGuiPluginWindowBase & obj, const wxString& /*name*/);
extern wxIcon ForPlugins_wxGuiPluginWindowBase_GetIconResource(wxGuiPluginWindowBase & obj, const wxString& /*name*/);

extern "C" {
DEMO_API void const *ForPlugins_GetHostAPI( unsigned const version, void (*const addr_of_wxuninit)(void) )
{
	// The next line ensures that the host program and
	// the plugin are both using the same wxWidgets
	// library. This is necessary, as neither the host
	// nor the plugin can link statically with wxWidgets,
	// and both the host and the plugin must link dynamically
	// with the exact same wxWidgets library files
	// -- specifically there can only be one instance of the
	// wxWidgets global variables inside the address space
	// of the current process.
	if ( &wxUninitialize != addr_of_wxuninit ) return nullptr;

	static constexpr HostAPIv1 api1 = {
		wxEVT_GUI_PLUGIN_INTEROP,
		ForPlugins_wxGuiPluginWindowBase_ShowToolTips,
		ForPlugins_wxGuiPluginWindowBase_Constructor_void,
		ForPlugins_wxGuiPluginWindowBase_Constructor_ManyArguments,
		ForPlugins_wxGuiPluginWindowBase_Create,
		ForPlugins_wxGuiPluginWindowBase_Init,
		ForPlugins_wxGuiPluginWindowBase_CreateControls,
		ForPlugins_wxGuiPluginWindowBase_GetBitmapResource,
		ForPlugins_wxGuiPluginWindowBase_GetIconResource,
	};

	/*
	static constexpr HostAPIv2 api2 = {
		. . .
		. . .   // 2nd version of API goes here
		. . .
	};
	*/

	switch ( version )
	{
	case 1u:   return &api1;
	case 2u: //return &api2;
	default:
		;  // fall through
	}

	return nullptr;
}
}  // extern "C"
