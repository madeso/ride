#include "wx.h"

#include "ride/driver.h"


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
    
    // some useful events
    /*
     
     void rightClick(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
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

/*
BEGIN_EVENT_TABLE(Pane, wxPanel)
    // some useful events
    // EVT_LEFT_DOWN(Pane::mouseDown)
    // EVT_LEFT_UP(Pane::mouseReleased)
    // EVT_RIGHT_DOWN(Pane::rightClick)
    // EVT_KEY_DOWN(Pane::keyPressed)
    // EVT_KEY_UP(Pane::keyReleased)
    // EVT_MOUSEWHEEL(Pane::mouseWheelMoved)

    // catch paint events
    EVT_PAINT(Pane::paintEvent)
END_EVENT_TABLE()
*/


// some useful events
/*
 
 void Pane::mouseWheelMoved(wxMouseEvent& event) {}

 void Pane::rightClick(wxMouseEvent& event) {}

 void Pane::keyPressed(wxKeyEvent& event) {}
 void Pane::keyReleased(wxKeyEvent& event) {}
 */

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
};


wxColor C(const ride::Rgb& c)
{
    return wxColor
    {
        static_cast<wxColourBase::ChannelType>(c.r),
        static_cast<wxColourBase::ChannelType>(c.g),
        static_cast<wxColourBase::ChannelType>(c.b)
    };
}


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

    void Text(const std::string& text, const ride::vec2& where, const ride::Rgb& color) override
    {
        dc->SetTextForeground(wxColor(C(color)));
        dc->DrawText(text.c_str(), where.x, where.y);
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
