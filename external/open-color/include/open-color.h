#pragma once

// open_color.h provides minimalistic RGB color definition from Open color
// license: MIT
//
// https://github.com/yeun/open-color/blob/master/open-color.js
// conveted from https://github.com/yeun/open-color/blob/master/open-color.js

#include <array>
#include <cstdint>

namespace open_color
{
    using Hex = std::uint32_t;
    using Hexs = std::array<Hex, 10>;

    constexpr Hex white = 0xffffff;
    constexpr Hex black = 0x000000;

    constexpr Hex gray_50  = 0xf8f9fa;
    constexpr Hex gray_100 = 0xf1f3f5;
    constexpr Hex gray_200 = 0xe9ecef;
    constexpr Hex gray_300 = 0xdee2e6;
    constexpr Hex gray_400 = 0xced4da;
    constexpr Hex gray_500 = 0xadb5bd;
    constexpr Hex gray_600 = 0x868e96;
    constexpr Hex gray_700 = 0x495057;
    constexpr Hex gray_800 = 0x343a40;
    constexpr Hex gray_900 = 0x212529;

    constexpr Hex red_50  = 0xfff5f5;
    constexpr Hex red_100 = 0xffe3e3;
    constexpr Hex red_200 = 0xffc9c9;
    constexpr Hex red_300 = 0xffa8a8;
    constexpr Hex red_400 = 0xff8787;
    constexpr Hex red_500 = 0xff6b6b;
    constexpr Hex red_600 = 0xfa5252;
    constexpr Hex red_700 = 0xf03e3e;
    constexpr Hex red_800 = 0xe03131;
    constexpr Hex red_900 = 0xc92a2a;

    constexpr Hex pink_50  = 0xfff0f6;
    constexpr Hex pink_100 = 0xffdeeb;
    constexpr Hex pink_200 = 0xfcc2d7;
    constexpr Hex pink_300 = 0xfaa2c1;
    constexpr Hex pink_400 = 0xf783ac;
    constexpr Hex pink_500 = 0xf06595;
    constexpr Hex pink_600 = 0xe64980;
    constexpr Hex pink_700 = 0xd6336c;
    constexpr Hex pink_800 = 0xc2255c;
    constexpr Hex pink_900 = 0xa61e4d;

    constexpr Hex grape_50  = 0xf8f0fc;
    constexpr Hex grape_100 = 0xf3d9fa;
    constexpr Hex grape_200 = 0xeebefa;
    constexpr Hex grape_300 = 0xe599f7;
    constexpr Hex grape_400 = 0xda77f2;
    constexpr Hex grape_500 = 0xcc5de8;
    constexpr Hex grape_600 = 0xbe4bdb;
    constexpr Hex grape_700 = 0xae3ec9;
    constexpr Hex grape_800 = 0x9c36b5;
    constexpr Hex grape_900 = 0x862e9c;

    constexpr Hex violet_50  = 0xf3f0ff;
    constexpr Hex violet_100 = 0xe5dbff;
    constexpr Hex violet_200 = 0xd0bfff;
    constexpr Hex violet_300 = 0xb197fc;
    constexpr Hex violet_400 = 0x9775fa;
    constexpr Hex violet_500 = 0x845ef7;
    constexpr Hex violet_600 = 0x7950f2;
    constexpr Hex violet_700 = 0x7048e8;
    constexpr Hex violet_800 = 0x6741d9;
    constexpr Hex violet_900 = 0x5f3dc4;

    constexpr Hex indigo_50  = 0xedf2ff;
    constexpr Hex indigo_100 = 0xdbe4ff;
    constexpr Hex indigo_200 = 0xbac8ff;
    constexpr Hex indigo_300 = 0x91a7ff;
    constexpr Hex indigo_400 = 0x748ffc;
    constexpr Hex indigo_500 = 0x5c7cfa;
    constexpr Hex indigo_600 = 0x4c6ef5;
    constexpr Hex indigo_700 = 0x4263eb;
    constexpr Hex indigo_800 = 0x3b5bdb;
    constexpr Hex indigo_900 = 0x364fc7;

    constexpr Hex blue_50  = 0xe7f5ff;
    constexpr Hex blue_100 = 0xd0ebff;
    constexpr Hex blue_200 = 0xa5d8ff;
    constexpr Hex blue_300 = 0x74c0fc;
    constexpr Hex blue_400 = 0x4dabf7;
    constexpr Hex blue_500 = 0x339af0;
    constexpr Hex blue_600 = 0x228be6;
    constexpr Hex blue_700 = 0x1c7ed6;
    constexpr Hex blue_800 = 0x1971c2;
    constexpr Hex blue_900 = 0x1864ab;

    constexpr Hex cyan_50 = 0xe3fafc;
    constexpr Hex cyan_100 = 0xc5f6fa;
    constexpr Hex cyan_200 = 0x99e9f2;
    constexpr Hex cyan_300 = 0x66d9e8;
    constexpr Hex cyan_400 = 0x3bc9db;
    constexpr Hex cyan_500 = 0x22b8cf;
    constexpr Hex cyan_600 = 0x15aabf;
    constexpr Hex cyan_700 = 0x1098ad;
    constexpr Hex cyan_800 = 0x0c8599;
    constexpr Hex cyan_900 = 0x0b7285;

