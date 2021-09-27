#pragma once

#include "base/color.h"
#include "base/side.h"

#include "api/units.h"

struct Theme
{
    pix line_spacing = pix{3};
    pix gutter_spacing_left = pix{3};
    pix gutter_spacing_right = pix{3};
    pix text_spacing = pix{3};

    pix horizontal_scroll = 21_px;
    pix vertical_scroll = 41_px;

    pix scrollbar_width = pix{12};

    Color window_background_color = {255, 255, 255, 255};
    Color logo_color = {200, 200, 200, 255};
    Color gutter_color = {0, 0, 0, 255};
    Color plain_text_color = {0, 0, 0, 255};

    Color gutter_background = {150, 150, 150, 255};
    Color edit_background = {230, 230, 230, 190};

    Color scroll_through_color = {100, 100, 100, 255};
    Color scroll_thumb_color = {200, 200, 200, 255};
    Color scroll_button_color = {150, 150, 150, 255};

    Color filesys_hidden_color = {100, 100, 100, 255};
    Color filesys_file_color = {0, 0, 0, 255};
    Color filesys_folder_color = {0, 0, 255, 255};
    Color filesys_background_color = {230, 230, 230, 190};
    Color filesys_hover_color = {200, 200, 200, 190};

    Color selection_background = {90, 90, 255, 190};

    Color current_line_background = {255, 255, 200, 200};

    Color caret_color = {0, 0, 0, 255};
    pix caret_width = 1_px;

    int half_blink_period = 10;

    bool highlight_current_line = true;

    pix filesys_indent = 12_px;
    pix filesys_left_padding = 6_px;

    bool sort_files = true;
    bool directories_first = true;

    bool gutter_side_default = true;
    bool horizontal_scroll_side_default = true;
    bool vertical_scroll_side_default = true;
};

Side get_gutter_side(const Theme& theme);
Side get_horizontal_scroll_side(const Theme& theme);
Side get_vertical_scroll_side(const Theme& theme);
