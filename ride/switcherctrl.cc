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

#include <algorithm>

#define wxSWITCHER_USE_BUFFERED_PAINTING 1

#define MODIFIER_KEY WXK_CONTROL
#define EXTRA_NAVIGATION_KEY 0

// In GTK+ we can't use Ctrl+Tab; we use Ctrl+/ instead and tell the switcher
// to treat / in the same was as tab (i.e. cycle through the names)
#ifdef __WXGTK__
#undef EXTRA_NAVIGATION_KEY
#define EXTRA_NAVIGATION_KEY wxT('/');
#endif

#ifdef __WXMAC__
#undef MODIFIER_KEY
#define MODIFIER_KEY WXK_ALT;
#endif

//////////////////////////////////////////////////////////////////////////
// move theese

SwitcherIndex GoToFirstItem(const SwitcherItemList& items) {
  for (size_t group_index = 0; group_index <= items.GetGroupCount();
       ++group_index) {
    if (items.GetGroupCount() > 0) {
      return SwitcherIndex(group_index, 0);
    }
  }
  return SWITCHER_NOT_FOUND;
}

SwitcherIndex GoToLastItem(const SwitcherItemList& items) {
  for (size_t group_index = items.GetGroupCount() - 1; group_index >= 0;
       --group_index) {
    if (items.GetGroupCount() > 0) {
      return SwitcherIndex(group_index, 0);
    }
  }
  return SWITCHER_NOT_FOUND;
}

int Wrap(int i, int size) {
  int r = i;
  while (r < 0) {
    r += size;
  }
  while (r >= size) {
    r -= size;
  }
  return r;
}

int StepGroup(const SwitcherItemList& items, int g, int change) {
  int group = g;
  do {
    group = Wrap(group + change, items.GetGroupCount());
    if (g == group) {
      return -1;
    }
  } while (items.GetGroup(group).GetItemCount() <= 0);
  return group;
}

SwitcherIndex GoToRelativeItem(const SwitcherItemList& items, SwitcherIndex i,
                               bool local, int change) {
  if (i == SWITCHER_NOT_FOUND) return SWITCHER_NOT_FOUND;

  int group = i.first;
  int index = i.second + change;
  if (local) {
    index = Wrap(index, items.GetGroup(group).GetItemCount());
    return SwitcherIndex(group, index);
  } else {
    if (index < 0) {
      group = StepGroup(items, group, -1);
      if (group == -1) {
        return SWITCHER_NOT_FOUND;
      }
      index = items.GetGroup(group).GetItemCount() - 1;
      return SwitcherIndex(group, index);
    }

    if (index >= items.GetGroup(group).GetItemCount()) {
      group = StepGroup(items, group, 1);
      if (group == -1) {
        return SWITCHER_NOT_FOUND;
      }
      index = 0;
      return SwitcherIndex(group, index);
    }

    return SwitcherIndex(group, index);
  }
}

SwitcherIndex GoToPreviousItem(const SwitcherItemList& items, SwitcherIndex i,
                               bool local) {
  return GoToRelativeItem(items, i, local, -1);
}

SwitcherIndex GoToNextItem(const SwitcherItemList& items, SwitcherIndex i,
                           bool local) {
  return GoToRelativeItem(items, i, local, 1);
}

SwitcherIndex GoToLeftItem(const SwitcherItemList& items, SwitcherIndex i) {
  return SWITCHER_NOT_FOUND;
}

SwitcherIndex GoToRightItem(const SwitcherItemList& items, SwitcherIndex i) {
  return SWITCHER_NOT_FOUND;
}

//////////////////////////////////////////////////////////////////////////

