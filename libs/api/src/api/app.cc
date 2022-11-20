#include "api/app.h"

#include "api/font.h"

App::App(PlatformArg p)
    : platform(p)
    , run(true)
    , client_size{Dp{0}, Dp{0}}
    , scale(platform->get_default_scale())
    , redraw_value(nullptr)
{
}

void App::redraw()
{
    assert(redraw_value);
    *redraw_value = true;
}

void App::on_exposed()
{
}

void App::on_file_dropped(const std::string&, Dp, Dp)
{
}

void App::on_resized(Dp, Dp)
{
}

void App::on_key_pressed(const Stroke&)
{
}

void App::on_key_released(const Stroke&)
{
}

void App::on_text_input(const std::string&)
{
}


void App::on_mouse_pressed(MouseButton, const Meta&, const Vec2<Dp>&, int)
{
}

void App::on_mouse_released(MouseButton, const Meta&, const Vec2<Dp>&)
{
}

void App::on_mouse_moved(const Meta&, const Vec2<Dp>&, Dp, Dp)
{
}

void App::on_mouse_wheel(int, int)
{
}

void App::on_quit()
{
    run = false;
}

void App::update()
{
}

Px App::Cpx(Dp p) const
{
    return Px{static_cast<Px::Type>(p.value * scale)};
}

Rect<Px> App::Cpx(const Rect<Dp>& p) const
{
    return Rect<Px>
    {
        Cpx(p.x),
        Cpx(p.y),
        Cpx(p.width),
        Cpx(p.height)
    };
}

Dp App::Cdp(Px p) const
{
    return Dp{static_cast<double>(p.value) / scale};
}


void App::set_scale(double d)
{
    scale = d;

    /*

    for(auto& f: loaded_fonts)
    {
        f->set_size(Cpx(f->unscaled_size));
    }

    */
}

std::shared_ptr<Font> App::load_font(const std::string_view& file, Dp size)
{
    return load_font(std::string(file), size);
}

std::shared_ptr<Font> App::load_font(const std::string& file, Dp size)
{
    auto r = platform->make_font();
    if(r->load_font(file.c_str(), Cpx(size)))
    {
        loaded_fonts.emplace_back(r);
        r->unscaled_size = size;
        return r;
    }
    else
    {
        return nullptr;
    }
}

