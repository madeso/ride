#include <iostream>

#include "wx.h"
#include <wx/dcclient.h>

#include "ride/driver.h"


wxString C(const std::string& text)
{
    return wxString::FromUTF8(text.c_str());
}


wxColor C(const ride::Rgb& c)
{
    return wxColor
    {
        static_cast<wxColourBase::ChannelType>(c.r),
        static_cast<wxColourBase::ChannelType>(c.g),
        static_cast<wxColourBase::ChannelType>(c.b)
    };
}


ride::Key ToKey(int k)
{
    switch(k)
    {
    case WXK_BACK: return ride::Key::Backspace;
    case WXK_TAB: return ride::Key::Tab;
    case WXK_RETURN: return ride::Key::Return;
    case WXK_ESCAPE: return ride::Key::Escape;
    case WXK_SPACE: return ride::Key::Space;
    case WXK_DELETE: return ride::Key::Delete;
    case WXK_START: return ride::Key::Start;
    case WXK_LBUTTON: return ride::Key::LButton;
    case WXK_RBUTTON: return ride::Key::RButton;
    case WXK_CANCEL: return ride::Key::Cancel;
    case WXK_MBUTTON: return ride::Key::MButton;
    case WXK_CLEAR: return ride::Key::Clear;
    case WXK_SHIFT: return ride::Key::Shift;
    case WXK_ALT: return ride::Key::Alt;
    case WXK_CONTROL: return ride::Key::Control;
    case WXK_MENU: return ride::Key::Menu;
    case WXK_PAUSE: return ride::Key::Pause;
    case WXK_CAPITAL: return ride::Key::Capital;
    case WXK_END: return ride::Key::End;
    case WXK_HOME: return ride::Key::Home;
    case WXK_LEFT: return ride::Key::Left;
    case WXK_UP: return ride::Key::Up;
    case WXK_RIGHT: return ride::Key::Right;
    case WXK_DOWN: return ride::Key::Down;
    case WXK_SELECT: return ride::Key::Select;
    case WXK_PRINT: return ride::Key::Print;
    case WXK_EXECUTE: return ride::Key::Execute;
    case WXK_SNAPSHOT: return ride::Key::Snapshot;
    case WXK_INSERT: return ride::Key::Insert;
    case WXK_HELP: return ride::Key::Help;
    case WXK_NUMPAD0: return ride::Key::Numpad0;
    case WXK_NUMPAD1: return ride::Key::Numpad1;
    case WXK_NUMPAD2: return ride::Key::Numpad2;
    case WXK_NUMPAD3: return ride::Key::Numpad3;
    case WXK_NUMPAD4: return ride::Key::Numpad4;
    case WXK_NUMPAD5: return ride::Key::Numpad5;
    case WXK_NUMPAD6: return ride::Key::Numpad6;
    case WXK_NUMPAD7: return ride::Key::Numpad7;
    case WXK_NUMPAD8: return ride::Key::Numpad8;
    case WXK_NUMPAD9: return ride::Key::Numpad9;
    case WXK_MULTIPLY: return ride::Key::Multiply;
    case WXK_ADD: return ride::Key::Add;
    case WXK_SEPARATOR: return ride::Key::Separator;
    case WXK_SUBTRACT: return ride::Key::Subtract;
    case WXK_DECIMAL: return ride::Key::Decimal;
    case WXK_DIVIDE: return ride::Key::Divide;
    case WXK_F1: return ride::Key::F1;
    case WXK_F2: return ride::Key::F2;
    case WXK_F3: return ride::Key::F3;
    case WXK_F4: return ride::Key::F4;
    case WXK_F5: return ride::Key::F5;
    case WXK_F6: return ride::Key::F6;
    case WXK_F7: return ride::Key::F7;
    case WXK_F8: return ride::Key::F8;
    case WXK_F9: return ride::Key::F9;
    case WXK_F10: return ride::Key::F10;
    case WXK_F11: return ride::Key::F11;
    case WXK_F12: return ride::Key::F12;
    case WXK_F13: return ride::Key::F13;
    case WXK_F14: return ride::Key::F14;
    case WXK_F15: return ride::Key::F15;
    case WXK_F16: return ride::Key::F16;
    case WXK_F17: return ride::Key::F17;
    case WXK_F18: return ride::Key::F18;
    case WXK_F19: return ride::Key::F19;
    case WXK_F20: return ride::Key::F20;
    case WXK_F21: return ride::Key::F21;
    case WXK_F22: return ride::Key::F22;
    case WXK_F23: return ride::Key::F23;
    case WXK_F24: return ride::Key::F24;
    case WXK_NUMLOCK: return ride::Key::Numlock;
    case WXK_SCROLL: return ride::Key::Scroll;
    case WXK_PAGEUP: return ride::Key::PageUp;
    case WXK_PAGEDOWN: return ride::Key::PageDown;
    case WXK_NUMPAD_SPACE: return ride::Key::NumpadSpace;
    case WXK_NUMPAD_TAB: return ride::Key::NumpadEnter;
    case WXK_NUMPAD_ENTER: return ride::Key::NumpadEnter;
    case WXK_NUMPAD_F1: return ride::Key::NumpadF1;
    case WXK_NUMPAD_F2: return ride::Key::NumpadF2;
    case WXK_NUMPAD_F3: return ride::Key::NumpadF3;
    case WXK_NUMPAD_F4: return ride::Key::NumpadF4;
    case WXK_NUMPAD_HOME: return ride::Key::NumpadHome;
    case WXK_NUMPAD_LEFT: return ride::Key::NumpadLeft;
    case WXK_NUMPAD_UP: return ride::Key::NumpadUp;
    case WXK_NUMPAD_RIGHT: return ride::Key::NumpadRight;
    case WXK_NUMPAD_DOWN: return ride::Key::NumpadDown;
    case WXK_NUMPAD_PAGEUP: return ride::Key::NumpadPageUp;
    case WXK_NUMPAD_PAGEDOWN: return ride::Key::NumpadPageDown;
    case WXK_NUMPAD_END: return ride::Key::NumpadEnd;
    case WXK_NUMPAD_BEGIN: return ride::Key::NumpadBegin;
    case WXK_NUMPAD_INSERT: return ride::Key::NumpadInsert;
    case WXK_NUMPAD_DELETE: return ride::Key::NumpadDelete;
    case WXK_NUMPAD_EQUAL: return ride::Key::NupadEqual;
    case WXK_NUMPAD_MULTIPLY: return ride::Key::NumpadMultiply;
    case WXK_NUMPAD_ADD: return ride::Key::NumpadAdd;
    case WXK_NUMPAD_SEPARATOR: return ride::Key::NumpadSeparator;
    case WXK_NUMPAD_SUBTRACT: return ride::Key::NumpadSubtract;
    case WXK_NUMPAD_DECIMAL: return ride::Key::NumpadDecimal;
    case WXK_NUMPAD_DIVIDE: return ride::Key::NumpadDivide;
    case WXK_WINDOWS_LEFT: return ride::Key::WindowLeft;
    case WXK_WINDOWS_RIGHT: return ride::Key::WindowsRight;
    case WXK_WINDOWS_MENU: return ride::Key::WindowsMenu;
    case WXK_SPECIAL1: return ride::Key::Special1;
    case WXK_SPECIAL2: return ride::Key::Special2;
    case WXK_SPECIAL3: return ride::Key::Special3;
    case WXK_SPECIAL4: return ride::Key::Special4;
    case WXK_SPECIAL5: return ride::Key::Special5;
    case WXK_SPECIAL6: return ride::Key::Special6;
    case WXK_SPECIAL7: return ride::Key::Special7;
    case WXK_SPECIAL8: return ride::Key::Special8;
    case WXK_SPECIAL9: return ride::Key::Special9;
    case WXK_SPECIAL10: return ride::Key::Special10;
    case WXK_SPECIAL11: return ride::Key::Special11;
    case WXK_SPECIAL12: return ride::Key::Special12;
    case WXK_SPECIAL13: return ride::Key::Special13;
    case WXK_SPECIAL14: return ride::Key::Special14;
    case WXK_SPECIAL15: return ride::Key::Special15;
    case WXK_SPECIAL16: return ride::Key::Special16;
    case WXK_SPECIAL17: return ride::Key::Special17;
    case WXK_SPECIAL18: return ride::Key::Special18;
    case WXK_SPECIAL19: return ride::Key::Special19;
    case WXK_SPECIAL20: return ride::Key::Special20;
    default:
        return ride::Key::None;
    }
}


