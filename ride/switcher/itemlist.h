/////////////////////////////////////////////////////////////////////////////
// Name:        switcherdlg.h
// Purpose:     Pane switcher dialog
// Author:      Julian Smart
// Modified by:
// Created:     2007-08-19
// RCS-ID:      $Id: switcherdlg.h,v 1.3 2007/08/20 17:38:24 anthemion Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef RIDE_SWITCHERITEMLIST_H_
#define RIDE_SWITCHERITEMLIST_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>
#include <utility>

#include "ride/switcher/item.h"
#include "ride/switcher/group.h"

typedef std::pair<int, int> SwitcherIndex;
#define SWITCHER_NOT_FOUND std::pair<int, int>(-1, -1)

class SwitcherItemList {
 public:
  SwitcherItemList();

  SwitcherGroup& AddGroup(const SwitcherGroup& group);

  // Find the index for the item associated with the current focus
  SwitcherIndex GetIndexForFocus() const;

  // Hit test, returning an index or -1
  SwitcherIndex HitTest(const wxPoint& pt) const;

  const SwitcherItem& GetItem(SwitcherIndex i) const;
  SwitcherItem& GetItem(SwitcherIndex i);

  int GetItemCount() const;

  const SwitcherGroup& GetGroup(int i) const;
  SwitcherGroup& GetGroup(int i);
  int GetGroupCount() const;

  void set_column_count(int cols);
  int column_count() const;

  void PaintItems(wxDC* dc, const SwitcherStyle& style, SwitcherIndex selection,
                  wxWindow* win);
  wxSize CalculateItemSize(wxDC* dc, const SwitcherStyle& style);

 protected:
  std::vector<SwitcherGroup> items_;
  int column_count_;
};

#endif  // RIDE_SWITCHERITEMLIST_H_
