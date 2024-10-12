#include "wx/wx.h"

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "MainFrame.h"
#include "wxModularHostApp.h"
#include "wxModularCore.h"

////@begin XPM images
////@end XPM images


/*
 * MainFrame type definition
 */

IMPLEMENT_CLASS( MainFrame, wxFrame )


/*
 * MainFrame event table definition
 */

BEGIN_EVENT_TABLE( MainFrame, wxFrame )

////@begin MainFrame event table entries
////@end MainFrame event table entries

END_EVENT_TABLE()


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

    itemFrame1->GetAuiManager().AddPane(m_Notebook, wxAuiPaneInfo()
        .Name(_T("Pane1")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false));

    GetAuiManager().Update();

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
	wxModularCore * pluginManager = wxGetApp().GetPluginManager();
	for(wxGuiPluginBaseList::Node * node = pluginManager->GetGuiPlugins().GetFirst();
		node; node = node->GetNext())
	{
		wxGuiPluginBase * plugin = node->GetData();
		if(plugin)
		{
			wxWindow * page = plugin->CreatePanel(m_Notebook);
			if(page)
			{
#if 1
                // The following is a workaround to
                // prevent a linker error on Linux
                bool (wxAuiNotebook::*const mfp)(wxWindow*,wxString const&,bool,int) = &wxAuiNotebook::AddPage;
                (m_Notebook->*mfp)(page, plugin->GetName(), false, -1);
#else
                // The next line causes a linker error on Linux
                // because it looks for:
                //     bool wxAuiNotebook::AddPage(wxWindow*,wxString const&,bool,wxBitmapBundle const&)
                // instead of:
                //     bool wxAuiNotebook::AddPage(wxWindow*,wxString const&,bool,int)
				m_Notebook->AddPage(page, plugin->GetName());
#endif
			}
		}
	}
}