constexpr char* DISPLAY_NAME = "Ride";


struct MyApp : public wxApp
{
    bool OnInit() override;
};


wxIMPLEMENT_APP(MyApp);



struct Pane : public wxPanel
{
    Pane(wxFrame* parent);

    std::shared_ptr<ride::App> app;
    
    void paintEvent(wxPaintEvent & evt);
    
    void render(wxDC& dc);


    void OnSize(wxSizeEvent& e);
    void OnMouseMoved(wxMouseEvent& event);
    void OnMouseLeftWindow(wxMouseEvent& event);
    void OnMouseButton(ride::MouseState state, ride::MouseButton button);
    void OnMouseWheel(wxMouseEvent& event);
    
    void OnKeyPressed(wxKeyEvent& event);
    void OnKeyReleased(wxKeyEvent& event);
    void OnChar(wxKeyEvent& event);
};


bool MyApp::OnInit()
{
    wxAppConsole* app_console = wxAppConsole::GetInstance();
    app_console->SetAppName("ride");
    app_console->SetAppDisplayName(DISPLAY_NAME);
    app_console->SetVendorName("madeso");
    app_console->SetVendorDisplayName("madeso");

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    auto* frame = new wxFrame((wxFrame *)NULL, -1,  DISPLAY_NAME, wxPoint(50,50), wxSize(800,600));
	
    auto* pane = new Pane( (wxFrame*) frame );
    sizer->Add(pane, 1, wxEXPAND);
	
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
	
    frame->Show(true);
    return true;
}


