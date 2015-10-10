#ifndef RIDE_SWITCHERDLG_H_
#define RIDE_SWITCHERDLG_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>

#include "item.h"
#include "itemlist.h"
#include "ctrl.h"

namespace switcher {

class SwitcherDlg : public wxDialog {
 public:
  SwitcherDlg(const SwitcherItemList& items, SwitcherIndex index,
              const SwitcherStyle& sstyle, wxWindow* parent, wxWindowID id = -1,
              const wxString& title = _("Pane Switcher"),
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style  // NOLINT
              = wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxBORDER_SIMPLE);

  void OnCloseWindow(wxCloseEvent& event);
  void OnActivate(wxActivateEvent& event);
  void OnSelectItem(wxCommandEvent& event);

  // Get the selected item
  SwitcherIndex GetSelection() const;

  void ShowDescription(const SwitcherItem& item);

  void set_border_color(const wxColour& colour);

  void AdvanceToNextSelection(bool forward);

 private:
  void UpdateDescription();

 private:
  SwitcherDlg();

 private:
  wxStaticText* title_ctrl_;
  wxStaticText* description_ctrl_;
  SwitcherCtrl* list_ctrl_;
  wxStaticText* path_ctrl_;
  bool is_closing_;
  long switcher_border_style_;  // NOLINT
  wxColour border_color_;
  SwitcherStyle style_;
};

}  // switcher

#endif  // RIDE_SWITCHERDLG_H_
