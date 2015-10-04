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

/*
 * A control for displaying several columns (not scrollable)
 */

void MultiColumnListCtrl::BindEvents() {
  Bind(wxEVT_LEFT_DOWN, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_LEFT_UP, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_LEFT_DCLICK, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_MIDDLE_DOWN, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_MIDDLE_UP, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_MIDDLE_DCLICK, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_RIGHT_DOWN, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_RIGHT_UP, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_RIGHT_DCLICK, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX1_DOWN, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX1_UP, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX1_DCLICK, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX2_DOWN, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX2_UP, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX2_DCLICK, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_MOTION, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_LEAVE_WINDOW, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_ENTER_WINDOW, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_MOUSEWHEEL, &MultiColumnListCtrl::OnMouseEvent, this);
  Bind(wxEVT_MAGNIFY, &MultiColumnListCtrl::OnMouseEvent, this);

  Bind(wxEVT_PAINT, &MultiColumnListCtrl::OnPaint, this);
  Bind(wxEVT_ERASE_BACKGROUND, &MultiColumnListCtrl::OnEraseBackground, this);
  Bind(wxEVT_CHAR, &MultiColumnListCtrl::OnChar, this);
  Bind(wxEVT_KEY_DOWN, &MultiColumnListCtrl::OnKey, this);
  Bind(wxEVT_KEY_UP, &MultiColumnListCtrl::OnKey, this);
}

IMPLEMENT_CLASS(MultiColumnListCtrl, wxControl)

MultiColumnListCtrl::MultiColumnListCtrl(wxWindow* parent, wxWindowID id,
                                         const wxPoint& pos, const wxSize& size,
                                         long style) {  // NOLINT
  Init();

  Create(parent, id, pos, size, style);
}

bool MultiColumnListCtrl::Create(wxWindow* parent, wxWindowID id,
                                 const wxPoint& pos, const wxSize& size,
                                 long style) {  // NOLINT
  wxControl::Create(parent, id, pos, size, style);

  SetInitialBestSize(size);

  return true;
}

// Overrides
wxSize MultiColumnListCtrl::DoGetBestSize() const { return overall_size_; }

