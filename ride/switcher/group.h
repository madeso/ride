#ifndef RIDE_SWITCHERGROUP_H_
#define RIDE_SWITCHERGROUP_H_

#include <ride/wx.h>

#include <vector>

#include "item.h"

namespace switcher {

class SwitcherStyle;

class SwitcherGroup {
 public:
  explicit SwitcherGroup(const wxString& title);
  SwitcherGroup(const wxString& title, const std::vector<SwitcherItem>& items);

  void set_title(const wxString& title);
  const wxString& title() const;

  SwitcherGroup& set_rect(const wxRect& rect);
  const wxRect& rect() const;

  SwitcherItem& AddItem(const SwitcherItem& item);

  // Find the index for the item associated with the current focus
  int GetIndexForFocus() const;

  // Hit test, returning an index or -1
  int HitTest(const wxPoint& pt) const;

  const SwitcherItem& GetItem(int i) const;
  SwitcherItem& GetItem(int i);

  int GetItemCount() const;

  void PaintItems(wxDC* dc, const SwitcherStyle& style, int selection);
  void CalculateItemSize(wxDC* dc, const SwitcherStyle& style, wxSize* sz);

 protected:
  std::vector<SwitcherItem> items_;
  wxRect rect_;
  wxString title_;
};

}  // switcher

#endif  // RIDE_SWITCHERGROUP_H_
