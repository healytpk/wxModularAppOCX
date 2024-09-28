/////////////////////////////////////////////////////////////////////////////
// Name:        wxGuiPluginWindowBase.cpp
// Purpose:     
// Author:      Volodymyr (T-Rex) Triapichko
// Modified by: 
// Created:     09/09/2013 23:54:21
// RCS-ID:      
// Copyright:   Volodymyr (T-Rex) Triapichko, 2013
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxGuiPluginWindowBase.h"
#include "wxGuiPluginBase.h"

extern "C" {

DEMO_API void ForPlugins_wxGuiPluginWindowBase_Init(wxGuiPluginWindowBase &obj)
{

}

DEMO_API void ForPlugins_wxGuiPluginWindowBase_CreateControls(wxGuiPluginWindowBase &obj)
{

}

DEMO_API bool ForPlugins_wxGuiPluginWindowBase_Create(wxGuiPluginWindowBase &obj, wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    obj.m_Plugin = plugin;
    obj.wxPanel::Create(parent, id, pos, size, style);
    ForPlugins_wxGuiPluginWindowBase_CreateControls(obj);
    return true;
}

DEMO_API void ForPlugins_wxGuiPluginWindowBase_Constructor_void(wxGuiPluginWindowBase &obj)
{
    ForPlugins_wxGuiPluginWindowBase_Init(obj);
}

DEMO_API void ForPlugins_wxGuiPluginWindowBase_Constructor_ManyArguments(wxGuiPluginWindowBase &obj, wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    ForPlugins_wxGuiPluginWindowBase_Init(obj);
    ForPlugins_wxGuiPluginWindowBase_Create(obj, plugin, parent, id, pos, size, style);
}

DEMO_API bool ForPlugins_wxGuiPluginWindowBase_ShowToolTips(wxGuiPluginWindowBase &obj)
{
    return true;
}

DEMO_API wxBitmap ForPlugins_wxGuiPluginWindowBase_GetBitmapResource(wxGuiPluginWindowBase &obj, const wxString& /*name*/ )
{
    return wxNullBitmap;
}

DEMO_API wxIcon ForPlugins_wxGuiPluginWindowBase_GetIconResource(wxGuiPluginWindowBase &obj, const wxString& /*name*/ )
{
    return wxNullIcon;
}

} // extern "C"
