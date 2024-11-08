#pragma once

#include <string>           // wstring
#include <string_view>      // wstring_view
#include <wx/wx.h>
#include <wx/object.h>      // wxObject
#include <wx/window.h>      // wxWindow
#include "Declarations.h"
#include "PixelContainer.hpp"  // PixelContainer

class wxGuiPluginBase : public wxObject {
public:
	virtual ~wxGuiPluginBase() = default;

	virtual bool ShouldInsertSpacers(void) const = 0;
	virtual wxString GetName() const = 0;
	virtual wxString GetId() const = 0;
	virtual wxWindow * CreatePanel(wxWindow * parent) = 0;

	virtual wxEvtHandler * GetEventHandler() = 0;
	virtual void SetEventHandler(wxEvtHandler * handler) = 0;
};

DECLARE_EXPORTED_EVENT_TYPE(DEMO_API, wxEVT_GUI_PLUGIN_INTEROP, wxEVT_USER_FIRST + 100)

typedef wxGuiPluginBase *(*CreatePlugin_function)( void const *(*const ForPlugins_GetHostAPI)( unsigned version, void (*addr_of_wxuninit)(void) ) );
typedef void (*DeletePlugin_function)(wxGuiPluginBase*);

class wxGuiPluginNativeHandle : public wxGuiPluginBase {
public:
	typedef bool (*FuncPtr_t)(void*);

protected:
	FuncPtr_t const pfnPopulate;

public:
	virtual bool ShouldInsertSpacers(void) const override { return false; }

	wxGuiPluginNativeHandle(FuncPtr_t);
	~wxGuiPluginNativeHandle(void) override;

	virtual wxEvtHandler *GetEventHandler() override { return nullptr; }
	virtual void SetEventHandler(wxEvtHandler*) override {}

	virtual wxString GetName(void) const override { return "Name NativeHandle Plugin 0"; }
	virtual wxString GetId(void) const override   { return "ID   NativeHandle Plugin 0"; }

	virtual wxWindow *CreatePanel(wxWindow *const parent) override;
};

class wxGuiPluginPixels : public wxGuiPluginBase {
public:
	typedef bool (*Funcptr_CreatePluginPixels_t)(void);
	typedef void (*Funcptr_RenderWidgets_t     )(PixelContainer &pixelData, unsigned w, unsigned h);

protected:
	Funcptr_CreatePluginPixels_t const pfnCreatePluginPixels;
	Funcptr_RenderWidgets_t      const pfnRenderWidgets;

public:
	virtual bool ShouldInsertSpacers(void) const override { return false; }

	wxGuiPluginPixels(Funcptr_CreatePluginPixels_t, Funcptr_RenderWidgets_t);
	~wxGuiPluginPixels(void) override;

	virtual wxEvtHandler *GetEventHandler() override { return nullptr; }
	virtual void SetEventHandler(wxEvtHandler*) override {}

	virtual wxString GetName(void) const override { return "Name Pixels Plugin 0"; }
	virtual wxString GetId(void) const override   { return "ID   Pixels Plugin 0"; }

	virtual wxWindow *CreatePanel(wxWindow *const parent) override;
};

class wxGuiPluginScreenCoord : public wxGuiPluginBase {
public:
	typedef bool (*FuncPtr_t)(int,int,int,int);

protected:
	FuncPtr_t const pfnPopulate;

public:
	virtual bool ShouldInsertSpacers(void) const override { return false; }

	wxGuiPluginScreenCoord(FuncPtr_t);
	~wxGuiPluginScreenCoord(void) override;

	virtual wxEvtHandler *GetEventHandler() override { return nullptr; }
	virtual void SetEventHandler(wxEvtHandler*) override {}

	virtual wxString GetName(void) const override { return "Name ScreenCoord Plugin 0"; }
	virtual wxString GetId(void) const override   { return "ID   ScreenCoord Plugin 0"; }

	virtual wxWindow *CreatePanel(wxWindow *const parent) override;
};

#ifdef __WXMSW__

class wxGuiPluginOCX : public wxGuiPluginBase {
protected:
	void *const p_IOleObject;

public:
	virtual bool ShouldInsertSpacers(void) const override { return false; }

	wxGuiPluginOCX(void *arg_p_IOleObject);
	~wxGuiPluginOCX(void) override;

	virtual wxEvtHandler *GetEventHandler() override { return nullptr; }
	virtual void SetEventHandler(wxEvtHandler*) override {}

	virtual wxString GetName(void) const override { return "Name OCX Plugin 0"; }
	virtual wxString GetId  (void) const override { return "ID   OCX Plugin 0"; }

	virtual wxWindow *CreatePanel(wxWindow *const parent) override;
};

class ICLRMetaHost;
class ICLRRuntimeInfo;
class ICLRRuntimeHost;

class wxGuiPluginDotNet : public wxGuiPluginBase {
protected:
	std::wstring const name;
	ICLRMetaHost *const metaHost = nullptr;
	ICLRRuntimeInfo *const runtimeInfo = nullptr;
	ICLRRuntimeHost *const runtimeHost = nullptr;

public:
	virtual bool ShouldInsertSpacers(void) const override { return false; }

	wxGuiPluginDotNet(std::wstring_view,ICLRRuntimeHost*);
	~wxGuiPluginDotNet(void) override;

	virtual wxEvtHandler *GetEventHandler() override { return nullptr; }
	virtual void SetEventHandler(wxEvtHandler*) override {}

	virtual wxString GetName(void) const override { return "Name DotNet Plugin 0"; }
	virtual wxString GetId(void) const override   { return "ID   DotNet Plugin 0"; }

	virtual wxWindow *CreatePanel(wxWindow *const parent) override;
};

#endif
