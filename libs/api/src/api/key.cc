#include "api/key.h"

#include <map>
#include <cassert>
#include <iostream>

#include "base/string.h"
#include "base/cc.h"
#include "base/c.h"
#include "base/enumstringmap.h"

namespace
{
    using KeyStringMap = EnumStringMap<Key>;

    KeyStringMap build_map()
    {
        KeyStringMap map;

        map.bind(Key::unknown, "unknown");
        map.bind(Key::return_key, "return");
        map.bind(Key::escape, "escape");
        map.bind(Key::backspace, "backspace");
        map.bind(Key::tab, "tab");
        map.bind(Key::space, "space");
        map.bind(Key::exclaim, "exclaim");
        map.bind(Key::quotedbl, "quotedbl");
        map.bind(Key::hash, "hash");
        map.bind(Key::percent, "percent");
        map.bind(Key::dollar, "dollar");
        map.bind(Key::ampersand, "ampersand");
        map.bind(Key::quote, "quote");
        map.bind(Key::leftparen, "leftparen");
        map.bind(Key::rightparen, "rightparen");
        map.bind(Key::asterisk, "asterisk");
        map.bind(Key::plus, "plus");
        map.bind(Key::comma, "comma");
        map.bind(Key::minus, "minus");
        map.bind(Key::period, "period");
        map.bind(Key::slash, "slash");
        map.bind(Key::n0, "0");
        map.bind(Key::n1, "1");
        map.bind(Key::n2, "2");
        map.bind(Key::n3, "3");
        map.bind(Key::n4, "4");
        map.bind(Key::n5, "5");
        map.bind(Key::n6, "6");
        map.bind(Key::n7, "7");
        map.bind(Key::n8, "8");
        map.bind(Key::n9, "9");
        map.bind(Key::colon, "colon");
        map.bind(Key::semicolon, "semicolon");
        map.bind(Key::less, "less");
        map.bind(Key::equals, "equals");
        map.bind(Key::greater, "greater");
        map.bind(Key::question, "question");
        map.bind(Key::at, "at");
        map.bind(Key::leftbracket, "leftbracket");
        map.bind(Key::backslash, "backslash");
        map.bind(Key::rightbracket, "rightbracket");
        map.bind(Key::caret, "caret");
        map.bind(Key::underscore, "underscore");
        map.bind(Key::backquote, "backquote");
        map.bind(Key::a, "a");
        map.bind(Key::b, "b");
        map.bind(Key::c, "c");
        map.bind(Key::d, "d");
        map.bind(Key::e, "e");
        map.bind(Key::f, "f");
        map.bind(Key::g, "g");
        map.bind(Key::h, "h");
        map.bind(Key::i, "i");
        map.bind(Key::j, "j");
        map.bind(Key::k, "k");
        map.bind(Key::l, "l");
        map.bind(Key::m, "m");
        map.bind(Key::n, "n");
        map.bind(Key::o, "o");
        map.bind(Key::p, "p");
        map.bind(Key::q, "q");
        map.bind(Key::r, "r");
        map.bind(Key::s, "s");
        map.bind(Key::t, "t");
        map.bind(Key::u, "u");
        map.bind(Key::v, "v");
        map.bind(Key::w, "w");
        map.bind(Key::x, "x");
        map.bind(Key::y, "y");
        map.bind(Key::z, "z");
        map.bind(Key::capslock, "capslock");
        map.bind(Key::f1, "f1");
        map.bind(Key::f2, "f2");
        map.bind(Key::f3, "f3");
        map.bind(Key::f4, "f4");
        map.bind(Key::f5, "f5");
        map.bind(Key::f6, "f6");
        map.bind(Key::f7, "f7");
        map.bind(Key::f8, "f8");
        map.bind(Key::f9, "f9");
        map.bind(Key::f10, "f10");
        map.bind(Key::f11, "f11");
        map.bind(Key::f12, "f12");
        map.bind(Key::printscreen, "printscreen");
        map.bind(Key::scrolllock, "scrolllock");
        map.bind(Key::pause, "pause");
        map.bind(Key::insert, "insert");
        map.bind(Key::home, "home");
        map.bind(Key::pageup, "pageup");
        map.bind(Key::delete_key, "delete");
        map.bind(Key::end, "end");
        map.bind(Key::pagedown, "pagedown");
        map.bind(Key::right, "right");
        map.bind(Key::left, "left");
        map.bind(Key::down, "down");
        map.bind(Key::up, "up");
        map.bind(Key::numlockclear, "numlockclear");
        map.bind(Key::kp_divide, "keypad divide");
        map.bind(Key::kp_multiply, "keypad multiply");
        map.bind(Key::kp_minus, "keypad minus");
        map.bind(Key::kp_plus, "keypad plus");
        map.bind(Key::kp_enter, "keypad enter");
        map.bind(Key::kp_1, "keypad 1");
        map.bind(Key::kp_2, "keypad 2");
        map.bind(Key::kp_3, "keypad 3");
        map.bind(Key::kp_4, "keypad 4");
        map.bind(Key::kp_5, "keypad 5");
        map.bind(Key::kp_6, "keypad 6");
        map.bind(Key::kp_7, "keypad 7");
        map.bind(Key::kp_8, "keypad 8");
        map.bind(Key::kp_9, "keypad 9");
        map.bind(Key::kp_0, "keypad 0");
        map.bind(Key::kp_period, "keypad period");
        map.bind(Key::application, "application");
        map.bind(Key::power, "power");
        map.bind(Key::kp_equals, "keypad equals");
        map.bind(Key::f13, "f13");
        map.bind(Key::f14, "f14");
        map.bind(Key::f15, "f15");
        map.bind(Key::f16, "f16");
        map.bind(Key::f17, "f17");
        map.bind(Key::f18, "f18");
        map.bind(Key::f19, "f19");
        map.bind(Key::f20, "f20");
        map.bind(Key::f21, "f21");
        map.bind(Key::f22, "f22");
        map.bind(Key::f23, "f23");
        map.bind(Key::f24, "f24");
        map.bind(Key::execute, "execute");
        map.bind(Key::help, "help");
        map.bind(Key::menu, "menu");
        map.bind(Key::select, "select");
        map.bind(Key::stop, "stop");
        map.bind(Key::again, "again");
        map.bind(Key::undo, "undo");
        map.bind(Key::cut, "cut");
        map.bind(Key::copy, "copy");
        map.bind(Key::paste, "paste");
        map.bind(Key::find, "find");
        map.bind(Key::mute, "mute");
        map.bind(Key::volumeup, "volumeup");
        map.bind(Key::volumedown, "volumedown");
        map.bind(Key::kp_comma, "keypad comma");
        map.bind(Key::kp_equalsas400, "keypad equalsas400");
        map.bind(Key::alterase, "alterase");
        map.bind(Key::sysreq, "sysreq");
        map.bind(Key::cancel, "cancel");
        map.bind(Key::clear, "clear");
        map.bind(Key::prior, "prior");
        map.bind(Key::return2, "return2");
        map.bind(Key::separator, "separator");
        map.bind(Key::out, "out");
        map.bind(Key::oper, "oper");
        map.bind(Key::clearagain, "clearagain");
        map.bind(Key::crsel, "crsel");
        map.bind(Key::exsel, "exsel");
        map.bind(Key::kp_00, "keypad 00");
        map.bind(Key::kp_000, "keypad 000");
        map.bind(Key::thousandsseparator, "thousandsseparator");
        map.bind(Key::decimalseparator, "decimalseparator");
        map.bind(Key::currencyunit, "currencyunit");
        map.bind(Key::currencysubunit, "currencysubunit");
        map.bind(Key::kp_leftparen, "keypad leftparen");
        map.bind(Key::kp_rightparen, "keypad rightparen");
        map.bind(Key::kp_leftbrace, "keypad leftbrace");
        map.bind(Key::kp_rightbrace, "keypad rightbrace");
        map.bind(Key::kp_tab, "keypad tab");
        map.bind(Key::kp_backspace, "keypad backspace");
        map.bind(Key::kp_a, "keypad a");
        map.bind(Key::kp_b, "keypad b");
        map.bind(Key::kp_c, "keypad c");
        map.bind(Key::kp_d, "keypad d");
        map.bind(Key::kp_e, "keypad e");
        map.bind(Key::kp_f, "keypad f");
        map.bind(Key::kp_xor, "keypad xor");
        map.bind(Key::kp_power, "keypad power");
        map.bind(Key::kp_percent, "keypad percent");
        map.bind(Key::kp_less, "keypad less");
        map.bind(Key::kp_greater, "keypad greater");
        map.bind(Key::kp_ampersand, "keypad ampersand");
        map.bind(Key::kp_dblampersand, "keypad dblampersand");
        map.bind(Key::kp_verticalbar, "keypad verticalbar");
        map.bind(Key::kp_dblverticalbar, "keypad dblverticalbar");
        map.bind(Key::kp_colon, "keypad colon");
        map.bind(Key::kp_hash, "keypad hash");
        map.bind(Key::kp_space, "keypad space");
        map.bind(Key::kp_at, "keypad at");
        map.bind(Key::kp_exclam, "keypad exclam");
        map.bind(Key::kp_memstore, "keypad memstore");
        map.bind(Key::kp_memrecall, "keypad memrecall");
        map.bind(Key::kp_memclear, "keypad memclear");
        map.bind(Key::kp_memadd, "keypad memadd");
        map.bind(Key::kp_memsubtract, "keypad memsubtract");
        map.bind(Key::kp_memmultiply, "keypad memmultiply");
        map.bind(Key::kp_memdivide, "keypad memdivide");
        map.bind(Key::kp_plusminus, "keypad plusminus");
        map.bind(Key::kp_clear, "keypad clear");
        map.bind(Key::kp_clearentry, "keypad clearentry");
        map.bind(Key::kp_binary, "keypad binary");
        map.bind(Key::kp_octal, "keypad octal");
        map.bind(Key::kp_decimal, "keypad decimal");
        map.bind(Key::kp_hexadecimal, "keypad hexadecimal");
        map.bind(Key::lctrl, "lctrl");
        map.bind(Key::lshift, "lshift");
        map.bind(Key::lalt, "lalt");
        map.bind(Key::lgui, "lgui");
        map.bind(Key::rctrl, "rctrl");
        map.bind(Key::rshift, "rshift");
        map.bind(Key::ralt, "ralt");
        map.bind(Key::rgui, "rgui");
        map.bind(Key::mode, "mode");
        map.bind(Key::audionext, "audionext");
        map.bind(Key::audioprev, "audioprev");
        map.bind(Key::audiostop, "audiostop");
        map.bind(Key::audioplay, "audioplay");
        map.bind(Key::audiomute, "audiomute");
        map.bind(Key::mediaselect, "mediaselect");
        map.bind(Key::www, "www");
        map.bind(Key::mail, "mail");
        map.bind(Key::calculator, "calculator");
        map.bind(Key::computer, "computer");
        map.bind(Key::ac_search, "ac_search");
        map.bind(Key::ac_home, "ac_home");
        map.bind(Key::ac_back, "ac_back");
        map.bind(Key::ac_forward, "ac_forward");
        map.bind(Key::ac_stop, "ac_stop");
        map.bind(Key::ac_refresh, "ac_refresh");
        map.bind(Key::ac_bookmarks, "ac_bookmarks");
        map.bind(Key::brightnessdown, "brightnessdown");
        map.bind(Key::brightnessup, "brightnessup");
        map.bind(Key::displayswitch, "displayswitch");
        map.bind(Key::kbdillumtoggle, "kbdillumtoggle");
        map.bind(Key::kbdillumdown, "kbdillumdown");
        map.bind(Key::kbdillumup, "kbdillumup");
        map.bind(Key::eject, "eject");
        map.bind(Key::sleep, "sleep");
        map.bind(Key::app1, "app1");
        map.bind(Key::app2, "app2");
        map.bind(Key::audiorewind, "audiorewind");
        map.bind(Key::audiofastforward, "audiofastforward");

        return map;
    }

