// Copyright (2015) Gustav

#include "ride/projectsettingsdlg.h"

#include <wx/editlbox.h>
#include <wx/fontenum.h>

#include "ride/generated/ui.h"

#include "ride/resources/commonimages.h"

#include "ride/cargo.h"
#include "ride/enabledisable.h"
#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/project.h"
#include "ride/wxutils.h"

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
  void EditorToGui(bool togui);

  void OnlyAllowNumberChars(wxKeyEvent& event);
  void OnTabWdithChanged(wxCommandEvent& event);
  void OnEditorUseTabsClicked(wxCommandEvent& event);

 protected:
  Cargo cargo_;

 private:
  MainWindow* main_window_;
  Project* project_;
  ride::Project project_backup_;
  bool allow_editor_to_gui_;
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
      project_(project),
      project_backup_(project->project()),
      allow_editor_to_gui_(true) {
  LoadCargoFile(project_->GetCargoFile(), &cargo_, uiCargoLoadError);

  AllToGui(true);

  // disable all cargo related gui as we can't save toml files yet
  EnableDisable(false) << uiCargoName << uiCargoVersion;
  // we can't select or scroll in disabled listboxes
  // so lets keep them enabled since we can't really do anything in them
  // << uiCargoAuthors
  // << uiCargoDependencies

  SetImageAndRemoveText(uiBuildConfigurationTargetHelp, wxART_HELP);
  SetImageAndRemoveText(uiBuildConfigurationCustomArgsHelp, wxART_TIP);
  SetImageAndRemoveText(uiBuildFeatureAdd, wxART_NEW);
  SetImageAndRemoveText(uiBuildFeatureRemove, wxART_DELETE);
  SetImageAndRemoveText(uiBuildFeatureEdit, image::edit_xpm);
  SetImageAndRemoveText(uiBuildFeatureUp, wxART_GO_UP);
  SetImageAndRemoveText(uiBuildFeatureDown, wxART_GO_DOWN);
}

void ProjectSettingsDlg::OnApply(wxCommandEvent& event) { Apply(); }

void ProjectSettingsDlg::OnCancel(wxCommandEvent& event) {
  project_->set_project(project_backup_);
  EditorToGui(true);
  EndModal(wxCANCEL);
}

void ProjectSettingsDlg::OnOk(wxCommandEvent& event) {
  if (Apply()) {
    EndModal(wxOK);
  }
}

void ProjectSettingsDlg::AllToGui(bool togui) {
  CargoToGui(togui);
  EditorToGui(togui);
}

//////////////////////////////////////////////////////////////////////////

bool ProjectSettingsDlg::Apply() {
  AllToGui(false);
  project_backup_ = project_->project();

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

//////////////////////////////////////////////////////////////////////////

void ProjectSettingsDlg::EditorToGui(bool togui) {
  if (allow_editor_to_gui_ == false) return;
  allow_editor_to_gui_ = false;
  ride::Project& project = *project_->project_ptr();
  DIALOG_DATA(project, tabwidth, uiEditorTabWidth, _I32);
  DIALOG_DATA(project, usetabs, uiEditorUseTabs, );

  if (togui == false) {
    main_window_->ProjectSettingsHasChanged();
    main_window_->ProjectSettingsHasChanged();
  }
  allow_editor_to_gui_ = true;
}

void ProjectSettingsDlg::OnlyAllowNumberChars(wxKeyEvent& event) {
  ::OnlyAllowNumberChars(event);
}
void ProjectSettingsDlg::OnTabWdithChanged(wxCommandEvent& event) {
  EditorToGui(false);
}
void ProjectSettingsDlg::OnEditorUseTabsClicked(wxCommandEvent& event) {
  EditorToGui(false);
}
