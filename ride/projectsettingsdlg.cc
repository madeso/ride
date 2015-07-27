#include "ride/projectsettingsdlg.h"

#include "ride/generated/ui.h"
#include "ride/project.h"
#include "ride/mainwindow.h"
#include <wx/fontenum.h>
#include "ride/wxutils.h"
#include "ride/enabledisable.h"
#include "ride/form.h"
#include "ride/cargo.h"
#include "wx/editlbox.h"
#include "ride/resources/commonimages.h"

class ProjectSettingsDlg : public ui::ProjectSettings {
 public:
  ProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow,
                     Project* project);

 protected:
  void OnApply(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);

 protected:
  bool Apply();
  void AllToGui(bool togui);
  void CargoToGui(bool togui);

 protected:
  Cargo cargo_;

 private:
  MainWindow* main_window_;
  Project* project_;
};

void DoProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow,
                          Project* project) {
  ProjectSettingsDlg dlg(parent, mainwindow, project);
  dlg.ShowModal();
}

bool LoadCargoFile(const wxString& cargo_file, Cargo* cargo,
                   wxStaticText* error_display) {
  if (cargo_file.IsEmpty()) {
    error_display->SetLabelText("No project loaded");
    return false;
  }
  const auto result = cargo->Load(cargo_file);
  if (result.IsOk()) {
    error_display->SetLabelText("");
    return true;
  }

  error_display->SetLabelText(result.message());
  return false;
}

ProjectSettingsDlg::ProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow,
                                       Project* project)
    : ::ui::ProjectSettings(parent, wxID_ANY),
      main_window_(mainwindow),
      project_(project) {
  LoadCargoFile(project_->GetCargoFile(), &cargo_, uiCargoLoadError);

  AllToGui(true);

  // disable all cargo related gui as we can't save toml files yet
  EnableDisable(false) << uiCargoName << uiCargoVersion
      // we can't select or scroll in disabled listboxes
      // so lets keep them enabled since we can't really do anything in them
      // << uiCargoAuthors
      // << uiCargoDependencies
      ;

  SetImageAndRemoveText(uiBuildConfigurationTargetHelp, wxART_HELP);
  SetImageAndRemoveText(uiBuildConfigurationCustomArgsHelp, wxART_TIP);
  SetImageAndRemoveText(uiBuildFeatureAdd, wxART_NEW);
  SetImageAndRemoveText(uiBuildFeatureRemove, wxART_DELETE);
  SetImageAndRemoveText(uiBuildFeatureEdit, image::edit_xpm);
  SetImageAndRemoveText(uiBuildFeatureUp, wxART_GO_UP);
  SetImageAndRemoveText(uiBuildFeatureDown, wxART_GO_DOWN);
}

void ProjectSettingsDlg::OnApply(wxCommandEvent& event) { Apply(); }

void ProjectSettingsDlg::OnCancel(wxCommandEvent& event) { EndModal(wxCANCEL); }

void ProjectSettingsDlg::OnOk(wxCommandEvent& event) {
  if (Apply()) {
    EndModal(wxOK);
  }
}

void ProjectSettingsDlg::AllToGui(bool togui) { CargoToGui(togui); }

//////////////////////////////////////////////////////////////////////////

bool ProjectSettingsDlg::Apply() {
  AllToGui(false);

  return true;
}

//////////////////////////////////////////////////////////////////////////

void ProjectSettingsDlg::CargoToGui(bool togui) {
  DIALOG_DATA(cargo_, name, uiCargoName, _Str);
  DIALOG_DATA(cargo_, version, uiCargoVersion, _Str);
  DIALOG_DATA(cargo_, authors, uiCargoAuthors, _Content);
  DIALOG_DATA(cargo_, dependencies, uiCargoDependencies, _Content);
  DIALOG_DATA(cargo_, features, uiCargoFeatures, _Content);
}