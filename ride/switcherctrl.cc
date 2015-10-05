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

#include "ride/switcherctrl.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

#define wxSWITCHER_USE_BUFFERED_PAINTING 1

SwitcherCtrl::SwitcherCtrl()
    : overall_size_(wxSize(200, 100)),
      modifier_key_(WXK_CONTROL),
      extra_navigation_key_(0) {
  Bind(wxEVT_LEFT_DOWN, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_LEFT_UP, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_LEFT_DCLICK, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_MIDDLE_DOWN, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_MIDDLE_UP, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_MIDDLE_DCLICK, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_RIGHT_DOWN, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_RIGHT_UP, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_RIGHT_DCLICK, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX1_DOWN, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX1_UP, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX1_DCLICK, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX2_DOWN, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX2_UP, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_AUX2_DCLICK, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_MOTION, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_LEAVE_WINDOW, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_ENTER_WINDOW, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_MOUSEWHEEL, &SwitcherCtrl::OnMouseEvent, this);
  Bind(wxEVT_MAGNIFY, &SwitcherCtrl::OnMouseEvent, this);

  Bind(wxEVT_PAINT, &SwitcherCtrl::OnPaint, this);
  Bind(wxEVT_ERASE_BACKGROUND, &SwitcherCtrl::OnEraseBackground, this);
  Bind(wxEVT_CHAR, &SwitcherCtrl::OnChar, this);
  Bind(wxEVT_KEY_DOWN, &SwitcherCtrl::OnKey, this);
  Bind(wxEVT_KEY_UP, &SwitcherCtrl::OnKey, this);
}

bool SwitcherCtrl::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                          const wxSize& size,
                          long style) {  // NOLINT
  wxControl::Create(parent, id, pos, size, style);

  SetInitialBestSize(size);

  return true;
}

void SwitcherCtrl::set_items(const SwitcherItemList& items) { items_ = items; }

const SwitcherItemList& SwitcherCtrl::items() const { return items_; }

SwitcherItemList& SwitcherCtrl::items() { return items_; }

void SwitcherCtrl::set_extra_navigation_key(int keyCode) {
  extra_navigation_key_ = keyCode;
}

int SwitcherCtrl::extra_navigation_key() const { return extra_navigation_key_; }

void SwitcherCtrl::set_modifier_key(int modifierKey) {
  modifier_key_ = modifierKey;
}

int SwitcherCtrl::modifier_key() const { return modifier_key_; }

void SwitcherCtrl::OnPaint(wxPaintEvent& WXUNUSED(event)) {  // NOLINT
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

void SwitcherCtrl::OnMouseEvent(wxMouseEvent& event) {
  if (event.LeftDown()) {
    SetFocus();

    int idx = items_.HitTest(event.GetPosition());
    if (idx != wxNOT_FOUND) {
      items_.set_selection(idx);

      SendCloseEvent();
    }
  }
}

void SwitcherCtrl::OnChar(wxKeyEvent& WXUNUSED(event)) {}  // NOLINT

void SwitcherCtrl::OnKey(wxKeyEvent& event) {
  if (event.GetEventType() == wxEVT_KEY_UP) {
    if (event.GetKeyCode() == modifier_key()) {
      SendCloseEvent();
    }
    event.Skip();
    return;
  }

  if (event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_RETURN) {
    if (event.GetKeyCode() == WXK_ESCAPE) items_.set_selection(-1);

    SendCloseEvent();
  } else if (event.GetKeyCode() == WXK_TAB ||
             event.GetKeyCode() == extra_navigation_key()) {
    if (event.ShiftDown()) {
      items_.set_selection(items_.selection() - 1);
      if (items_.selection() < 0)
        items_.set_selection(items_.GetItemCount() - 1);

      MakeSureGroupIsNotSelected(-1);
    } else {
      items_.set_selection(items_.selection() + 1);
      if (items_.selection() >= items_.GetItemCount()) items_.set_selection(0);

      MakeSureGroupIsNotSelected(1);
    }

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_DOWN ||
             event.GetKeyCode() == WXK_NUMPAD_DOWN) {
    items_.set_selection(items_.selection() + 1);
    if (items_.selection() >= items_.GetItemCount()) items_.set_selection(0);

    MakeSureGroupIsNotSelected(1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_UP ||
             event.GetKeyCode() == WXK_NUMPAD_UP) {
    items_.set_selection(items_.selection() - 1);
    if (items_.selection() < 0) items_.set_selection(items_.GetItemCount() - 1);

    MakeSureGroupIsNotSelected(-1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_HOME ||
             event.GetKeyCode() == WXK_NUMPAD_HOME) {
    items_.set_selection(0);

    MakeSureGroupIsNotSelected(1);

    GenerateSelectionEvent();

    Refresh();
  } else if (event.GetKeyCode() == WXK_END ||
             event.GetKeyCode() == WXK_NUMPAD_END) {
    items_.set_selection(items_.GetItemCount() - 1);

    MakeSureGroupIsNotSelected(-1);

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

    MakeSureGroupIsNotSelected(-1);

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

    MakeSureGroupIsNotSelected(1);

    GenerateSelectionEvent();

    Refresh();
  } else {
    event.Skip();
  }
}

void SwitcherCtrl::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {  // NOLINT
  // Do nothing
}

wxSize SwitcherCtrl::DoGetBestSize() const { return overall_size_; }

void SwitcherCtrl::CalculateLayout() {
  wxClientDC dc(this);
  CalculateLayout(dc);
}

void SwitcherCtrl::CalculateLayout(wxDC& dc) {  // NOLINT
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
void SwitcherCtrl::InvalidateLayout() {
  items_.set_column_count(0);
  Refresh();
}

void SwitcherCtrl::GenerateSelectionEvent() {
  wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, GetId());
  event.SetEventObject(this);
  event.SetInt(items_.selection());

  GetEventHandler()->ProcessEvent(event);
}

void SwitcherCtrl::MakeSureGroupIsNotSelected(int direction) {
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

void SwitcherCtrl::SendCloseEvent() {
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

void SwitcherCtrl::AdvanceToNextSelection(bool forward) {
  if (forward == false) {
    items_.set_selection(items_.selection() - 1);
    if (items_.selection() < 0) items_.set_selection(items_.GetItemCount() - 1);

    MakeSureGroupIsNotSelected(-1);
  } else {
    items_.set_selection(items_.selection() + 1);
    if (items_.selection() >= items_.GetItemCount()) items_.set_selection(0);

    MakeSureGroupIsNotSelected(1);
  }
}
