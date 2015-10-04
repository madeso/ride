/////////////////////////////////////////////////////////////////////////////
// Name:        switcherdlg.h
// Purpose:     Pane switcher dialog
// Author:      Julian Smart
// Modified by:
// Created:     2007-08-19
// RCS-ID:      $Id: switcherdlg.cpp,v 1.6 2007/08/20 17:38:24 anthemion Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "ride/switcherdlg.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

#if defined(__WXMSW__) && wxUSE_UXTHEME
// #include <wx/msw/uxtheme.h>
#endif

#define wxSWITCHER_TEXT_MARGIN_X 4
#define wxSWITCHER_TEXT_MARGIN_Y 2
#define wxSWITCHER_USE_BUFFERED_PAINTING 1

/*!
 * An object containing information about one item
 */

IMPLEMENT_CLASS(wxSwitcherItem, wxObject)

void wxSwitcherItem::Init() {
  id_ = 0;
  is_group_ = false;
  break_column_ = false;
  row_pos_ = 0;
  col_pos_ = 0;
  window_ = NULL;
}

void wxSwitcherItem::Copy(const wxSwitcherItem& item) {
  id_ = item.id_;
  name_ = item.name_;
  title_ = item.title_;
  is_group_ = item.is_group_;
  break_column_ = item.break_column_;
  rect_ = item.rect_;
  font_ = item.font_;
  text_color_ = item.text_color_;
  bitmap_ = item.bitmap_;
  description_ = item.description_;
  row_pos_ = item.row_pos_;
  col_pos_ = item.col_pos_;
  window_ = item.window_;
}

bool wxSwitcherItem::operator==(const wxSwitcherItem& item) const {
  return (id_ == item.id_ && name_ == item.name_ && title_ == item.title_ &&
          description_ == item.description_ && is_group_ == item.is_group_ &&
          break_column_ == item.break_column_ && rect_ == item.rect_ &&
          font_ == item.font_ && text_color_ == item.text_color_ &&
          col_pos_ == item.col_pos_ && row_pos_ == item.row_pos_ &&
          window_ == item.window_);
}

/*!
 * wxSwitcherItems
 * An object containing switcher items
 */

IMPLEMENT_CLASS(wxSwitcherItems, wxObject)

bool wxSwitcherItems::operator==(const wxSwitcherItems& items) const {
  if (items_.size() != items.items_.size()) return false;

  if (selection_ != items.selection_ || row_count_ != items.row_count_ ||
      column_count_ != items.column_count_)
    return false;

  if (background_color_ != items.background_color_ ||
      text_color_ != items.text_color_ ||
      selection_color_ != items.selection_color_ ||
      selection_outline_color_ != items.selection_outline_color_ ||
      item_font_ != items.item_font_)
    return false;

  size_t i;
  for (i = 0; i < items_.size(); i++) {
    if (!(items_[i] == items.items_[i])) return false;
  }

  return true;
}

void wxSwitcherItems::Init() {
  selection_ = -1;
  row_count_ = 10;
  column_count_ = 0;

#if defined(__WXMSW__) && wxUSE_UXTHEME
// If on Windows XP/Vista, use more appropriate colours.
/*
if (wxUxThemeEngine::GetIfActive()) {
  SetSelectionOutlineColour(wxColour(49, 106, 197));
  SetSelectionColour(wxColour(193, 210, 238));
}
*/
#endif
}

void wxSwitcherItems::Copy(const wxSwitcherItems& items) {
  Clear();

  size_t i;
  for (i = 0; i < items.items_.size(); i++) {
    items_.push_back(items.items_[i]);
  }

  selection_ = items.selection_;
  row_count_ = items.row_count_;
  column_count_ = items.column_count_;

  background_color_ = items.background_color_;
  text_color_ = items.text_color_;
  selection_color_ = items.selection_color_;
  selection_outline_color_ = items.selection_outline_color_;
  item_font_ = items.item_font_;
}

