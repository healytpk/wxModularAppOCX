#ifndef _WXGUIPLUGINWINDOWBASE_H_
#define _WXGUIPLUGINWINDOWBASE_H_

#include <wx/bitmap.h>
#include <wx/panel.h>
#include <wx/icon.h>

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

    friend bool ForPlugins_wxGuiPluginWindowBase_Create(wxGuiPluginWindowBase &obj, wxGuiPluginBase * plugin, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);

    wxGuiPluginBase * GetPlugin() const { return m_Plugin ; }
    void SetPlugin(wxGuiPluginBase * value) { m_Plugin = value ; }

    wxGuiPluginWindowBase()
    {
        hostapi->wxGuiPluginWindowBase_Constructor_void(*this);
    }

    wxGuiPluginWindowBase(wxGuiPluginBase * plugin, 
        wxWindow* parent, 
        wxWindowID id = ID_WXGUIPLUGINWINDOWBASE, 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, 
        long style = wxNO_BORDER|wxTAB_TRAVERSAL)
    {
        hostapi->wxGuiPluginWindowBase_Constructor_ManyArguments(*this, plugin, parent, id, pos, size, style);
    }

    bool Create(wxGuiPluginBase * plugin, 
        wxWindow* parent, 
        wxWindowID id = ID_WXGUIPLUGINWINDOWBASE, 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, 
        long style = wxNO_BORDER|wxTAB_TRAVERSAL)
    {
        return hostapi->wxGuiPluginWindowBase_Create(*this, plugin, parent, id, pos, size, style);
    }

    void Init()
    {
        return hostapi->wxGuiPluginWindowBase_Init(*this);
    }

    void CreateControls()
    {
        return hostapi->wxGuiPluginWindowBase_CreateControls(*this);
    }

    wxBitmap GetBitmapResource(const wxString& name)
    {
        return hostapi->wxGuiPluginWindowBase_GetBitmapResource(*this, name);
    }

    wxIcon GetIconResource(const wxString& name)
    {
        return hostapi->wxGuiPluginWindowBase_GetIconResource(*this, name);
    }

    static bool ShowToolTips()
    {
        return hostapi->wxGuiPluginWindowBase_ShowToolTips();
    }
};

#endif
