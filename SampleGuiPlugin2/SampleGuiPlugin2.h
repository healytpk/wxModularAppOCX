#pragma once

#include <wxGuiPluginBase.h>

class SampleGuiPlugin2 : public wxGuiPluginBase {
	DECLARE_DYNAMIC_CLASS(SampleGuiPlugin2)
protected:
	wxEvtHandler *m_Handler;
public:
	virtual bool ShouldInsertSpacers(void) const override { return true; }
	SampleGuiPlugin2() noexcept = default;
	SampleGuiPlugin2(wxEvtHandler *handler);
	virtual wxString GetName() const override;
	virtual wxString GetId() const override;
	virtual wxWindow *CreatePanel(wxWindow *parent) override;
	virtual wxEvtHandler *GetEventHandler() override { return m_Handler; }
	virtual void SetEventHandler(wxEvtHandler *const handler) override { m_Handler = handler; }
};
