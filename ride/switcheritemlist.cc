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

#include "ride/switcheritemlist.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

SwitcherItemList::SwitcherItemList()
    : selection_(-1),
      row_count_(10),
      column_count_(0),
      text_margin_x_(4),
      text_margin_y_(2),
      background_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)),
      text_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)),
      selection_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT)),
      selection_outline_color_(
          wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)),
      item_font_(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT)) {}

SwitcherItem& SwitcherItemList::AddItem(const wxString& title,
                                        const wxString& name, int id,
                                        const wxBitmap& bitmap) {
  SwitcherItem item(title, name, id, bitmap);
  return AddItem(item);
}

SwitcherItem& SwitcherItemList::AddItem(const SwitcherItem& item) {
  items_.push_back(item);
  return *items_.rbegin();
}

SwitcherItem& SwitcherItemList::AddGroup(const wxString& title,
                                         const wxString& name, int id,
                                         const wxBitmap& bitmap) {
  SwitcherItem& item = AddItem(title, name, id, bitmap);
  item.set_is_group(true);

  return item;
}

int SwitcherItemList::FindItemByName(const wxString& name) const {
  size_t i;
  for (i = 0; i < items_.size(); i++) {
    if (items_[i].name() == name) return i;
  }

  return -1;
}

int SwitcherItemList::FindItemById(int id) const {
  size_t i;
  for (i = 0; i < items_.size(); i++) {
    if (items_[i].id() == id) return i;
  }

  return -1;
}

void SwitcherItemList::set_selection(int sel) { selection_ = sel; }

void SwitcherItemList::SelectByName(const wxString& name) {
  int idx = FindItemByName(name);
  if (idx != -1) set_selection(idx);
}

int SwitcherItemList::selection() const { return selection_; }

int SwitcherItemList::GetIndexForFocus() const {
  for (size_t i = 0; i < items_.size(); i++) {
    const SwitcherItem& item = items_[i];
    if (item.window()) {
      if (wxFindFocusDescendant(item.window())) return i;
    }
  }

  return -1;
}

int SwitcherItemList::HitTest(const wxPoint& pt) const {
  for (size_t i = 0; i < items_.size(); i++) {
    const SwitcherItem& item = items_[i];
    if (item.rect().Contains(pt)) return static_cast<int>(i);
  }

  return -1;
}

const SwitcherItem& SwitcherItemList::GetItem(int i) const { return items_[i]; }

SwitcherItem& SwitcherItemList::GetItem(int i) { return items_[i]; }

int SwitcherItemList::GetItemCount() const { return items_.size(); }

void SwitcherItemList::set_row_count(int rows) { row_count_ = rows; }
int SwitcherItemList::row_count() const { return row_count_; }

void SwitcherItemList::set_column_count(int cols) { column_count_ = cols; }
int SwitcherItemList::column_count() const { return column_count_; }

void SwitcherItemList::set_background_color(const wxColour& colour) {
  background_color_ = colour;
}
const wxColour& SwitcherItemList::background_color() const {
  return background_color_;
}

void SwitcherItemList::set_text_color(const wxColour& colour) {
  text_color_ = colour;
}
const wxColour& SwitcherItemList::text_color() const { return text_color_; }

void SwitcherItemList::set_selection_color(const wxColour& colour) {
  selection_color_ = colour;
}
const wxColour& SwitcherItemList::selection_color() const {
  return selection_color_;
}

void SwitcherItemList::set_selection_outline_color(const wxColour& colour) {
  selection_outline_color_ = colour;
}
const wxColour& SwitcherItemList::selection_outline_color() const {
  return selection_outline_color_;
}

void SwitcherItemList::set_item_font(const wxFont& font) { item_font_ = font; }
const wxFont& SwitcherItemList::item_font() const { return item_font_; }

void SwitcherItemList::PaintItems(wxDC& dc, wxWindow* win) {  // NOLINT
  const wxFont groupFont(item_font_.GetPointSize(), item_font_.GetFamily(),
                         item_font_.GetStyle(), wxBOLD,
                         item_font_.GetUnderlined(), item_font_.GetFaceName());

  dc.SetLogicalFunction(wxCOPY);
  dc.SetBrush(wxBrush(background_color_));
  dc.SetPen(*wxTRANSPARENT_PEN);
  dc.DrawRectangle(win->GetClientRect());
  dc.SetBackgroundMode(wxTRANSPARENT);

  for (size_t i = 0; i < items_.size(); i++) {
    SwitcherItem& item = items_[i];
    bool selected = (static_cast<int>(i) == selection_);

    if (selected) {
      dc.SetPen(wxPen(selection_outline_color_));
      dc.SetBrush(wxBrush(selection_color_));
      dc.DrawRectangle(item.rect());
    }

    wxRect clippingRect(item.rect());
    clippingRect.Deflate(1, 1);

    dc.SetClippingRegion(clippingRect);

    dc.SetTextForeground(text_color_);
    dc.SetFont(item.is_group() ? groupFont : item_font_);

    int w, h;
    dc.GetTextExtent(item.title(), &w, &h);

    int x = item.rect().x;
    x += text_margin_x_;

    if (!item.is_group()) {
      if (item.bitmap().Ok() && item.bitmap().GetWidth() <= 16 &&
          item.bitmap().GetHeight() <= 16) {
        dc.DrawBitmap(item.bitmap(), x,
                      item.rect().y +
                          (item.rect().height - item.bitmap().GetHeight()) / 2,
                      true);
      }

      x += 16;

      x += text_margin_x_;
    }

    int y = item.rect().y + (item.rect().height - h) / 2;
    dc.DrawText(item.title(), x, y);

    dc.DestroyClippingRegion();
  }
}

wxSize SwitcherItemList::CalculateItemSize(wxDC& dc) {  // NOLINT
  // Start off allowing for an icon
  wxSize sz(150, 16);
  const wxFont groupFont(item_font_.GetPointSize(), item_font_.GetFamily(),
                         item_font_.GetStyle(), wxBOLD,
                         item_font_.GetUnderlined(), item_font_.GetFaceName());

  int textMarginX = text_margin_x_;
  int textMarginY = text_margin_y_;
  const int maxWidth = 300;
  const int maxHeight = 40;

  size_t i;
  for (i = 0; i < items_.size(); i++) {
    SwitcherItem& item = items_[i];

    dc.SetFont(item.is_group() ? groupFont : item_font_);

    int w, h;
    dc.GetTextExtent(item.title(), &w, &h);

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
