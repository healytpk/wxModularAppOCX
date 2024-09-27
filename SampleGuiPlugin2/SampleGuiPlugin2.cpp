#include "stdwx.h"
#include "SampleGuiPlugin2.h"
#include "SampleGuiPluginWindow2.h"

IMPLEMENT_DYNAMIC_CLASS(SampleGuiPlugin2, wxObject)

SampleGuiPlugin2::SampleGuiPlugin2(wxEvtHandler *const handler) : m_Handler(handler)
{
}

wxString SampleGuiPlugin2::GetName() const
{
	return _("GUI Plugin 2");
}

wxString SampleGuiPlugin2::GetId() const
{
	return wxT("{1B226C84-6436-4092-9AB8-B2B0D6731EBE}");
}

wxWindow *SampleGuiPlugin2::CreatePanel(wxWindow *const parent)
{
	return new SampleGuiPluginWindow2(this, parent);
}
