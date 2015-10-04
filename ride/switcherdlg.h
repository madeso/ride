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

typedef std::vector<SwitcherItem> wxSwitcherItemArray;

/*!
 * wxSwitcherItems
 * An object containing switcher items
 */

class SwitcherItemList : public wxObject {
  DECLARE_CLASS(SwitcherItemList)

 public:
  SwitcherItemList() { Init(); }
  SwitcherItemList(const SwitcherItemList& items) {
    Init();
    Copy(items);
  }

  bool operator==(const SwitcherItemList& items) const;

  void operator=(const SwitcherItemList& items) { Copy(items); }

  void Init();

  void Copy(const SwitcherItemList& items);

  // Public API

  SwitcherItem& AddItem(const wxString& title, const wxString& name, int id = 0,
                        const wxBitmap& bitmap = wxNullBitmap);
  SwitcherItem& AddItem(const SwitcherItem& item);
  SwitcherItem& AddGroup(const wxString& title, const wxString& name,
                         int id = 0, const wxBitmap& bitmap = wxNullBitmap);

  void Clear();

  int FindItemByName(const wxString& name) const;
  int FindItemById(int id) const;

  void SetSelection(int sel);
  void SetSelection(const wxString& name);  // by name
  int GetSelection() const { return selection_; }

  // Find the index for the item associated with the current focus
  int GetIndexForFocus() const;

  // Hit test, returning an index or -1
  int HitTest(const wxPoint& pt) const;

  const SwitcherItem& GetItem(int i) const;
  SwitcherItem& GetItem(int i);

  int GetItemCount() const { return items_.size(); }

  void SetRowCount(int rows) { row_count_ = rows; }
  int GetRowCount() const { return row_count_; }

  void SetColumnCount(int cols) { column_count_ = cols; }
  int GetColumnCount() const { return column_count_; }

  void SetBackgroundColour(const wxColour& colour) {
    background_color_ = colour;
  }
  const wxColour& GetBackgroundColour() const { return background_color_; }

  void SetTextColour(const wxColour& colour) { text_color_ = colour; }
  const wxColour& GetTextColour() const { return text_color_; }

  void SetSelectionColour(const wxColour& colour) { selection_color_ = colour; }
  const wxColour& GetSelectionColour() const { return selection_color_; }

  void SetSelectionOutlineColour(const wxColour& colour) {
    selection_outline_color_ = colour;
  }
  const wxColour& GetSelectionOutlineColour() const {
    return selection_outline_color_;
  }

  void SetItemFont(const wxFont& font) { item_font_ = font; }
  const wxFont& GetItemFont() const { return item_font_; }

  // Implementation

  void PaintItems(wxDC& dc, wxWindow* win);  // NOLINT
  wxSize CalculateItemSize(wxDC& dc);        // NOLINT

 protected:
  wxSwitcherItemArray items_;
  int selection_;
  int row_count_;
  int column_count_;

  wxColour background_color_;
  wxColour text_color_;
  wxColour selection_color_;
  wxColour selection_outline_color_;

  wxFont item_font_;
};

/*!
 * wxMultiColumnListCtrl
 * A control for displaying several columns (not scrollable)
 */

class MultiColumnListCtrl : public wxControl {
  DECLARE_CLASS(MultiColumnListCtrl)

 public:
  MultiColumnListCtrl(wxWindow* parent, wxWindowID id,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = 0);  // NOLINT
  MultiColumnListCtrl() {
    BindEvents();
    Init();
  }

  bool Create(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = 0);  // NOLINT

  // Public API

  void SetItems(const SwitcherItemList& items) { items_ = items; }
  const SwitcherItemList& GetItems() const { return items_; }
  SwitcherItemList& GetItems() { return items_; }

  // Set an extra key that can be used to cycle through items,
  // in case not using the Ctrl+Tab combination
  void SetExtraNavigationKey(int keyCode) { extra_navigation_key_ = keyCode; }
  int GetExtraNavigationKey() const { return extra_navigation_key_; }

  // Set the modifier used to invoke the dialog, and therefore to test for
  // release
  void SetModifierKey(int modifierKey) { modifier_key_ = modifierKey; }
  int GetModifierKey() const { return modifier_key_; }

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
  void InvalidateLayout() {
    items_.SetColumnCount(0);
    Refresh();
  }
  void Init();
  void BindEvents();
  void GenerateSelectionEvent();
  void AdvanceToNextSelectableItem(int direction);
  void SendCloseEvent();

 protected:
  SwitcherItemList items_;
  wxSize overall_size_;
  int extra_navigation_key_;
  int modifier_key_;
};

/*!
 * wxSwitcherDialog shows a wxMultiColumnListCtrl with a list of panes
 * and tabs for the user to choose. Ctrl+Tab cycles through them.
 */

class SwitcherDialog : public wxDialog {
 public:
  // constructors and destructors
  SwitcherDialog() {
    BindEvents();
    Init();
  }
  SwitcherDialog(const SwitcherItemList& items, wxWindow* parent,
                 wxWindowID id = -1, const wxString& title = _("Pane Switcher"),
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style  // NOLINT
                 = wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxBORDER_SIMPLE);

  bool Create(const SwitcherItemList& items, wxWindow* parent,
              wxWindowID id = -1, const wxString& title = _("Pane Switcher"),
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style  // NOLINT
              = wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxBORDER_SIMPLE);

  void Init();
  void BindEvents();

  void OnCloseWindow(wxCloseEvent& event);
  void OnActivate(wxActivateEvent& event);
  void OnSelectItem(wxCommandEvent& event);
  void OnPaint(wxPaintEvent& event);

  // Get the selected item
  int GetSelection() const;

  void ShowDescription(int i);

  void SetBorderColour(const wxColour& colour) { border_color_ = colour; }

  // Set an extra key that can be used to cycle through items,
  // in case not using the Ctrl+Tab combination
  void SetExtraNavigationKey(int keyCode);
  int GetExtraNavigationKey() const { return extra_navigation_key_; }

  // Set the modifier used to invoke the dialog, and therefore to test for
  // release
  void SetModifierKey(int modifierKey);
  int GetModifierKey() const { return modifier_key_; }

 private:
  MultiColumnListCtrl* list_ctrl_;
  wxHtmlWindow* description_ctrl_;
  bool is_closing_;
  long switcher_border_style_;  // NOLINT
  wxColour border_color_;
  int extra_navigation_key_;
  int modifier_key_;
};

#endif  // RIDE_SWITCHERDLG_H_
