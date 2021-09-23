#pragma once

#include "base/color.h"
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
};