wxSwitcherItem& wxSwitcherItems::AddItem(const wxString& title,
                                         const wxString& name, int id,
                                         const wxBitmap& bitmap) {
  wxSwitcherItem item;
  item.SetTitle(title);
  item.SetName(name);
  item.SetId(id);
  item.SetBitmap(bitmap);

  return AddItem(item);
}

wxSwitcherItem& wxSwitcherItems::AddItem(const wxSwitcherItem& item) {
  items_.push_back(item);
  return items_[GetItemCount() - 1];
}

wxSwitcherItem& wxSwitcherItems::AddGroup(const wxString& title,
                                          const wxString& name, int id,
                                          const wxBitmap& bitmap) {
  wxSwitcherItem& item = AddItem(title, name, id, bitmap);
  item.SetIsGroup(true);

  return item;
}

void wxSwitcherItems::Clear() { items_.resize(0); }

int wxSwitcherItems::FindItemByName(const wxString& name) const {
  size_t i;
  for (i = 0; i < items_.size(); i++) {
    if (items_[i].GetName() == name) return i;
  }

  return -1;
}

int wxSwitcherItems::FindItemById(int id) const {
  size_t i;
  for (i = 0; i < items_.size(); i++) {
    if (items_[i].GetId() == id) return i;
  }

  return -1;
}

void wxSwitcherItems::SetSelection(int sel) { selection_ = sel; }

void wxSwitcherItems::SetSelection(const wxString& name) {
  int idx = FindItemByName(name);
  if (idx != -1) SetSelection(idx);
}

const wxSwitcherItem& wxSwitcherItems::GetItem(int i) const {
  return items_[i];
}

wxSwitcherItem& wxSwitcherItems::GetItem(int i) { return items_[i]; }

void wxSwitcherItems::PaintItems(wxDC& dc, wxWindow* win) {  // NOLINT
  wxColour backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
  wxColour standardTextColour =
      wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
  wxColour selectionColour =
      wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
  wxColour selectionOutlineColour =
      wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
  wxFont standardFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
  wxFont groupFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
  groupFont.SetWeight(wxBOLD);

  if (GetBackgroundColour().Ok()) backgroundColour = GetBackgroundColour();

  if (GetTextColour().Ok()) standardTextColour = GetTextColour();

  if (GetSelectionColour().Ok()) selectionColour = GetSelectionColour();

  if (GetSelectionOutlineColour().Ok())
    selectionOutlineColour = GetSelectionOutlineColour();

  if (GetItemFont().Ok()) {
    standardFont = GetItemFont();
    groupFont =
        wxFont(standardFont.GetPointSize(), standardFont.GetFamily(),
               standardFont.GetStyle(), wxBOLD, standardFont.GetUnderlined(),
               standardFont.GetFaceName());
  }

  int textMarginX = wxSWITCHER_TEXT_MARGIN_X;

  dc.SetLogicalFunction(wxCOPY);
  dc.SetBrush(wxBrush(backgroundColour));
  dc.SetPen(*wxTRANSPARENT_PEN);
  dc.DrawRectangle(win->GetClientRect());
  dc.SetBackgroundMode(wxTRANSPARENT);

  size_t i;
  for (i = 0; i < items_.size(); i++) {
    wxSwitcherItem& item = items_[i];
    bool selected = (static_cast<int>(i) == selection_);

    if (selected) {
      dc.SetPen(wxPen(selectionOutlineColour));
      dc.SetBrush(wxBrush(selectionColour));
      dc.DrawRectangle(item.GetRect());
    }

    wxRect clippingRect(item.GetRect());
    clippingRect.Deflate(1, 1);

    dc.SetClippingRegion(clippingRect);

    if (item.GetTextColour().Ok())
      dc.SetTextForeground(item.GetTextColour());
    else
      dc.SetTextForeground(standardTextColour);

    if (item.GetFont().Ok()) {
      dc.SetFont(item.GetFont());
    } else {
      if (item.GetIsGroup())
        dc.SetFont(groupFont);
      else
        dc.SetFont(standardFont);
    }

    int w, h;
    dc.GetTextExtent(item.GetTitle(), &w, &h);

    int x = item.GetRect().x;

    x += textMarginX;

    if (!item.GetIsGroup()) {
      if (item.GetBitmap().Ok() && item.GetBitmap().GetWidth() <= 16 &&
          item.GetBitmap().GetHeight() <= 16) {
        dc.DrawBitmap(
            item.GetBitmap(), x,
            item.GetRect().y +
                (item.GetRect().height - item.GetBitmap().GetHeight()) / 2,
            true);
      }

      x += 16;

      x += textMarginX;
    }

    int y = item.GetRect().y + (item.GetRect().height - h) / 2;
    dc.DrawText(item.GetTitle(), x, y);

    dc.DestroyClippingRegion();
  }
}

