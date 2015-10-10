#ifndef RIDE_SWITCHERCTRL_H_
#define RIDE_SWITCHERCTRL_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>

#include "item.h"
#include "itemlist.h"
#include "style.h"

namespace switcher {

class Ctrl : public wxControl {
 public:
  explicit Ctrl(const ItemList& items, const Style& style);

  bool Create(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = 0);  // NOLINT

  // Public API
  void set_items(const ItemList& items);
  const ItemList& items() const;
  ItemList& items();

  void SelectOrFirst(Index index, int group, bool forward);

  Index selection() const;

  // Event handlers

  void OnPaint(wxPaintEvent& event);
  void OnMouseEvent(wxMouseEvent& event);
  void OnChar(wxKeyEvent& event);
  void OnKey(wxKeyEvent& event);
  void OnEraseBackground(wxEraseEvent& event);

  // Overrides
  virtual wxSize DoGetBestClientSize() const;

  // Implementation

  void CalculateLayout();
  void CalculateLayout(wxDC& dc);  // NOLINT
  void InvalidateLayout();
  void GenerateSelectionEvent();
  void SendCloseEvent();
  void AdvanceToNextSelection(bool forward);

 protected:
  ItemList items_;
  Index selection_;
  Style style_;
  wxSize overall_size_;
};

}  // switcher

#endif  // RIDE_SWITCHERCTRL_H_
