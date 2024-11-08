#include "wxGuiPluginBase.h"
#include <wx/rawbmp.h>                  // wxNativePixelData

#ifdef __WXGTK__
#   include <dlfcn.h>                   // dlopen, dlsym
#endif

/* Just for timing the execution of code:
#include <iostream>
#include <chrono>
*/

DEFINE_EVENT_TYPE(wxEVT_GUI_PLUGIN_INTEROP)

wxGuiPluginNativeHandle::wxGuiPluginNativeHandle(FuncPtr_t const arg) : pfnPopulate(arg)
{
    assert( nullptr != arg );
}

wxGuiPluginNativeHandle::~wxGuiPluginNativeHandle(void)
{
}

wxWindow *wxGuiPluginNativeHandle::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent );
    wxPanel *const mypanel = new(std::nothrow) wxPanel(parent, wxID_ANY);
    if ( nullptr == mypanel ) return nullptr;
    bool retval = false;
    try
    {
        void *handle = (void*)mypanel->GetHandle();
        assert( nullptr != handle );
#ifdef __WXGTK__
        // extern "C" void *gtk_widget_get_window (void *widget  ); // GdkWindow (*)(GtkWidget*  )
        // extern "C" void *gdk_x11_window_get_xid(void *drawable); // XID       (*)(GdkDrawable*)

        void *const prog = dlopen(nullptr, RTLD_LAZY);
        assert( nullptr != prog );

        auto const pfnGtk_GetWindow = (void *(*)(void*))dlsym(prog,"gtk_widget_get_window" );
        auto const pfnGdk_GetXID    = (void *(*)(void*))dlsym(prog,"gdk_x11_window_get_xid");
        assert( nullptr != pfnGtk_GetWindow );
        assert( nullptr != pfnGdk_GetXID    );

        void *const pGdkWindow = pfnGtk_GetWindow(handle);
        assert( nullptr != pGdkWindow );
        void *const myXID = pfnGdk_GetXID(pGdkWindow);
        assert( nullptr != myXID );
        handle = myXID;
#endif
        retval = pfnPopulate(handle);
    }
    catch(...){ retval = false; }
    if ( retval ) return mypanel;
    delete mypanel;
    return nullptr;
}

wxGuiPluginPixels::wxGuiPluginPixels(Funcptr_CreatePluginPixels_t const argCreate, Funcptr_RenderWidgets_t const argRender)
  : pfnCreatePluginPixels(argCreate), pfnRenderWidgets(argRender)
{
    assert( nullptr != argCreate );
    assert( nullptr != argRender );
}

wxGuiPluginPixels::~wxGuiPluginPixels(void)
{
}

class PanelForPixels : public wxPanel {
protected:
    wxGuiPluginPixels::Funcptr_RenderWidgets_t const pfnRender;
    PixelContainer pixels;

public:
    PanelForPixels(wxWindow *const parent, wxGuiPluginPixels::Funcptr_RenderWidgets_t const f)
        : wxPanel(parent), pfnRender(f)
    {
        this->Bind(wxEVT_PAINT, &PanelForPixels::OnPaint, this);
    }

protected:
    void OnPaint(wxPaintEvent& event)
    {
        //auto const start = high_resolution_clock::now();

        wxPaintDC dc(this);
        unsigned const w = dc.GetSize().GetWidth ();
        unsigned const h = dc.GetSize().GetHeight();

        wxColour const background = this->GetBackgroundColour();

        //auto const start_qt = high_resolution_clock::now();
        pfnRender( this->pixels, w, h );  // Invoke Qt plugin to render widgets
        //auto const stop_qt = high_resolution_clock::now();

        if ( (w != this->pixels.width()) || (h != this->pixels.height()) )
        {
            dc.SetBackground(background);
            dc.Clear();
            return;
        }

        wxBitmap bmp(w, h, 24);      // Create a bitmap that matches the panel size
        typedef wxNativePixelData PixelData;
        PixelData data(bmp);
        PixelData::Iterator it(data);
        it.Offset(data, 0, 0);

        for ( unsigned y = 0u; y < h; ++y )
        {
            PixelData::Iterator rowStart = it;

            for ( unsigned x = 0u; x < w; ++x, ++it )
            {
                unsigned const pixel = pixels[x][y];
                long unsigned const transparency = (pixel >> 24u) & 0xFF;
                it.Red  () = (((pixel >> 16u) & 0xFF) * transparency + background.Red  () * (0xFF - transparency)) / 0xFFlu;
                it.Green() = (((pixel >>  8u) & 0xFF) * transparency + background.Green() * (0xFF - transparency)) / 0xFFlu;
                it.Blue () = (((pixel >>  0u) & 0xFF) * transparency + background.Blue () * (0xFF - transparency)) / 0xFFlu;
            }

            it = rowStart;
            it.OffsetY(data, 1);
        }

        dc.DrawBitmap(bmp, 0, 0);

        /*
        auto const stop = high_resolution_clock::now();
        auto const duration    = duration_cast<milliseconds>(stop    - start   );
        auto const duration_qt = duration_cast<milliseconds>(stop_qt - start_qt);
        std::cout << "pfnRender took " << duration_qt.count() << " milliseconds.\n";
        std::cout << "Entire paint took " << duration.count() << " milliseconds.\n";;
        */
    }
};

