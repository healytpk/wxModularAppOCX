#include "stdwx.h"
#include "SampleGuiPlugin1.h"
#include "SampleGuiPluginWindow1.h"

IMPLEMENT_DYNAMIC_CLASS(SampleGuiPlugin1, wxObject)

SampleGuiPlugin1::SampleGuiPlugin1(wxEvtHandler * handler) : m_Handler(handler)
{
}

wxString SampleGuiPlugin1::GetName() const
{
	return _("GUI Plugin 1");
}

wxString SampleGuiPlugin1::GetId() const
{
	return wxT("{4E97DF66-5FBB-4719-AF17-76C1C82D3FE1}");
}

wxWindow * SampleGuiPlugin1::CreatePanel(wxWindow * parent)
{
	return new SampleGuiPluginWindow1(this, parent);
}
