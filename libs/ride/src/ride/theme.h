#pragma once

#include "core/color.h"
#include "core/side.h"

#include "core/units.h"

#include "core/color.open-color.h"



namespace ride::libride
{

struct Theme
{
    Dp line_spacing = Dp{3};
    Dp gutter_spacing_left = Dp{3};
    Dp gutter_spacing_right = Dp{3};
    Dp text_spacing = Dp{3};

    Dp horizontal_scroll = 21_dp;
    Dp vertical_scroll = 41_dp;

    Dp scrollbar_width = Dp{12};

    Color window_background_color = colors::white;
    Color logo_color = colors::gray_500;
    Color gutter_color = colors::gray_800;
    Color plain_text_color = colors::black;

    Color gutter_background = colors::gray_300;
    Color edit_background = Color::with_alpha(colors::white, 190);

    Color scroll_through_color = colors::gray_300;
    Color scroll_thumb_color = colors::gray_500;
    Color scroll_button_color = colors::gray_600;

    Color filesys_hidden_color = colors::gray_500;
    Color filesys_file_color = colors::gray_900;
    Color filesys_folder_color = colors::blue_900;
    Color filesys_background_color = Color::with_alpha(colors::white, 190);
    Color filesys_hover_color = colors::gray_200;

    Color selection_background = colors::blue_200;

    Color current_line_background = colors::yellow_200;

    Color caret_color = colors::black;
    Dp caret_width = 1_dp;

    int half_blink_period = 10;

    bool highlight_current_line = true;

    Dp filesys_indent = 12_dp;
    Dp filesys_left_padding = 6_dp;

    Dp miminal_scroll_offset = 40_dp; // pixels to keep above and below curser

    bool sort_files = true;
    bool directories_first = true;

    bool gutter_side_default = true;
    bool horizontal_scroll_side_default = true;
    bool vertical_scroll_side_default = true;

    bool color_inactive_view = false;
    Color inactive_view_color = Color::with_alpha(colors::black, 20);
};

Side get_gutter_side(const Theme& theme);
Side get_horizontal_scroll_side(const Theme& theme);
Side get_vertical_scroll_side(const Theme& theme);

}

