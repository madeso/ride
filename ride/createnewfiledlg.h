#ifndef RIDE_CREATENEWFILEDLG_H
#define RIDE_CREATENEWFILEDLG_H

#include "ride/generated/ui.h"

class CreateNewFileDlg : public ui::CreateNewFile
{
public:
  CreateNewFileDlg(wxWindow* parent, const wxString& fodler_hint);

protected:
  void OnNameEnter(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
};

#endif  // RIDE_CREATENEWFILEDLG_H
