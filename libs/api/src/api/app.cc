#include "api/app.h"

#include "api/font.h"

App::App(PlatformArg p)
    : platform(p)
    , run(true)
    , client_size{pix{0}, pix{0}}
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

void App::on_file_dropped(const std::string&, pix, pix)
{
}

void App::on_resized(pix, pix)
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


void App::on_mouse_pressed(MouseButton, const Meta&, const Vec2<pix>&, int)
{
}

void App::on_mouse_released(MouseButton, const Meta&, const Vec2<pix>&)
{
}

void App::on_mouse_moved(const Meta&, const Vec2<pix>&, pix, pix)
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

dip App::to_dip(pix p) const
{
    return dip{static_cast<dip::Type>(p.value * scale)};
}

Rect<dip> App::to_dip(const Rect<pix>& p) const
{
    return Rect<dip>
    {
        to_dip(p.x),
        to_dip(p.y),
        to_dip(p.width),
        to_dip(p.height)
    };
}

pix App::to_pix(dip p) const
{
    return pix{static_cast<double>(p.value) / scale};
}


void App::set_scale(double d)
{
    scale = d;

    /*

    for(auto& f: loaded_fonts)
    {
        f->set_size(to_dip(f->unscaled_size));
    }

    */
}

std::shared_ptr<Font> App::load_font(const std::string_view& file, pix size)
{
    return load_font(std::string(file), size);
}

std::shared_ptr<Font> App::load_font(const std::string& file, pix size)
{
    auto r = platform->make_font();
    if(r->load_font(file.c_str(), to_dip(size)))
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

