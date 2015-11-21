// Copyright (2015) Gustav

#include "ride/buildconfigurationsdlg.h"

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

struct ProjectBuildFunctions {
  static const wxString ADD_TEXT;
  static const wxString EDIT_TEXT;

  static int Size(ride::Project* p) { return p->build_settings_size(); }

  static wxString GetDisplayString(ride::Project* p, int i) {
    return p->build_settings(i).name();
  }

  static void SetDisplayString(ride::Project* p, int i,
                               const wxString& new_string) {
    return p->mutable_build_settings(i)->set_name(new_string);
  }

  static void Add(ride::Project* p, const wxString& name) {
    ride::BuildSetting* build = p->add_build_settings();
    build->set_name(name);
  }

  static void Remove(ride::Project* p, int i) {
    p->mutable_build_settings()->DeleteSubrange(i, 1);
  }

  static void Swap(ride::Project* p, int selection, int next_index) {
    std::swap(*p->mutable_build_settings(selection),
              *p->mutable_build_settings(next_index));
  }
};

const wxString ProjectBuildFunctions::ADD_TEXT = "Name of build to create";
const wxString ProjectBuildFunctions::EDIT_TEXT =
    "Please specify the new build name";

typedef ui::Configurations Configurations;

class BuildConfigurationsDlg : public Configurations {
 public:
  BuildConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                         Project* project, int selected);

  bool has_applied() const { return has_applied_; }

 protected:
  void OnApply(wxCommandEvent& event) override;
  void OnCancel(wxCommandEvent& event) override;
  void OnOk(wxCommandEvent& event) override;

 protected:
  bool Apply();
  void AllToGui(bool togui);

 protected:
  void OnAdd(wxCommandEvent& event) override;
  void OnEdit(wxCommandEvent& event) override;
  void OnRemove(wxCommandEvent& event) override;
  void OnUp(wxCommandEvent& event) override;
  void OnDown(wxCommandEvent& event) override;

 private:
  Project* project_;
  ride::Project project_backup_;
  GuiList<ride::Project, ProjectBuildFunctions> feature_list_;
  bool has_applied_;
};

bool DoBuildConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                              Project* project, int selected) {
  BuildConfigurationsDlg dlg(parent, mainwindow, project, selected);
  dlg.ShowModal();

  return dlg.has_applied();
}

BuildConfigurationsDlg::BuildConfigurationsDlg(wxWindow* parent,
                                               MainWindow* mainwindow,
                                               Project* project, int selected)
    : ::ui::Configurations(parent, wxID_ANY),
      project_(project),
      project_backup_(project->project()),
      feature_list_(uiList, this),
      has_applied_(false) {
  AllToGui(true);
  Configurations::SetTitle("Build Configuration Manager");

  feature_list_.Select(selected, project->project_ptr());
  feature_list_.Setup(uiListAdd, uiListRemove, uiListChange, uiListUp,
                      uiListDown);
}

void BuildConfigurationsDlg::OnApply(wxCommandEvent& event) { Apply(); }

void BuildConfigurationsDlg::OnCancel(wxCommandEvent& event) {
  project_->set_project(project_backup_);
  EndModal(wxCANCEL);
}

void BuildConfigurationsDlg::OnOk(wxCommandEvent& event) {
  if (Apply()) {
    EndModal(wxOK);
  }
}

void BuildConfigurationsDlg::AllToGui(bool togui) {
  feature_list_.ToGui(project_->project_ptr(), togui);
}

//////////////////////////////////////////////////////////////////////////

bool BuildConfigurationsDlg::Apply() {
  AllToGui(false);

  project_backup_ = project_->project();

  has_applied_ = true;
  return true;
}

void BuildConfigurationsDlg::OnAdd(wxCommandEvent& event) {
  if (false == feature_list_.Add(project_->project_ptr())) {
    return;
  }
  AllToGui(true);
}

void BuildConfigurationsDlg::OnEdit(wxCommandEvent& event) {
  if (false == feature_list_.Edit(project_->project_ptr())) {
    return;
  }
  AllToGui(true);
}

void BuildConfigurationsDlg::OnRemove(wxCommandEvent& event) {
  if (false == feature_list_.Remove(project_->project_ptr())) {
    return;
  }

  AllToGui(true);
}

void BuildConfigurationsDlg::OnUp(wxCommandEvent& event) {
  if (false == feature_list_.Up(project_->project_ptr())) {
    return;
  }
  AllToGui(true);
}

void BuildConfigurationsDlg::OnDown(wxCommandEvent& event) {
  if (false == feature_list_.Down(project_->project_ptr())) {
    return;
  }
  AllToGui(true);
}
