#include "group.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

#include "style.h"

namespace switcher {

SwitcherGroup::SwitcherGroup(const wxString& title) : title_(title) {}

SwitcherGroup::SwitcherGroup(const wxString& title,
                             const std::vector<SwitcherItem>& items)
    : title_(title), items_(items) {}

void SwitcherGroup::set_title(const wxString& title) { title_ = title; }

const wxString& SwitcherGroup::title() const { return title_; }

SwitcherGroup& SwitcherGroup::set_rect(const wxRect& rect) {
  rect_ = rect;
  return (*this);
}

const wxRect& SwitcherGroup::rect() const { return rect_; }

SwitcherItem& SwitcherGroup::AddItem(const SwitcherItem& item) {
  items_.push_back(item);
  return *items_.rbegin();
}

int SwitcherGroup::GetIndexForFocus() const {
  for (size_t i = 0; i < items_.size(); i++) {
    const SwitcherItem& item = items_[i];
    if (item.window()) {
      if (wxFindFocusDescendant(item.window())) return i;
    }
  }

  return -1;
}

int SwitcherGroup::HitTest(const wxPoint& pt) const {
  for (size_t i = 0; i < items_.size(); i++) {
    const SwitcherItem& item = items_[i];
    if (item.rect().Contains(pt)) return static_cast<int>(i);
  }

  return -1;
}

const SwitcherItem& SwitcherGroup::GetItem(int i) const { return items_[i]; }

SwitcherItem& SwitcherGroup::GetItem(int i) {
  assert(i >= 0);
  assert(i < items_.size());
  return items_[i];
}

int SwitcherGroup::GetItemCount() const { return items_.size(); }

void PaintItem(const wxRect& rect, const wxString& title,
               const wxBitmap& bitmap, wxDC* dc, const SwitcherStyle& style,
               const wxFont& font, bool draw_image) {
  wxRect clippingRect(rect);
  clippingRect.Deflate(1, 1);

  dc->SetClippingRegion(clippingRect);

  dc->SetTextForeground(style.text_color());
  dc->SetFont(font);

  int w, h;
  dc->GetTextExtent(title, &w, &h);

  int x = rect.x;
  x += style.text_margin_x();

  if (draw_image) {
    if (bitmap.Ok() && bitmap.GetWidth() <= 16 && bitmap.GetHeight() <= 16) {
      dc->DrawBitmap(bitmap, x, rect.y + (rect.height - bitmap.GetHeight()) / 2,
                     true);
    }

    x += 16;

    x += style.text_margin_x();
  }

  int y = rect.y + (rect.height - h) / 2;
  dc->DrawText(title, x, y);

  dc->DestroyClippingRegion();
}

void SwitcherGroup::PaintItems(wxDC* dc, const SwitcherStyle& style,
                               int selection) {
  const wxFont groupFont(
      style.item_font().GetPointSize(), style.item_font().GetFamily(),
      style.item_font().GetStyle(), wxBOLD, style.item_font().GetUnderlined(),
      style.item_font().GetFaceName());

  PaintItem(rect_, title_, wxNullBitmap, dc, style, groupFont, false);

  for (size_t i = 0; i < items_.size(); i++) {
    SwitcherItem& item = items_[i];
    const bool selected = (static_cast<int>(i) == selection);

    if (selected) {
      dc->SetPen(wxPen(style.selection_outline_color()));
      dc->SetBrush(wxBrush(style.selection_color()));
      dc->DrawRectangle(item.rect());
    }

    PaintItem(item.rect(), item.title(), item.bitmap(), dc, style,
              style.item_font(), true);
  }
}

void ExtendSize(wxDC* dc, wxFont font, wxString title,
                const SwitcherStyle& style, wxSize* sz) {
  dc->SetFont(font);

  int w, h;
  dc->GetTextExtent(title, &w, &h);

  // TODO(Gustav): should we care if this adds 16px to the group too?
  w += 16 + 2 * style.text_margin_x();

  sz->x = wxMax(w, sz->x);
  sz->y = wxMax(h, sz->y);
}

void SwitcherGroup::CalculateItemSize(wxDC* dc, const SwitcherStyle& style,
                                      wxSize* sz) {  // NOLINT
  const wxFont groupFont(
      style.item_font().GetPointSize(), style.item_font().GetFamily(),
      style.item_font().GetStyle(), wxBOLD, style.item_font().GetUnderlined(),
      style.item_font().GetFaceName());

  ExtendSize(dc, groupFont, title_, style, sz);

  size_t i;
  for (i = 0; i < items_.size(); i++) {
    SwitcherItem& item = items_[i];

    ExtendSize(dc, style.item_font(), item.title(), style, sz);
  }
}

}  // switcher