wxSize wxSwitcherItems::CalculateItemSize(wxDC& dc) {  // NOLINT
  // Start off allowing for an icon
  wxSize sz(150, 16);
  wxFont standardFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
  wxFont groupFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
  groupFont.SetWeight(wxBOLD);

  int textMarginX = wxSWITCHER_TEXT_MARGIN_X;
  int textMarginY = wxSWITCHER_TEXT_MARGIN_Y;
  int maxWidth = 300;
  int maxHeight = 40;

  if (GetItemFont().Ok()) standardFont = GetItemFont();

  size_t i;
  for (i = 0; i < items_.size(); i++) {
    wxSwitcherItem& item = items_[i];

    if (item.GetFont().Ok()) {
      dc.SetFont(item.GetFont());
    } else {
      if (item.GetIsGroup())
        dc.SetFont(groupFont);
      else
        dc.SetFont(standardFont);
    }

    int w, h;
    dc.GetTextExtent(item.GetTitle(), &w, &h);

    w += 16 + 2 * textMarginX;

    if (w > sz.x) sz.x = wxMin(w, maxWidth);
    if (h > sz.y) sz.y = wxMin(h, maxHeight);
  }

  if (sz == wxSize(16, 16)) {
    sz = wxSize(100, 25);
  } else {
    sz.x += textMarginX * 2;
    sz.y += textMarginY * 2;
  }

  return sz;
}

// Find the index for the item associated with the current focus
int wxSwitcherItems::GetIndexForFocus() const {
  for (size_t i = 0; i < items_.size(); i++) {
    const wxSwitcherItem& item = items_[i];
    if (item.GetWindow()) {
      if (wxFindFocusDescendant(item.GetWindow())) return i;
    }
  }

  return wxNOT_FOUND;
}

// Hit test, returning an index or -1
int wxSwitcherItems::HitTest(const wxPoint& pt) const {
  for (size_t i = 0; i < items_.size(); i++) {
    const wxSwitcherItem& item = items_[i];
    if (item.GetRect().Contains(pt)) return static_cast<int>(i);
  }

  return wxNOT_FOUND;
}

/*
 * A control for displaying several columns (not scrollable)
 */

BEGIN_EVENT_TABLE(wxMultiColumnListCtrl, wxControl)
EVT_PAINT(wxMultiColumnListCtrl::OnPaint)
EVT_ERASE_BACKGROUND(wxMultiColumnListCtrl::OnEraseBackground)
EVT_MOUSE_EVENTS(wxMultiColumnListCtrl::OnMouseEvent)
EVT_CHAR(wxMultiColumnListCtrl::OnChar)
EVT_KEY_DOWN(wxMultiColumnListCtrl::OnKey)
EVT_KEY_UP(wxMultiColumnListCtrl::OnKey)
END_EVENT_TABLE()

IMPLEMENT_CLASS(wxMultiColumnListCtrl, wxControl)

