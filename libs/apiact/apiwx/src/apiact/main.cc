#include "apiact/main.h"

#include "apiact/key.h"
#include "apiact/mouse_button.h"

#include <wx/dcbuffer.h>


namespace ride::apigl
{


struct Pane : public wxPanel
{
    Pane(wxFrame* parent, std::unique_ptr<App> app);

    std::unique_ptr<App> app;

    void paintEvent(wxPaintEvent & evt);

    void render(wxBufferedPaintDC& dc);


    void OnSize(wxSizeEvent& e);
    void OnMouseMoved(wxMouseEvent& event);
    void OnMouseLeftWindow(wxMouseEvent& event);
    void OnMouseWheel(wxMouseEvent& event);

    void OnKeyPressed(wxKeyEvent& event);
    void OnKeyReleased(wxKeyEvent& event);
    void OnChar(wxKeyEvent& event);
};


bool ApiApp::create_app(CreateAppFunction create_app)
{
    wxAppConsole* app_console = wxAppConsole::GetInstance();
    app_console->SetAppName("ride");
    app_console->SetAppDisplayName("ride");
    app_console->SetVendorName("madeso");
    app_console->SetVendorDisplayName("madeso");

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    auto* frame = new wxFrame(nullptr, -1, "app", wxPoint(50,50), wxSize(800,600));

    auto* pane = new Pane{frame, create_app()};
    sizer->Add(pane, 1, wxEXPAND);

    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);

    frame->Show(true);
    return true;
}

}

