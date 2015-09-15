// Copyright (2015) Gustav

#include "ride/projectsettingsdlg.h"

#include <wx/editlbox.h>
#include <wx/fontenum.h>
#include <wx/textdlg.h>

#include <algorithm>
#include <string>

#include "ride/generated/ui.h"

#include "ride/resources/commonimages.h"

#include "ride/cargo.h"
#include "ride/enabledisable.h"
#include "ride/form.h"
#include "ride/guilist.h"
#include "ride/mainwindow.h"
#include "ride/project.h"
#include "ride/wxutils.h"
#include "ride/configurationsdlg.h"

struct FeatureFunctions {
  static const wxString ADD_TEXT;
  static const wxString EDIT_TEXT;

  static int Size(ride::BuildSetting* bs) { return bs->features_size(); }

  static wxString GetDisplayString(ride::BuildSetting* bs, int i) {
    return bs->features(i);
  }

  static void SetDisplayString(ride::BuildSetting* bs, int i,
                               const wxString& new_string) {
    bs->set_features(i, new_string);
  }

  static void Add(ride::BuildSetting* bs, const wxString& name) {
    std::string* new_feature = bs->add_features();
    *new_feature = name;
  }

  static void Remove(ride::BuildSetting* bs, int i) {
    bs->mutable_features()->DeleteSubrange(i, 1);
  }

  static void Swap(ride::BuildSetting* bs, int selection, int next_index) {
    std::swap(*bs->mutable_features(selection),
              *bs->mutable_features(next_index));
  }
};

const wxString FeatureFunctions::ADD_TEXT = "Name of the feature to add";
const wxString FeatureFunctions::EDIT_TEXT =
    "Please specify the new feature name";

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
  void BuildToGui(bool togui);
  void RunToGui(bool togui);
  ride::BuildSetting* GetSelectedBuildSetting();
  ride::RunSetting* GetSelectedRunSetting();

 protected:
  void OnlyAllowNumberChars(wxKeyEvent& event);
  void OnTabWdithChanged(wxCommandEvent& event);
  void OnEditorUseTabsClicked(wxCommandEvent& event);

  void OnBuildConfiguration(wxCommandEvent& event);
  void OnBuildConfigurationModify(wxCommandEvent& event);
  void OnBuildCheckbox(wxCommandEvent& event);
  void OnBuildText(wxCommandEvent& event);
  void OnBuildTargetHelp(wxCommandEvent& event);
  void OnBuildCustomArgHelp(wxCommandEvent& event);

  void OnBuildFeatureAdd(wxCommandEvent& event);
  void OnBuildFeatureEdit(wxCommandEvent& event);
  void OnBuildFeatureRemove(wxCommandEvent& event);
  void OnBuildFeatureUp(wxCommandEvent& event);
  void OnBuildFeatureDown(wxCommandEvent& event);

  void OnRunConfiguration(wxCommandEvent& event);
  void OnRunConfigurationModify(wxCommandEvent& event);
  void OnRunText(wxCommandEvent& event);
  void OnRunApplication(wxCommandEvent& event);
  void OnRunArguments(wxCommandEvent& event);
  void OnRunFolder(wxCommandEvent& event);
  void OnCmdBeforeLaunch(wxCommandEvent& event);
  void OnRunCheck(wxCommandEvent& event);

 protected:
  Cargo cargo_;

 private:
  MainWindow* main_window_;
  Project* project_;
  ride::Project project_backup_;
  ride::UserProject user_backup_;
  bool allow_editor_to_gui_;
  bool allow_build_to_gui_;
  bool allow_run_to_gui_;
  GuiList<ride::BuildSetting, FeatureFunctions> feature_list_;
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
      user_backup_(project->user()),
      allow_editor_to_gui_(true),
      allow_build_to_gui_(true),
      allow_run_to_gui_(true),
      feature_list_(uiBuildFeatures, this) {
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

  feature_list_.Setup(uiBuildFeatureAdd, uiBuildFeatureRemove,
                      uiBuildFeatureEdit, uiBuildFeatureUp, uiBuildFeatureDown);
}

