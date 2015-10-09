#ifndef RIDE_SWITCHERCTRL_H_
#define RIDE_SWITCHERCTRL_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>

#include "ride/switcher/item.h"
#include "ride/switcher/itemlist.h"
#include "ride/switcher/style.h"

class SwitcherCtrl : public wxControl {
 public:
  explicit SwitcherCtrl(const SwitcherItemList& items,
                        const SwitcherStyle& style);

  bool Create(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = 0);  // NOLINT

  // Public API
  void set_items(const SwitcherItemList& items);
  const SwitcherItemList& items() const;
  SwitcherItemList& items();

  void SelectOrFirst(SwitcherIndex index);

  SwitcherIndex selection() const;

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
  SwitcherItemList items_;
  SwitcherIndex selection_;
  SwitcherStyle style_;
  wxSize overall_size_;
};

#endif  // RIDE_SWITCHERCTRL_H_
