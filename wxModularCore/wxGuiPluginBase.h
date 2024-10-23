#pragma once

#include <wx/wx.h>
#include <wx/object.h>      // wxObject
#include <wx/window.h>      // wxWindow
#include "Declarations.h"

class wxGuiPluginBase : public wxObject {
public:
	virtual ~wxGuiPluginBase() = default;

	virtual bool ShouldInsertSpacers(void) const = 0;
	virtual wxString GetName() const = 0;
	virtual wxString GetId() const = 0;
	virtual wxWindow * CreatePanel(wxWindow * parent) = 0;

	virtual wxEvtHandler * GetEventHandler() = 0;
	virtual void SetEventHandler(wxEvtHandler * handler) = 0;
};

DECLARE_EXPORTED_EVENT_TYPE(DEMO_API, wxEVT_GUI_PLUGIN_INTEROP, wxEVT_USER_FIRST + 100)

typedef wxGuiPluginBase *(*CreatePlugin_function)( void const *(*const ForPlugins_GetHostAPI)( unsigned version, void (*addr_of_wxuninit)(void) ) );
typedef void (*DeletePlugin_function)(wxGuiPluginBase*);

#ifdef __WXMSW__

class wxGuiPluginOCX : public wxGuiPluginBase {
protected:
	void *const p_IOleObject;

public:
	virtual bool ShouldInsertSpacers(void) const override { return false; }

	wxGuiPluginOCX(void *arg_p_IOleObject);
	~wxGuiPluginOCX(void) override;

	virtual wxEvtHandler *GetEventHandler() override { return nullptr; }
	virtual void SetEventHandler(wxEvtHandler*) override {}

	virtual wxString GetName(void) const override { return "Name OCX Plugin 0"; }
	virtual wxString GetId  (void) const override { return "ID   OCX Plugin 0"; }

	virtual wxWindow *CreatePanel(wxWindow *const parent) override;
};

#endif
