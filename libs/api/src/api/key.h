#pragma once

#include "core/ints.h"


namespace ride::api
{


struct Meta
{
    bool ctrl = false;
    bool shift = false;
    bool alt = false;
    bool gui = false;

    Meta() = default;
    constexpr Meta(bool c, bool s, bool a, bool g) : ctrl(c), shift(s), alt(a), gui(g) {}
};

enum class Key
{
    unknown,
    return_key,
    escape,
    backspace,
    tab,
    space,
    exclaim,
    quotedbl,
    hash,
    percent,
    dollar,
    ampersand,
    quote,
    leftparen,
    rightparen,
    asterisk,
    plus,
    comma,
    minus,
    period,
    slash,
    n0,
    n1,
    n2,
    n3,
    n4,
    n5,
    n6,
    n7,
    n8,
    n9,
    colon,
    semicolon,
    less,
    equals,
    greater,
    question,
    at,
    leftbracket,
    backslash,
    rightbracket,
    caret,
    underscore,
    backquote,
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h,
    i,
    j,
    k,
    l,
    m,
    n,
    o,
    p,
    q,
    r,
    s,
    t,
    u,
    v,
    w,
    x,
    y,
    z,
    capslock,
    f1,
    f2,
    f3,
    f4,
    f5,
    f6,
    f7,
    f8,
    f9,
    f10,
    f11,
    f12,
    printscreen,
    scrolllock,
    pause,
    insert,
    home,
    pageup,
    delete_key,
    end,
    pagedown,
    right,
    left,
    down,
    up,
    numlockclear,
    kp_divide,
    kp_multiply,
    kp_minus,
    kp_plus,
    kp_enter,
    kp_1,
    kp_2,
    kp_3,
    kp_4,
    kp_5,
    kp_6,
    kp_7,
    kp_8,
    kp_9,
    kp_0,
    kp_period,
    application,
    power,
    kp_equals,
    f13,
    f14,
    f15,
    f16,
    f17,
    f18,
    f19,
    f20,
    f21,
    f22,
    f23,
    f24,
    execute,
    help,
    menu,
    select,
    stop,
    again,
    undo,
    cut,
    copy,
    paste,
    find,
    mute,
    volumeup,
    volumedown,
    kp_comma,
    kp_equalsas400,
    alterase,
    sysreq,
    cancel,
    clear,
    prior,
    return2,
    separator,
    out,
    oper,
    clearagain,
    crsel,
    exsel,
    kp_00,
    kp_000,
    thousandsseparator,
    decimalseparator,
    currencyunit,
    currencysubunit,
    kp_leftparen,
    kp_rightparen,
    kp_leftbrace,
    kp_rightbrace,
    kp_tab,
    kp_backspace,
    kp_a,
    kp_b,
    kp_c,
    kp_d,
    kp_e,
    kp_f,
    kp_xor,
    kp_power,
    kp_percent,
    kp_less,
    kp_greater,
    kp_ampersand,
    kp_dblampersand,
    kp_verticalbar,
    kp_dblverticalbar,
    kp_colon,
    kp_hash,
    kp_space,
    kp_at,
    kp_exclam,
    kp_memstore,
    kp_memrecall,
    kp_memclear,
    kp_memadd,
    kp_memsubtract,
    kp_memmultiply,
    kp_memdivide,
    kp_plusminus,
    kp_clear,
    kp_clearentry,
    kp_binary,
    kp_octal,
    kp_decimal,
    kp_hexadecimal,
    lctrl,
    lshift,
    lalt,
    lgui,
    rctrl,
    rshift,
    ralt,
    rgui,
    mode,
    audionext,
    audioprev,
    audiostop,
    audioplay,
    audiomute,
    mediaselect,
    www,
    mail,
    calculator,
    computer,
    ac_search,
    ac_home,
    ac_back,
    ac_forward,
    ac_stop,
    ac_refresh,
    ac_bookmarks,
    brightnessdown,
    brightnessup,
    displayswitch,
    kbdillumtoggle,
    kbdillumdown,
    kbdillumup,
    eject,
    sleep,
    app1,
    app2,
    audiorewind,
    audiofastforward,
};


struct Stroke
{
    Key key;
    Meta meta;

    constexpr Stroke(Key k, const Meta& m)
        : key(k)
        , meta(m)
    {
    }
};

bool operator<(const Stroke& lhs, const Stroke& rhs);

std::optional<Stroke> stroke_from_string(const std::string& data);
std::string to_string(const Stroke& stroke);

std::optional<Key> key_from_string(const std::string& str);
std::string to_string(Key key);


}

