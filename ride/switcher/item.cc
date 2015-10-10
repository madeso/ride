#include "item.h"

namespace switcher {

Item::Item()
    : id_(0), row_pos_(0), col_pos_(0), window_(NULL) {}

Item::Item(const wxString& title, const wxString& name, int id,
                           const wxBitmap& bitmap)
    : title_(title),
      name_(name),
      id_(id),
      bitmap_(bitmap),
      row_pos_(0),
      col_pos_(0),
      window_(NULL) {}

bool Item::operator==(const Item& rhs) const {
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

Item& Item::set_title(const wxString& title) {
  title_ = title;
  return (*this);
}

const wxString& Item::title() const { return title_; }

Item& Item::set_name(const wxString& name) {
  name_ = name;
  return (*this);
}

const wxString& Item::name() const { return name_; }

Item& Item::set_description(const wxString& descr) {
  description_ = descr;
  return (*this);
}

const wxString& Item::description() const { return description_; }

Item& Item::set_path(const wxString& path) {
  path_ = path;
  return (*this);
}

const wxString& Item::path() const { return path_; }

Item& Item::set_id(int id) {
  id_ = id;
  return (*this);
}

int Item::id() const { return id_; }

Item& Item::set_rect(const wxRect& rect) {
  rect_ = rect;
  return (*this);
}

const wxRect& Item::rect() const { return rect_; }

Item& Item::set_bitmap(const wxBitmap& bitmap) {
  bitmap_ = bitmap;
  return (*this);
}

const wxBitmap& Item::bitmap() const { return bitmap_; }

Item& Item::set_row_pos(int pos) {
  row_pos_ = pos;
  return (*this);
}

int Item::row_pos() const { return row_pos_; }

Item& Item::set_col_pos(int pos) {
  col_pos_ = pos;
  return (*this);
}

int Item::get_col_pos() const { return col_pos_; }

Item& Item::set_window(wxWindow* win) {
  window_ = win;
  return (*this);
}

wxWindow* Item::window() const { return window_; }

}  // switcher