wxMultiColumnListCtrl::wxMultiColumnListCtrl(wxWindow* parent, wxWindowID id,
                                             const wxPoint& pos,
                                             const wxSize& size,
                                             long style) {  // NOLINT
  Init();

  Create(parent, id, pos, size, style);
}

bool wxMultiColumnListCtrl::Create(wxWindow* parent, wxWindowID id,
                                   const wxPoint& pos, const wxSize& size,
                                   long style) {  // NOLINT
  wxControl::Create(parent, id, pos, size, style);

  SetInitialBestSize(size);

  return true;
}

// Overrides
wxSize wxMultiColumnListCtrl::DoGetBestSize() const { return overall_size_; }

void wxMultiColumnListCtrl::SendCloseEvent() {
  wxWindow* topLevel = GetParent();
  while (topLevel && !topLevel->IsTopLevel()) topLevel = topLevel->GetParent();

  if (topLevel) {
    wxCloseEvent closeEvent(wxEVT_CLOSE_WINDOW, topLevel->GetId());
    closeEvent.SetEventObject(topLevel);
    closeEvent.SetCanVeto(false);

    topLevel->GetEventHandler()->ProcessEvent(closeEvent);
    return;
  }
}

void wxMultiColumnListCtrl::OnEraseBackground(
    wxEraseEvent& WXUNUSED(event)) {  // NOLINT
                                      // Do nothing
}

void wxMultiColumnListCtrl::OnPaint(wxPaintEvent& WXUNUSED(event)) {  // NOLINT
#if wxSWITCHER_USE_BUFFERED_PAINTING
  wxBufferedPaintDC dc(this);
#else
  wxPaintDC dc(this);
#endif

  wxRect rect = GetClientRect();

  if (items_.GetColumnCount() == 0) CalculateLayout(dc);

  if (items_.GetColumnCount() == 0) return;

  items_.PaintItems(dc, this);
}

void wxMultiColumnListCtrl::OnMouseEvent(wxMouseEvent& event) {
  if (event.LeftDown()) {
    SetFocus();

    int idx = items_.HitTest(event.GetPosition());
    if (idx != wxNOT_FOUND) {
      items_.SetSelection(idx);

      SendCloseEvent();
    }
  }
}

void wxMultiColumnListCtrl::OnChar(wxKeyEvent& WXUNUSED(event)) {}  // NOLINT

