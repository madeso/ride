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
#include "ride/switcherstyle.h"

/*!
 * wxMultiColumnListCtrl
 * A control for displaying several columns (not scrollable)
 */

class SwitcherCtrl : public wxControl {
 public:
  explicit SwitcherCtrl(const SwitcherItemList& items);

  bool Create(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = 0);  // NOLINT

  // Public API
  void set_items(const SwitcherItemList& items);
  const SwitcherItemList& items() const;
  SwitcherItemList& items();

  void SelectOrFirst(SwitcherIndex index);

  SwitcherIndex selection() const;

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
  void SendCloseEvent();
  void AdvanceToNextSelection(bool forward);

 protected:
  SwitcherItemList items_;
  SwitcherIndex selection_;
  SwitcherStyle style_;
  wxSize overall_size_;
};

#endif  // RIDE_SWITCHERCTRL_H_