struct WxFont : ride::Font
{
    wxFont font;

    WxFont(wxFont f) : font(f) {}
};


wxFont C(std::shared_ptr<ride::Font> font)
{
    auto* wx_font = static_cast<WxFont*>(font.get());
    return wx_font->font;
}


struct WxDriver : ride::Driver
{
    Pane* pane;

    WxDriver(Pane* p)
        : pane(p)
    {
    }

    void Refresh() override
    {
        pane->Refresh();
    }

    std::shared_ptr<ride::Font> MakeFont(wxFont font)
    {
        return std::make_shared<WxFont>(font);
    }

    ride::TextSize GetSizeOfString(std::shared_ptr<ride::Font> font, const std::string& str) override
    {
        int width = 0;
        int height = 0;
        int descent = 0;
        int external_leading = 0;

        auto dc = wxClientDC{pane};
        auto wx_font = C(font);
        dc.GetTextExtent(C(str), &width, &height, &descent, &external_leading, &wx_font);

        return {width, height, descent, external_leading};
    }

    std::shared_ptr<ride::Font> CreateCodeFont(int pixel_size) override
    {
        return MakeFont
        (
            wxFont
            {
                wxFontInfo{pixel_size}
                .Family(wxFONTFAMILY_TELETYPE)
            }
        );
    }

    std::shared_ptr<ride::Font> CreateUiFont(int pixel_size) override
    {
        return MakeFont
        (
            wxFont
            {
                wxFontInfo{pixel_size}
                .Family(wxFONTFAMILY_ROMAN)
            }
        );
    }
};


void SetBrush(wxDC* dc, std::optional<ride::Rgb> fill)
{
    if(fill)
    {
        wxBrush brush(C(*fill));
        dc->SetBrush(brush);
    }
    else
    {
        dc->SetBrush(wxNullBrush);
    }
}


void SetPen(wxDC* dc, std::optional<ride::Line> line_color)
{
    if(line_color)
    {
        dc->SetPen( wxPen( C(line_color->color), line_color->width ) );
    }
    else
    {
        dc->SetPen(wxNullPen);
    }
}


struct WxPainter : public ride::Painter
{
    wxDC* dc;

    WxPainter(wxDC* w)
         : dc(w)
    {
    }

    void Rect(const ride::vec2& point, const ride::vec2& size, std::optional<ride::Rgb> fill, std::optional<ride::Line> line_color) override
    {
        SetBrush(dc, fill);
        SetPen(dc, line_color);
        dc->DrawRectangle(point.x, point.y, size.x, size.y);
    }

    void Circle(const ride::vec2& point, int radius, std::optional<ride::Rgb> fill, std::optional<ride::Line> line_color) override
    {
        SetBrush(dc, fill);
        SetPen(dc, line_color);
        dc->DrawCircle(point.x, point.y, radius);
    }

    void Line(const ride::vec2& from, const ride::vec2& to, const ride::Line& line) override
    {
        dc->SetPen(wxPen(C(line.color), line.width));
        dc->DrawLine(from.x, from.y, to.x, to.y);
    }

