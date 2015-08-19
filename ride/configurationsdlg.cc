// Copyright (2015) Gustav

#include "ride/configurationsdlg.h"

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

struct ProjectFunctions {
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

class ConfigurationsDlg : public ui::Configurations {
 public:
  ConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow, Project* project);

  bool has_applied() const { return has_applied_; }

 protected:
  void OnApply(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);

 protected:
  bool Apply();
  void AllToGui(bool togui);

 protected:
  void OnAdd(wxCommandEvent& event);
  void OnEdit(wxCommandEvent& event);
  void OnRemove(wxCommandEvent& event);
  void OnUp(wxCommandEvent& event);
  void OnDown(wxCommandEvent& event);

 private:
  MainWindow* main_window_;
  Project* project_;
  ride::Project project_backup_;
  GuiList<ride::Project, ProjectFunctions> feature_list_;
  bool has_applied_;
};

bool DoConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                         Project* project) {
  ConfigurationsDlg dlg(parent, mainwindow, project);
  dlg.ShowModal();

  return dlg.has_applied();
}

ConfigurationsDlg::ConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                                     Project* project)
    : ::ui::Configurations(parent, wxID_ANY),
      main_window_(mainwindow),
      project_(project),
      project_backup_(project->project()),
      feature_list_(uiList, this),
      has_applied_(false) {
  AllToGui(true);

  feature_list_.Setup(uiListAdd, uiListRemove, uiListChange, uiListUp,
                      uiListDown);
}

void ConfigurationsDlg::OnApply(wxCommandEvent& event) { Apply(); }

void ConfigurationsDlg::OnCancel(wxCommandEvent& event) {
  project_->set_project(project_backup_);
  EndModal(wxCANCEL);
}

void ConfigurationsDlg::OnOk(wxCommandEvent& event) {
  if (Apply()) {
    EndModal(wxOK);
  }
}

void ConfigurationsDlg::AllToGui(bool togui) {
  feature_list_.ToGui(project_->project_ptr(), togui);
}

//////////////////////////////////////////////////////////////////////////

bool ConfigurationsDlg::Apply() {
  AllToGui(false);

  project_backup_ = project_->project();

  has_applied_ = true;
  return true;
}

void ConfigurationsDlg::OnAdd(wxCommandEvent& event) {
  if (false == feature_list_.Add(project_->project_ptr())) {
    return;
  }
  AllToGui(true);
}

void ConfigurationsDlg::OnEdit(wxCommandEvent& event) {
  if (false == feature_list_.Edit(project_->project_ptr())) {
    return;
  }
  AllToGui(true);
}

void ConfigurationsDlg::OnRemove(wxCommandEvent& event) {
  if (false == feature_list_.Remove(project_->project_ptr())) {
    return;
  }

  AllToGui(true);
}

void ConfigurationsDlg::OnUp(wxCommandEvent& event) {
  if (false == feature_list_.Up(project_->project_ptr())) {
    return;
  }
  AllToGui(true);
}

void ConfigurationsDlg::OnDown(wxCommandEvent& event) {
  if (false == feature_list_.Down(project_->project_ptr())) {
    return;
  }
  AllToGui(true);
}
