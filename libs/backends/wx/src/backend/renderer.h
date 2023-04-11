#pragma once

#include "api/renderer.h"
#include "backend/dependency_wx.h"


struct WxRenderer : ::Renderer
{
    wxBufferedPaintDC* dc;
    
    explicit WxRenderer(wxBufferedPaintDC* adc);
    ~WxRenderer() = default;

    void draw_rect(const Rect<Px>& rect, Color color) override;
    Px draw_text(std::shared_ptr<Font> font, const std::string& text, Px x, Px y, Color color) override;
    void push_clip_rect(const Rect<Px>& r) override;
    void pop_clip_rect() override;
    void draw_image(std::shared_ptr<Texture> texture, const Rect<Px>& rect, Color tint, std::optional<Rectf> sub = {}, Submit submit=Submit::yes) override;
    void submit_renderer() override;
};

