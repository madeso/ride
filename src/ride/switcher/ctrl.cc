#include "ctrl.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

#include <algorithm>

namespace switcher {

#define wxSWITCHER_USE_BUFFERED_PAINTING 1

#define MODIFIER_KEY WXK_CONTROL
#define EXTRA_NAVIGATION_KEY 0

// In GTK+ we can't use Ctrl+Tab; we use Ctrl+/ instead and tell the switcher
// to treat / in the same was as tab (i.e. cycle through the names)
#ifdef __WXGTK__
#undef EXTRA_NAVIGATION_KEY
#define EXTRA_NAVIGATION_KEY wxT('/')
#endif

#ifdef __WXMAC__
#undef MODIFIER_KEY
#define MODIFIER_KEY WXK_RAW_CONTROL
#define DONT_USE_ARROWKEYS
#endif

// vim style navigation
#define VIM_LEFT 'H'
#define VIM_DOWN 'J'
#define VIM_UP 'K'
#define VIM_RIGHT 'L'

//////////////////////////////////////////////////////////////////////////
// move these

bool IsValid(const ItemList& items, Index index) {
  if (index.first < 0) return false;
  if (index.first >= items.GetGroupCount()) return false;
  if (index.second < 0) return false;
  if (index.second >= items.GetGroup(index.first).GetItemCount()) return false;
  return true;
}

Index GoToFirstItem(const ItemList& items, int group = 0) {
  for (size_t group_index = group; group_index <= items.GetGroupCount();
       ++group_index) {
    if (items.GetGroupCount() > 0) {
      return Index(group_index, 0);
    }
  }
  if (group != 0) {
    return GoToFirstItem(items, 0);
  } else {
    return SWITCHER_NOT_FOUND;
  }
}

Index GoToLastItem(const ItemList& items) {
  for (int group_index = items.GetGroupCount() - 1; group_index >= 0;
       --group_index) {
    int count = items.GetGroup(group_index).GetItemCount();
    if (count > 0) {
      return Index(group_index, count - 1);
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

int StepGroup(const ItemList& items, int g, int change) {
  int group = g;
  do {
    group = Wrap(group + change, items.GetGroupCount());
    if (g == group) {
      return -1;
    }
  } while (items.GetGroup(group).GetItemCount() <= 0);
  return group;
}

Index GoToRelativeItem(const ItemList& items, Index i, bool local, int change) {
  if (i == SWITCHER_NOT_FOUND) return SWITCHER_NOT_FOUND;

  int group = i.first;
  int index = i.second + change;
  if (local) {
    index = Wrap(index, items.GetGroup(group).GetItemCount());
    return Index(group, index);
  } else {
    if (index < 0) {
      group = StepGroup(items, group, -1);
      if (group == -1) {
        return SWITCHER_NOT_FOUND;
      }
      index = items.GetGroup(group).GetItemCount() - 1;
      return Index(group, index);
    }

    if (index >= items.GetGroup(group).GetItemCount()) {
      group = StepGroup(items, group, 1);
      if (group == -1) {
        return SWITCHER_NOT_FOUND;
      }
      index = 0;
      return Index(group, index);
    }

    return Index(group, index);
  }
}

Index GoToPreviousItem(const ItemList& items, Index i, bool local) {
  return GoToRelativeItem(items, i, local, -1);
}

Index GoToNextItem(const ItemList& items, Index i, bool local) {
  return GoToRelativeItem(items, i, local, 1);
}

Index GoHorizontal(const ItemList& items, Index i, const Style& style,
                   int change) {
  const int row_count =
      style.row_count() - 1;  // -1 since we should ignore the group header item
  // first, try to move withing the group
  const int internal_item = i.second + change * row_count;
  const int first_group_count = items.GetGroup(i.first).GetItemCount();
  if ((internal_item >= 0) && internal_item < first_group_count) {
    // this was ok, return it
    return Index(i.first, internal_item);
  }

  if (internal_item >= first_group_count) {
    const int last_item = first_group_count - 1;
    const int col_index_before = i.second - (i.second % row_count);
    const int col_index_end = last_item - (last_item % row_count);
    if (col_index_before != col_index_end) {
      return Index(i.first, last_item);
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

  return Index(group, item);
}

Index GoToLeftItem(const ItemList& items, const Style& style, Index i) {
  return GoHorizontal(items, i, style, -1);
}

Index GoToRightItem(const ItemList& items, const Style& style, Index i) {
  return GoHorizontal(items, i, style, 1);
}

//////////////////////////////////////////////////////////////////////////

enum { ID_FIRST = wxID_HIGHEST, ID_TAB_PREV, ID_TAB_NEXT };

Ctrl::Ctrl(const ItemList& items, const Style& style)
    : items_(items),
      selection_(SWITCHER_NOT_FOUND),
      style_(style),
      overall_size_(wxSize(200, 100)) {
  Bind(wxEVT_LEFT_UP, &Ctrl::OnMouseClick, this);
  Bind(wxEVT_MOTION, &Ctrl::OnMouseMove, this);
  Bind(wxEVT_ENTER_WINDOW, &Ctrl::OnMouseMove, this);

  Bind(wxEVT_PAINT, &Ctrl::OnPaint, this);
  Bind(wxEVT_ERASE_BACKGROUND, &Ctrl::OnEraseBackground, this);
  Bind(wxEVT_CHAR, &Ctrl::OnChar, this);
  Bind(wxEVT_KEY_DOWN, &Ctrl::OnKey, this);
  Bind(wxEVT_KEY_UP, &Ctrl::OnKey, this);

  Bind(wxEVT_MENU, &Ctrl::OnTabNext, this, ID_TAB_NEXT);
  Bind(wxEVT_MENU, &Ctrl::OnTabPrev, this, ID_TAB_PREV);

  std::vector<wxAcceleratorEntry> entries;
  entries.push_back(wxAcceleratorEntry(wxACCEL_RAW_CTRL, WXK_TAB, ID_TAB_NEXT));
  entries.push_back(wxAcceleratorEntry(wxACCEL_RAW_CTRL | wxACCEL_SHIFT,
                                       WXK_TAB, ID_TAB_PREV));
  wxAcceleratorTable acc(entries.size(), &entries[0]);
  wxControl::SetAcceleratorTable(acc);
}

bool Ctrl::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                  const wxSize& size,
                  long style) {  // NOLINT
  wxControl::Create(parent, id, pos, size, style);

  CalculateLayout();
  SetInitialSize(overall_size_);
  SetMinClientSize(overall_size_);

  return true;
}

void Ctrl::set_items(const ItemList& items) { items_ = items; }

const ItemList& Ctrl::items() const { return items_; }

ItemList& Ctrl::items() { return items_; }

void Ctrl::SelectOrFirst(Index index, int group, bool forward) {
  selection_ = index;
  if (false == IsValid(items_, selection_)) {
    selection_ = GoToFirstItem(items_, group);
  } else {
    AdvanceToNextSelection(forward);
  }
}

Index Ctrl::selection() const { return selection_; }

void Ctrl::OnPaint(wxPaintEvent& WXUNUSED(event)) {  // NOLINT
#if wxSWITCHER_USE_BUFFERED_PAINTING
  wxBufferedPaintDC dc(this);
#else
  wxPaintDC dc(this);
#endif

  if (items_.column_count() == 0) CalculateLayout(dc);

  if (items_.column_count() == 0) return;
  // wxRect rect = GetClientRect();
  items_.PaintItems(&dc, style_, selection_, this);
}

void Ctrl::OnMouseClick(wxMouseEvent& event) {
  SetFocus();

  const wxPoint pos = event.GetPosition();
  Index idx = items_.HitTest(pos);
  if (idx != SWITCHER_NOT_FOUND) {
    selection_ = idx;

    SendCloseEvent();
  }
}

void Ctrl::OnMouseMove(wxMouseEvent& event) {
  const wxPoint pos = event.GetPosition();
  Index idx = items_.HitTest(pos);
  if (idx != SWITCHER_NOT_FOUND) {
  }

  SetCursor(idx == SWITCHER_NOT_FOUND ? wxNullCursor : wxCursor(wxCURSOR_HAND));
}

void Ctrl::OnChar(wxKeyEvent& WXUNUSED(event)) {}  // NOLINT

void Ctrl::OnTab(bool forward) {
  if (forward) {
    selection_ = GoToNextItem(items_, selection_, true);
  } else {
    selection_ = GoToPreviousItem(items_, selection_, true);
  }

  GenerateSelectionEvent();
  Refresh();
}

void Ctrl::OnKey(wxKeyEvent& event) {
  const int key_code = event.GetKeyCode();
  if (event.GetEventType() == wxEVT_KEY_UP) {
    if (key_code == MODIFIER_KEY) {
      SendCloseEvent();
    }
    event.Skip();
    return;
  }

  if (key_code == WXK_ESCAPE || key_code == WXK_RETURN) {
    if (key_code == WXK_ESCAPE) selection_ = SWITCHER_NOT_FOUND;

    SendCloseEvent();
  } else if (
#ifndef DONT_USE_ARROWKEYS
      key_code == WXK_DOWN ||
#endif
      key_code == WXK_NUMPAD_DOWN || key_code == VIM_DOWN) {
    selection_ = GoToNextItem(items_, selection_, false);

    GenerateSelectionEvent();
    Refresh();
  } else if (
#ifndef DONT_USE_ARROWKEYS
      key_code == WXK_UP ||
#endif
      key_code == WXK_NUMPAD_UP || key_code == VIM_UP) {
    selection_ = GoToPreviousItem(items_, selection_, false);

    GenerateSelectionEvent();
    Refresh();
  } else if (key_code == WXK_HOME || key_code == WXK_NUMPAD_HOME) {
    selection_ = GoToFirstItem(items_);

    GenerateSelectionEvent();
    Refresh();
  } else if (key_code == WXK_END || key_code == WXK_NUMPAD_END) {
    selection_ = GoToLastItem(items_);

    GenerateSelectionEvent();
    Refresh();
  } else if (
#ifndef DONT_USE_ARROWKEYS
      key_code == WXK_LEFT ||
#endif
      key_code == WXK_NUMPAD_LEFT || key_code == VIM_LEFT) {
    selection_ = GoToLeftItem(items_, style_, selection_);

    GenerateSelectionEvent();
    Refresh();
  } else if (
#ifndef DONT_USE_ARROWKEYS
      key_code == WXK_RIGHT ||
#endif
      key_code == WXK_NUMPAD_RIGHT || key_code == VIM_RIGHT) {
    selection_ = GoToRightItem(items_, style_, selection_);

    GenerateSelectionEvent();
    Refresh();
  } else {
    event.Skip();
  }
}

void Ctrl::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {  // NOLINT
                                                               // Do nothing
}

wxSize Ctrl::DoGetBestClientSize() const { return overall_size_; }

void Ctrl::CalculateLayout() {
  wxClientDC dc(this);
  CalculateLayout(dc);
}

class LayoutCalculator {
 public:
  LayoutCalculator(wxSize is, const Style& style) : style_(style) {
    columnCount = 1;

    itemSize = is;
    overall_size_ = wxSize(style.min_width(), style.min_height());

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
  const Style& style_;
  int columnCount;

  wxSize itemSize;
  wxSize overall_size_;

  int currentRow;
  int x;
  int y;
};

void Ctrl::CalculateLayout(wxDC& dc) {  // NOLINT
  if (selection_ == SWITCHER_NOT_FOUND) selection_ = GoToFirstItem(items_);

  wxSize items_size = items_.CalculateItemSize(&dc, style_);
  items_size.x = std::min(items_size.x, style_.item_maxwidth());
  items_size.y = std::min(items_size.y, style_.item_maxheight());
  LayoutCalculator calc(items_size, style_);

  for (size_t group_index = 0; group_index < items_.GetGroupCount();
       ++group_index) {
    Group& group = items_.GetGroup(group_index);

    group.set_rect(calc.rect());
    calc.UpdateOverallSize();
    calc.GoToNextRow();

    for (size_t item_index = 0; item_index < group.GetItemCount();
         ++item_index) {
      if (calc.get_currentRow() >= style_.row_count()) {
        calc.GoToNextCol();
        calc.GoToNextRow();  // only groups are on first row
      }

      Item& item = group.GetItem(item_index);
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

void Ctrl::InvalidateLayout() {
  items_.set_column_count(0);
  Refresh();
}

void Ctrl::GenerateSelectionEvent() {
  wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, GetId());
  event.SetEventObject(this);
  event.SetInt(selection_.first);

  GetEventHandler()->ProcessEvent(event);
}

void Ctrl::SendCloseEvent() {
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

void Ctrl::AdvanceToNextSelection(bool forward) {
  if (forward == false) {
    selection_ = GoToPreviousItem(items_, selection_, true);
  } else {
    selection_ = GoToNextItem(items_, selection_, true);
  }
}

}  // namespace switcher
