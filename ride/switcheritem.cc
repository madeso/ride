/////////////////////////////////////////////////////////////////////////////
// Name:        switcherdlg.h
// Purpose:     Pane switcher dialog
// Author:      Julian Smart
// Modified by:
// Created:     2007-08-19
// RCS-ID:      $Id: switcherdlg.cpp,v 1.6 2007/08/20 17:38:24 anthemion Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "ride/switcheritem.h"

IMPLEMENT_CLASS(SwitcherItem, wxObject)

SwitcherItem::SwitcherItem() { Init(); }
SwitcherItem::SwitcherItem(const SwitcherItem& item) {
  Init();
  Copy(item);
}

bool SwitcherItem::operator==(const SwitcherItem& item) const {
  return (id_ == item.id_ && name_ == item.name_ && title_ == item.title_ &&
          description_ == item.description_ && is_group_ == item.is_group_ &&
          break_column_ == item.break_column_ && rect_ == item.rect_ &&
          font_ == item.font_ && text_color_ == item.text_color_ &&
          col_pos_ == item.col_pos_ && row_pos_ == item.row_pos_ &&
          window_ == item.window_);
}

void SwitcherItem::operator=(const SwitcherItem& item) { Copy(item); }

void SwitcherItem::Init() {
  id_ = 0;
  is_group_ = false;
  break_column_ = false;
  row_pos_ = 0;
  col_pos_ = 0;
  window_ = NULL;
}

void SwitcherItem::Copy(const SwitcherItem& item) {
  id_ = item.id_;
  name_ = item.name_;
  title_ = item.title_;
  is_group_ = item.is_group_;
  break_column_ = item.break_column_;
  rect_ = item.rect_;
  font_ = item.font_;
  text_color_ = item.text_color_;
  bitmap_ = item.bitmap_;
  description_ = item.description_;
  row_pos_ = item.row_pos_;
  col_pos_ = item.col_pos_;
  window_ = item.window_;
}

SwitcherItem& SwitcherItem::set_title(const wxString& title) {
  title_ = title;
  return (*this);
}
const wxString& SwitcherItem::title() const { return title_; }

SwitcherItem& SwitcherItem::set_name(const wxString& name) {
  name_ = name;
  return (*this);
}
const wxString& SwitcherItem::name() const { return name_; }

SwitcherItem& SwitcherItem::set_description(const wxString& descr) {
  description_ = descr;
  return (*this);
}
const wxString& SwitcherItem::description() const { return description_; }

SwitcherItem& SwitcherItem::set_id(int id) {
  id_ = id;
  return (*this);
}
int SwitcherItem::id() const { return id_; }

SwitcherItem& SwitcherItem::set_is_group(bool is_group) {
  is_group_ = is_group;
  return (*this);
}
bool SwitcherItem::is_group() const { return is_group_; }

SwitcherItem& SwitcherItem::set_break_column(bool break_column) {
  break_column_ = break_column;
  return (*this);
}
bool SwitcherItem::break_column() const { return break_column_; }

SwitcherItem& SwitcherItem::set_rect(const wxRect& rect) {
  rect_ = rect;
  return (*this);
}
const wxRect& SwitcherItem::rect() const { return rect_; }

SwitcherItem& SwitcherItem::set_text_color(const wxColour& colour) {
  text_color_ = colour;
  return (*this);
}
const wxColour& SwitcherItem::text_color() const { return text_color_; }

SwitcherItem& SwitcherItem::set_font(const wxFont& font) {
  font_ = font;
  return (*this);
}
const wxFont& SwitcherItem::get_font() const { return font_; }

SwitcherItem& SwitcherItem::set_bitmap(const wxBitmap& bitmap) {
  bitmap_ = bitmap;
  return (*this);
}
const wxBitmap& SwitcherItem::bitmap() const { return bitmap_; }

SwitcherItem& SwitcherItem::set_row_pos(int pos) {
  row_pos_ = pos;
  return (*this);
}
int SwitcherItem::row_pos() const { return row_pos_; }

SwitcherItem& SwitcherItem::set_col_pos(int pos) {
  col_pos_ = pos;
  return (*this);
}
int SwitcherItem::get_col_pos() const { return col_pos_; }

SwitcherItem& SwitcherItem::set_window(wxWindow* win) {
  window_ = win;
  return (*this);
}

wxWindow* SwitcherItem::window() const { return window_; }
