#include <iostream>

#include "base/str.h"
#include "base/cc.h"
#include "base/filesystem.h"

#include "api/app.h"
// #include "api/main.h"
#include "api/renderer.h"
#include "api/image.h"
#include "api/font.h"
#include "api/texture.h"

#include "libride/command.h"
#include "libride/document.h"
#include "libride/document.commands.h"

#include "ride/keybind.h"
#include "ride/theme.h"
#include "ride/view.h"

#include "ride/view.document.h"
#include "ride/view.filesystem.h"

#include "logo_256text_png.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////




double change_scale(double current, bool increase)
{
    return keep_within
    (
        0.1,
        current + 0.1 * (increase ? 1.0 : -1.0),
        2.0
    );
}

struct RideApp : App
{
    vec2<pix> mouse = {pix{0}, pix{0}};

    std::shared_ptr<Texture> logo;
    std::shared_ptr<Font> font;
    std::unique_ptr<filesystem> fs;

    Theme theme;

    CommandList commands;
    ViewDoc root;
    ViewFilesystem browser;
    KeyBind keybind;

    void on_key_pressed(const Stroke& key) override
    {
        std::cout << "key pressed: " << to_string(key) << "\n";
        keybind.run(key, &commands);
    }

    void setup_view(View* view)
    {
        view->theme = &theme;
        view->app = this;
    }

    RideApp(PlatformArg p)
        : App(p)
        , logo(p->load_shared_texture(LOGO_256TEXT_PNG))
        , font(load_font(default_font, pix{12}))
        , fs(create_local_filesystem())
    {
        setup_view(&root);
        root.font = font;
        root.set_document(std::make_shared<Document>());
        root.doc->LoadFile(fs.get(), __FILE__);

        setup_view(&browser);
        browser.font = font;
        browser.fs = fs.get();
        if(auto cd = fs->get_current_directory(); cd)
        {
            browser.root = *cd;
            browser.setup();
        }

        commands.add()
            ("core.quit", [this](){ this->run = false; })
            (
                "scale.+", [this]()
                {
                    this->set_scale(change_scale(this->scale, true));
                }
            )
            (
                "scale.-", [this]()
                {
                    this->set_scale(change_scale(this->scale, false));
                }
            )
            ;
        add_edit_commands
        (
            &commands,
            [this]() -> VirtualView*
            {
                auto* active = this->get_active_view();
                if(active == nullptr) { return nullptr; }
                if(active != &root) { return nullptr; }
                return &root;
            }
        );

        std::cout << "commands:\n";
        for(const auto& c: commands.map)
        {
            std::cout << " - " << c.first << "\n";
        }

        keybind.add(*stroke_from_string("ctrl+q"), {"core.quit"});
        keybind.add(*stroke_from_string("return"), {"doc.insert-newline"});

        // keybind.add(*stroke_from_string("ctrl+up"),   {"scale.+"});
        // keybind.add(*stroke_from_string("ctrl+down"), {"scale.-"});

        auto add_move_select = [this](const std::string& stroke_name, const std::string& command)
        {
            auto stroke = *stroke_from_string(stroke_name);

            stroke.meta.shift = false;
            this->keybind.add(stroke, {"doc.move-" + command});

            stroke.meta.shift = true;
            this->keybind.add(stroke, {"doc.select-" + command});
        };

        add_move_select("left", "left-char");
        add_move_select("right", "right-char");
        add_move_select("ctrl+left", "left-word");
        add_move_select("ctrl+right", "right-word");
        add_move_select("up", "prev-line");
        add_move_select("down", "next-line");
        add_move_select("ctrl+up", "prev-block");
        add_move_select("ctrl+down", "next-block");
        add_move_select("home", "home");
        add_move_select("end", "end");
        add_move_select("ctrl+home", "doc-start");
        add_move_select("ctrl+end", "doc-end");

        // temporary
        add_move_select("alt+left", "word-start");
        add_move_select("alt+right", "word-end");
    }

    void draw(Renderer* cache) override
    {
        auto rect = ::rect<pix>::from_size(client_size);
        cache->draw_rect(to_dip(rect), theme.window_background_color);

        cache->draw_image(logo, to_dip(pix{10}), to_dip(pix{10}), theme.logo_color);

        const auto browser_rect = rect.cut_left(150_px);
        const auto border_size = 3_px;

        browser.on_layout(browser_rect.get_inset(border_size));
        root.on_layout(rect.get_inset(border_size));

        draw_view(&browser, cache);
        draw_view(&root, cache);
    }

    void draw_view(View* view, Renderer* cache)
    {
        view->draw(cache);

        if(theme.color_inactive_view)
        {
            if(view != get_active_view())
            {
                cache->draw_rect(this->to_dip(view->body_rect), theme.inactive_view_color);
            }
        }
    }

    View* get_active_view()
    {
        return get_mouse_hovering_view();
    }

    View* get_mouse_hovering_view()
    {
        if(root.client_rect.contains(mouse))
        {
            return &root;
        }
        else if(browser.client_rect.contains(mouse))
        {
            return &browser;
        }
        else
        {
            return nullptr;
        }
    }

    void on_mouse_pressed(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse, int clicks) override
    {
        mouse = new_mouse;

        View* view = get_active_view();
        if(view == nullptr) { return; }

        view->on_mouse_pressed(button, meta, new_mouse, clicks);
    }

    void on_mouse_moved(const Meta& meta, const vec2<pix>& new_mouse, pix, pix) override
    {
        mouse = new_mouse;

        View* view = get_active_view();
        if(view == nullptr) { return; }
        view->on_mouse_moved(meta, new_mouse);
    }

    void on_mouse_released(MouseButton button, const Meta& meta, const vec2<pix>& new_mouse) override
    {
        mouse = new_mouse;

        View* view = get_active_view();
        if(view == nullptr) { return; }

        view->on_mouse_released(button, meta, new_mouse);
    }

    void on_text_input(const std::string& t) override
    {
        View* view = get_active_view();
        if(view)
        {
            view->on_text(t);
        }
        else
        {
            std::cout << "Not hovering...\n";
        }
    }

    void on_mouse_wheel(int dx, int dy) override
    {
        View* view = get_mouse_hovering_view();
        if(view)
        {
            view->on_mouse_wheel(dx, dy);
        }
        else
        {
            std::cout << "Not hovering...\n";
        }
    }

    void update() override
    {
        auto* view = get_mouse_hovering_view();
        if(view)
        {
            cursor = view->cursor;
        }
        else
        {
            cursor = cursor_type::arrow;
        }
    }
};

std::unique_ptr<App> create_ride_app(PlatformArg arg)
{
    return std::make_unique<RideApp>(arg);
}

