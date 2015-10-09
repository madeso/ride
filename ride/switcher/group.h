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

#ifndef RIDE_SWITCHERGROUP_H_
#define RIDE_SWITCHERGROUP_H_

#include <ride/wx.h>

#include <vector>

#include "ride/switcher/item.h"

class SwitcherStyle;

/*!
 * wxSwitcherItems
 * An object containing switcher items
 */

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

#endif  // RIDE_SWITCHERGROUP_H_
