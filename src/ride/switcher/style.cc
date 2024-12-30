#include "style.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

namespace switcher
{

Style::Style()
	: row_count_(12)
	, text_margin_x_(4)
	, text_margin_y_(2)
	, background_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE))
	, text_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT))
	, selection_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT))
	, selection_outline_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT))
	, item_font_(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT))
	, x_margin_(6)
	, y_margin_(6)
	, row_spacing_(1)
	, col_spacing_(1)
	, item_maxwidth_(300)
	, item_maxheight_(40)
	, dlg_main_border_(8)
	, dlg_item_border_(3)
	, dialog_color_(*wxBLACK)
{
}

void Style::set_text_margin_x(int margin)
{
	text_margin_x_ = margin;
}

int Style::text_margin_x() const
{
	return text_margin_x_;
}

void Style::set_text_margin_y(int margin)
{
	text_margin_y_ = margin;
}

int Style::text_margin_y() const
{
	return text_margin_y_;
}

void Style::set_row_count(int rows)
{
	row_count_ = rows;
}

int Style::row_count() const
{
	return row_count_;
}

void Style::set_background_color(const wxColour& colour)
{
	background_color_ = colour;
}

const wxColour& Style::background_color() const
{
	return background_color_;
}

void Style::set_text_color(const wxColour& colour)
{
	text_color_ = colour;
}

const wxColour& Style::text_color() const
{
	return text_color_;
}

void Style::set_selection_color(const wxColour& colour)
{
	selection_color_ = colour;
}

const wxColour& Style::selection_color() const
{
	return selection_color_;
}

void Style::set_selection_outline_color(const wxColour& colour)
{
	selection_outline_color_ = colour;
}

const wxColour& Style::selection_outline_color() const
{
	return selection_outline_color_;
}

void Style::set_item_font(const wxFont& font)
{
	item_font_ = font;
}

const wxFont& Style::item_font() const
{
	return item_font_;
}

int Style::x_margin() const
{
	return x_margin_;
}

int Style::y_margin() const
{
	return y_margin_;
}

void Style::set_x_margin(int x)
{
	x_margin_ = x;
}

void Style::set_y_margin(int y)
{
	y_margin_ = y;
}

int Style::row_spacing() const
{
	return row_spacing_;
}

int Style::col_spacing() const
{
	return col_spacing_;
}

void Style::set_row_spacing(int spacing)
{
	row_spacing_ = spacing;
}

void Style::set_col_spacing(int spacing)
{
	col_spacing_ = spacing;
}

int Style::item_maxwidth() const
{
	return item_maxwidth_;
}

int Style::item_maxheight() const
{
	return item_maxheight_;
}

void Style::set_item_maxwidth(int max)
{
	item_maxwidth_ = max;
}

void Style::set_item_maxheight(int max)
{
	item_maxheight_ = max;
}

int Style::dlg_main_border() const
{
	return dlg_main_border_;
}

int Style::dlg_item_border() const
{
	return dlg_item_border_;
}

void Style::set_dlg_main_border(int border)
{
	dlg_main_border_ = border;
}

void Style::set_dlg_item_border(int border)
{
	dlg_item_border_ = border;
}

wxColor Style::dialog_color() const
{
	return dialog_color_;
}

void Style::set_dialog_color(wxColor color)
{
	dialog_color_ = color;
}

wxColor Style::base_color() const
{
	return base_color_;
}

void Style::set_base_color(wxColor color)
{
	base_color_ = color;
}

int Style::min_width() const
{
	return min_width_;
}

int Style::min_height() const
{
	return min_height_;
}

void Style::set_min_width(int w)
{
	min_width_ = w;
}

void Style::set_min_height(int h)
{
	min_height_ = h;
}

}  // namespace switcher
