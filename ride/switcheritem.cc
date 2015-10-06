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

SwitcherItem::SwitcherItem()
    : id_(0), row_pos_(0), col_pos_(0), window_(NULL) {}

SwitcherItem::SwitcherItem(const wxString& title, const wxString& name, int id,
                           const wxBitmap& bitmap)
    : title_(title),
      name_(name),
      id_(id),
      bitmap_(bitmap),
      row_pos_(0),
      col_pos_(0),
      window_(NULL) {}

bool SwitcherItem::operator==(const SwitcherItem& rhs) const {
#define EQUAL_CHECK(mem)        \
  do {                          \
    if (this->mem != rhs.mem) { \
      return false;             \
    }                           \
  } while (false)
  EQUAL_CHECK(title_);
  EQUAL_CHECK(name_);
  EQUAL_CHECK(description_);
  EQUAL_CHECK(path_);
  EQUAL_CHECK(id_);
  EQUAL_CHECK(rect_);
  EQUAL_CHECK(row_pos_);
  EQUAL_CHECK(col_pos_);
  EQUAL_CHECK(window_);
  return true;
#undef EQUAL_CHECK
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

SwitcherItem& SwitcherItem::set_path(const wxString& path) {
  path_ = path;
  return (*this);
}

const wxString& SwitcherItem::path() const { return path_; }

SwitcherItem& SwitcherItem::set_id(int id) {
  id_ = id;
  return (*this);
}

int SwitcherItem::id() const { return id_; }

SwitcherItem& SwitcherItem::set_rect(const wxRect& rect) {
  rect_ = rect;
  return (*this);
}

const wxRect& SwitcherItem::rect() const { return rect_; }

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
