#include "backend/main.h"

#include "core/color.h"

#include "backend/key.h"
#include "backend/mouse_button.h"

#include <wx/dcbuffer.h>

#include "backend/renderer.h"


namespace ride::apigl
{





struct Pane : public wxPanel
{
    Pane(wxFrame* parent, std::unique_ptr<App> aapp)
        : wxPanel(parent), app(std::move(aapp))
    {
        SetDoubleBuffered(true);

        Bind(wxEVT_PAINT, &Pane::paintEvent, this);
        Bind(wxEVT_SIZE, &Pane::OnSize, this);

        Bind(wxEVT_MOTION, &Pane::OnMouseMoved, this);
        Bind(wxEVT_LEAVE_WINDOW, &Pane::OnMouseLeftWindow, this);

        Bind(wxEVT_LEFT_DOWN, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Down, ride::MouseButton::Left);});
        Bind(wxEVT_LEFT_UP, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Up, ride::MouseButton::Left);});
        Bind(wxEVT_LEFT_DCLICK, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::DoubleClick, ride::MouseButton::Left);});

        Bind(wxEVT_MIDDLE_DOWN, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Down, ride::MouseButton::Middle);});
        Bind(wxEVT_MIDDLE_UP, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Up, ride::MouseButton::Middle);});
        Bind(wxEVT_MIDDLE_DCLICK, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::DoubleClick, ride::MouseButton::Middle);});

        Bind(wxEVT_RIGHT_DOWN, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Down, ride::MouseButton::Right);});
        Bind(wxEVT_RIGHT_UP, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Up, ride::MouseButton::Right);});
        Bind(wxEVT_RIGHT_DCLICK, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::DoubleClick, ride::MouseButton::Right);});

        Bind(wxEVT_AUX1_DOWN, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Down, ride::MouseButton::X1);});
        Bind(wxEVT_AUX1_UP, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Up, ride::MouseButton::X1);});
        Bind(wxEVT_AUX1_DCLICK, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::DoubleClick, ride::MouseButton::X1);});

        Bind(wxEVT_AUX2_DOWN, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Down, ride::MouseButton::X2);});
        Bind(wxEVT_AUX2_UP, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::Up, ride::MouseButton::X2);});
        Bind(wxEVT_AUX2_DCLICK, [this] (wxMouseEvent& e) {this->OnMouseButton(e, ride::MouseState::DoubleClick, ride::MouseButton::X2);});

        Bind(wxEVT_MOUSEWHEEL, &Pane::OnMouseWheel, this);
        Bind(wxEVT_KEY_UP, &Pane::OnKeyReleased, this);
        Bind(wxEVT_KEY_DOWN, &Pane::OnKeyPressed, this);
        Bind(wxEVT_CHAR, &Pane::OnChar, this);
    }

    std::unique_ptr<App> app;

    void paintEvent(wxPaintEvent&) {}
    void render(wxBufferedPaintDC&) {}


    void OnSize(wxSizeEvent& ) {}
    void OnMouseMoved(wxMouseEvent& ) {}
    void OnMouseLeftWindow(wxMouseEvent& ) {}
    void OnMouseWheel(wxMouseEvent& ) {}
    void OnKeyPressed(wxKeyEvent& ) {}
    void OnKeyReleased(wxKeyEvent& ) {}
    void OnChar(wxKeyEvent& ) {}
};




void Pane::paintEvent(wxPaintEvent&)
{
    wxBufferedPaintDC dc(this);
    render(dc);
}


void Pane::render(wxBufferedPaintDC& dc)
{
    auto painter = WxPainter{&dc};
    app->OnPaint(&painter);
}


void Pane::OnSize(wxSizeEvent&)
{
    int width = 0;
    int height = 0;
    GetClientSize(&width, &height);
    app->OnSize(ride::vec2{width, height});
}


void Pane::OnMouseMoved(wxMouseEvent& event)
{
    app->OnMouseMoved(ride::vec2{event.GetX(), event.GetY()});
}


void Pane::OnMouseLeftWindow(wxMouseEvent&)
{
    app->OnMouseLeftWindow();
}


void Pane::OnMouseButton(wxMouseEvent& event, ride::MouseState state, ride::MouseButton button)
{
    app->OnMouseMoved(ride::vec2{event.GetX(), event.GetY()});
    app->OnMouseButton(state, button);
}


void Pane::OnMouseWheel(wxMouseEvent& e)
{
    const float rotation = static_cast<float>(e.GetWheelRotation()) / 120.0f;
    const int lines = e.GetLinesPerAction();
    app->OnMouseScroll(rotation, lines);
}

void Pane::OnChar(wxKeyEvent& e)
{
    const auto unicode_key = e.GetUnicodeKey();
    if ( unicode_key == WXK_NONE ) { return; }

    if(handle_special)
    {
        // std::cout << "unicode " << unicode_key << "\n";
        for(const auto& [special_unicode_key, special_ride_key]: special_characters)
        {
            if(unicode_key == special_unicode_key)
            {
                app->OnKey(true, special_ride_key);
                app->OnKey(false, special_ride_key);
                return;
            }
        }
    }

    // ignore all other weird keys
    if(unicode_key < 32)
    {
        std::cout << "blocked weird character\n";
        return;
    }


    const auto wx = wxString{unicode_key};
    const auto buff = wx.utf8_str();
    const std::string str = buff.data();

    app->OnChar(str);
}


bool ApiApp::create_app(CreateAppFunction create_app)
{
    wxAppConsole* app_console = wxAppConsole::GetInstance();
    app_console->SetAppName("ride");
    app_console->SetAppDisplayName("ride");
    app_console->SetVendorName("madeso");
    app_console->SetVendorDisplayName("madeso");

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    auto* frame = new wxFrame(nullptr, -1, "app", wxPoint(50,50), wxSize(800,600));

    auto* pane = new Pane{frame, create_app(StartupArguments{})};
    sizer->Add(pane, 1, wxEXPAND);

    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);

    frame->Show(true);
    return true;
}

}

