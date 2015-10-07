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

bool IsValid(const SwitcherItemList& items, SwitcherIndex index) {
  if (index.first < 0) return false;
  if (index.first >= items.GetGroupCount()) return false;
  if (index.second < 0) return false;
  if (index.second >= items.GetGroup(index.first).GetItemCount()) return false;
  return true;
}

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

SwitcherIndex GoHorizontal(const SwitcherItemList& items, SwitcherIndex i,
                           const SwitcherStyle& style, int change) {
  const int row_count =
      style.row_count() - 1;  // -1 since we should ignore the group header item
  // first, try to move withing the group
  const int internal_item = i.second + change * row_count;
  const int first_group_count = items.GetGroup(i.first).GetItemCount();
  if ((internal_item >= 0) && internal_item < first_group_count) {
    // this was ok, return it
    return SwitcherIndex(i.first, internal_item);
  }

  if (internal_item >= first_group_count) {
    const int last_item = first_group_count - 1;
    const int col_index_before = i.second - (i.second % row_count);
    const int col_index_end = last_item - (last_item % row_count);
    if (col_index_before != col_index_end) {
      return SwitcherIndex(i.first, last_item);
    }
  }

  // move to a second group
  int group = StepGroup(items, i.first, change);
  if (group == -1) return SWITCHER_NOT_FOUND;
  int item_in_col = i.second % row_count;
  int item = item_in_col;

  if (change < 0) {
    // Get the starting index of the last/rightmost column
    int last_col_index = 0;
    while (last_col_index + row_count < items.GetGroup(group).GetItemCount()) {
      last_col_index += row_count;
    }

    // use that index with our offset or the maximum index, whatever comes first
    item = std::min(last_col_index + item_in_col,
                    items.GetGroup(group).GetItemCount() - 1);
  }

  return SwitcherIndex(group, item);
}

SwitcherIndex GoToLeftItem(const SwitcherItemList& items,
                           const SwitcherStyle& style, SwitcherIndex i) {
  return GoHorizontal(items, i, style, -1);
}

SwitcherIndex GoToRightItem(const SwitcherItemList& items,
                            const SwitcherStyle& style, SwitcherIndex i) {
  return GoHorizontal(items, i, style, 1);
}

//////////////////////////////////////////////////////////////////////////

SwitcherCtrl::SwitcherCtrl(const SwitcherItemList& items,
                           const SwitcherStyle& style)
    : items_(items),
      style_(style),
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

  CalculateLayout();
  SetInitialBestSize(overall_size_);
  SetMinClientSize(overall_size_);

  return true;
}

void SwitcherCtrl::set_items(const SwitcherItemList& items) { items_ = items; }

const SwitcherItemList& SwitcherCtrl::items() const { return items_; }

SwitcherItemList& SwitcherCtrl::items() { return items_; }

void SwitcherCtrl::SelectOrFirst(SwitcherIndex index) {
  selection_ = index;
  if (false == IsValid(items_, selection_)) {
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

  if (items_.column_count() == 0) CalculateLayout(dc);

  if (items_.column_count() == 0) return;
  wxRect rect = GetClientRect();
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
    selection_ = GoToLeftItem(items_, style_, selection_);

    GenerateSelectionEvent();
    Refresh();
  } else if (event.GetKeyCode() == WXK_RIGHT ||
             event.GetKeyCode() == WXK_NUMPAD_RIGHT) {
    selection_ = GoToRightItem(items_, style_, selection_);

    GenerateSelectionEvent();
    Refresh();
  } else {
    event.Skip();
  }
}

void SwitcherCtrl::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {  // NOLINT
  // Do nothing
}

wxSize SwitcherCtrl::DoGetBestClientSize() const { return overall_size_; }

void SwitcherCtrl::CalculateLayout() {
  wxClientDC dc(this);
  CalculateLayout(dc);
}

class LayoutCalculator {
 public:
  LayoutCalculator(wxSize is, const SwitcherStyle& style) : style_(style) {
    columnCount = 1;

    itemSize = is;
    overall_size_ = wxSize(1, 1);

    currentRow = 0;
    x = style_.x_margin();
    y = style_.y_margin();
  }

  wxRect rect() const { return wxRect(x, y, itemSize.x, itemSize.y); }

  int col_pos() const { return columnCount - 1; }

  int row_pos() const { return currentRow; }

  void GoToNextCol() {
    currentRow = 0;
    columnCount++;
    x += (style_.col_spacing() + itemSize.x);
    y = style_.y_margin();
  }

  void GoToNextRow() {
    currentRow++;
    y += (style_.row_spacing() + itemSize.y);
  }

  void UpdateOverallSize() {
    wxRect r = rect();
    overall_size_.y =
        std::max(overall_size_.y, r.GetBottom() + 1 + style_.y_margin());
    overall_size_.x =
        std::max(overall_size_.x, r.GetRight() + 1 + style_.x_margin());
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

  wxSize items_size = items_.CalculateItemSize(&dc, style_);
  items_size.x = std::min(items_size.x, style_.item_maxwidth());
  items_size.y = std::min(items_size.y, style_.item_maxheight());
  LayoutCalculator calc(items_size, style_);

  for (size_t group_index = 0; group_index < items_.GetGroupCount();
       ++group_index) {
    SwitcherGroup& group = items_.GetGroup(group_index);

    group.set_rect(calc.rect());
    calc.UpdateOverallSize();
    calc.GoToNextRow();

    for (size_t item_index = 0; item_index < group.GetItemCount();
         ++item_index) {
      if (calc.get_currentRow() >= style_.row_count()) {
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

  // fix b/c margin isn't considered in client size with boxsizer
  overall_size_.x += style_.dlg_main_border() * 2;
  overall_size_.y += style_.dlg_main_border() * 2;

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
