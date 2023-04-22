#pragma once

#include <array>

#include "open-color.h"

#include "core/color.h"

namespace ride::colors
{
    using ColorList = std::array<Color, 10>;

    constexpr Color white = Color::from_hex(open_color::white);
    constexpr Color black = Color::from_hex(open_color::black);

    constexpr Color gray_50  = Color::from_hex(open_color::gray_50);
    constexpr Color gray_100 = Color::from_hex(open_color::gray_100);
    constexpr Color gray_200 = Color::from_hex(open_color::gray_200);
    constexpr Color gray_300 = Color::from_hex(open_color::gray_300);
    constexpr Color gray_400 = Color::from_hex(open_color::gray_400);
    constexpr Color gray_500 = Color::from_hex(open_color::gray_500);
    constexpr Color gray_600 = Color::from_hex(open_color::gray_600);
    constexpr Color gray_700 = Color::from_hex(open_color::gray_700);
    constexpr Color gray_800 = Color::from_hex(open_color::gray_800);
    constexpr Color gray_900 = Color::from_hex(open_color::gray_900);

    constexpr Color red_50  = Color::from_hex(open_color::red_50);
    constexpr Color red_100 = Color::from_hex(open_color::red_100);
    constexpr Color red_200 = Color::from_hex(open_color::red_200);
    constexpr Color red_300 = Color::from_hex(open_color::red_300);
    constexpr Color red_400 = Color::from_hex(open_color::red_400);
    constexpr Color red_500 = Color::from_hex(open_color::red_500);
    constexpr Color red_600 = Color::from_hex(open_color::red_600);
    constexpr Color red_700 = Color::from_hex(open_color::red_700);
    constexpr Color red_800 = Color::from_hex(open_color::red_800);
    constexpr Color red_900 = Color::from_hex(open_color::red_900);

    constexpr Color pink_50  = Color::from_hex(open_color::pink_50);
    constexpr Color pink_100 = Color::from_hex(open_color::pink_100);
    constexpr Color pink_200 = Color::from_hex(open_color::pink_200);
    constexpr Color pink_300 = Color::from_hex(open_color::pink_300);
    constexpr Color pink_400 = Color::from_hex(open_color::pink_400);
    constexpr Color pink_500 = Color::from_hex(open_color::pink_500);
    constexpr Color pink_600 = Color::from_hex(open_color::pink_600);
    constexpr Color pink_700 = Color::from_hex(open_color::pink_700);
    constexpr Color pink_800 = Color::from_hex(open_color::pink_800);
    constexpr Color pink_900 = Color::from_hex(open_color::pink_900);

    constexpr Color grape_50  = Color::from_hex(open_color::grape_50);
    constexpr Color grape_100 = Color::from_hex(open_color::grape_100);
    constexpr Color grape_200 = Color::from_hex(open_color::grape_200);
    constexpr Color grape_300 = Color::from_hex(open_color::grape_300);
    constexpr Color grape_400 = Color::from_hex(open_color::grape_400);
    constexpr Color grape_500 = Color::from_hex(open_color::grape_500);
    constexpr Color grape_600 = Color::from_hex(open_color::grape_600);
    constexpr Color grape_700 = Color::from_hex(open_color::grape_700);
    constexpr Color grape_800 = Color::from_hex(open_color::grape_800);
    constexpr Color grape_900 = Color::from_hex(open_color::grape_900);

    constexpr Color violet_50  = Color::from_hex(open_color::violet_50);
    constexpr Color violet_100 = Color::from_hex(open_color::violet_100);
    constexpr Color violet_200 = Color::from_hex(open_color::violet_200);
    constexpr Color violet_300 = Color::from_hex(open_color::violet_300);
    constexpr Color violet_400 = Color::from_hex(open_color::violet_400);
    constexpr Color violet_500 = Color::from_hex(open_color::violet_500);
    constexpr Color violet_600 = Color::from_hex(open_color::violet_600);
    constexpr Color violet_700 = Color::from_hex(open_color::violet_700);
    constexpr Color violet_800 = Color::from_hex(open_color::violet_800);
    constexpr Color violet_900 = Color::from_hex(open_color::violet_900);

