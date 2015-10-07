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

#ifndef RIDE_SWITCHERDLG_H_
#define RIDE_SWITCHERDLG_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>

#include "ride/switcheritem.h"
#include "ride/switcheritemlist.h"
#include "ride/switcherctrl.h"

/*!
 * wxSwitcherDialog shows a wxMultiColumnListCtrl with a list of panes
 * and tabs for the user to choose. Ctrl+Tab cycles through them.
 */

class SwitcherDlg : public wxDialog {
 public:
  SwitcherDlg(const SwitcherItemList& items, SwitcherIndex index,
              const SwitcherStyle& sstyle, wxWindow* parent, wxWindowID id = -1,
              const wxString& title = _("Pane Switcher"),
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style  // NOLINT
              = wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxBORDER_SIMPLE);

  void OnCloseWindow(wxCloseEvent& event);
  void OnActivate(wxActivateEvent& event);
  void OnSelectItem(wxCommandEvent& event);

  // Get the selected item
  SwitcherIndex GetSelection() const;

  void ShowDescription(const SwitcherItem& item);

  void set_border_color(const wxColour& colour);

  void AdvanceToNextSelection(bool forward);

 private:
  void UpdateDescription();

 private:
  SwitcherDlg();

 private:
  wxStaticText* title_ctrl_;
  wxStaticText* description_ctrl_;
  SwitcherCtrl* list_ctrl_;
  wxStaticText* path_ctrl_;
  bool is_closing_;
  long switcher_border_style_;  // NOLINT
  wxColour border_color_;
  SwitcherStyle style_;
};

#endif  // RIDE_SWITCHERDLG_H_
