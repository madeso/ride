#ifndef RIDE_SWITCHERGROUP_H_
#define RIDE_SWITCHERGROUP_H_

#include <ride/wx.h>

#include <vector>

#include "item.h"

namespace switcher {

class Style;

class Group {
 public:
  explicit Group(const wxString& title);
  Group(const wxString& title, const std::vector<Item>& items);

  void set_title(const wxString& title);
  const wxString& title() const;

  Group& set_rect(const wxRect& rect);
  const wxRect& rect() const;

  Item& AddItem(const Item& item);

  // Find the index for the item associated with the current focus
  int GetIndexForFocus() const;

  // Hit test, returning an index or -1
  int HitTest(const wxPoint& pt) const;

  const Item& GetItem(int i) const;
  Item& GetItem(int i);

  int GetItemCount() const;

  void PaintItems(wxDC* dc, const Style& style, int selection);
  void CalculateItemSize(wxDC* dc, const Style& style, wxSize* sz);

 protected:
  std::vector<Item> items_;
  wxRect rect_;
  wxString title_;
};

}  // switcher

#endif  // RIDE_SWITCHERGROUP_H_
