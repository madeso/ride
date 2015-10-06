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

#ifndef RIDE_SWITCHERITEM_H_
#define RIDE_SWITCHERITEM_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

/*!
 * wxSwitcherItem
 * An object containing information about one item
 */

class SwitcherItem : public wxObject {
 public:
  SwitcherItem();
  SwitcherItem(const wxString& title, const wxString& name, int id = 0,
               const wxBitmap& bitmap = wxNullBitmap);

  bool operator==(const SwitcherItem& item) const;

  SwitcherItem& set_title(const wxString& title);
  const wxString& title() const;

  SwitcherItem& set_name(const wxString& name);
  const wxString& name() const;

  SwitcherItem& set_description(const wxString& descr);
  const wxString& description() const;

  SwitcherItem& set_path(const wxString& descr);
  const wxString& path() const;

  SwitcherItem& set_id(int id);
  int id() const;

  SwitcherItem& set_rect(const wxRect& rect);
  const wxRect& rect() const;

  SwitcherItem& set_bitmap(const wxBitmap& bitmap);
  const wxBitmap& bitmap() const;

  SwitcherItem& set_row_pos(int pos);
  int row_pos() const;

  SwitcherItem& set_col_pos(int pos);
  int get_col_pos() const;

  SwitcherItem& set_window(wxWindow* win);
  wxWindow* window() const;

 protected:
  wxString title_;
  wxString name_;
  wxString description_;
  wxString path_;
  int id_;
  wxRect rect_;
  wxBitmap bitmap_;
  int row_pos_;
  int col_pos_;
  wxWindow* window_;
};

#endif  // RIDE_SWITCHERITEM_H_