void wxMultiColumnListCtrl::OnKey(wxKeyEvent& event) {
  if (event.GetEventType() == wxEVT_KEY_UP) {
    if (event.GetKeyCode() == GetModifierKey()) {
      SendCloseEvent();
    }
    event.Skip();
    return;
  }

  if (event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_RETURN) {
    if (event.GetKeyCode() == WXK_ESCAPE) items_.SetSelection(-1);

    SendCloseEvent();
  } else if (event.GetKeyCode() == WXK_TAB ||
             event.GetKeyCode() == GetExtraNavigationKey()) {
    if (event.ShiftDown()) {
      items_.SetSelection(items_.GetSelection() - 1);
      if (items_.GetSelection() < 0)
        items_.SetSelection(items_.GetItemCount() - 1);

      AdvanceToNextSelectableItem(-1);
    } else {
      items_.SetSelection(items_.GetSelection() + 1);
      if (items_.GetSelection() >= items_.GetItemCount())
        items_.SetSelection(0);

      AdvanceToNextSelectableItem(1);
    }

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_DOWN ||
             event.GetKeyCode() == WXK_NUMPAD_DOWN) {
    items_.SetSelection(items_.GetSelection() + 1);
    if (items_.GetSelection() >= items_.GetItemCount()) items_.SetSelection(0);

    AdvanceToNextSelectableItem(1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_UP ||
             event.GetKeyCode() == WXK_NUMPAD_UP) {
    items_.SetSelection(items_.GetSelection() - 1);
    if (items_.GetSelection() < 0)
      items_.SetSelection(items_.GetItemCount() - 1);

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_HOME ||
             event.GetKeyCode() == WXK_NUMPAD_HOME) {
    items_.SetSelection(0);

    AdvanceToNextSelectableItem(1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_END ||
             event.GetKeyCode() == WXK_NUMPAD_END) {
    items_.SetSelection(items_.GetItemCount() - 1);

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_LEFT ||
             event.GetKeyCode() == WXK_NUMPAD_LEFT) {
    wxSwitcherItem& item = items_.GetItem(items_.GetSelection());

    int row = item.GetRowPos();
    int newCol = item.GetColPos() - 1;
    if (newCol < 0) newCol = (items_.GetColumnCount() - 1);

    // Find the first item from the end whose row matches and whose column is
    // equal or lower
    int i;
    for (i = items_.GetItemCount() - 1; i >= 0; i--) {
      wxSwitcherItem& item2 = items_.GetItem(i);
      if (item2.GetColPos() == newCol && item2.GetRowPos() <= row) {
        items_.SetSelection(i);
        break;
      }
    }

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_RIGHT ||
             event.GetKeyCode() == WXK_NUMPAD_RIGHT) {
    wxSwitcherItem& item = items_.GetItem(items_.GetSelection());

    int row = item.GetRowPos();
    int newCol = item.GetColPos() + 1;
    if (newCol >= items_.GetColumnCount()) newCol = 0;

    // Find the first item from the end whose row matches and whose column is
    // equal or lower
    int i;
    for (i = items_.GetItemCount() - 1; i >= 0; i--) {
      wxSwitcherItem& item2 = items_.GetItem(i);
      if (item2.GetColPos() == newCol && item2.GetRowPos() <= row) {
        items_.SetSelection(i);
        break;
      }
    }

    AdvanceToNextSelectableItem(1);

    GenerateSelectionEvent();

    Refresh();
  } else {
    event.Skip();
  }
}

// Advance to the next selectable item
void wxMultiColumnListCtrl::AdvanceToNextSelectableItem(int direction) {
  if (items_.GetItemCount() < 2) return;

  if (items_.GetSelection() == -1) items_.SetSelection(0);

  int oldSel = items_.GetSelection();

  while (true) {
    if (items_.GetItem(items_.GetSelection()).GetIsGroup()) {
      items_.SetSelection(items_.GetSelection() + direction);
      if (items_.GetSelection() == -1)
        items_.SetSelection(items_.GetItemCount() - 1);
      else if (items_.GetSelection() == items_.GetItemCount())
        items_.SetSelection(0);

      if (items_.GetSelection() == oldSel) break;
    } else {
      break;
    }
  }
}

void wxMultiColumnListCtrl::GenerateSelectionEvent() {
  wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, GetId());
  event.SetEventObject(this);
  event.SetInt(items_.GetSelection());

  GetEventHandler()->ProcessEvent(event);
}

void wxMultiColumnListCtrl::CalculateLayout() {
  wxClientDC dc(this);
  CalculateLayout(dc);
}

void wxMultiColumnListCtrl::CalculateLayout(wxDC& dc) {  // NOLINT
  if (items_.GetSelection() == -1) items_.SetSelection(0);

  int columnCount = 1;

  // Spacing between edge of window or between columns
  int xMargin = 4;
  int yMargin = 4;

  // Inter-row spacing
  int rowSpacing = 2;

  wxSize itemSize = items_.CalculateItemSize(dc);
  overall_size_ = wxSize(350, 200);

  size_t i;
  int currentRow = 0;
  int x = xMargin;
  int y = yMargin;

  bool breaking = false;

  for (i = 0; i < (size_t)items_.GetItemCount(); i++) {
    wxSize oldOverallSize = overall_size_;

    items_.GetItem(i).SetRect(wxRect(x, y, itemSize.x, itemSize.y));
    items_.GetItem(i).SetColPos(columnCount - 1);
    items_.GetItem(i).SetRowPos(currentRow);

    if (items_.GetItem(i).GetRect().GetBottom() > overall_size_.y)
      overall_size_.y = items_.GetItem(i).GetRect().GetBottom() + yMargin;

    if (items_.GetItem(i).GetRect().GetRight() > overall_size_.x)
      overall_size_.x = items_.GetItem(i).GetRect().GetRight() + xMargin;

    currentRow++;

    y += (rowSpacing + itemSize.y);

    bool stopBreaking = breaking;

    if ((currentRow > items_.GetRowCount()) ||
        (items_.GetItem(i).GetBreakColumn() && !breaking &&
         (currentRow != 1))) {
      currentRow = 0;
      columnCount++;
      x += (xMargin + itemSize.x);
      y = yMargin;

      // Make sure we don't orphan a group
      if (items_.GetItem(i).GetIsGroup() ||
          (items_.GetItem(i).GetBreakColumn() && !breaking)) {
        overall_size_ = oldOverallSize;

        if (items_.GetItem(i).GetBreakColumn()) breaking = true;

        // Repeat the last item, in the next column
        i--;
      }
    }

    if (stopBreaking) breaking = false;
  }

  items_.SetColumnCount(columnCount);

  InvalidateBestSize();
}

void wxMultiColumnListCtrl::Init() {
  overall_size_ = wxSize(200, 100);
  modifier_key_ = WXK_CONTROL;
  extra_navigation_key_ = 0;
}

/*!
 * wxSwitcherDialog shows a wxMultiColumnListCtrl with a list of panes
 * and tabs for the user to choose. Ctrl+Tab cycles through them.
 */

BEGIN_EVENT_TABLE(wxSwitcherDialog, wxDialog)
EVT_CLOSE(wxSwitcherDialog::OnCloseWindow)
EVT_ACTIVATE(wxSwitcherDialog::OnActivate)
EVT_LISTBOX(wxID_ANY, wxSwitcherDialog::OnSelectItem)
EVT_PAINT(wxSwitcherDialog::OnPaint)
END_EVENT_TABLE()

wxSwitcherDialog::wxSwitcherDialog(const wxSwitcherItems& items,
                                   wxWindow* parent, wxWindowID id,
                                   const wxString& title,
                                   const wxPoint& position, const wxSize& size,
                                   long style) {  // NOLINT
  Init();

  Create(items, parent, id, title, position, size, style);
}

bool wxSwitcherDialog::Create(const wxSwitcherItems& items, wxWindow* parent,
                              wxWindowID id, const wxString& title,
                              const wxPoint& position, const wxSize& size,
                              long style) {  // NOLINT
  switcher_border_style_ = (style & wxBORDER_MASK);
  if (switcher_border_style_ == wxBORDER_NONE)
    switcher_border_style_ = wxBORDER_SIMPLE;

  style &= wxBORDER_MASK;
  style |= wxBORDER_NONE;

  wxDialog::Create(parent, id, title, position, size, style);

  list_ctrl_ = new wxMultiColumnListCtrl();
  list_ctrl_->SetItems(items);
  list_ctrl_->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                     wxWANTS_CHARS | wxNO_BORDER);
  list_ctrl_->CalculateLayout();

  if (extra_navigation_key_ != -1)
    list_ctrl_->SetExtraNavigationKey(extra_navigation_key_);

  if (modifier_key_ != -1) list_ctrl_->SetModifierKey(modifier_key_);

  int borderStyle = wxSIMPLE_BORDER;

  borderStyle = wxBORDER_NONE;
#if defined(__WXMSW__) && wxCHECK_VERSION(2, 8, 5)
// borderStyle = wxBORDER_THEME;
#endif

  description_ctrl_ = new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition,
                                       wxSize(-1, 100), borderStyle);
  description_ctrl_->SetHTMLBackgroundColour(GetBackgroundColour());

