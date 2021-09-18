#include "api/key.h"

#include <map>
#include <cassert>
#include <iostream>

#include "SDL.h"

#include "api/string.h"
#include "api/cc.h"
#include "api/c.h"

namespace
{
    struct KeyStringMap
    {
        std::map<std::string, Key> string_to_key;
        std::map<Key, std::string> key_to_string;

        void bind(Key key, const std::string& name)
        {
            const auto id = to_lower(name);
            assert(key_to_string.find(key) == key_to_string.end());
            assert(string_to_key.find(id) == string_to_key.end());

            string_to_key[id] = key;
            key_to_string[key] = name;
        }

        std::optional<Key> from_string(const std::string& name) const
        {
            const auto id = to_lower(name);
            const auto found_key = string_to_key.find(id);
            if(found_key == string_to_key.end())
            {
                return {};
            }
            return found_key->second;
        }

        std::string to_string(Key key) const
        {
            const auto found_name = key_to_string.find(key);
            if(found_name == key_to_string.end())
            {
                return "<?>";
            }
            return found_name->second;
        }
    };

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

bool operator<(const Keybind& lhs, const Keybind& rhs)
{
    if(lhs.meta.alt != rhs.meta.alt) { return lhs.meta.alt; }
    if(lhs.meta.shift != rhs.meta.shift) { return lhs.meta.shift; }
    if(lhs.meta.ctrl != rhs.meta.ctrl) { return lhs.meta.ctrl; }
    return cast_to_base(lhs.key) < cast_to_base(rhs.key);
}



std::optional<Keybind> keybind_from_string(const std::string& data)
{
    const auto split = Split(data, '+');

    auto key = Key::unknown;
    auto meta = Meta{};

    const auto size = C(split.size());
    for(int index=0; index<size; index+=1)
    {
        const auto name = Trim(split[Cs(index)]);
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

    return Keybind{key, meta};
}

std::optional<Key> key_from_string(const std::string& str)
{
    return global_key_string_map.from_string(str);
}

std::string to_string(Key key)
{
    return global_key_string_map.to_string(key);
}

Key key_from_sdl_keycode(SDL_KeyCode kc)
{
    switch(kc)
    {
    case SDLK_UNKNOWN: return Key::unknown;
    case SDLK_RETURN: return Key::return_key;
    case SDLK_ESCAPE: return Key::escape;
    case SDLK_BACKSPACE: return Key::backspace;
    case SDLK_TAB: return Key::tab;
    case SDLK_SPACE: return Key::space;
    case SDLK_EXCLAIM: return Key::exclaim;
    case SDLK_QUOTEDBL: return Key::quotedbl;
    case SDLK_HASH: return Key::hash;
    case SDLK_PERCENT: return Key::percent;
    case SDLK_DOLLAR: return Key::dollar;
    case SDLK_AMPERSAND: return Key::ampersand;
    case SDLK_QUOTE: return Key::quote;
    case SDLK_LEFTPAREN: return Key::leftparen;
    case SDLK_RIGHTPAREN: return Key::rightparen;
    case SDLK_ASTERISK: return Key::asterisk;
    case SDLK_PLUS: return Key::plus;
    case SDLK_COMMA: return Key::comma;
    case SDLK_MINUS: return Key::minus;
    case SDLK_PERIOD: return Key::period;
    case SDLK_SLASH: return Key::slash;
    case SDLK_0: return Key::n0;
    case SDLK_1: return Key::n1;
    case SDLK_2: return Key::n2;
    case SDLK_3: return Key::n3;
    case SDLK_4: return Key::n4;
    case SDLK_5: return Key::n5;
    case SDLK_6: return Key::n6;
    case SDLK_7: return Key::n7;
    case SDLK_8: return Key::n8;
    case SDLK_9: return Key::n9;
    case SDLK_COLON: return Key::colon;
    case SDLK_SEMICOLON: return Key::semicolon;
    case SDLK_LESS: return Key::less;
    case SDLK_EQUALS: return Key::equals;
    case SDLK_GREATER: return Key::greater;
    case SDLK_QUESTION: return Key::question;
    case SDLK_AT: return Key::at;
    case SDLK_LEFTBRACKET: return Key::leftbracket;
    case SDLK_BACKSLASH: return Key::backslash;
    case SDLK_RIGHTBRACKET: return Key::rightbracket;
    case SDLK_CARET: return Key::caret;
    case SDLK_UNDERSCORE: return Key::underscore;
    case SDLK_BACKQUOTE: return Key::backquote;
    case SDLK_a: return Key::a;
    case SDLK_b: return Key::b;
    case SDLK_c: return Key::c;
    case SDLK_d: return Key::d;
    case SDLK_e: return Key::e;
    case SDLK_f: return Key::f;
    case SDLK_g: return Key::g;
    case SDLK_h: return Key::h;
    case SDLK_i: return Key::i;
    case SDLK_j: return Key::j;
    case SDLK_k: return Key::k;
    case SDLK_l: return Key::l;
    case SDLK_m: return Key::m;
    case SDLK_n: return Key::n;
    case SDLK_o: return Key::o;
    case SDLK_p: return Key::p;
    case SDLK_q: return Key::q;
    case SDLK_r: return Key::r;
    case SDLK_s: return Key::s;
    case SDLK_t: return Key::t;
    case SDLK_u: return Key::u;
    case SDLK_v: return Key::v;
    case SDLK_w: return Key::w;
    case SDLK_x: return Key::x;
    case SDLK_y: return Key::y;
    case SDLK_z: return Key::z;
    case SDLK_CAPSLOCK: return Key::capslock;
    case SDLK_F1: return Key::f1;
    case SDLK_F2: return Key::f2;
    case SDLK_F3: return Key::f3;
    case SDLK_F4: return Key::f4;
    case SDLK_F5: return Key::f5;
    case SDLK_F6: return Key::f6;
    case SDLK_F7: return Key::f7;
    case SDLK_F8: return Key::f8;
    case SDLK_F9: return Key::f9;
    case SDLK_F10: return Key::f10;
    case SDLK_F11: return Key::f11;
    case SDLK_F12: return Key::f12;
    case SDLK_PRINTSCREEN: return Key::printscreen;
    case SDLK_SCROLLLOCK: return Key::scrolllock;
    case SDLK_PAUSE: return Key::pause;
    case SDLK_INSERT: return Key::insert;
    case SDLK_HOME: return Key::home;
    case SDLK_PAGEUP: return Key::pageup;
    case SDLK_DELETE: return Key::delete_key;
    case SDLK_END: return Key::end;
    case SDLK_PAGEDOWN: return Key::pagedown;
    case SDLK_RIGHT: return Key::right;
    case SDLK_LEFT: return Key::left;
    case SDLK_DOWN: return Key::down;
    case SDLK_UP: return Key::up;
    case SDLK_NUMLOCKCLEAR: return Key::numlockclear;
    case SDLK_KP_DIVIDE: return Key::kp_divide;
    case SDLK_KP_MULTIPLY: return Key::kp_multiply;
    case SDLK_KP_MINUS: return Key::kp_minus;
    case SDLK_KP_PLUS: return Key::kp_plus;
    case SDLK_KP_ENTER: return Key::kp_enter;
    case SDLK_KP_1: return Key::kp_1;
    case SDLK_KP_2: return Key::kp_2;
    case SDLK_KP_3: return Key::kp_3;
    case SDLK_KP_4: return Key::kp_4;
    case SDLK_KP_5: return Key::kp_5;
    case SDLK_KP_6: return Key::kp_6;
    case SDLK_KP_7: return Key::kp_7;
    case SDLK_KP_8: return Key::kp_8;
    case SDLK_KP_9: return Key::kp_9;
    case SDLK_KP_0: return Key::kp_0;
    case SDLK_KP_PERIOD: return Key::kp_period;
    case SDLK_APPLICATION: return Key::application;
    case SDLK_POWER: return Key::power;
    case SDLK_KP_EQUALS: return Key::kp_equals;
    case SDLK_F13: return Key::f13;
    case SDLK_F14: return Key::f14;
    case SDLK_F15: return Key::f15;
    case SDLK_F16: return Key::f16;
    case SDLK_F17: return Key::f17;
    case SDLK_F18: return Key::f18;
    case SDLK_F19: return Key::f19;
    case SDLK_F20: return Key::f20;
    case SDLK_F21: return Key::f21;
    case SDLK_F22: return Key::f22;
    case SDLK_F23: return Key::f23;
    case SDLK_F24: return Key::f24;
    case SDLK_EXECUTE: return Key::execute;
    case SDLK_HELP: return Key::help;
    case SDLK_MENU: return Key::menu;
    case SDLK_SELECT: return Key::select;
    case SDLK_STOP: return Key::stop;
    case SDLK_AGAIN: return Key::again;
    case SDLK_UNDO: return Key::undo;
    case SDLK_CUT: return Key::cut;
    case SDLK_COPY: return Key::copy;
    case SDLK_PASTE: return Key::paste;
    case SDLK_FIND: return Key::find;
    case SDLK_MUTE: return Key::mute;
    case SDLK_VOLUMEUP: return Key::volumeup;
    case SDLK_VOLUMEDOWN: return Key::volumedown;
    case SDLK_KP_COMMA: return Key::kp_comma;
    case SDLK_KP_EQUALSAS400: return Key::kp_equalsas400;
    case SDLK_ALTERASE: return Key::alterase;
    case SDLK_SYSREQ: return Key::sysreq;
    case SDLK_CANCEL: return Key::cancel;
    case SDLK_CLEAR: return Key::clear;
    case SDLK_PRIOR: return Key::prior;
    case SDLK_RETURN2: return Key::return2;
    case SDLK_SEPARATOR: return Key::separator;
    case SDLK_OUT: return Key::out;
    case SDLK_OPER: return Key::oper;
    case SDLK_CLEARAGAIN: return Key::clearagain;
    case SDLK_CRSEL: return Key::crsel;
    case SDLK_EXSEL: return Key::exsel;
    case SDLK_KP_00: return Key::kp_00;
    case SDLK_KP_000: return Key::kp_000;
    case SDLK_THOUSANDSSEPARATOR: return Key::thousandsseparator;
    case SDLK_DECIMALSEPARATOR: return Key::decimalseparator;
    case SDLK_CURRENCYUNIT: return Key::currencyunit;
    case SDLK_CURRENCYSUBUNIT: return Key::currencysubunit;
    case SDLK_KP_LEFTPAREN: return Key::kp_leftparen;
    case SDLK_KP_RIGHTPAREN: return Key::kp_rightparen;
    case SDLK_KP_LEFTBRACE: return Key::kp_leftbrace;
    case SDLK_KP_RIGHTBRACE: return Key::kp_rightbrace;
    case SDLK_KP_TAB: return Key::kp_tab;
    case SDLK_KP_BACKSPACE: return Key::kp_backspace;
    case SDLK_KP_A: return Key::kp_a;
    case SDLK_KP_B: return Key::kp_b;
    case SDLK_KP_C: return Key::kp_c;
    case SDLK_KP_D: return Key::kp_d;
    case SDLK_KP_E: return Key::kp_e;
    case SDLK_KP_F: return Key::kp_f;
    case SDLK_KP_XOR: return Key::kp_xor;
    case SDLK_KP_POWER: return Key::kp_power;
    case SDLK_KP_PERCENT: return Key::kp_percent;
    case SDLK_KP_LESS: return Key::kp_less;
    case SDLK_KP_GREATER: return Key::kp_greater;
    case SDLK_KP_AMPERSAND: return Key::kp_ampersand;
    case SDLK_KP_DBLAMPERSAND: return Key::kp_dblampersand;
    case SDLK_KP_VERTICALBAR: return Key::kp_verticalbar;
    case SDLK_KP_DBLVERTICALBAR: return Key::kp_dblverticalbar;
    case SDLK_KP_COLON: return Key::kp_colon;
    case SDLK_KP_HASH: return Key::kp_hash;
    case SDLK_KP_SPACE: return Key::kp_space;
    case SDLK_KP_AT: return Key::kp_at;
    case SDLK_KP_EXCLAM: return Key::kp_exclam;
    case SDLK_KP_MEMSTORE: return Key::kp_memstore;
    case SDLK_KP_MEMRECALL: return Key::kp_memrecall;
    case SDLK_KP_MEMCLEAR: return Key::kp_memclear;
    case SDLK_KP_MEMADD: return Key::kp_memadd;
    case SDLK_KP_MEMSUBTRACT: return Key::kp_memsubtract;
    case SDLK_KP_MEMMULTIPLY: return Key::kp_memmultiply;
    case SDLK_KP_MEMDIVIDE: return Key::kp_memdivide;
    case SDLK_KP_PLUSMINUS: return Key::kp_plusminus;
    case SDLK_KP_CLEAR: return Key::kp_clear;
    case SDLK_KP_CLEARENTRY: return Key::kp_clearentry;
    case SDLK_KP_BINARY: return Key::kp_binary;
    case SDLK_KP_OCTAL: return Key::kp_octal;
    case SDLK_KP_DECIMAL: return Key::kp_decimal;
    case SDLK_KP_HEXADECIMAL: return Key::kp_hexadecimal;
    case SDLK_LCTRL: return Key::lctrl;
    case SDLK_LSHIFT: return Key::lshift;
    case SDLK_LALT: return Key::lalt;
    case SDLK_LGUI: return Key::lgui;
    case SDLK_RCTRL: return Key::rctrl;
    case SDLK_RSHIFT: return Key::rshift;
    case SDLK_RALT: return Key::ralt;
    case SDLK_RGUI: return Key::rgui;
    case SDLK_MODE: return Key::mode;
    case SDLK_AUDIONEXT: return Key::audionext;
    case SDLK_AUDIOPREV: return Key::audioprev;
    case SDLK_AUDIOSTOP: return Key::audiostop;
    case SDLK_AUDIOPLAY: return Key::audioplay;
    case SDLK_AUDIOMUTE: return Key::audiomute;
    case SDLK_MEDIASELECT: return Key::mediaselect;
    case SDLK_WWW: return Key::www;
    case SDLK_MAIL: return Key::mail;
    case SDLK_CALCULATOR: return Key::calculator;
    case SDLK_COMPUTER: return Key::computer;
    case SDLK_AC_SEARCH: return Key::ac_search;
    case SDLK_AC_HOME: return Key::ac_home;
    case SDLK_AC_BACK: return Key::ac_back;
    case SDLK_AC_FORWARD: return Key::ac_forward;
    case SDLK_AC_STOP: return Key::ac_stop;
    case SDLK_AC_REFRESH: return Key::ac_refresh;
    case SDLK_AC_BOOKMARKS: return Key::ac_bookmarks;
    case SDLK_BRIGHTNESSDOWN: return Key::brightnessdown;
    case SDLK_BRIGHTNESSUP: return Key::brightnessup;
    case SDLK_DISPLAYSWITCH: return Key::displayswitch;
    case SDLK_KBDILLUMTOGGLE: return Key::kbdillumtoggle;
    case SDLK_KBDILLUMDOWN: return Key::kbdillumdown;
    case SDLK_KBDILLUMUP: return Key::kbdillumup;
    case SDLK_EJECT: return Key::eject;
    case SDLK_SLEEP: return Key::sleep;
    case SDLK_APP1: return Key::app1;
    case SDLK_APP2: return Key::app2;
    case SDLK_AUDIOREWIND: return Key::audiorewind;
    case SDLK_AUDIOFASTFORWARD: return Key::audiofastforward;
    default:
        assert(false && "unknown key");
        return Key::unknown;
    }
}
