#ifndef RIDE_SWITCHERDLG_H_
#define RIDE_SWITCHERDLG_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>

#include "item.h"
#include "itemlist.h"
#include "ctrl.h"

namespace switcher {

class Dialog : public wxDialog {
 public:
  Dialog(const ItemList& items, Index index, const Style& sstyle,
         wxWindow* parent, wxWindowID id = -1,
         const wxString& title = _("Pane Switcher"),
         const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize,
         long style  // NOLINT
         = wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxBORDER_SIMPLE);

  void OnCloseWindow(wxCloseEvent& event);
  void OnActivate(wxActivateEvent& event);
  void OnSelectItem(wxCommandEvent& event);

  // Get the selected item
  Index GetSelection() const;

  void ShowDescription(const Item& item);

  void set_border_color(const wxColour& colour);

  void AdvanceToNextSelection(bool forward);

 private:
  void UpdateDescription();

 private:
  Dialog();

 private:
  wxStaticText* title_ctrl_;
  wxStaticText* description_ctrl_;
  Ctrl* list_ctrl_;
  wxStaticText* path_ctrl_;
  bool is_closing_;
  long switcher_border_style_;  // NOLINT
  wxColour border_color_;
  Style style_;
};

}  // switcher

#endif  // RIDE_SWITCHERDLG_H_
