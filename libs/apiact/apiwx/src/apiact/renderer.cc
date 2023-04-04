#include "apigl/renderer.h"





wxColor C(const Color& c)
{
    return wxColor
    {
        static_cast<wxColourBase::ChannelType>(c.r),
        static_cast<wxColourBase::ChannelType>(c.g),
        static_cast<wxColourBase::ChannelType>(c.b)
    };
}


void SetBrush(wxDC* dc, std::optional<Color> fill)
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

struct Line
{
    Color color;
    int width;
}

void SetPen(wxDC* dc, std::optional<Line> line_color)
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


WxRenderer::WxRenderer(wxBufferedPaintDC* adc)
    : dc(adc)
{
}



void WxRenderer::draw_rect(const Rect<Px>& rect, Color color)
{
// void Rect(const ride::Rect& rect, std::optional<ride::Rgb> fill, std::optional<ride::Line> line_color) override
    SetBrush(dc, color);
    // SetPen(dc, line_color);
    dc->DrawRectangle(rect.position.x, rect.position.y, rect.size.x, rect.size.y);
}

Px WxRenderer::draw_text(std::shared_ptr<Font> font, const std::string& text, Px x, Px y, Color color)
{
}

void WxRenderer::push_clip_rect(const Rect<Px>& r)
{
    dc->SetClippingRegion(rect.position.x, rect.position.y, rect.size.x, rect.size.y);
}

void WxRenderer::pop_clip_rect()
{
    dc->DestroyClippingRegion();
}

void WxRenderer::draw_image(std::shared_ptr<Texture> texture, const Rect<Px>& rect, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes)
{
}

void WxRenderer::submit_renderer()
{
}