#ifdef __WXGTK20__
  int fontSize = 11;
  description_ctrl_->SetStandardFonts(fontSize);
#endif

  wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(sizer);

  sizer->Add(list_ctrl_, 1, wxALL | wxEXPAND, 10);
  sizer->Add(description_ctrl_, 0, wxALL | wxEXPAND, 10);

  sizer->SetSizeHints(this);

  list_ctrl_->SetFocus();

  Centre(wxBOTH);

  if (list_ctrl_->GetItems().GetSelection() == -1)
    list_ctrl_->GetItems().SetSelection(0);

  list_ctrl_->AdvanceToNextSelectableItem(1);

  ShowDescription(list_ctrl_->GetItems().GetSelection());

  return true;
}

void wxSwitcherDialog::Init() {
  list_ctrl_ = NULL;
  description_ctrl_ = NULL;
  is_closing_ = false;
  switcher_border_style_ = 0;

  modifier_key_ = -1;
  extra_navigation_key_ = -1;

#if defined(__WXMSW__) && wxUSE_UXTHEME
/* if (wxUxThemeEngine::GetIfActive())
  m_borderColour = wxColour(49, 106, 197);
else
*/
#endif
  border_color_ = *wxBLACK;
}

void wxSwitcherDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {  // NOLINT
  if (is_closing_) return;

  if (IsModal()) {
    is_closing_ = true;

    if (GetSelection() == -1)
      EndModal(wxID_CANCEL);
    else
      EndModal(wxID_OK);
  }
}

