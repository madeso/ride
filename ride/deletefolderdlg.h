#ifndef RIDE_DELETEFOLDERDLG_H
#define RIDE_DELETEFOLDERDLG_H

#include "ride/wx.h"
#include "ride/generated/ui.h"

class DeleteFolderDlg : public ui::DeleteFolder {
public:
  DeleteFolderDlg(wxWindow* parent, const wxString& message, const wxString& title);

  bool full() const;
  bool recursive() const;

 void set_full(bool new_full) const;
 void set_recursive(bool new_recursive) const;

protected:
  void OnNo(wxCommandEvent& event);
  void OnYes(wxCommandEvent& event);
};

#endif  // RIDE_DELETEFOLDERDLG_H
