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

/*!
 * wxSwitcherItem
 * An object containing information about one item
 */

class wxSwitcherItem : public wxObject {
  DECLARE_CLASS(wxSwitcherItem)

 public:
  wxSwitcherItem() { Init(); }
  wxSwitcherItem(const wxSwitcherItem& item) {
    Init();
    Copy(item);
  }

  bool operator==(const wxSwitcherItem& item) const;

  void operator=(const wxSwitcherItem& item) { Copy(item); }

  void Init();

  void Copy(const wxSwitcherItem& item);

  wxSwitcherItem& SetTitle(const wxString& title) {
    title_ = title;
    return (*this);
  }
  const wxString& GetTitle() const { return title_; }

  wxSwitcherItem& SetName(const wxString& name) {
    name_ = name;
    return (*this);
  }
  const wxString& GetName() const { return name_; }

  wxSwitcherItem& SetDescription(const wxString& descr) {
    description_ = descr;
    return (*this);
  }
  const wxString& GetDescription() const { return description_; }

  wxSwitcherItem& SetId(int id) {
    id_ = id;
    return (*this);
  }
  int GetId() const { return id_; }

  wxSwitcherItem& SetIsGroup(bool isGroup) {
    is_group_ = isGroup;
    return (*this);
  }
  bool GetIsGroup() const { return is_group_; }

  wxSwitcherItem& BreakColumn(bool breakCol = true) {
    break_column_ = breakCol;
    return (*this);
  }
  bool GetBreakColumn() const { return break_column_; }

  wxSwitcherItem& SetRect(const wxRect& rect) {
    rect_ = rect;
    return (*this);
  }
  const wxRect& GetRect() const { return rect_; }

  wxSwitcherItem& SetTextColour(const wxColour& colour) {
    text_color_ = colour;
    return (*this);
  }
  const wxColour& GetTextColour() const { return text_color_; }

  wxSwitcherItem& SetFont(const wxFont& font) {
    font_ = font;
    return (*this);
  }
  const wxFont& GetFont() const { return font_; }

  wxSwitcherItem& SetBitmap(const wxBitmap& bitmap) {
    bitmap_ = bitmap;
    return (*this);
  }
  const wxBitmap& GetBitmap() const { return bitmap_; }

  wxSwitcherItem& SetRowPos(int pos) {
    row_pos_ = pos;
    return (*this);
  }
  int GetRowPos() const { return row_pos_; }

  wxSwitcherItem& SetColPos(int pos) {
    col_pos_ = pos;
    return (*this);
  }
  int GetColPos() const { return col_pos_; }

  wxSwitcherItem& SetWindow(wxWindow* win) {
    window_ = win;
    return (*this);
  }
  wxWindow* GetWindow() const { return window_; }

 protected:
  wxString title_;
  wxString name_;
  wxString description_;
  int id_;
  bool is_group_;
  bool break_column_;
  wxRect rect_;
  wxBitmap bitmap_;
  wxColour text_color_;
  wxFont font_;
  int row_pos_;
  int col_pos_;
  wxWindow* window_;
};

typedef std::vector<wxSwitcherItem> wxSwitcherItemArray;

/*!
 * wxSwitcherItems
 * An object containing switcher items
 */

class wxSwitcherItems : public wxObject {
  DECLARE_CLASS(wxSwitcherItems)

 public:
  wxSwitcherItems() { Init(); }
  wxSwitcherItems(const wxSwitcherItems& items) {
    Init();
    Copy(items);
  }

  bool operator==(const wxSwitcherItems& items) const;

  void operator=(const wxSwitcherItems& items) { Copy(items); }

  void Init();

  void Copy(const wxSwitcherItems& items);

  // Public API

  wxSwitcherItem& AddItem(const wxString& title, const wxString& name,
                          int id = 0, const wxBitmap& bitmap = wxNullBitmap);
  wxSwitcherItem& AddItem(const wxSwitcherItem& item);
  wxSwitcherItem& AddGroup(const wxString& title, const wxString& name,
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

  const wxSwitcherItem& GetItem(int i) const;
  wxSwitcherItem& GetItem(int i);

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

class wxMultiColumnListCtrl : public wxControl {
  DECLARE_CLASS(wxMultiColumnListCtrl)

 public:
  wxMultiColumnListCtrl(wxWindow* parent, wxWindowID id,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = 0);  // NOLINT
  wxMultiColumnListCtrl() { Init(); }

  bool Create(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = 0);  // NOLINT

  // Public API

  void SetItems(const wxSwitcherItems& items) { items_ = items; }
  const wxSwitcherItems& GetItems() const { return items_; }
  wxSwitcherItems& GetItems() { return items_; }

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
  void GenerateSelectionEvent();
  void AdvanceToNextSelectableItem(int direction);
  void SendCloseEvent();

 protected:
  wxSwitcherItems items_;
  wxSize overall_size_;
  int extra_navigation_key_;
  int modifier_key_;

  DECLARE_EVENT_TABLE()
};

/*!
 * wxSwitcherDialog shows a wxMultiColumnListCtrl with a list of panes
 * and tabs for the user to choose. Ctrl+Tab cycles through them.
 */

class wxSwitcherDialog : public wxDialog {
 public:
  // constructors and destructors
  wxSwitcherDialog() { Init(); }
  wxSwitcherDialog(const wxSwitcherItems& items, wxWindow* parent,
                   wxWindowID id = -1,
                   const wxString& title = _("Pane Switcher"),
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style  // NOLINT
                   = wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxBORDER_SIMPLE);

  bool Create(const wxSwitcherItems& items, wxWindow* parent,
              wxWindowID id = -1, const wxString& title = _("Pane Switcher"),
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style  // NOLINT
              = wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxBORDER_SIMPLE);

  void Init();

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
  wxMultiColumnListCtrl* list_ctrl_;
  wxHtmlWindow* description_ctrl_;
  bool is_closing_;
  long switcher_border_style_;  // NOLINT
  wxColour border_color_;
  int extra_navigation_key_;
  int modifier_key_;

  DECLARE_EVENT_TABLE()
};

#endif  // RIDE_SWITCHERDLG_H_