SwitcherCtrl::SwitcherCtrl(const SwitcherItemList& items)
    : items_(items),
      selection_(SWITCHER_NOT_FOUND),
      overall_size_(wxSize(200, 100)) {
  // TODO(Gustav): Remove unused events
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

void SwitcherCtrl::SelectActiveOrFirst() {
  selection_ = items_.GetIndexForFocus();
  if (selection_ == SWITCHER_NOT_FOUND) {
    selection_ = GoToFirstItem(items_);
  }
}

SwitcherIndex SwitcherCtrl::selection() const { return selection_; }

void SwitcherCtrl::OnPaint(wxPaintEvent& WXUNUSED(event)) {  // NOLINT
#if wxSWITCHER_USE_BUFFERED_PAINTING
  wxBufferedPaintDC dc(this);
#else
  wxPaintDC dc(this);
#endif

  wxRect rect = GetClientRect();

  if (items_.column_count() == 0) CalculateLayout(dc);

  if (items_.column_count() == 0) return;

  items_.PaintItems(&dc, style_, selection_, this);
}

void SwitcherCtrl::OnMouseEvent(wxMouseEvent& event) {
  if (event.LeftDown()) {
    SetFocus();

    SwitcherIndex idx = items_.HitTest(event.GetPosition());
    if (idx != SWITCHER_NOT_FOUND) {
      selection_ = idx;

      SendCloseEvent();
    }
  }
}

void SwitcherCtrl::OnChar(wxKeyEvent& WXUNUSED(event)) {}  // NOLINT

void SwitcherCtrl::OnKey(wxKeyEvent& event) {
  if (event.GetEventType() == wxEVT_KEY_UP) {
    if (event.GetKeyCode() == MODIFIER_KEY) {
      SendCloseEvent();
    }
    event.Skip();
    return;
  }

  if (event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_RETURN) {
    if (event.GetKeyCode() == WXK_ESCAPE) selection_ = SWITCHER_NOT_FOUND;

    SendCloseEvent();
  } else if (event.GetKeyCode() == WXK_TAB ||
             event.GetKeyCode() == EXTRA_NAVIGATION_KEY) {
    if (event.ShiftDown()) {
      selection_ = GoToPreviousItem(items_, selection_, true);
    } else {
      selection_ = GoToNextItem(items_, selection_, true);
    }

    GenerateSelectionEvent();
    Refresh();
  } else if (event.GetKeyCode() == WXK_DOWN ||
             event.GetKeyCode() == WXK_NUMPAD_DOWN) {
    selection_ = GoToNextItem(items_, selection_, false);

    GenerateSelectionEvent();
    Refresh();
  } else if (event.GetKeyCode() == WXK_UP ||
             event.GetKeyCode() == WXK_NUMPAD_UP) {
    selection_ = GoToPreviousItem(items_, selection_, false);

    GenerateSelectionEvent();
    Refresh();
  } else if (event.GetKeyCode() == WXK_HOME ||
             event.GetKeyCode() == WXK_NUMPAD_HOME) {
    selection_ = GoToFirstItem(items_);

    GenerateSelectionEvent();
    Refresh();
  } else if (event.GetKeyCode() == WXK_END ||
             event.GetKeyCode() == WXK_NUMPAD_END) {
    selection_ = GoToLastItem(items_);

    GenerateSelectionEvent();
    Refresh();
  } else if (event.GetKeyCode() == WXK_LEFT ||
             event.GetKeyCode() == WXK_NUMPAD_LEFT) {
    selection_ = GoToLeftItem(items_, selection_);

    GenerateSelectionEvent();
    Refresh();
  } else if (event.GetKeyCode() == WXK_RIGHT ||
             event.GetKeyCode() == WXK_NUMPAD_RIGHT) {
    selection_ = GoToRightItem(items_, selection_);

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

class LayoutCalculator {
 public:
  LayoutCalculator(wxSize is, const SwitcherStyle& style) : style_(style) {
    columnCount = 1;

    itemSize = is;
    overall_size_ = wxSize(350, 200);

    currentRow = 0;
    x = style_.xMargin();
    y = style_.yMargin();
  }

  wxRect rect() const { return wxRect(x, y, itemSize.x, itemSize.y); }

  int col_pos() const { return columnCount - 1; }

  int row_pos() const { return currentRow; }

  void GoToNextCol() {
    currentRow = 0;
    columnCount++;
    x += (style_.xMargin() + itemSize.x);
    y = style_.yMargin();
  }

  void GoToNextRow() {
    currentRow++;
    y += (style_.rowSpacing() + itemSize.y);
  }

  void UpdateOverallSize() {
    overall_size_.y =
        std::max(overall_size_.y, rect().GetBottom() + style_.yMargin());
    overall_size_.x =
        std::max(overall_size_.x, rect().GetRight() + style_.xMargin());
  }

  int get_currentRow() const { return currentRow; }

  int get_columnCount() const { return columnCount; }

  const wxSize& overall_size() const { return overall_size_; }

 private:
  const SwitcherStyle& style_;
  int columnCount;

  wxSize itemSize;
  wxSize overall_size_;

  int currentRow;
  int x;
  int y;
};

void SwitcherCtrl::CalculateLayout(wxDC& dc) {  // NOLINT
  if (selection_ == SWITCHER_NOT_FOUND) selection_ = GoToFirstItem(items_);

  LayoutCalculator calc(items_.CalculateItemSize(&dc, style_), style_);

  for (size_t group_index = 0; group_index < items_.GetGroupCount();
       ++group_index) {
    SwitcherGroup& group = items_.GetGroup(group_index);

    group.set_rect(calc.rect());
    calc.UpdateOverallSize();
    calc.GoToNextRow();

    for (size_t item_index = 0; item_index < group.GetItemCount();
         ++item_index) {
      if (calc.get_currentRow() > style_.row_count()) {
        calc.GoToNextCol();
        calc.GoToNextRow();  // only groups are on first row
      }

      SwitcherItem& item = group.GetItem(item_index);
      item.set_rect(calc.rect());
      item.set_col_pos(calc.col_pos());
      item.set_row_pos(calc.row_pos());

      calc.UpdateOverallSize();

      calc.GoToNextRow();
    }
    calc.GoToNextCol();
  }

  items_.set_column_count(calc.get_columnCount());
  overall_size_ = calc.overall_size();

  InvalidateBestSize();
}

void SwitcherCtrl::InvalidateLayout() {
  items_.set_column_count(0);
  Refresh();
}

void SwitcherCtrl::GenerateSelectionEvent() {
  wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, GetId());
  event.SetEventObject(this);
  event.SetInt(selection_.first);

  GetEventHandler()->ProcessEvent(event);
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
    selection_ = GoToPreviousItem(items_, selection_, true);
  } else {
    selection_ = GoToNextItem(items_, selection_, true);
  }
}
