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

  wxString target() const;
  wxString cargo_command_line() const;

private:
  wxWindow* parent_;

  wxString project_folder_;
  wxString project_name_;
  wxString target_;
  wxString cargo_command_line_;
};

#endif  // RIDE_CREATENEWPROJECTDLG_H