    void Text(std::shared_ptr<ride::Font> font, const std::string& text, const ride::vec2& where, const ride::Rgb& color) override
    {
        const auto str = C(text);
        dc->SetTextForeground(wxColor(C(color)));
        dc->SetFont(C(font));
        dc->DrawText(str, where.x, where.y);
        dc->SetFont(wxNullFont);

        // todo(Gustav): change api so we don't have to clear font for several lines
    }
};


Pane::Pane(wxFrame* parent)
    : wxPanel(parent)
{
    app = ride::CreateApp(std::make_shared<WxDriver>(this));

    Bind(wxEVT_PAINT, &Pane::paintEvent, this);
    Bind(wxEVT_SIZE, &Pane::OnSize, this);

    Bind(wxEVT_MOTION, &Pane::OnMouseMoved, this);
    Bind(wxEVT_LEAVE_WINDOW, &Pane::OnMouseLeftWindow, this);

    Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Down, ride::MouseButton::Left);});
    Bind(wxEVT_LEFT_UP, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Up, ride::MouseButton::Left);});
    Bind(wxEVT_LEFT_DCLICK, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::DoubleClick, ride::MouseButton::Left);});

    Bind(wxEVT_MIDDLE_DOWN, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Down, ride::MouseButton::Middle);});
    Bind(wxEVT_MIDDLE_UP, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Up, ride::MouseButton::Middle);});
    Bind(wxEVT_MIDDLE_DCLICK, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::DoubleClick, ride::MouseButton::Middle);});

    Bind(wxEVT_RIGHT_DOWN, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Down, ride::MouseButton::Right);});
    Bind(wxEVT_RIGHT_UP, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Up, ride::MouseButton::Right);});
    Bind(wxEVT_RIGHT_DCLICK, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::DoubleClick, ride::MouseButton::Right);});

    Bind(wxEVT_AUX1_DOWN, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Down, ride::MouseButton::X1);});
    Bind(wxEVT_AUX1_UP, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Up, ride::MouseButton::X1);});
    Bind(wxEVT_AUX1_DCLICK, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::DoubleClick, ride::MouseButton::X1);});

    Bind(wxEVT_AUX2_DOWN, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Down, ride::MouseButton::X2);});
    Bind(wxEVT_AUX2_UP, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::Up, ride::MouseButton::X2);});
    Bind(wxEVT_AUX2_DCLICK, [this](wxMouseEvent&) {this->OnMouseButton(ride::MouseState::DoubleClick, ride::MouseButton::X2);});

    Bind(wxEVT_MOUSEWHEEL, &Pane::OnMouseWheel, this);
    Bind(wxEVT_KEY_UP, &Pane::OnKeyReleased, this);
    Bind(wxEVT_KEY_DOWN, &Pane::OnKeyPressed, this);
    Bind(wxEVT_CHAR, &Pane::OnChar, this);
}


void Pane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}


void Pane::render(wxDC& dc)
{
    auto painter = WxPainter{&dc};
    app->OnPaint(&painter);
}


void Pane::OnSize(wxSizeEvent& e)
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


void Pane::OnMouseLeftWindow(wxMouseEvent& event)
{
    app->OnMouseLeftWindow();
}


void Pane::OnMouseButton(ride::MouseState state, ride::MouseButton button)
{
    app->OnMouseButton(state, button);
}


void Pane::OnMouseWheel(wxMouseEvent& e)
{
    const float rotation = e.GetWheelRotation() / 120.0f;
    const int lines = e.GetLinesPerAction();
    app->OnMouseScroll(rotation, lines);
}


void Pane::OnKeyPressed(wxKeyEvent& e)
{
    const auto key = ToKey(e.GetKeyCode());
    if(key == ride::Key::None) { e.Skip(); return; }
    const auto handled = app->OnKey(true, key);
    if(!handled)
    {
        e.Skip();
    }
}


void Pane::OnKeyReleased(wxKeyEvent& e)
{
    const auto key = ToKey(e.GetKeyCode());
    if(key == ride::Key::None) { e.Skip(); return; }
    app->OnKey(false, key);
    const auto handled = app->OnKey(false, key);
    if(!handled)
    {
        e.Skip();
    }
}

void Pane::OnChar(wxKeyEvent& e)
{
    const auto unicode_key = e.GetUnicodeKey();
    if ( unicode_key == WXK_NONE ) { return; }

    const auto wx = wxString{unicode_key};
    const auto buff = wx.utf8_str();
    const std::string str = buff.data();

    app->OnChar(str);
}