// Get the selected item
int wxSwitcherDialog::GetSelection() const {
  return list_ctrl_->GetItems().GetSelection();
}

void wxSwitcherDialog::OnActivate(wxActivateEvent& event) {
  if (!event.GetActive()) {
    if (!is_closing_) {
      is_closing_ = true;
      EndModal(wxID_CANCEL);
    }
  }
}

void wxSwitcherDialog::OnPaint(wxPaintEvent& WXUNUSED(event)) {  // NOLINT
  wxPaintDC dc(this);

  if (switcher_border_style_ == wxBORDER_SIMPLE) {
    dc.SetPen(wxPen(border_color_));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    wxRect rect(GetClientRect());
    dc.DrawRectangle(rect);

    // Draw border around the HTML control
    rect = description_ctrl_->GetRect();
    rect.Inflate(1, 1);
    dc.DrawRectangle(rect);
  }
}

void wxSwitcherDialog::OnSelectItem(wxCommandEvent& event) {
  ShowDescription(event.GetSelection());
}

// Convert a colour to a 6-digit hex string
static wxString ColourToHexString(const wxColour& col) {
  wxString hex;

  hex += wxDecToHex(col.Red());
  hex += wxDecToHex(col.Green());
  hex += wxDecToHex(col.Blue());

  return hex;
}

void wxSwitcherDialog::ShowDescription(int i) {
  wxSwitcherItem& item = list_ctrl_->GetItems().GetItem(i);

  wxColour colour = list_ctrl_->GetItems().GetBackgroundColour();
  if (!colour.Ok()) colour = GetBackgroundColour();

  wxString backgroundColourHex = ColourToHexString(colour);

  wxString html = wxT("<body bgcolor=\"#") + backgroundColourHex +
                  wxT("\"><b>") + item.GetTitle() + wxT("</b>");

  if (!item.GetDescription().IsEmpty()) {
    html += wxT("<p>");
    html += item.GetDescription();
  }

  html += wxT("</body>");

  description_ctrl_->SetPage(html);
}

void wxSwitcherDialog::SetExtraNavigationKey(int keyCode) {
  extra_navigation_key_ = keyCode;
  if (list_ctrl_) list_ctrl_->SetExtraNavigationKey(keyCode);
}

void wxSwitcherDialog::SetModifierKey(int modifierKey) {
  modifier_key_ = modifierKey;
  if (list_ctrl_) list_ctrl_->SetModifierKey(modifierKey);
}
