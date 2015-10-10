#include "style.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

namespace switcher {

Style::Style()
    : row_count_(12),
      text_margin_x_(4),
      text_margin_y_(2),
      background_color_(*wxRED),
      text_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)),
      selection_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT)),
      selection_outline_color_(
          wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)),
      item_font_(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT)) {}

void Style::set_text_margin_x(int margin) { text_margin_x_ = margin; }

int Style::text_margin_x() const { return text_margin_x_; }

void Style::set_text_margin_y(int margin) { text_margin_y_ = margin; }

int Style::text_margin_y() const { return text_margin_y_; }

void Style::set_row_count(int rows) { row_count_ = rows; }

int Style::row_count() const { return row_count_; }

void Style::set_background_color(const wxColour& colour) {
  background_color_ = colour;
}

const wxColour& Style::background_color() const { return background_color_; }

void Style::set_text_color(const wxColour& colour) { text_color_ = colour; }

const wxColour& Style::text_color() const { return text_color_; }

void Style::set_selection_color(const wxColour& colour) {
  selection_color_ = colour;
}

const wxColour& Style::selection_color() const { return selection_color_; }

void Style::set_selection_outline_color(const wxColour& colour) {
  selection_outline_color_ = colour;
}

const wxColour& Style::selection_outline_color() const {
  return selection_outline_color_;
}

void Style::set_item_font(const wxFont& font) { item_font_ = font; }

const wxFont& Style::item_font() const { return item_font_; }

int Style::x_margin() const { return 0; }

int Style::y_margin() const { return 0; }

int Style::row_spacing() const { return 0; }

int Style::col_spacing() const { return 0; }

int Style::item_maxwidth() const { return 300; }

int Style::item_maxheight() const { return 40; }

int Style::dlg_main_border() const { return 8; }

int Style::dlg_item_border() const { return 3; }

wxColor Style::border_color() const { return *wxBLACK; }

}  // switcher
