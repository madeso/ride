#include "style.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

namespace switcher {

SwitcherStyle::SwitcherStyle()
    : row_count_(12),
      text_margin_x_(4),
      text_margin_y_(2),
      background_color_(*wxRED),
      text_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)),
      selection_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT)),
      selection_outline_color_(
          wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)),
      item_font_(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT)) {}

void SwitcherStyle::set_text_margin_x(int margin) { text_margin_x_ = margin; }

int SwitcherStyle::text_margin_x() const { return text_margin_x_; }

void SwitcherStyle::set_text_margin_y(int margin) { text_margin_y_ = margin; }

int SwitcherStyle::text_margin_y() const { return text_margin_y_; }

void SwitcherStyle::set_row_count(int rows) { row_count_ = rows; }

int SwitcherStyle::row_count() const { return row_count_; }

void SwitcherStyle::set_background_color(const wxColour& colour) {
  background_color_ = colour;
}

const wxColour& SwitcherStyle::background_color() const {
  return background_color_;
}

void SwitcherStyle::set_text_color(const wxColour& colour) {
  text_color_ = colour;
}

const wxColour& SwitcherStyle::text_color() const { return text_color_; }

void SwitcherStyle::set_selection_color(const wxColour& colour) {
  selection_color_ = colour;
}

const wxColour& SwitcherStyle::selection_color() const {
  return selection_color_;
}

void SwitcherStyle::set_selection_outline_color(const wxColour& colour) {
  selection_outline_color_ = colour;
}

const wxColour& SwitcherStyle::selection_outline_color() const {
  return selection_outline_color_;
}

void SwitcherStyle::set_item_font(const wxFont& font) { item_font_ = font; }

const wxFont& SwitcherStyle::item_font() const { return item_font_; }

int SwitcherStyle::x_margin() const { return 0; }

int SwitcherStyle::y_margin() const { return 0; }

int SwitcherStyle::row_spacing() const { return 0; }

int SwitcherStyle::col_spacing() const { return 0; }

int SwitcherStyle::item_maxwidth() const { return 300; }

int SwitcherStyle::item_maxheight() const { return 40; }

int SwitcherStyle::dlg_main_border() const { return 8; }

int SwitcherStyle::dlg_item_border() const { return 3; }

wxColor SwitcherStyle::border_color() const { return *wxBLACK; }

}  // switcher
