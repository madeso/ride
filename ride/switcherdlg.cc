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
  m_id = 0;
  m_isGroup = false;
  m_breakColumn = false;
  m_rowPos = 0;
  m_colPos = 0;
  m_window = NULL;
}

void wxSwitcherItem::Copy(const wxSwitcherItem& item) {
  m_id = item.m_id;
  m_name = item.m_name;
  m_title = item.m_title;
  m_isGroup = item.m_isGroup;
  m_breakColumn = item.m_breakColumn;
  m_rect = item.m_rect;
  m_font = item.m_font;
  m_textColour = item.m_textColour;
  m_bitmap = item.m_bitmap;
  m_description = item.m_description;
  m_rowPos = item.m_rowPos;
  m_colPos = item.m_colPos;
  m_window = item.m_window;
}

bool wxSwitcherItem::operator==(const wxSwitcherItem& item) const {
  return (m_id == item.m_id && m_name == item.m_name &&
          m_title == item.m_title && m_description == item.m_description &&
          m_isGroup == item.m_isGroup && m_breakColumn == item.m_breakColumn &&
          m_rect == item.m_rect && m_font == item.m_font &&
          m_textColour == item.m_textColour && m_colPos == item.m_colPos &&
          m_rowPos == item.m_rowPos && m_window == item.m_window);
}

/*!
 * wxSwitcherItems
 * An object containing switcher items
 */

IMPLEMENT_CLASS(wxSwitcherItems, wxObject)

bool wxSwitcherItems::operator==(const wxSwitcherItems& items) const {
  if (m_items.size() != items.m_items.size()) return false;

  if (m_selection != items.m_selection || m_rowCount != items.m_rowCount ||
      m_columnCount != items.m_columnCount)
    return false;

  if (m_backgroundColour != items.m_backgroundColour ||
      m_textColour != items.m_textColour ||
      m_selectionColour != items.m_selectionColour ||
      m_selectionOutlineColour != items.m_selectionOutlineColour ||
      m_itemFont != items.m_itemFont)
    return false;

  size_t i;
  for (i = 0; i < m_items.size(); i++) {
    if (!(m_items[i] == items.m_items[i])) return false;
  }

  return true;
}

void wxSwitcherItems::Init() {
  m_selection = -1;
  m_rowCount = 10;
  m_columnCount = 0;

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
  for (i = 0; i < items.m_items.size(); i++) {
    m_items.push_back(items.m_items[i]);
  }

  m_selection = items.m_selection;
  m_rowCount = items.m_rowCount;
  m_columnCount = items.m_columnCount;

  m_backgroundColour = items.m_backgroundColour;
  m_textColour = items.m_textColour;
  m_selectionColour = items.m_selectionColour;
  m_selectionOutlineColour = items.m_selectionOutlineColour;
  m_itemFont = items.m_itemFont;
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
  m_items.push_back(item);
  return m_items[GetItemCount() - 1];
}

wxSwitcherItem& wxSwitcherItems::AddGroup(const wxString& title,
                                          const wxString& name, int id,
                                          const wxBitmap& bitmap) {
  wxSwitcherItem& item = AddItem(title, name, id, bitmap);
  item.SetIsGroup(true);

  return item;
}

void wxSwitcherItems::Clear() { m_items.resize(0); }

int wxSwitcherItems::FindItemByName(const wxString& name) const {
  size_t i;
  for (i = 0; i < m_items.size(); i++) {
    if (m_items[i].GetName() == name) return i;
  }

  return -1;
}

int wxSwitcherItems::FindItemById(int id) const {
  size_t i;
  for (i = 0; i < m_items.size(); i++) {
    if (m_items[i].GetId() == id) return i;
  }

  return -1;
}

void wxSwitcherItems::SetSelection(int sel) { m_selection = sel; }

void wxSwitcherItems::SetSelection(const wxString& name) {
  int idx = FindItemByName(name);
  if (idx != -1) SetSelection(idx);
}

const wxSwitcherItem& wxSwitcherItems::GetItem(int i) const {
  return m_items[i];
}

