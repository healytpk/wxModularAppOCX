#include "wx/wx.h"
#include "wxGuiPluginWindowBase.h"
#include "wxGuiPluginBase.h"

bool ForPlugins_wxGuiPluginWindowBase_ShowToolTips(void)  // static member function
{
    return true;
}

void ForPlugins_wxGuiPluginWindowBase_Init(wxGuiPluginWindowBase &obj)
{

}

void ForPlugins_wxGuiPluginWindowBase_CreateControls(wxGuiPluginWindowBase &obj)
{

}

bool ForPlugins_wxGuiPluginWindowBase_Create(wxGuiPluginWindowBase &obj, wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    obj.m_Plugin = plugin;
    obj.wxPanel::Create(parent, id, pos, size, style);
    ForPlugins_wxGuiPluginWindowBase_CreateControls(obj);
    return true;
}

void ForPlugins_wxGuiPluginWindowBase_Constructor_void(wxGuiPluginWindowBase &obj)
{
    ForPlugins_wxGuiPluginWindowBase_Init(obj);
}

void ForPlugins_wxGuiPluginWindowBase_Constructor_ManyArguments(wxGuiPluginWindowBase &obj, wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    ForPlugins_wxGuiPluginWindowBase_Init(obj);
    ForPlugins_wxGuiPluginWindowBase_Create(obj, plugin, parent, id, pos, size, style);
}

wxBitmap ForPlugins_wxGuiPluginWindowBase_GetBitmapResource(wxGuiPluginWindowBase &obj, const wxString& /*name*/ )
{
    return wxNullBitmap;
}

wxIcon ForPlugins_wxGuiPluginWindowBase_GetIconResource(wxGuiPluginWindowBase &obj, const wxString& /*name*/ )
{
    return wxNullIcon;
}