    constexpr Hex teal_50 = 0xe6fcf5;
    constexpr Hex teal_100 = 0xc3fae8;
    constexpr Hex teal_200 = 0x96f2d7;
    constexpr Hex teal_300 = 0x63e6be;
    constexpr Hex teal_400 = 0x38d9a9;
    constexpr Hex teal_500 = 0x20c997;
    constexpr Hex teal_600 = 0x12b886;
    constexpr Hex teal_700 = 0x0ca678;
    constexpr Hex teal_800 = 0x099268;
    constexpr Hex teal_900 = 0x087f5b;

    constexpr Hex green_50 = 0xebfbee;
    constexpr Hex green_100 = 0xd3f9d8;
    constexpr Hex green_200 = 0xb2f2bb;
    constexpr Hex green_300 = 0x8ce99a;
    constexpr Hex green_400 = 0x69db7c;
    constexpr Hex green_500 = 0x51cf66;
    constexpr Hex green_600 = 0x40c057;
    constexpr Hex green_700 = 0x37b24d;
    constexpr Hex green_800 = 0x2f9e44;
    constexpr Hex green_900 = 0x2b8a3e;

    constexpr Hex lime_50  = 0xf4fce3;
    constexpr Hex lime_100 = 0xe9fac8;
    constexpr Hex lime_200 = 0xd8f5a2;
    constexpr Hex lime_300 = 0xc0eb75;
    constexpr Hex lime_400 = 0xa9e34b;
    constexpr Hex lime_500 = 0x94d82d;
    constexpr Hex lime_600 = 0x82c91e;
    constexpr Hex lime_700 = 0x74b816;
    constexpr Hex lime_800 = 0x66a80f;
    constexpr Hex lime_900 = 0x5c940d;

    constexpr Hex yellow_50  = 0xfff9db;
    constexpr Hex yellow_100 = 0xfff3bf;
    constexpr Hex yellow_200 = 0xffec99;
    constexpr Hex yellow_300 = 0xffe066;
    constexpr Hex yellow_400 = 0xffd43b;
    constexpr Hex yellow_500 = 0xfcc419;
    constexpr Hex yellow_600 = 0xfab005;
    constexpr Hex yellow_700 = 0xf59f00;
    constexpr Hex yellow_800 = 0xf08c00;
    constexpr Hex yellow_900 = 0xe67700;

    constexpr Hex orange_50  = 0xfff4e6;
    constexpr Hex orange_100 = 0xffe8cc;
    constexpr Hex orange_200 = 0xffd8a8;
    constexpr Hex orange_300 = 0xffc078;
    constexpr Hex orange_400 = 0xffa94d;
    constexpr Hex orange_500 = 0xff922b;
    constexpr Hex orange_600 = 0xfd7e14;
    constexpr Hex orange_700 = 0xf76707;
    constexpr Hex orange_800 = 0xe8590c;
    constexpr Hex orange_900 = 0xd9480f;

    constexpr Hexs gray = { gray_50, gray_100, gray_200, gray_300, gray_400, gray_500, gray_600, gray_700, gray_800, gray_900 };
    constexpr Hexs red = { red_50, red_100, red_200, red_300, red_400, red_500, red_600, red_700, red_800, red_900 };
    constexpr Hexs pink = { pink_50, pink_100, pink_200, pink_300, pink_400, pink_500, pink_600, pink_700, pink_800, pink_900 };
    constexpr Hexs grape = { grape_50, grape_100, grape_200, grape_300, grape_400, grape_500, grape_600, grape_700, grape_800, grape_900 };
    constexpr Hexs violet = { violet_50, violet_100, violet_200, violet_300, violet_400, violet_500, violet_600, violet_700, violet_800, violet_900 };
    constexpr Hexs indigo = { indigo_50, indigo_100, indigo_200, indigo_300, indigo_400, indigo_500, indigo_600, indigo_700, indigo_800, indigo_900 };
    constexpr Hexs blue = { blue_50, blue_100, blue_200, blue_300, blue_400, blue_500, blue_600, blue_700, blue_800, blue_900 };
    constexpr Hexs cyan = { cyan_50, cyan_100, cyan_200, cyan_300, cyan_400, cyan_500, cyan_600, cyan_700, cyan_800, cyan_900 };
    constexpr Hexs teal = { teal_50, teal_100, teal_200, teal_300, teal_400, teal_500, teal_600, teal_700, teal_800, teal_900 };
    constexpr Hexs green = { green_50, green_100, green_200, green_300, green_400, green_500, green_600, green_700, green_800, green_900 };
    constexpr Hexs lime = { lime_50, lime_100, lime_200, lime_300, lime_400, lime_500, lime_600, lime_700, lime_800, lime_900 };
    constexpr Hexs yellow = { yellow_50, yellow_100, yellow_200, yellow_300, yellow_400, yellow_500, yellow_600, yellow_700, yellow_800, yellow_900 };
    constexpr Hexs orange = { orange_50, orange_100, orange_200, orange_300, orange_400, orange_500, orange_600, orange_700, orange_800, orange_900 };
}

