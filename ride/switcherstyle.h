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

#ifndef RIDE_SWITCHERSTYLE_H_
#define RIDE_SWITCHERSTYLE_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>

#include "ride/switcheritem.h"
#include "ride/switchergroup.h"

/*!
 * wxSwitcherItems
 * An object containing switcher items
 */

class SwitcherStyle {
 public:
  SwitcherStyle();

  void set_row_count(int rows);
  int row_count() const;

  void set_text_margin_x(int margin);

  int text_margin_x() const;

  void set_text_margin_y(int margin);

  int text_margin_y() const;

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

  int x_margin() const;
  int y_margin() const;
  int row_spacing() const;
  int col_spacing() const;

  int item_maxwidth() const;
  int item_maxheight() const;

  int dlg_main_border() const;
  int dlg_item_border() const;

  wxColor border_color() const;

 protected:
  int row_count_;
  int text_margin_x_;
  int text_margin_y_;

  wxColour background_color_;
  wxColour text_color_;
  wxColour selection_color_;
  wxColour selection_outline_color_;

  wxFont item_font_;
};

#endif  // RIDE_SWITCHERSTYLE_H_
