#ifndef RIDE_CREATENEWPROJECTDLG_H
#define RIDE_CREATENEWPROJECTDLG_H

#include "ride/generated/ui.h"

class CreateNewProjectDlg : public ui::CreateNewProject
{
public:
  CreateNewProjectDlg(wxWindow* parent);

  const wxString project_folder() const;
  const wxString project_name() const;

  wxString GenerateCargoCommandline() const;

protected:
  virtual void OnProjectNameEnter(wxCommandEvent& event);

  virtual void OnProjectNameChanged(wxCommandEvent& event);
  virtual void OnProjectFolderChanged(wxCommandEvent& event);
  virtual void OnBrowseProjectFolder(wxCommandEvent& event);
  virtual void OnCancel(wxCommandEvent& event);
  virtual void OnOk(wxCommandEvent& event);
private:
  wxString GetTarget() const;
  wxString GetVcsName() const;
  void UpdateTarget();
};

#endif  // RIDE_CREATENEWPROJECTDLG_H
