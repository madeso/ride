#ifndef RIDE_SWITCHERITEMLIST_H_
#define RIDE_SWITCHERITEMLIST_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>
#include <utility>

#include "item.h"
#include "group.h"

namespace switcher {

typedef std::pair<int, int> Index;
#define SWITCHER_NOT_FOUND std::pair<int, int>(-1, -1)

class ItemList {
 public:
  ItemList();

  Group& AddGroup(const Group& group);

  // Find the index for the item associated with the current focus
  Index GetIndexForFocus() const;

  // Hit test, returning an index or -1
  Index HitTest(const wxPoint& pt) const;

  const Item& GetItem(Index i) const;
  Item& GetItem(Index i);

  int GetItemCount() const;

  const Group& GetGroup(int i) const;
  Group& GetGroup(int i);
  int GetGroupCount() const;

  void set_column_count(int cols);
  int column_count() const;

  void PaintItems(wxDC* dc, const Style& style, Index selection, wxWindow* win);
  wxSize CalculateItemSize(wxDC* dc, const Style& style);

 protected:
  std::vector<Group> items_;
  int column_count_;
};

}  // switcher

#endif  // RIDE_SWITCHERITEMLIST_H_