    constexpr Color indigo_50  = Color::from_hex(open_color::indigo_50);
    constexpr Color indigo_100 = Color::from_hex(open_color::indigo_100);
    constexpr Color indigo_200 = Color::from_hex(open_color::indigo_200);
    constexpr Color indigo_300 = Color::from_hex(open_color::indigo_300);
    constexpr Color indigo_400 = Color::from_hex(open_color::indigo_400);
    constexpr Color indigo_500 = Color::from_hex(open_color::indigo_500);
    constexpr Color indigo_600 = Color::from_hex(open_color::indigo_600);
    constexpr Color indigo_700 = Color::from_hex(open_color::indigo_700);
    constexpr Color indigo_800 = Color::from_hex(open_color::indigo_800);
    constexpr Color indigo_900 = Color::from_hex(open_color::indigo_900);

    constexpr Color blue_50  = Color::from_hex(open_color::blue_50);
    constexpr Color blue_100 = Color::from_hex(open_color::blue_100);
    constexpr Color blue_200 = Color::from_hex(open_color::blue_200);
    constexpr Color blue_300 = Color::from_hex(open_color::blue_300);
    constexpr Color blue_400 = Color::from_hex(open_color::blue_400);
    constexpr Color blue_500 = Color::from_hex(open_color::blue_500);
    constexpr Color blue_600 = Color::from_hex(open_color::blue_600);
    constexpr Color blue_700 = Color::from_hex(open_color::blue_700);
    constexpr Color blue_800 = Color::from_hex(open_color::blue_800);
    constexpr Color blue_900 = Color::from_hex(open_color::blue_900);

    constexpr Color cyan_50 = Color::from_hex(open_color::cyan_50);
    constexpr Color cyan_100 = Color::from_hex(open_color::cyan_100);
    constexpr Color cyan_200 = Color::from_hex(open_color::cyan_200);
    constexpr Color cyan_300 = Color::from_hex(open_color::cyan_300);
    constexpr Color cyan_400 = Color::from_hex(open_color::cyan_400);
    constexpr Color cyan_500 = Color::from_hex(open_color::cyan_500);
    constexpr Color cyan_600 = Color::from_hex(open_color::cyan_600);
    constexpr Color cyan_700 = Color::from_hex(open_color::cyan_700);
    constexpr Color cyan_800 = Color::from_hex(open_color::cyan_800);
    constexpr Color cyan_900 = Color::from_hex(open_color::cyan_900);

    constexpr Color teal_50 = Color::from_hex(open_color::teal_50);
    constexpr Color teal_100 = Color::from_hex(open_color::teal_100);
    constexpr Color teal_200 = Color::from_hex(open_color::teal_200);
    constexpr Color teal_300 = Color::from_hex(open_color::teal_300);
    constexpr Color teal_400 = Color::from_hex(open_color::teal_400);
    constexpr Color teal_500 = Color::from_hex(open_color::teal_500);
    constexpr Color teal_600 = Color::from_hex(open_color::teal_600);
    constexpr Color teal_700 = Color::from_hex(open_color::teal_700);
    constexpr Color teal_800 = Color::from_hex(open_color::teal_800);
    constexpr Color teal_900 = Color::from_hex(open_color::teal_900);

    constexpr Color green_50 = Color::from_hex(open_color::green_50);
    constexpr Color green_100 = Color::from_hex(open_color::green_100);
    constexpr Color green_200 = Color::from_hex(open_color::green_200);
    constexpr Color green_300 = Color::from_hex(open_color::green_300);
    constexpr Color green_400 = Color::from_hex(open_color::green_400);
    constexpr Color green_500 = Color::from_hex(open_color::green_500);
    constexpr Color green_600 = Color::from_hex(open_color::green_600);
    constexpr Color green_700 = Color::from_hex(open_color::green_700);
    constexpr Color green_800 = Color::from_hex(open_color::green_800);
    constexpr Color green_900 = Color::from_hex(open_color::green_900);

