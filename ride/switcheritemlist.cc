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

#define wxSWITCHER_TEXT_MARGIN_X 4
#define wxSWITCHER_TEXT_MARGIN_Y 2

/*!
 * wxSwitcherItems
 * An object containing switcher items
 */

IMPLEMENT_CLASS(SwitcherItemList, wxObject)

SwitcherItemList::SwitcherItemList() { Init(); }

SwitcherItemList::SwitcherItemList(const SwitcherItemList& items) {
  Init();
  Copy(items);
}

bool SwitcherItemList::operator==(const SwitcherItemList& items) const {
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

void SwitcherItemList::operator=(const SwitcherItemList& items) { Copy(items); }

void SwitcherItemList::Init() {
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

void SwitcherItemList::Copy(const SwitcherItemList& items) {
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

SwitcherItem& SwitcherItemList::AddItem(const wxString& title,
                                        const wxString& name, int id,
                                        const wxBitmap& bitmap) {
  SwitcherItem item;
  item.set_title(title);
  item.set_name(name);
  item.set_id(id);
  item.set_bitmap(bitmap);

  return AddItem(item);
}

SwitcherItem& SwitcherItemList::AddItem(const SwitcherItem& item) {
  items_.push_back(item);
  return items_[GetItemCount() - 1];
}

SwitcherItem& SwitcherItemList::AddGroup(const wxString& title,
                                         const wxString& name, int id,
                                         const wxBitmap& bitmap) {
  SwitcherItem& item = AddItem(title, name, id, bitmap);
  item.set_is_group(true);

  return item;
}

void SwitcherItemList::Clear() { items_.resize(0); }

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

  return wxNOT_FOUND;
}

int SwitcherItemList::HitTest(const wxPoint& pt) const {
  for (size_t i = 0; i < items_.size(); i++) {
    const SwitcherItem& item = items_[i];
    if (item.rect().Contains(pt)) return static_cast<int>(i);
  }

  return wxNOT_FOUND;
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

  if (background_color().Ok()) backgroundColour = background_color();

  if (text_color().Ok()) standardTextColour = text_color();

  if (selection_color().Ok()) selectionColour = selection_color();

  if (selection_outline_color().Ok())
    selectionOutlineColour = selection_outline_color();

  if (item_font().Ok()) {
    standardFont = item_font();
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
    SwitcherItem& item = items_[i];
    bool selected = (static_cast<int>(i) == selection_);

    if (selected) {
      dc.SetPen(wxPen(selectionOutlineColour));
      dc.SetBrush(wxBrush(selectionColour));
      dc.DrawRectangle(item.rect());
    }

    wxRect clippingRect(item.rect());
    clippingRect.Deflate(1, 1);

    dc.SetClippingRegion(clippingRect);

    if (item.text_color().Ok())
      dc.SetTextForeground(item.text_color());
    else
      dc.SetTextForeground(standardTextColour);

    if (item.get_font().Ok()) {
      dc.SetFont(item.get_font());
    } else {
      if (item.is_group())
        dc.SetFont(groupFont);
      else
        dc.SetFont(standardFont);
    }

    int w, h;
    dc.GetTextExtent(item.title(), &w, &h);

    int x = item.rect().x;

    x += textMarginX;

    if (!item.is_group()) {
      if (item.bitmap().Ok() && item.bitmap().GetWidth() <= 16 &&
          item.bitmap().GetHeight() <= 16) {
        dc.DrawBitmap(item.bitmap(), x,
                      item.rect().y +
                          (item.rect().height - item.bitmap().GetHeight()) / 2,
                      true);
      }

      x += 16;

      x += textMarginX;
    }

    int y = item.rect().y + (item.rect().height - h) / 2;
    dc.DrawText(item.title(), x, y);

    dc.DestroyClippingRegion();
  }
}

wxSize SwitcherItemList::CalculateItemSize(wxDC& dc) {  // NOLINT
  // Start off allowing for an icon
  wxSize sz(150, 16);
  wxFont standardFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
  wxFont groupFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
  groupFont.SetWeight(wxBOLD);

  int textMarginX = wxSWITCHER_TEXT_MARGIN_X;
  int textMarginY = wxSWITCHER_TEXT_MARGIN_Y;
  int maxWidth = 300;
  int maxHeight = 40;

  if (item_font().Ok()) standardFont = item_font();

  size_t i;
  for (i = 0; i < items_.size(); i++) {
    SwitcherItem& item = items_[i];

    if (item.get_font().Ok()) {
      dc.SetFont(item.get_font());
    } else {
      if (item.is_group())
        dc.SetFont(groupFont);
      else
        dc.SetFont(standardFont);
    }

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