wxSwitcherItem& wxSwitcherItems::GetItem(int i) { return m_items[i]; }

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
  for (i = 0; i < m_items.size(); i++) {
    wxSwitcherItem& item = m_items[i];
    bool selected = (static_cast<int>(i) == m_selection);

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
  for (i = 0; i < m_items.size(); i++) {
    wxSwitcherItem& item = m_items[i];

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
  for (size_t i = 0; i < m_items.size(); i++) {
    const wxSwitcherItem& item = m_items[i];
    if (item.GetWindow()) {
      if (wxFindFocusDescendant(item.GetWindow())) return i;
    }
  }

  return wxNOT_FOUND;
}

// Hit test, returning an index or -1
int wxSwitcherItems::HitTest(const wxPoint& pt) const {
  for (size_t i = 0; i < m_items.size(); i++) {
    const wxSwitcherItem& item = m_items[i];
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
wxSize wxMultiColumnListCtrl::DoGetBestSize() const { return m_overallSize; }

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

  if (m_items.GetColumnCount() == 0) CalculateLayout(dc);

  if (m_items.GetColumnCount() == 0) return;

  m_items.PaintItems(dc, this);
}

void wxMultiColumnListCtrl::OnMouseEvent(wxMouseEvent& event) {
  if (event.LeftDown()) {
    SetFocus();

    int idx = m_items.HitTest(event.GetPosition());
    if (idx != wxNOT_FOUND) {
      m_items.SetSelection(idx);

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
    if (event.GetKeyCode() == WXK_ESCAPE) m_items.SetSelection(-1);

    SendCloseEvent();
  } else if (event.GetKeyCode() == WXK_TAB ||
             event.GetKeyCode() == GetExtraNavigationKey()) {
    if (event.ShiftDown()) {
      m_items.SetSelection(m_items.GetSelection() - 1);
      if (m_items.GetSelection() < 0)
        m_items.SetSelection(m_items.GetItemCount() - 1);

      AdvanceToNextSelectableItem(-1);
    } else {
      m_items.SetSelection(m_items.GetSelection() + 1);
      if (m_items.GetSelection() >= m_items.GetItemCount())
        m_items.SetSelection(0);

      AdvanceToNextSelectableItem(1);
    }

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_DOWN ||
             event.GetKeyCode() == WXK_NUMPAD_DOWN) {
    m_items.SetSelection(m_items.GetSelection() + 1);
    if (m_items.GetSelection() >= m_items.GetItemCount())
      m_items.SetSelection(0);

    AdvanceToNextSelectableItem(1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_UP ||
             event.GetKeyCode() == WXK_NUMPAD_UP) {
    m_items.SetSelection(m_items.GetSelection() - 1);
    if (m_items.GetSelection() < 0)
      m_items.SetSelection(m_items.GetItemCount() - 1);

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_HOME ||
             event.GetKeyCode() == WXK_NUMPAD_HOME) {
    m_items.SetSelection(0);

    AdvanceToNextSelectableItem(1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_END ||
             event.GetKeyCode() == WXK_NUMPAD_END) {
    m_items.SetSelection(m_items.GetItemCount() - 1);

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_LEFT ||
             event.GetKeyCode() == WXK_NUMPAD_LEFT) {
    wxSwitcherItem& item = m_items.GetItem(m_items.GetSelection());

    int row = item.GetRowPos();
    int newCol = item.GetColPos() - 1;
    if (newCol < 0) newCol = (m_items.GetColumnCount() - 1);

    // Find the first item from the end whose row matches and whose column is
    // equal or lower
    int i;
    for (i = m_items.GetItemCount() - 1; i >= 0; i--) {
      wxSwitcherItem& item2 = m_items.GetItem(i);
      if (item2.GetColPos() == newCol && item2.GetRowPos() <= row) {
        m_items.SetSelection(i);
        break;
      }
    }

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_RIGHT ||
             event.GetKeyCode() == WXK_NUMPAD_RIGHT) {
    wxSwitcherItem& item = m_items.GetItem(m_items.GetSelection());

    int row = item.GetRowPos();
    int newCol = item.GetColPos() + 1;
    if (newCol >= m_items.GetColumnCount()) newCol = 0;

    // Find the first item from the end whose row matches and whose column is
    // equal or lower
    int i;
    for (i = m_items.GetItemCount() - 1; i >= 0; i--) {
      wxSwitcherItem& item2 = m_items.GetItem(i);
      if (item2.GetColPos() == newCol && item2.GetRowPos() <= row) {
        m_items.SetSelection(i);
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
  if (m_items.GetItemCount() < 2) return;

  if (m_items.GetSelection() == -1) m_items.SetSelection(0);

  int oldSel = m_items.GetSelection();

  while (true) {
    if (m_items.GetItem(m_items.GetSelection()).GetIsGroup()) {
      m_items.SetSelection(m_items.GetSelection() + direction);
      if (m_items.GetSelection() == -1)
        m_items.SetSelection(m_items.GetItemCount() - 1);
      else if (m_items.GetSelection() == m_items.GetItemCount())
        m_items.SetSelection(0);

      if (m_items.GetSelection() == oldSel) break;
    } else {
      break;
    }
  }
}

void wxMultiColumnListCtrl::GenerateSelectionEvent() {
  wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, GetId());
  event.SetEventObject(this);
  event.SetInt(m_items.GetSelection());

  GetEventHandler()->ProcessEvent(event);
}

void wxMultiColumnListCtrl::CalculateLayout() {
  wxClientDC dc(this);
  CalculateLayout(dc);
}

void wxMultiColumnListCtrl::CalculateLayout(wxDC& dc) {  // NOLINT
  if (m_items.GetSelection() == -1) m_items.SetSelection(0);

  int columnCount = 1;

  // Spacing between edge of window or between columns
  int xMargin = 4;
  int yMargin = 4;

  // Inter-row spacing
  int rowSpacing = 2;

  wxSize itemSize = m_items.CalculateItemSize(dc);
  m_overallSize = wxSize(350, 200);

  size_t i;
  int currentRow = 0;
  int x = xMargin;
  int y = yMargin;

  bool breaking = false;

  for (i = 0; i < (size_t)m_items.GetItemCount(); i++) {
    wxSize oldOverallSize = m_overallSize;

    m_items.GetItem(i).SetRect(wxRect(x, y, itemSize.x, itemSize.y));
    m_items.GetItem(i).SetColPos(columnCount - 1);
    m_items.GetItem(i).SetRowPos(currentRow);

    if (m_items.GetItem(i).GetRect().GetBottom() > m_overallSize.y)
      m_overallSize.y = m_items.GetItem(i).GetRect().GetBottom() + yMargin;

    if (m_items.GetItem(i).GetRect().GetRight() > m_overallSize.x)
      m_overallSize.x = m_items.GetItem(i).GetRect().GetRight() + xMargin;

    currentRow++;

    y += (rowSpacing + itemSize.y);

    bool stopBreaking = breaking;

    if ((currentRow > m_items.GetRowCount()) ||
        (m_items.GetItem(i).GetBreakColumn() && !breaking &&
         (currentRow != 1))) {
      currentRow = 0;
      columnCount++;
      x += (xMargin + itemSize.x);
      y = yMargin;

      // Make sure we don't orphan a group
      if (m_items.GetItem(i).GetIsGroup() ||
          (m_items.GetItem(i).GetBreakColumn() && !breaking)) {
        m_overallSize = oldOverallSize;

        if (m_items.GetItem(i).GetBreakColumn()) breaking = true;

        // Repeat the last item, in the next column
        i--;
      }
    }

    if (stopBreaking) breaking = false;
  }

  m_items.SetColumnCount(columnCount);

  InvalidateBestSize();
}

void wxMultiColumnListCtrl::Init() {
  m_overallSize = wxSize(200, 100);
  m_modifierKey = WXK_CONTROL;
  m_extraNavigationKey = 0;
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
  m_switcherBorderStyle = (style & wxBORDER_MASK);
  if (m_switcherBorderStyle == wxBORDER_NONE)
    m_switcherBorderStyle = wxBORDER_SIMPLE;

  style &= wxBORDER_MASK;
  style |= wxBORDER_NONE;

  wxDialog::Create(parent, id, title, position, size, style);

  m_listCtrl = new wxMultiColumnListCtrl();
  m_listCtrl->SetItems(items);
  m_listCtrl->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                     wxWANTS_CHARS | wxNO_BORDER);
  m_listCtrl->CalculateLayout();

  if (m_extraNavigationKey != -1)
    m_listCtrl->SetExtraNavigationKey(m_extraNavigationKey);

  if (m_modifierKey != -1) m_listCtrl->SetModifierKey(m_modifierKey);

  int borderStyle = wxSIMPLE_BORDER;

  borderStyle = wxBORDER_NONE;
#if defined(__WXMSW__) && wxCHECK_VERSION(2, 8, 5)
// borderStyle = wxBORDER_THEME;
#endif

  m_descriptionCtrl = new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition,
                                       wxSize(-1, 100), borderStyle);
  m_descriptionCtrl->SetHTMLBackgroundColour(GetBackgroundColour());

#ifdef __WXGTK20__
  int fontSize = 11;
  m_descriptionCtrl->SetStandardFonts(fontSize);
#endif

  wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(sizer);

  sizer->Add(m_listCtrl, 1, wxALL | wxEXPAND, 10);
  sizer->Add(m_descriptionCtrl, 0, wxALL | wxEXPAND, 10);

  sizer->SetSizeHints(this);

  m_listCtrl->SetFocus();

  Centre(wxBOTH);

  if (m_listCtrl->GetItems().GetSelection() == -1)
    m_listCtrl->GetItems().SetSelection(0);

  m_listCtrl->AdvanceToNextSelectableItem(1);

  ShowDescription(m_listCtrl->GetItems().GetSelection());

  return true;
}

void wxSwitcherDialog::Init() {
  m_listCtrl = NULL;
  m_descriptionCtrl = NULL;
  m_closing = false;
  m_switcherBorderStyle = 0;

  m_modifierKey = -1;
  m_extraNavigationKey = -1;

#if defined(__WXMSW__) && wxUSE_UXTHEME
/* if (wxUxThemeEngine::GetIfActive())
  m_borderColour = wxColour(49, 106, 197);
else
*/
#endif
  m_borderColour = *wxBLACK;
}

void wxSwitcherDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {  // NOLINT
  if (m_closing) return;

  if (IsModal()) {
    m_closing = true;

    if (GetSelection() == -1)
      EndModal(wxID_CANCEL);
    else
      EndModal(wxID_OK);
  }
}

// Get the selected item
int wxSwitcherDialog::GetSelection() const {
  return m_listCtrl->GetItems().GetSelection();
}

void wxSwitcherDialog::OnActivate(wxActivateEvent& event) {
  if (!event.GetActive()) {
    if (!m_closing) {
      m_closing = true;
      EndModal(wxID_CANCEL);
    }
  }
}

void wxSwitcherDialog::OnPaint(wxPaintEvent& WXUNUSED(event)) {  // NOLINT
  wxPaintDC dc(this);

  if (m_switcherBorderStyle == wxBORDER_SIMPLE) {
    dc.SetPen(wxPen(m_borderColour));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    wxRect rect(GetClientRect());
    dc.DrawRectangle(rect);

    // Draw border around the HTML control
    rect = m_descriptionCtrl->GetRect();
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
  wxSwitcherItem& item = m_listCtrl->GetItems().GetItem(i);

  wxColour colour = m_listCtrl->GetItems().GetBackgroundColour();
  if (!colour.Ok()) colour = GetBackgroundColour();

  wxString backgroundColourHex = ColourToHexString(colour);

  wxString html = wxT("<body bgcolor=\"#") + backgroundColourHex +
                  wxT("\"><b>") + item.GetTitle() + wxT("</b>");

  if (!item.GetDescription().IsEmpty()) {
    html += wxT("<p>");
    html += item.GetDescription();
  }

  html += wxT("</body>");

  m_descriptionCtrl->SetPage(html);
}

void wxSwitcherDialog::SetExtraNavigationKey(int keyCode) {
  m_extraNavigationKey = keyCode;
  if (m_listCtrl) m_listCtrl->SetExtraNavigationKey(keyCode);
}

void wxSwitcherDialog::SetModifierKey(int modifierKey) {
  m_modifierKey = modifierKey;
  if (m_listCtrl) m_listCtrl->SetModifierKey(modifierKey);
}
