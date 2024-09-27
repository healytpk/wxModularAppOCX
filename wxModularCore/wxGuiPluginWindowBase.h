/////////////////////////////////////////////////////////////////////////////
// Name:        wxGuiPluginWindowBase.h
// Purpose:     
// Author:      Volodymyr (T-Rex) Triapichko
// Modified by: 
// Created:     09/09/2013 23:54:21
// RCS-ID:      
// Copyright:   Volodymyr (T-Rex) Triapichko, 2013
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXGUIPLUGINWINDOWBASE_H_
#define _WXGUIPLUGINWINDOWBASE_H_

#include "host_interaction.hpp"
#include "Declarations.h"

class wxGuiPluginBase;

#define ID_WXGUIPLUGINWINDOWBASE 10000
#define SYMBOL_WXGUIPLUGINWINDOWBASE_STYLE wxNO_BORDER|wxTAB_TRAVERSAL
#define SYMBOL_WXGUIPLUGINWINDOWBASE_IDNAME ID_WXGUIPLUGINWINDOWBASE
#define SYMBOL_WXGUIPLUGINWINDOWBASE_SIZE wxDefaultSize
#define SYMBOL_WXGUIPLUGINWINDOWBASE_POSITION wxDefaultPosition

class wxGuiPluginWindowBase : public wxPanel {
protected:
    wxGuiPluginBase * m_Plugin;
public:
    wxGuiPluginBase * GetPlugin() const { return m_Plugin ; }
    void SetPlugin(wxGuiPluginBase * value) { m_Plugin = value ; }

    wxGuiPluginWindowBase()
    {
        hostfuncs.wxGuiPluginWindowBase_Constructor_void(*this);
    }

    wxGuiPluginWindowBase(wxGuiPluginBase * plugin, 
        wxWindow* parent, 
        wxWindowID id = ID_WXGUIPLUGINWINDOWBASE, 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, 
        long style = wxNO_BORDER|wxTAB_TRAVERSAL)
    {
        hostfuncs.wxGuiPluginWindowBase_Constructor_ManyArguments(*this, plugin, parent, id, pos, size, style);
    }

    bool Create(wxGuiPluginBase * plugin, 
        wxWindow* parent, 
        wxWindowID id = ID_WXGUIPLUGINWINDOWBASE, 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, 
        long style = wxNO_BORDER|wxTAB_TRAVERSAL)
    {
        return hostfuncs.wxGuiPluginWindowBase_Create(*this, plugin, parent, id, pos, size, style);
    }

    void Init()
    {
        return hostfuncs.wxGuiPluginWindowBase_Init(*this);
    }

    void CreateControls()
    {
        return hostfuncs.wxGuiPluginWindowBase_CreateControls(*this);
    }

    wxBitmap GetBitmapResource(const wxString& name)
    {
        return hostfuncs.wxGuiPluginWindowBase_GetBitmapResource(*this, name);
    }

    wxIcon GetIconResource(const wxString& name)
    {
        return hostfuncs.wxGuiPluginWindowBase_GetIconResource(*this, name);
    }

    static bool ShowToolTips()
    {
        return hostfuncs.wxGuiPluginWindowBase_ShowToolTips();
    }
};

#endif
