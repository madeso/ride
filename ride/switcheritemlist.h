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

#include "ride/switcheritem.h"

/*!
 * wxSwitcherItems
 * An object containing switcher items
 */

class SwitcherItemList {
 public:
  SwitcherItemList();

  SwitcherItem& AddItem(const wxString& title, const wxString& name, int id = 0,
                        const wxBitmap& bitmap = wxNullBitmap);
  SwitcherItem& AddItem(const SwitcherItem& item);
  SwitcherItem& AddGroup(const wxString& title, const wxString& name,
                         int id = 0, const wxBitmap& bitmap = wxNullBitmap);

  int FindItemByName(const wxString& name) const;
  int FindItemById(int id) const;

  void set_selection(int sel);
  void SelectByName(const wxString& name);
  int selection() const;

  // Find the index for the item associated with the current focus
  int GetIndexForFocus() const;

  // Hit test, returning an index or -1
  int HitTest(const wxPoint& pt) const;

  const SwitcherItem& GetItem(int i) const;
  SwitcherItem& GetItem(int i);

  int GetItemCount() const;

  void set_row_count(int rows);
  int row_count() const;

  void set_column_count(int cols);
  int column_count() const;

  void set_background_color(const wxColour& colour);
  const wxColour& background_color() const;

  void set_text_color(const wxColour& colour);
  const wxColour& text_color() const;

  void set_selection_color(const wxColour& colour);
  const wxColour& selection_color() const;

  void set_selection_outline_color(const wxColour& colour);
  const wxColour& selection_outline_color() const;

  void set_item_font(const wxFont& font);
  const wxFont& item_font() const;

  void PaintItems(wxDC& dc, wxWindow* win);  // NOLINT
  wxSize CalculateItemSize(wxDC& dc);        // NOLINT

 protected:
  std::vector<SwitcherItem> items_;
  int selection_;
  int row_count_;
  int column_count_;

  int text_margin_x_;
  int text_margin_y_;

  wxColour background_color_;
  wxColour text_color_;
  wxColour selection_color_;
  wxColour selection_outline_color_;

  wxFont item_font_;
};

#endif  // RIDE_SWITCHERITEMLIST_H_
