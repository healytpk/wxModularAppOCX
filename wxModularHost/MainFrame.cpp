#include <cassert>
#include "wx/wx.h"

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "MainFrame.h"
#include "wxModularHostApp.h"
#include "wxModularCore.h"
#include "Auto.h"

////@begin XPM images
////@end XPM images

/*
 * MainFrame type definition
 */

IMPLEMENT_CLASS( MainFrame, wxFrame )

/*
 * MainFrame constructors
 */

MainFrame::MainFrame()
{
    Init();
}

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * MainFrame creator
 */

bool MainFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin MainFrame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end MainFrame creation
    return true;
}


/*
 * MainFrame destructor
 */

MainFrame::~MainFrame()
{
////@begin MainFrame destruction
    GetAuiManager().UnInit();
////@end MainFrame destruction
}


/*
 * Member initialisation
 */

void MainFrame::Init()
{
////@begin MainFrame member initialisation
    m_Notebook = NULL;
////@end MainFrame member initialisation
}


/*
 * Control creation for MainFrame
 */

void MainFrame::CreateControls()
{    
////@begin MainFrame content construction
    MainFrame* itemFrame1 = this;

    GetAuiManager().SetManagedWindow(this);

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu4 = new wxMenu;
    itemMenu4->Append(wxID_EXIT, _("Exit\tAlt+F4"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu4, _("File"));
    itemFrame1->SetMenuBar(menuBar);

    wxStatusBar* itemStatusBar2 = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar2->SetFieldsCount(2);
    itemFrame1->SetStatusBar(itemStatusBar2);

    m_Notebook = new wxAuiNotebook( itemFrame1, ID_AUINOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE|wxAUI_NB_TOP|wxNO_BORDER );

    m_Notebook->Connect( wxEVT_AUINOTEBOOK_PAGE_CHANGED , wxAuiNotebookEventHandler( MainFrame::OnNotebook_Page_Changed  ), NULL, this );
    m_Notebook->Connect( wxEVT_AUINOTEBOOK_PAGE_CHANGING, wxAuiNotebookEventHandler( MainFrame::OnNotebook_Page_Changing ), NULL, this );

    itemFrame1->GetAuiManager().AddPane(m_Notebook, wxAuiPaneInfo()
        .Name(_T("Pane1")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    GetAuiManager().Update();

    auto const panel  = new wxPanel(m_Notebook);
    auto const label  = new wxStaticText(panel, wxID_STATIC, _("No plugins loaded until you click their tab"));
    auto const bsizer = new wxBoxSizer(wxVERTICAL);

    bsizer->AddStretchSpacer(1);
    bsizer->Add(label, 1, wxALIGN_CENTER_HORIZONTAL);
    bsizer->AddStretchSpacer(1);

    panel->SetSizer(bsizer);
    bsizer->Fit(panel);
    panel->Layout();

    m_Notebook->AddPage(panel, _("Home"));

////@end MainFrame content construction
	AddPagesFromGuiPlugins();

}


/*
 * Should we show tooltips?
 */

bool MainFrame::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MainFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin MainFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end MainFrame bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MainFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin MainFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end MainFrame icon retrieval
}

void MainFrame::AddPagesFromGuiPlugins()
{
	wxModularCore *const pluginManager = wxGetApp().GetPluginManager();
	for ( auto filename : pluginManager->GetGuiPluginFilenames() )
	{
        if ( filename.IsEmpty() ) continue;
        wxWindow *const page = new TabWindowForPlugin(pluginManager, m_Notebook, filename);
        auto const slash = wxFileName::GetPathSeparator();
        std::size_t const pos = filename.find_last_of(slash);
        if ( (-1 != pos) && (slash != filename.Last()) ) filename.erase(0u, pos + 1u);
        m_Notebook->AddPage(page, filename, false, -1);
	}
}

void MainFrame::OnNotebook_Page_Changing( wxAuiNotebookEvent& event )
{
    event.Skip();
}

void MainFrame::OnNotebook_Page_Changed( wxAuiNotebookEvent& event )
{
    auto *const p = dynamic_cast<TabWindowForPlugin*>( this->m_Notebook->GetPage(event.GetSelection()) );
    if ( p ) p->ShowPluginWidgets();
}
