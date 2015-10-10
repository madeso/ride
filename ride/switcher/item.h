#ifndef RIDE_SWITCHERITEM_H_
#define RIDE_SWITCHERITEM_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

namespace switcher {

class Item : public wxObject {
 public:
  Item();
  Item(const wxString& title, const wxString& name, int id = 0,
               const wxBitmap& bitmap = wxNullBitmap);

  bool operator==(const Item& item) const;

  Item& set_title(const wxString& title);
  const wxString& title() const;

  Item& set_name(const wxString& name);
  const wxString& name() const;

  Item& set_description(const wxString& descr);
  const wxString& description() const;

  Item& set_path(const wxString& descr);
  const wxString& path() const;

  Item& set_id(int id);
  int id() const;

  Item& set_rect(const wxRect& rect);
  const wxRect& rect() const;

  Item& set_bitmap(const wxBitmap& bitmap);
  const wxBitmap& bitmap() const;

  Item& set_row_pos(int pos);
  int row_pos() const;

  Item& set_col_pos(int pos);
  int get_col_pos() const;

  Item& set_window(wxWindow* win);
  wxWindow* window() const;

 protected:
  wxString title_;
  wxString name_;
  wxString description_;
  wxString path_;
  int id_;
  wxRect rect_;
  wxBitmap bitmap_;
  int row_pos_;
  int col_pos_;
  wxWindow* window_;
};

}  // switcher

#endif  // RIDE_SWITCHERITEM_H_
