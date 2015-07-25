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

bool LoadCargoFile(const wxString& cargo_file, Cargo* cargo, wxStaticText* error_display) {
  if (cargo_file.IsEmpty()) {
    error_display->SetLabelText("No project loaded");
    return false;
  }
  const auto result = cargo->Load(cargo_file);
  if (result.IsOk())
  {
    error_display->SetLabelText("");
    return true;
  }

  error_display->SetLabelText(result.message());
  return false;
}

class EnableDisable
{
public:
  EnableDisable(bool enable) : enable_(enable) {}
  const EnableDisable& operator()(wxWindowBase* b) const {
    b->Enable(enable_);
    return *this;
  }
private:
  bool enable_;
};

ProjectSettingsDlg::ProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow, Project* project) :
::ui::ProjectSettings(parent, wxID_ANY), main_window_(mainwindow), project_(project)
{
  if (false == LoadCargoFile(project_->GetCargoFile(), &cargo_, uiCargoLoadError)) {
    EnableDisable(false)
      (uiCargoName)
      (uiCargoVersion)
      (uiCargoAuthors)
      ;
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