void ProjectSettingsDlg::OnApply(wxCommandEvent& event) { Apply(); }

void ProjectSettingsDlg::OnCancel(wxCommandEvent& event) {
  project_->set_project(project_backup_);
  project_->set_user(user_backup_);
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
  BuildToGui(togui);
  RunToGui(togui);
}

//////////////////////////////////////////////////////////////////////////

bool ProjectSettingsDlg::Apply() {
  AllToGui(false);

  project_backup_ = project_->project();
  user_backup_ = project_->user();

  project_->SetMainStatusbarText();
  return project_->Save();
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

//////////////////////////////////////////////////////////////////////////

void ProjectSettingsDlg::OnBuildConfiguration(wxCommandEvent& event) {
  BuildToGui(true);
}

void ProjectSettingsDlg::OnBuildConfigurationModify(wxCommandEvent& event) {
  const bool need_update = DoConfigurationsDlg(this, main_window_, project_);
  if (need_update == false) return;
  uiBuildConfiguration->Clear();
  BuildToGui(true);
}

void ProjectSettingsDlg::OnBuildCheckbox(wxCommandEvent& event) {
  BuildToGui(false);
}

void ProjectSettingsDlg::OnBuildText(wxCommandEvent& event) {
  BuildToGui(false);
}

void ProjectSettingsDlg::OnBuildTargetHelp(wxCommandEvent& event) {
  ShowInfo(this, "this is a build target info", "info");
}

void ProjectSettingsDlg::OnBuildCustomArgHelp(wxCommandEvent& event) {
  ShowInfo(this, "this is a custom argument info", "info");
}

void ProjectSettingsDlg::BuildToGui(bool togui) {
  if (togui && static_cast<wxItemContainer*>(uiBuildConfiguration)->IsEmpty()) {
    for (int i = 0; i < project_->project().build_settings_size(); ++i) {
      ride::BuildSetting* bs =
          project_->project_ptr()->mutable_build_settings(i);
      uiBuildConfiguration->Append(bs->name().c_str(),
                                   reinterpret_cast<void*>(bs));
    }
    uiBuildConfiguration->Select(project_->user().build_setting());
  }

  ride::BuildSetting* setting_ptr = GetSelectedBuildSetting();
  EnableDisable(setting_ptr != NULL)
      << uiBuildConfigurationRelease << uiBuildConfigurationDefaultFeatures
      << uiBuildConfigurationVerbose << uiBuildConfigurationTarget
      << uiBuildConfigurationTargetHelp << uiBuildConfigurationCustomArgs
      << uiBuildConfigurationCustomArgsHelp << uiBuildFeatures
      << uiBuildFeatureAdd << uiBuildFeatureRemove << uiBuildFeatureUp
      << uiBuildFeatureDown << uiBuildFeatureEdit;

  if (setting_ptr == NULL) return;

  ride::BuildSetting& setting = *setting_ptr;

  DIALOG_DATA(setting, release, uiBuildConfigurationRelease, );
  DIALOG_DATA(setting, default_features, uiBuildConfigurationDefaultFeatures, );
  DIALOG_DATA(setting, verbose, uiBuildConfigurationVerbose, );

  DIALOG_DATA(setting, target, uiBuildConfigurationTarget, _Str);
  DIALOG_DATA(setting, custom_arguments, uiBuildConfigurationCustomArgs, _Str);

  feature_list_.ToGui(&setting, togui);

  if (togui == false) {
    uiBuildCommandLine->SetValue(BuildCommandLine(setting));
  }
}

ride::BuildSetting* ProjectSettingsDlg::GetSelectedBuildSetting() {
  const int selection = uiBuildConfiguration->GetSelection();
  const bool found = selection != wxNOT_FOUND;

  if (!found) return NULL;
  void* d = uiBuildConfiguration->GetClientData(selection);
  ride::BuildSetting* setting = reinterpret_cast<ride::BuildSetting*>(d);

  return setting;
}

void ProjectSettingsDlg::OnBuildFeatureAdd(wxCommandEvent& event) {
  if (false == feature_list_.Add(GetSelectedBuildSetting())) {
    return;
  }
  BuildToGui(true);
}

void ProjectSettingsDlg::OnBuildFeatureEdit(wxCommandEvent& event) {
  if (false == feature_list_.Edit(GetSelectedBuildSetting())) {
    return;
  }
  BuildToGui(true);
}

void ProjectSettingsDlg::OnBuildFeatureRemove(wxCommandEvent& event) {
  if (false == feature_list_.Remove(GetSelectedBuildSetting())) {
    return;
  }

  BuildToGui(true);
}

void ProjectSettingsDlg::OnBuildFeatureUp(wxCommandEvent& event) {
  if (false == feature_list_.Up(GetSelectedBuildSetting())) {
    return;
  }
  BuildToGui(true);
}

void ProjectSettingsDlg::OnBuildFeatureDown(wxCommandEvent& event) {
  if (false == feature_list_.Down(GetSelectedBuildSetting())) {
    return;
  }
  BuildToGui(true);
}

//////////////////////////////////////////////////////////////////////////

ride::RunSetting* ProjectSettingsDlg::GetSelectedRunSetting() {
  const int selection = uiRunConfigurations->GetSelection();
  const bool found = selection != wxNOT_FOUND;

  if (!found) return NULL;
  void* d = uiRunConfigurations->GetClientData(selection);
  ride::RunSetting* setting = reinterpret_cast<ride::RunSetting*>(d);

  return setting;
}

void ProjectSettingsDlg::RunToGui(bool togui) {
  if (allow_run_to_gui_ == false) return;
  allow_run_to_gui_ = false;

  if (togui && static_cast<wxItemContainer*>(uiRunConfigurations)->IsEmpty()) {
    for (int i = 0; i < project_->user().run_size(); ++i) {
      ride::RunSetting* rs = project_->user_ptr()->mutable_run(i);
      uiRunConfigurations->Append(rs->name().c_str(),
                                  reinterpret_cast<void*>(rs));
    }
    uiRunConfigurations->Select(project_->user().run_setting());
  }

  ride::RunSetting* setting_ptr = GetSelectedRunSetting();
  EnableDisable(setting_ptr != NULL)
      << uiRunApplication << uiRunApplicationCmd << uiRunArguments
      << uiRunArgumentsCmd << uiRunFolder << uiRunFolderCmd << uiCmdBeforeLaunch
      << uiCmdBeforeLaunchCmd << uiRunWaitForExit;

  if (setting_ptr == NULL) {
    allow_run_to_gui_ = true;
    return;
  }

  ride::RunSetting& setting = *setting_ptr;

  DIALOG_DATA(setting, application, uiRunApplication, _Str);
  DIALOG_DATA(setting, arguments, uiRunArguments, _Str);
  DIALOG_DATA(setting, folder, uiRunFolder, _Str);
  DIALOG_DATA(setting, cmd_before_launch, uiCmdBeforeLaunch, _Str);
  DIALOG_DATA(setting, wait_for_exit, uiRunWaitForExit, );

  allow_run_to_gui_ = true;
}

void ProjectSettingsDlg::OnRunConfiguration(wxCommandEvent& event) {
  RunToGui(true);
}

void ProjectSettingsDlg::OnRunConfigurationModify(wxCommandEvent& event) {
  ShowInfo(this, "This is not implemented yet", "Not implemented");
}

void ProjectSettingsDlg::OnRunText(wxCommandEvent& event) { RunToGui(false); }

void ProjectSettingsDlg::OnRunApplication(wxCommandEvent& event) {
  ShowInfo(this, "This is not implemented yet", "Not implemented");
}

void ProjectSettingsDlg::OnRunArguments(wxCommandEvent& event) {
  ShowInfo(this, "This is not implemented yet", "Not implemented");
}

void ProjectSettingsDlg::OnRunFolder(wxCommandEvent& event) {
  ShowInfo(this, "This is not implemented yet", "Not implemented");
}

void ProjectSettingsDlg::OnCmdBeforeLaunch(wxCommandEvent& event) {
  ShowInfo(this, "This is not implemented yet", "Not implemented");
}

void ProjectSettingsDlg::OnRunCheck(wxCommandEvent& event) { RunToGui(false); }