void MultiColumnListCtrl::SendCloseEvent() {
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

void MultiColumnListCtrl::OnEraseBackground(
    wxEraseEvent& WXUNUSED(event)) {  // NOLINT
                                      // Do nothing
}

void MultiColumnListCtrl::OnPaint(wxPaintEvent& WXUNUSED(event)) {  // NOLINT
#if wxSWITCHER_USE_BUFFERED_PAINTING
  wxBufferedPaintDC dc(this);
#else
  wxPaintDC dc(this);
#endif

  wxRect rect = GetClientRect();

  if (items_.column_count() == 0) CalculateLayout(dc);

  if (items_.column_count() == 0) return;

  items_.PaintItems(dc, this);
}

void MultiColumnListCtrl::OnMouseEvent(wxMouseEvent& event) {
  if (event.LeftDown()) {
    SetFocus();

    int idx = items_.HitTest(event.GetPosition());
    if (idx != wxNOT_FOUND) {
      items_.set_selection(idx);

      SendCloseEvent();
    }
  }
}

void MultiColumnListCtrl::OnChar(wxKeyEvent& WXUNUSED(event)) {}  // NOLINT

void MultiColumnListCtrl::OnKey(wxKeyEvent& event) {
  if (event.GetEventType() == wxEVT_KEY_UP) {
    if (event.GetKeyCode() == GetModifierKey()) {
      SendCloseEvent();
    }
    event.Skip();
    return;
  }

  if (event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_RETURN) {
    if (event.GetKeyCode() == WXK_ESCAPE) items_.set_selection(-1);

    SendCloseEvent();
  } else if (event.GetKeyCode() == WXK_TAB ||
             event.GetKeyCode() == GetExtraNavigationKey()) {
    if (event.ShiftDown()) {
      items_.set_selection(items_.selection() - 1);
      if (items_.selection() < 0)
        items_.set_selection(items_.GetItemCount() - 1);

      AdvanceToNextSelectableItem(-1);
    } else {
      items_.set_selection(items_.selection() + 1);
      if (items_.selection() >= items_.GetItemCount()) items_.set_selection(0);

      AdvanceToNextSelectableItem(1);
    }

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_DOWN ||
             event.GetKeyCode() == WXK_NUMPAD_DOWN) {
    items_.set_selection(items_.selection() + 1);
    if (items_.selection() >= items_.GetItemCount()) items_.set_selection(0);

    AdvanceToNextSelectableItem(1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_UP ||
             event.GetKeyCode() == WXK_NUMPAD_UP) {
    items_.set_selection(items_.selection() - 1);
    if (items_.selection() < 0) items_.set_selection(items_.GetItemCount() - 1);

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_HOME ||
             event.GetKeyCode() == WXK_NUMPAD_HOME) {
    items_.set_selection(0);

    AdvanceToNextSelectableItem(1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_END ||
             event.GetKeyCode() == WXK_NUMPAD_END) {
    items_.set_selection(items_.GetItemCount() - 1);

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_LEFT ||
             event.GetKeyCode() == WXK_NUMPAD_LEFT) {
    SwitcherItem& item = items_.GetItem(items_.selection());

    int row = item.row_pos();
    int newCol = item.get_col_pos() - 1;
    if (newCol < 0) newCol = (items_.column_count() - 1);

    // Find the first item from the end whose row matches and whose column is
    // equal or lower
    int i;
    for (i = items_.GetItemCount() - 1; i >= 0; i--) {
      SwitcherItem& item2 = items_.GetItem(i);
      if (item2.get_col_pos() == newCol && item2.row_pos() <= row) {
        items_.set_selection(i);
        break;
      }
    }

    AdvanceToNextSelectableItem(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_RIGHT ||
             event.GetKeyCode() == WXK_NUMPAD_RIGHT) {
    SwitcherItem& item = items_.GetItem(items_.selection());

    int row = item.row_pos();
    int newCol = item.get_col_pos() + 1;
    if (newCol >= items_.column_count()) newCol = 0;

    // Find the first item from the end whose row matches and whose column is
    // equal or lower
    int i;
    for (i = items_.GetItemCount() - 1; i >= 0; i--) {
      SwitcherItem& item2 = items_.GetItem(i);
      if (item2.get_col_pos() == newCol && item2.row_pos() <= row) {
        items_.set_selection(i);
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
void MultiColumnListCtrl::AdvanceToNextSelectableItem(int direction) {
  if (items_.GetItemCount() < 2) return;

  if (items_.selection() == -1) items_.set_selection(0);

  int oldSel = items_.selection();

  while (true) {
    if (items_.GetItem(items_.selection()).is_group()) {
      items_.set_selection(items_.selection() + direction);
      if (items_.selection() == -1)
        items_.set_selection(items_.GetItemCount() - 1);
      else if (items_.selection() == items_.GetItemCount())
        items_.set_selection(0);

      if (items_.selection() == oldSel) break;
    } else {
      break;
    }
  }
}

void MultiColumnListCtrl::GenerateSelectionEvent() {
  wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, GetId());
  event.SetEventObject(this);
  event.SetInt(items_.selection());

  GetEventHandler()->ProcessEvent(event);
}

void MultiColumnListCtrl::CalculateLayout() {
  wxClientDC dc(this);
  CalculateLayout(dc);
}

void MultiColumnListCtrl::CalculateLayout(wxDC& dc) {  // NOLINT
  if (items_.selection() == -1) items_.set_selection(0);

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

    items_.GetItem(i).set_rect(wxRect(x, y, itemSize.x, itemSize.y));
    items_.GetItem(i).set_col_pos(columnCount - 1);
    items_.GetItem(i).set_row_pos(currentRow);

    if (items_.GetItem(i).rect().GetBottom() > overall_size_.y)
      overall_size_.y = items_.GetItem(i).rect().GetBottom() + yMargin;

    if (items_.GetItem(i).rect().GetRight() > overall_size_.x)
      overall_size_.x = items_.GetItem(i).rect().GetRight() + xMargin;

    currentRow++;

    y += (rowSpacing + itemSize.y);

    bool stopBreaking = breaking;

    if ((currentRow > items_.row_count()) ||
        (items_.GetItem(i).break_column() && !breaking && (currentRow != 1))) {
      currentRow = 0;
      columnCount++;
      x += (xMargin + itemSize.x);
      y = yMargin;

      // Make sure we don't orphan a group
      if (items_.GetItem(i).is_group() ||
          (items_.GetItem(i).break_column() && !breaking)) {
        overall_size_ = oldOverallSize;

        if (items_.GetItem(i).break_column()) breaking = true;

        // Repeat the last item, in the next column
        i--;
      }
    }

    if (stopBreaking) breaking = false;
  }

  items_.set_column_count(columnCount);

  InvalidateBestSize();
}

void MultiColumnListCtrl::Init() {
  overall_size_ = wxSize(200, 100);
  modifier_key_ = WXK_CONTROL;
  extra_navigation_key_ = 0;
}

void SwitcherDialog::BindEvents() {
  Bind(wxEVT_CLOSE_WINDOW, &SwitcherDialog::OnCloseWindow, this);
  Bind(wxEVT_ACTIVATE, &SwitcherDialog::OnActivate, this);
  Bind(wxEVT_LISTBOX, &SwitcherDialog::OnSelectItem, this, wxID_ANY);
  Bind(wxEVT_PAINT, &SwitcherDialog::OnPaint, this);
}

SwitcherDialog::SwitcherDialog(const SwitcherItemList& items, wxWindow* parent,
                               wxWindowID id, const wxString& title,
                               const wxPoint& position, const wxSize& size,
                               long style) {  // NOLINT
  BindEvents();
  Init();

  Create(items, parent, id, title, position, size, style);
}

bool SwitcherDialog::Create(const SwitcherItemList& items, wxWindow* parent,
                            wxWindowID id, const wxString& title,
                            const wxPoint& position, const wxSize& size,
                            long style) {  // NOLINT
  switcher_border_style_ = (style & wxBORDER_MASK);
  if (switcher_border_style_ == wxBORDER_NONE)
    switcher_border_style_ = wxBORDER_SIMPLE;

  style &= wxBORDER_MASK;
  style |= wxBORDER_NONE;

  wxDialog::Create(parent, id, title, position, size, style);

  list_ctrl_ = new MultiColumnListCtrl();
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

  if (list_ctrl_->GetItems().selection() == -1)
    list_ctrl_->GetItems().set_selection(0);

  list_ctrl_->AdvanceToNextSelectableItem(1);

  ShowDescription(list_ctrl_->GetItems().selection());

  return true;
}

void SwitcherDialog::Init() {
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

void SwitcherDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {  // NOLINT
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
int SwitcherDialog::GetSelection() const {
  return list_ctrl_->GetItems().selection();
}

void SwitcherDialog::OnActivate(wxActivateEvent& event) {
  if (!event.GetActive()) {
    if (!is_closing_) {
      is_closing_ = true;
      EndModal(wxID_CANCEL);
    }
  }
}

void SwitcherDialog::OnPaint(wxPaintEvent& WXUNUSED(event)) {  // NOLINT
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

void SwitcherDialog::OnSelectItem(wxCommandEvent& event) {
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

void SwitcherDialog::ShowDescription(int i) {
  SwitcherItem& item = list_ctrl_->GetItems().GetItem(i);

  wxColour colour = list_ctrl_->GetItems().background_color();
  if (!colour.Ok()) colour = GetBackgroundColour();

  wxString backgroundColourHex = ColourToHexString(colour);

  wxString html = wxT("<body bgcolor=\"#") + backgroundColourHex +
                  wxT("\"><b>") + item.title() + wxT("</b>");

  if (!item.description().IsEmpty()) {
    html += wxT("<p>");
    html += item.description();
  }

  html += wxT("</body>");

  description_ctrl_->SetPage(html);
}

void SwitcherDialog::SetExtraNavigationKey(int keyCode) {
  extra_navigation_key_ = keyCode;
  if (list_ctrl_) list_ctrl_->SetExtraNavigationKey(keyCode);
}

void SwitcherDialog::SetModifierKey(int modifierKey) {
  modifier_key_ = modifierKey;
  if (list_ctrl_) list_ctrl_->SetModifierKey(modifierKey);
}
