#include "ride/theme.h"

Side get_gutter_side(const Theme& theme)
{
    return theme.gutter_side_default ? Side::left : Side::right;
}

Side get_horizontal_scroll_side(const Theme& theme)
{
    return theme.horizontal_scroll_side_default ? Side::bottom : Side::top;
}

Side get_vertical_scroll_side(const Theme& theme)
{
    return theme.vertical_scroll_side_default ? Side::right : Side::left;
}
