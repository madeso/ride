#include "ride/projectsettingsdlg.h"

#include "ride/generated/ui.h"
#include "ride/project.h"
#include "ride/mainwindow.h"
#include <wx/fontenum.h>
#include "ride/wxutils.h"
#include "ride/form.h"
#include "ride/cargo.h"

class ProjectSettingsDlg : public ui::ProjectSettings
{
public:
  ProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow, Project* project);

protected:
  void OnApply(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);

protected:
  void AllToGui(bool togui);
  void CargoToGui(bool togui);

protected:
  Cargo cargo_;

private:
  MainWindow* main_window_;
  Project* project_;
};

void DoProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow, Project* project)
{
  ProjectSettingsDlg dlg(parent, mainwindow, project);
  dlg.ShowModal();
}

ProjectSettingsDlg::ProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow, Project* project) :
::ui::ProjectSettings(parent, wxID_ANY), main_window_(mainwindow), project_(project)
{
  const wxString cargo_file = project_->GetCargoFile();
  if (false == cargo_file.IsEmpty()) {
    cargo_.Load( cargo_file );
  }
  AllToGui(true);
}

void ProjectSettingsDlg::OnApply( wxCommandEvent& event )
{
  AllToGui(false);
}

void ProjectSettingsDlg::OnCancel( wxCommandEvent& event )
{
  EndModal(wxCANCEL);
}

void ProjectSettingsDlg::OnOk( wxCommandEvent& event )
{
  AllToGui(false);
  EndModal(wxOK);
}

void ProjectSettingsDlg::AllToGui(bool togui) {
  CargoToGui(togui);
}

//////////////////////////////////////////////////////////////////////////


void ProjectSettingsDlg::CargoToGui(bool togui) {
  DIALOG_DATA(cargo_, name, uiCargoName, _Str);
}