#ifndef RIDE_CREATENEWFILEDLG_H
#define RIDE_CREATENEWFILEDLG_H

#include "ride/generated/ui.h"

class CreateNewFileDlg : public ui::CreateNewFile
{
public:
  CreateNewFileDlg(wxWindow* parent, const wxString& project_folder, const wxString& fodler_hint);

  const wxString GetFilePath() const;
  const wxString GetTemplateSource() const;

protected:
  void OnTextChanged(wxCommandEvent& event);
  void OnComboChanged(wxCommandEvent& event);
  void OnCheckChanged(wxCommandEvent& event);

  void OnNameEnter(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);

private:
  void UpdateTemplateSource();
  wxString project_folder_;
};

#endif  // RIDE_CREATENEWFILEDLG_H
