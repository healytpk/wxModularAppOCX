#include "wx/wx.h"
#include "SampleGuiPluginWindow2.h"
#include <wxGuiPluginBase.h>
#include "host_interaction.hpp"

IMPLEMENT_DYNAMIC_CLASS( SampleGuiPluginWindow2, wxGuiPluginWindowBase )

BEGIN_EVENT_TABLE( SampleGuiPluginWindow2, wxGuiPluginWindowBase )
    EVT_WINDOW_DESTROY( SampleGuiPluginWindow2::OnDestroy )
END_EVENT_TABLE()

SampleGuiPluginWindow2::SampleGuiPluginWindow2()
{
    Init();
}

SampleGuiPluginWindow2::SampleGuiPluginWindow2( wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(plugin, parent, id, pos, size, style);
}

bool SampleGuiPluginWindow2::Create( wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
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

SampleGuiPluginWindow2::~SampleGuiPluginWindow2()
{

}

void SampleGuiPluginWindow2::Init()
{
    m_MessageTextCtrl = nullptr;
}

void SampleGuiPluginWindow2::CreateControls()
{
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( this, wxID_STATIC, _("This text box receives messages from GUI Plugin 1:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    m_MessageTextCtrl = new wxTextCtrl( this, ID_GUI_PLUGIN2_MESSAGE_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    itemBoxSizer2->Add(m_MessageTextCtrl, 0, wxGROW|wxALL, 5);

    // Connect events and objects
    this->Connect(ID_SAMPLEGUIPLUGINWINDOW2, wxEVT_DESTROY, wxWindowDestroyEventHandler(SampleGuiPluginWindow2::OnDestroy), NULL, this);

    GetPlugin()->GetEventHandler()->Bind( hostapi->wxEVT_GUI_PLUGIN_INTEROP, wxCommandEventHandler(SampleGuiPluginWindow2::OnInteropMessageReceived), this );
}

bool SampleGuiPluginWindow2::ShowToolTips()
{
    return true;
}

wxBitmap SampleGuiPluginWindow2::GetBitmapResource( const wxString& /*name*/ )
{
    return wxNullBitmap;
}

wxIcon SampleGuiPluginWindow2::GetIconResource( const wxString& /*name*/ )
{
    return wxNullIcon;
}

void SampleGuiPluginWindow2::OnInteropMessageReceived(wxCommandEvent & event)
{
    m_MessageTextCtrl->SetValue(event.GetString());
}

void SampleGuiPluginWindow2::OnDestroy( wxWindowDestroyEvent& event )
{
    GetPlugin()->GetEventHandler()->Unbind( hostapi->wxEVT_GUI_PLUGIN_INTEROP, wxCommandEventHandler(SampleGuiPluginWindow2::OnInteropMessageReceived), this );
    event.Skip();
}
