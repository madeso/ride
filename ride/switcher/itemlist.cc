#include "itemlist.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

#include "style.h"

SwitcherItemList::SwitcherItemList() : column_count_(0) {}

SwitcherGroup& SwitcherItemList::AddGroup(const SwitcherGroup& item) {
  items_.push_back(item);
  return *items_.rbegin();
}

SwitcherIndex SwitcherItemList::GetIndexForFocus() const {
  for (size_t i = 0; i < items_.size(); i++) {
    int item = items_[i].GetIndexForFocus();
    if (item != -1) {
      return SwitcherIndex(i, item);
    }
  }

  return SWITCHER_NOT_FOUND;
}

SwitcherIndex SwitcherItemList::HitTest(const wxPoint& pt) const {
  for (size_t i = 0; i < items_.size(); i++) {
    int item = items_[i].HitTest(pt);
    if (item != -1) {
      std::make_pair(i, item);
    }
  }

  return SWITCHER_NOT_FOUND;
}

#define GET_ITEM()                              \
  do {                                          \
    return GetGroup(i.first).GetItem(i.second); \
  } while (false)

const SwitcherItem& SwitcherItemList::GetItem(SwitcherIndex i) const {
  GET_ITEM();
}

SwitcherItem& SwitcherItemList::GetItem(SwitcherIndex i) { GET_ITEM(); }

#undef GET_ITEM

int SwitcherItemList::GetItemCount() const {
  int count = 0;
  for (const SwitcherGroup& group : items_) {
    count += group.GetItemCount();
  }
  return count;
}

#define GET_GROUP()            \
  do {                         \
    assert(i >= 0);            \
    assert(i < items_.size()); \
    return items_[i];          \
  } while (false)
const SwitcherGroup& SwitcherItemList::GetGroup(int i) const { GET_GROUP(); }
SwitcherGroup& SwitcherItemList::GetGroup(int i) { GET_GROUP(); }
#undef GET_GROUP

int SwitcherItemList::GetGroupCount() const { return items_.size(); }

void SwitcherItemList::set_column_count(int cols) { column_count_ = cols; }

int SwitcherItemList::column_count() const { return column_count_; }

void SwitcherItemList::PaintItems(wxDC* dc, const SwitcherStyle& style,
                                  SwitcherIndex selection, wxWindow* win) {
  dc->SetLogicalFunction(wxCOPY);
  dc->SetBrush(wxBrush(style.background_color()));
  dc->SetPen(*wxTRANSPARENT_PEN);
  dc->DrawRectangle(win->GetClientRect());
  dc->SetBackgroundMode(wxTRANSPARENT);

  for (size_t i = 0; i < items_.size(); i++) {
    items_[i].PaintItems(dc, style,
                         i == selection.first ? selection.second : -1);
  }
}

wxSize SwitcherItemList::CalculateItemSize(wxDC* dc,
                                           const SwitcherStyle& style) {
  // Start off allowing for an icon
  wxSize sz(150, 16);

  for (SwitcherGroup& item : items_) {
    item.CalculateItemSize(dc, style, &sz);
  }

  if (sz == wxSize(16, 16)) {
    sz = wxSize(100, 25);
  } else {
    sz.x += style.col_spacing() * 2;
    sz.y += style.row_spacing() * 2;
  }

  return sz;
}
