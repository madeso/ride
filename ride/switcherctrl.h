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

#ifndef RIDE_SWITCHERCTRL_H_
#define RIDE_SWITCHERCTRL_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>

#include "ride/switcheritem.h"
#include "ride/switcheritemlist.h"

/*!
 * wxMultiColumnListCtrl
 * A control for displaying several columns (not scrollable)
 */

class SwitcherCtrl : public wxControl {
 public:
  SwitcherCtrl();

  bool Create(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = 0);  // NOLINT

  // Public API

  void set_items(const SwitcherItemList& items);
  const SwitcherItemList& items() const;
  SwitcherItemList& items();

  // Set an extra key that can be used to cycle through items,
  // in case not using the Ctrl+Tab combination
  void set_extra_navigation_key(int keyCode);
  int extra_navigation_key() const;

  // Set the modifier used to invoke the dialog, and therefore to test for
  // release
  void set_modifier_key(int modifierKey);
  int modifier_key() const;

  // Event handlers

  void OnPaint(wxPaintEvent& event);
  void OnMouseEvent(wxMouseEvent& event);
  void OnChar(wxKeyEvent& event);
  void OnKey(wxKeyEvent& event);
  void OnEraseBackground(wxEraseEvent& event);

  // Overrides
  virtual wxSize DoGetBestSize() const;

  // Implementation

  void CalculateLayout();
  void CalculateLayout(wxDC& dc);  // NOLINT
  void InvalidateLayout();
  void GenerateSelectionEvent();
  void AdvanceToNextSelectableItem(int direction);
  void SendCloseEvent();

 protected:
  SwitcherItemList items_;
  wxSize overall_size_;
  int extra_navigation_key_;
  int modifier_key_;
};

#endif  // RIDE_SWITCHERCTRL_H_
