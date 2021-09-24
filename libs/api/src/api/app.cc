#include "api/app.h"

#include "api/sdl_dependency.h"

#include "base/vec2.h"

#include "api/renderer.h"
#include "api/rencache.h"
#include "api/font.h"

#include <cassert>
#include <algorithm>
#include <optional>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#endif

double calculate_scale()
{
    float dpi;
    SDL_GetDisplayDPI(0, NULL, &dpi, NULL);
#if _WIN32
    return dpi / 96.0;
#else
    return 1.0;
#endif
}

App::App()
    : run(true)
    , client_size{pix{0}, pix{0}}
    , scale(calculate_scale())
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

void App::on_key_pressed(Key)
{
}

void App::on_key_released(Key)
{
}

void App::on_text_input(const std::string&)
{
}


void App::on_mouse_pressed(MouseButton, pix, pix, int)
{
}

void App::on_mouse_released(MouseButton, pix, pix)
{
}

void App::on_mouse_moved(const vec2<pix>&, pix, pix)
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
    return dip{static_cast<dip::type>(p.value * scale)};
}

rect<dip> App::to_dip(const rect<pix>& p) const
{
    return rect<dip>
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

    for(auto& f: loaded_fonts)
    {
        f->set_size(to_dip(f->unscaled_size));
    }

     // todo(Gustav): invalidate rendercache...
}

std::shared_ptr<Font> App::load_font(const std::string_view& file, pix size)
{
    return load_font(std::string(file), size);
}

std::shared_ptr<Font> App::load_font(const std::string& file, pix size)
{
    auto r = std::make_shared<Font>();
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
