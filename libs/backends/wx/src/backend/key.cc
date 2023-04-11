#include "backend/key.h"

#include <iostream>

#include "backend/dependency_wx.h"

namespace ride::backends::gl
{

Key key_from_wx_keycode(i32 k)
{
    switch(k)
    {
    case WXK_BACK: return Key::backspace;
    case WXK_TAB: return Key::tab;
    case WXK_RETURN: return Key::return_key;
    case WXK_ESCAPE: return Key::escape;
    case WXK_SPACE: return Key::space;
    case WXK_DELETE: return Key::delete_key;
    // case WXK_START: return Key::start;
    // case WXK_LBUTTON: return Key::lbutton;
    // case WXK_RBUTTON: return Key::rbutton;
    case WXK_CANCEL: return Key::cancel;
    // case WXK_MBUTTON: return Key::mbutton;
    case WXK_CLEAR: return Key::clear;
    case WXK_SHIFT: return Key::lshift;
    case WXK_ALT: return Key::lalt;
    case WXK_CONTROL: return Key::lctrl;
    case WXK_MENU: return Key::menu;
    case WXK_PAUSE: return Key::pause;
    // case WXK_CAPITAL: return Key::capital;
    case WXK_END: return Key::end;
    case WXK_HOME: return Key::home;
    case WXK_LEFT: return Key::left;
    case WXK_UP: return Key::up;
    case WXK_RIGHT: return Key::right;
    case WXK_DOWN: return Key::down;
    case WXK_SELECT: return Key::select;
    // case WXK_PRINT: return Key::print;
    case WXK_EXECUTE: return Key::execute;
    // case WXK_SNAPSHOT: return Key::snapshot;
    case WXK_INSERT: return Key::insert;
    case WXK_HELP: return Key::help;
    case WXK_NUMPAD0: return Key::kp_0;
    case WXK_NUMPAD1: return Key::kp_1;
    case WXK_NUMPAD2: return Key::kp_2;
    case WXK_NUMPAD3: return Key::kp_3;
    case WXK_NUMPAD4: return Key::kp_4;
    case WXK_NUMPAD5: return Key::kp_5;
    case WXK_NUMPAD6: return Key::kp_6;
    case WXK_NUMPAD7: return Key::kp_7;
    case WXK_NUMPAD8: return Key::kp_8;
    case WXK_NUMPAD9: return Key::kp_9;
    case WXK_MULTIPLY: return Key::kp_multiply;
    case WXK_ADD: return Key::plus; // ?
    case WXK_SEPARATOR: return Key::separator;
    case WXK_SUBTRACT: return Key::kp_minus;
    case WXK_DECIMAL: return Key::kp_decimal;
    case WXK_DIVIDE: return Key::kp_divide;
    case WXK_F1: return Key::f1;
    case WXK_F2: return Key::f2;
    case WXK_F3: return Key::f3;
    case WXK_F4: return Key::f4;
    case WXK_F5: return Key::f5;
    case WXK_F6: return Key::f6;
    case WXK_F7: return Key::f7;
    case WXK_F8: return Key::f8;
    case WXK_F9: return Key::f9;
    case WXK_F10: return Key::f10;
    case WXK_F11: return Key::f11;
    case WXK_F12: return Key::f12;
    case WXK_F13: return Key::f13;
    case WXK_F14: return Key::f14;
    case WXK_F15: return Key::f15;
    case WXK_F16: return Key::f16;
    case WXK_F17: return Key::f17;
    case WXK_F18: return Key::f18;
    case WXK_F19: return Key::f19;
    case WXK_F20: return Key::f20;
    case WXK_F21: return Key::f21;
    case WXK_F22: return Key::f22;
    case WXK_F23: return Key::f23;
    case WXK_F24: return Key::f24;
    case WXK_NUMLOCK: return Key::numlockclear; // ?
    case WXK_SCROLL: return Key::scrolllock;
    case WXK_PAGEUP: return Key::pageup;
    case WXK_PAGEDOWN: return Key::pagedown;
    case WXK_NUMPAD_SPACE: return Key::kp_space;
    case WXK_NUMPAD_TAB: return Key::kp_tab;
    case WXK_NUMPAD_ENTER: return Key::kp_enter;
    // case WXK_NUMPAD_F1: return Key::kp_f1;
    // case WXK_NUMPAD_F2: return Key::kp_f2;
    // case WXK_NUMPAD_F3: return Key::kp_f3;
    // case WXK_NUMPAD_F4: return Key::kp_f4;
    // case WXK_NUMPAD_HOME: return Key::kp_home;
    // case WXK_NUMPAD_LEFT: return Key::kp_left;
    // case WXK_NUMPAD_UP: return Key::kp_up;
    // case WXK_NUMPAD_RIGHT: return Key::kp_right;
    // case WXK_NUMPAD_DOWN: return Key::kp_down;
    // case WXK_NUMPAD_PAGEUP: return Key::kp_pageup;
    // case WXK_NUMPAD_PAGEDOWN: return Key::kp_pagedown;
    // case WXK_NUMPAD_END: return Key::kp_end;
    // case WXK_NUMPAD_BEGIN: return Key::kp_begin;
    // case WXK_NUMPAD_INSERT: return Key::kp_insert;
    // case WXK_NUMPAD_DELETE: return Key::kp_delete;
    // case WXK_NUMPAD_EQUAL: return Key::kp_equal;
    case WXK_NUMPAD_MULTIPLY: return Key::kp_multiply;
    // case WXK_NUMPAD_ADD: return Key::kp_add;
    // case WXK_NUMPAD_SEPARATOR: return Key::kp_separator;
    // case WXK_NUMPAD_SUBTRACT: return Key::kp_subtract;
    case WXK_NUMPAD_DECIMAL: return Key::kp_decimal;
    case WXK_NUMPAD_DIVIDE: return Key::kp_divide;
    // case WXK_WINDOWS_LEFT: return Key::windowleft;
    // case WXK_WINDOWS_RIGHT: return Key::windowsright;
    // case WXK_WINDOWS_MENU: return Key::windowsmenu;
    // case WXK_SPECIAL1: return Key::special1;
    // case WXK_SPECIAL2: return Key::special2;
    // case WXK_SPECIAL3: return Key::special3;
    // case WXK_SPECIAL4: return Key::special4;
    // case WXK_SPECIAL5: return Key::special5;
    // case WXK_SPECIAL6: return Key::special6;
    // case WXK_SPECIAL7: return Key::special7;
    // case WXK_SPECIAL8: return Key::special8;
    // case WXK_SPECIAL9: return Key::special9;
    // case WXK_SPECIAL10: return Key::special10;
    // case WXK_SPECIAL11: return Key::special11;
    // case WXK_SPECIAL12: return Key::special12;
    // case WXK_SPECIAL13: return Key::special13;
    // case WXK_SPECIAL14: return Key::special14;
    // case WXK_SPECIAL15: return Key::special15;
    // case WXK_SPECIAL16: return Key::special16;
    // case WXK_SPECIAL17: return Key::special17;
    // case WXK_SPECIAL18: return Key::special18;
    // case WXK_SPECIAL19: return Key::special19;
    // case WXK_SPECIAL20: return Key::special20;

    // case 33: return Key::exclamationmark;
    // case 34: return Key::doublequotes;
    // case 35: return Key::number;
    case 36: return Key::dollar;
    case 37: return Key::percent;
    case 38: return Key::ampersand;
    // case 39: return Key::singlequote;
    // case 40: return Key::openparenthesis;
    // case 41: return Key::closeparenthesis;
    // case 42: return Key::asterisk;
    case 43: return Key::plus;
    case 44: return Key::comma;
    // case 45: return Key::hyphen;
    // case 46: return Key::dot;
    case 47: return Key::slash;
    case 48: return Key::n0;
    case 49: return Key::n1;
    case 50: return Key::n2;
    case 51: return Key::n3;
    case 52: return Key::n4;
    case 53: return Key::n5;
    case 54: return Key::n6;
    case 55: return Key::n7;
    case 56: return Key::n8;
    case 57: return Key::n9;
    case 58: return Key::colon;
    case 59: return Key::semicolon;
    // case 60: return Key::lessthan;
    case 61: return Key::equals;
    // case 62: return Key::greaterthan;
    // case 63: return Key::questionmark;
    case 64: return Key::at;
    case 65: return Key::a;
    case 66: return Key::b;
    case 67: return Key::c;
    case 68: return Key::d;
    case 69: return Key::e;
    case 70: return Key::f;
    case 71: return Key::g;
    case 72: return Key::h;
    case 73: return Key::i;
    case 74: return Key::j;
    case 75: return Key::k;
    case 76: return Key::l;
    case 77: return Key::m;
    case 78: return Key::n;
    case 79: return Key::o;
    case 80: return Key::p;
    case 81: return Key::q;
    case 82: return Key::r;
    case 83: return Key::s;
    case 84: return Key::t;
    case 85: return Key::u;
    case 86: return Key::v;
    case 87: return Key::w;
    case 88: return Key::x;
    case 89: return Key::y;
    case 90: return Key::z;
    // case 91: return Key::openingbracket;
    case 92: return Key::backslash;
    // case 93: return Key::closingbracket;
    case 94: return Key::caret;
    case 95: return Key::underscore;
    // case 96: return Key::graveaccent;
    case 97: return Key::a;
    case 98: return Key::b;
    case 99: return Key::c;
    case 100: return Key::d;
    case 101: return Key::e;
    case 102: return Key::f;
    case 103: return Key::g;
    case 104: return Key::h;
    case 105: return Key::i;
    case 106: return Key::j;
    case 107: return Key::k;
    case 108: return Key::l;
    case 109: return Key::m;
    case 110: return Key::n;
    case 111: return Key::o;
    case 112: return Key::p;
    case 113: return Key::q;
    case 114: return Key::r;
    case 115: return Key::s;
    case 116: return Key::t;
    case 117: return Key::u;
    case 118: return Key::v;
    case 119: return Key::w;
    case 120: return Key::x;
    case 121: return Key::y;
    case 122: return Key::z;
    // case 123: return Key::openingbrace;
    // case 124: return Key::verticalvar;
    // case 125: return Key::closingbrace;
    // case 126: return Key::tilde;

    default:
        return Key::unknown;
    }
}

}


