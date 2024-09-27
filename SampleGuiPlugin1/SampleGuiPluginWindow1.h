/////////////////////////////////////////////////////////////////////////////
// Name:        SampleGuiPluginWindow1.h
// Purpose:     
// Author:      Volodymyr (T-Rex) Triapichko
// Modified by: 
// Created:     10/09/2013 00:01:49
// RCS-ID:      
// Copyright:   Volodymyr (T-Rex) Triapichko, 2013
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SAMPLEGUIPLUGINWINDOW1_H_
#define _SAMPLEGUIPLUGINWINDOW1_H_

#include <wxGuiPluginWindowBase.h>

#define ID_SAMPLEGUIPLUGINWINDOW1 10000
#define ID_SAMPLE_TEXTCTRL 10001
#define ID_SEND_EVENT_BUTTON 10002
#define SYMBOL_SAMPLEGUIPLUGINWINDOW1_STYLE wxTAB_TRAVERSAL
#define SYMBOL_SAMPLEGUIPLUGINWINDOW1_TITLE _("SampleGuiPluginWindow1")
#define SYMBOL_SAMPLEGUIPLUGINWINDOW1_IDNAME ID_SAMPLEGUIPLUGINWINDOW1
#define SYMBOL_SAMPLEGUIPLUGINWINDOW1_SIZE wxSize(400, 300)
#define SYMBOL_SAMPLEGUIPLUGINWINDOW1_POSITION wxDefaultPosition

class SampleGuiPluginWindow1 : public wxGuiPluginWindowBase {

    DECLARE_DYNAMIC_CLASS( SampleGuiPluginWindow1 )
    DECLARE_EVENT_TABLE()

public:

    SampleGuiPluginWindow1();
    SampleGuiPluginWindow1(wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id = SYMBOL_SAMPLEGUIPLUGINWINDOW1_IDNAME, const wxPoint& pos = SYMBOL_SAMPLEGUIPLUGINWINDOW1_POSITION, const wxSize& size = SYMBOL_SAMPLEGUIPLUGINWINDOW1_SIZE, long style = SYMBOL_SAMPLEGUIPLUGINWINDOW1_STYLE );

    bool Create(wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id = SYMBOL_SAMPLEGUIPLUGINWINDOW1_IDNAME, const wxPoint& pos = SYMBOL_SAMPLEGUIPLUGINWINDOW1_POSITION, const wxSize& size = SYMBOL_SAMPLEGUIPLUGINWINDOW1_SIZE, long style = SYMBOL_SAMPLEGUIPLUGINWINDOW1_STYLE );

    ~SampleGuiPluginWindow1();

    void Init();

    void CreateControls();

    void OnSENDEVENTBUTTONClick( wxCommandEvent& event );

    wxBitmap GetBitmapResource( const wxString& name );

    wxIcon GetIconResource( const wxString& name );

    static bool ShowToolTips();

    wxTextCtrl* m_SamppleTextCtrl;
};

#endif
