#pragma once

#include "Declarations.h"
#include "wxPluginBase.h"

class DEMO_API wxGuiPluginBase : public wxPluginBase
{
	DECLARE_ABSTRACT_CLASS(wxGuiPluginBase)
public:
	static constexpr bool is_gui_plugin = true;
	wxGuiPluginBase(wxEvtHandler * handler);
	virtual ~wxGuiPluginBase();
	
	virtual wxString GetName() const = 0;
	virtual wxString GetId() const = 0;
	virtual wxWindow * CreatePanel(wxWindow * parent) = 0;

	wxEvtHandler * GetEventHandler();
	virtual void SetEventHandler(wxEvtHandler * handler);
protected:
	wxEvtHandler * m_Handler;
};

DECLARE_EXPORTED_EVENT_TYPE(DEMO_API, wxEVT_GUI_PLUGIN_INTEROP, wxEVT_USER_FIRST + 100)
