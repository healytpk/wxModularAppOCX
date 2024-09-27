#pragma once

#include <wxGuiPluginBase.h>

class SampleGuiPlugin1 : public wxGuiPluginBase {
	DECLARE_DYNAMIC_CLASS(SampleGuiPlugin1)
protected:
	wxEvtHandler *m_Handler;
public:
	SampleGuiPlugin1() noexcept = default;
	SampleGuiPlugin1(wxEvtHandler *handler);
	virtual wxString GetName() const override;
	virtual wxString GetId() const override;
	virtual wxWindow *CreatePanel(wxWindow *parent) override;
	virtual wxEvtHandler *GetEventHandler() override { return m_Handler; }
	virtual void SetEventHandler(wxEvtHandler *const handler) override { m_Handler = handler; }
};