    constexpr Color lime_50  = Color::from_hex(open_color::lime_50 );
    constexpr Color lime_100 = Color::from_hex(open_color::lime_100);
    constexpr Color lime_200 = Color::from_hex(open_color::lime_200);
    constexpr Color lime_300 = Color::from_hex(open_color::lime_300);
    constexpr Color lime_400 = Color::from_hex(open_color::lime_400);
    constexpr Color lime_500 = Color::from_hex(open_color::lime_500);
    constexpr Color lime_600 = Color::from_hex(open_color::lime_600);
    constexpr Color lime_700 = Color::from_hex(open_color::lime_700);
    constexpr Color lime_800 = Color::from_hex(open_color::lime_800);
    constexpr Color lime_900 = Color::from_hex(open_color::lime_900);

    constexpr Color yellow_50  = Color::from_hex(open_color::yellow_50 );
    constexpr Color yellow_100 = Color::from_hex(open_color::yellow_100);
    constexpr Color yellow_200 = Color::from_hex(open_color::yellow_200);
    constexpr Color yellow_300 = Color::from_hex(open_color::yellow_300);
    constexpr Color yellow_400 = Color::from_hex(open_color::yellow_400);
    constexpr Color yellow_500 = Color::from_hex(open_color::yellow_500);
    constexpr Color yellow_600 = Color::from_hex(open_color::yellow_600);
    constexpr Color yellow_700 = Color::from_hex(open_color::yellow_700);
    constexpr Color yellow_800 = Color::from_hex(open_color::yellow_800);
    constexpr Color yellow_900 = Color::from_hex(open_color::yellow_900);

    constexpr Color orange_50  = Color::from_hex(open_color::orange_50 );
    constexpr Color orange_100 = Color::from_hex(open_color::orange_100);
    constexpr Color orange_200 = Color::from_hex(open_color::orange_200);
    constexpr Color orange_300 = Color::from_hex(open_color::orange_300);
    constexpr Color orange_400 = Color::from_hex(open_color::orange_400);
    constexpr Color orange_500 = Color::from_hex(open_color::orange_500);
    constexpr Color orange_600 = Color::from_hex(open_color::orange_600);
    constexpr Color orange_700 = Color::from_hex(open_color::orange_700);
    constexpr Color orange_800 = Color::from_hex(open_color::orange_800);
    constexpr Color orange_900 = Color::from_hex(open_color::orange_900);

    constexpr ColorList gray = { gray_50, gray_100, gray_200, gray_300, gray_400, gray_500, gray_600, gray_700, gray_800, gray_900 };
    constexpr ColorList red = { red_50, red_100, red_200, red_300, red_400, red_500, red_600, red_700, red_800, red_900 };
    constexpr ColorList pink = { pink_50, pink_100, pink_200, pink_300, pink_400, pink_500, pink_600, pink_700, pink_800, pink_900 };
    constexpr ColorList grape = { grape_50, grape_100, grape_200, grape_300, grape_400, grape_500, grape_600, grape_700, grape_800, grape_900 };
    constexpr ColorList violet = { violet_50, violet_100, violet_200, violet_300, violet_400, violet_500, violet_600, violet_700, violet_800, violet_900 };
    constexpr ColorList indigo = { indigo_50, indigo_100, indigo_200, indigo_300, indigo_400, indigo_500, indigo_600, indigo_700, indigo_800, indigo_900 };
    constexpr ColorList blue = { blue_50, blue_100, blue_200, blue_300, blue_400, blue_500, blue_600, blue_700, blue_800, blue_900 };
    constexpr ColorList cyan = { cyan_50, cyan_100, cyan_200, cyan_300, cyan_400, cyan_500, cyan_600, cyan_700, cyan_800, cyan_900 };
    constexpr ColorList teal = { teal_50, teal_100, teal_200, teal_300, teal_400, teal_500, teal_600, teal_700, teal_800, teal_900 };
    constexpr ColorList green = { green_50, green_100, green_200, green_300, green_400, green_500, green_600, green_700, green_800, green_900 };
    constexpr ColorList lime = { lime_50, lime_100, lime_200, lime_300, lime_400, lime_500, lime_600, lime_700, lime_800, lime_900 };
    constexpr ColorList yellow = { yellow_50, yellow_100, yellow_200, yellow_300, yellow_400, yellow_500, yellow_600, yellow_700, yellow_800, yellow_900 };
    constexpr ColorList orange = { orange_50, orange_100, orange_200, orange_300, orange_400, orange_500, orange_600, orange_700, orange_800, orange_900 };
}
