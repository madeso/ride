#ifndef RIDE_CREATENEWPROJECTDLG_H
#define RIDE_CREATENEWPROJECTDLG_H

#include "ride/wx.h"

class CreateNewProjectDlgHandler
{
public:
  CreateNewProjectDlgHandler(wxWindow* parent);

  bool ShowModal();

  const wxString project_folder() const;
  const wxString project_name() const;

  wxString GetTarget() const;
  wxString GenerateCargoCommandline() const;

private:
  wxWindow* parent_;

  wxString project_folder_;
  wxString project_name_;
  wxString Target;
  wxString CargoCommandline;
};

#endif  // RIDE_CREATENEWPROJECTDLG_H
