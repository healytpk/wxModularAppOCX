#include "wx/wx.h"
#include "SampleGuiPluginWindow1.h"
#include <wxGuiPluginBase.h>
#include "host_interaction.hpp"

IMPLEMENT_DYNAMIC_CLASS( SampleGuiPluginWindow1, wxGuiPluginWindowBase )

BEGIN_EVENT_TABLE( SampleGuiPluginWindow1, wxGuiPluginWindowBase )
    EVT_BUTTON( ID_SEND_EVENT_BUTTON, SampleGuiPluginWindow1::OnSENDEVENTBUTTONClick )
END_EVENT_TABLE()

SampleGuiPluginWindow1::SampleGuiPluginWindow1()
{
    Init();
}

SampleGuiPluginWindow1::SampleGuiPluginWindow1( wxGuiPluginBase * plugin, 
											   wxWindow* parent, wxWindowID id, 
											   const wxPoint& pos, const wxSize& size, 
											   long style )
{
    Init();
    Create(plugin, parent, id, pos, size, style);
}

bool SampleGuiPluginWindow1::Create(wxGuiPluginBase * plugin, 
									wxWindow* parent, wxWindowID id, 
									const wxPoint& pos, const wxSize& size, 
									long style )
{
    wxGuiPluginWindowBase::Create(plugin, parent, id, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
    return true;
}

SampleGuiPluginWindow1::~SampleGuiPluginWindow1()
{

}

void SampleGuiPluginWindow1::Init()
{
    m_SamppleTextCtrl = nullptr;
}

void SampleGuiPluginWindow1::CreateControls()
{    
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( this, wxID_STATIC, _("Enter some text here:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    m_SamppleTextCtrl = new wxTextCtrl( this, ID_SAMPLE_TEXTCTRL, _("Hello, GUI Plugin 2!"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(m_SamppleTextCtrl, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxButton* itemButton5 = new wxButton( this, ID_SEND_EVENT_BUTTON, _("Send event"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}

bool SampleGuiPluginWindow1::ShowToolTips()
{
    return true;
}

wxBitmap SampleGuiPluginWindow1::GetBitmapResource( const wxString& /*name*/ )
{
    return wxNullBitmap;
}

wxIcon SampleGuiPluginWindow1::GetIconResource( const wxString& /*name*/ )
{
    return wxNullIcon;
}

void SampleGuiPluginWindow1::OnSENDEVENTBUTTONClick( wxCommandEvent& event )
{
	wxCommandEvent e( hostapi->wxEVT_GUI_PLUGIN_INTEROP );
	e.SetString(m_SamppleTextCtrl->GetValue());
	GetPlugin()->GetEventHandler()->AddPendingEvent(e);
    event.Skip();
}