wxWindow *wxGuiPluginPixels::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent );
    if ( false == pfnCreatePluginPixels() ) return nullptr;
    return new(std::nothrow) PanelForPixels(parent, pfnRenderWidgets);
}

wxGuiPluginScreenCoord::wxGuiPluginScreenCoord(FuncPtr_t const arg) : pfnPopulate(arg)
{
    assert( nullptr != arg );
}

wxGuiPluginScreenCoord::~wxGuiPluginScreenCoord(void)
{
}

wxWindow *wxGuiPluginScreenCoord::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent );
    wxPanel *const mypanel = new(std::nothrow) wxPanel(parent, wxID_ANY);
    if ( nullptr == mypanel ) return nullptr;
    bool retval = false;
    try
    {
        auto const rect = parent->GetScreenRect();
        retval = pfnPopulate( rect.x, rect.y, rect.width, rect.height );
    }
    catch(...){ retval = false; }
    if ( retval ) return mypanel;
    delete mypanel;
    return nullptr;
}

#ifdef __WXMSW__

#include <cassert>       // assert
#include <cstdint>       // int32_t, uintptr_t
#include <string>        // to_wstring
#include <new>           // new(nothrow)
#include <metahost.h>    // ICLRMetaHost, ICLRRuntimeInfo, ICLRRuntimeHost
#include <wx/dynlib.h>   // wxDynamicLibrary
#include <wx/panel.h>    // wxPanel
#include <wx/string.h>   // wxString
#include <wx/window.h>   // wxWindow
#include "Auto.h"        // Auto

extern void OCX_Release_IOleObject(void *const p_IOleObject);

// I don't know why the Microsoft VC++ compiler requires
// the following function to have C linkage
extern "C" std::int32_t DynamicallyLoaded_AtlAxAttachControl(void *arg0, void *arg1);

wxGuiPluginOCX::wxGuiPluginOCX(void *const arg_p_IOleObject) : p_IOleObject(arg_p_IOleObject)
{
    assert( nullptr != arg_p_IOleObject );
}

wxGuiPluginOCX::~wxGuiPluginOCX(void)
{
    assert( nullptr != this->p_IOleObject );
    OCX_Release_IOleObject(this->p_IOleObject);
}

wxWindow *wxGuiPluginOCX::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent             );
    assert( nullptr != this->p_IOleObject );
    wxPanel *const mypanel = new(std::nothrow) wxPanel(parent, wxID_ANY);
    if ( nullptr == mypanel ) return nullptr;
    std::int32_t retval = 666;
    try { retval = DynamicallyLoaded_AtlAxAttachControl( this->p_IOleObject, mypanel->GetHandle() ); }
    catch(...){ retval = 666; }
    if ( S_OK == retval ) return mypanel;
    delete mypanel;
    return nullptr;
}

wxGuiPluginDotNet::wxGuiPluginDotNet(std::wstring_view const arg_name, ICLRRuntimeHost *const arg_runtimeHost)
  : name(arg_name), runtimeHost(arg_runtimeHost)
{
    assert( false   == name.empty());
    assert( nullptr != runtimeHost );
}

wxGuiPluginDotNet::~wxGuiPluginDotNet(void)
{
    if ( runtimeHost ) runtimeHost->Release();
}

wxWindow *wxGuiPluginDotNet::CreatePanel(wxWindow *const parent)
{
    assert( nullptr != parent );
    wxPanel *const mypanel = new(std::nothrow) wxPanel(parent, wxID_ANY);
    if ( nullptr == mypanel ) return nullptr;
    bool success = false;
    try
    {
        DWORD retval_from_DotNet_method = 0u;

        HRESULT const res = runtimeHost->ExecuteInDefaultAppDomain(
            (name + L".dll"   ).c_str(),
            (name + L".Plugin").c_str(),
            L"PopulatePanelNativeHandle",
            std::to_wstring( (std::uintptr_t)mypanel->GetHandle() ).c_str(),
            &retval_from_DotNet_method);

        if ( (S_OK == res) && (667 == retval_from_DotNet_method) ) success = true;
    }
    catch(...){ success = false; }
    if ( success ) return mypanel;
    delete mypanel;
    return nullptr;
}

#endif  // ifdef __WXMSW__