    static const KeyStringMap global_key_string_map = build_map();

    bool set_string_meta(Meta* meta, const std::string& name_big)
    {
        const auto name = to_lower(name_big);

        if(name == "ctrl" || name == "control")
        {
            meta->ctrl = true;
            return true;
        }
        else if(name == "shift")
        {
            meta->shift = true;
            return true;
        }
        else if(name == "alt")
        {
            meta->alt = true;
            return true;
        }
        else if(name == "gui")
        {
            meta->gui = true;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool operator<(const Stroke& lhs, const Stroke& rhs)
{
    if(lhs.meta.alt != rhs.meta.alt) { return lhs.meta.alt; }
    if(lhs.meta.shift != rhs.meta.shift) { return lhs.meta.shift; }
    if(lhs.meta.ctrl != rhs.meta.ctrl) { return lhs.meta.ctrl; }
    return cast_to_base(lhs.key) < cast_to_base(rhs.key);
}



std::optional<Stroke> stroke_from_string(const std::string& data)
{
    const auto split = split_on_delim(data, '+');

    auto key = Key::unknown;
    auto meta = Meta{};

    const auto size = C(split.size());
    for(int index=0; index<size; index+=1)
    {
        const auto name = trim(split[Cs(index)]);
        const auto is_last = index == size-1;
        if(is_last)
        {
            auto found_key = key_from_string(name);
            if(found_key)
            {
                key = *found_key;
            }
            else
            {
                return {};
            }

        }
        else
        {
            if(false == set_string_meta(&meta, name))
            {
                return {};
            }
        }
    }

    return Stroke{key, meta};
}

std::string to_string(const Stroke& stroke)
{
    std::string r;

    const auto add_meta = [&r](bool add, const std::string& s)
    {
        if(add)
        {
            r += s + "+";
        }
    };

    add_meta(stroke.meta.ctrl, "ctrl");
    add_meta(stroke.meta.shift, "shift");
    add_meta(stroke.meta.alt, "alt");
    add_meta(stroke.meta.gui, "gui");
    return r + to_string(stroke.key);
}

std::optional<Key> key_from_string(const std::string& str)
{
    return global_key_string_map.from_string(str);
}

std::string to_string(Key key)
{
    return global_key_string_map.to_string(key);
}

