/////////////////////////////////////////////////////////////////////////////
// Name:        SampleGuiPluginWindow2.h
// Purpose:     
// Author:      Volodymyr (T-Rex) Triapichko
// Modified by: 
// Created:     10/09/2013 00:01:49
// RCS-ID:      
// Copyright:   Volodymyr (T-Rex) Triapichko, 2013
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SAMPLEGUIPLUGINWINDOW2_H_
#define _SAMPLEGUIPLUGINWINDOW2_H_

#include <wxGuiPluginWindowBase.h>

#define ID_SAMPLEGUIPLUGINWINDOW2 10000
#define ID_GUI_PLUGIN2_MESSAGE_TEXTCTRL 10001
#define SYMBOL_SAMPLEGUIPLUGINWINDOW2_STYLE wxTAB_TRAVERSAL
#define SYMBOL_SAMPLEGUIPLUGINWINDOW2_TITLE _("SampleGuiPluginWindow2")
#define SYMBOL_SAMPLEGUIPLUGINWINDOW2_IDNAME ID_SAMPLEGUIPLUGINWINDOW2
#define SYMBOL_SAMPLEGUIPLUGINWINDOW2_SIZE wxSize(400, 300)
#define SYMBOL_SAMPLEGUIPLUGINWINDOW2_POSITION wxDefaultPosition

class SampleGuiPluginWindow2: public wxGuiPluginWindowBase {

    DECLARE_DYNAMIC_CLASS( SampleGuiPluginWindow2 )
    DECLARE_EVENT_TABLE()

    wxTextCtrl* m_MessageTextCtrl;

public:

    SampleGuiPluginWindow2();
    SampleGuiPluginWindow2(wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id = SYMBOL_SAMPLEGUIPLUGINWINDOW2_IDNAME, const wxPoint& pos = SYMBOL_SAMPLEGUIPLUGINWINDOW2_POSITION, const wxSize& size = SYMBOL_SAMPLEGUIPLUGINWINDOW2_SIZE, long style = SYMBOL_SAMPLEGUIPLUGINWINDOW2_STYLE );

    bool Create(wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id = SYMBOL_SAMPLEGUIPLUGINWINDOW2_IDNAME, const wxPoint& pos = SYMBOL_SAMPLEGUIPLUGINWINDOW2_POSITION, const wxSize& size = SYMBOL_SAMPLEGUIPLUGINWINDOW2_SIZE, long style = SYMBOL_SAMPLEGUIPLUGINWINDOW2_STYLE );

    ~SampleGuiPluginWindow2();

    void Init();

    void CreateControls();

    wxBitmap GetBitmapResource( const wxString& name );

    wxIcon GetIconResource( const wxString& name );

    void OnDestroy( wxWindowDestroyEvent& event );
    void OnInteropMessageReceived(wxCommandEvent & event);

    static bool ShowToolTips();
};

#endif
