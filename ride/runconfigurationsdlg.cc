// Copyright (2015) Gustav

#include "ride/runconfigurationsdlg.h"

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

struct ProjectRunFunctions {
  static const wxString ADD_TEXT;
  static const wxString EDIT_TEXT;

  static int Size(ride::UserProject* p) { return p->run_size(); }

  static wxString GetDisplayString(ride::UserProject* p, int i) {
    return p->run(i).name();
  }

  static void SetDisplayString(ride::UserProject* p, int i,
                               const wxString& new_string) {
    return p->mutable_run(i)->set_name(new_string);
  }

  static void Add(ride::UserProject* p, const wxString& name) {
    ride::RunSetting* build = p->add_run();
    build->set_name(name);
  }

  static void Remove(ride::UserProject* p, int i) {
    p->mutable_run()->DeleteSubrange(i, 1);
  }

  static void Swap(ride::UserProject* p, int selection, int next_index) {
    std::swap(*p->mutable_run(selection), *p->mutable_run(next_index));
  }
};

const wxString ProjectRunFunctions::ADD_TEXT = "Name of run to create";
const wxString ProjectRunFunctions::EDIT_TEXT =
    "Please specify the new run name";

class RunConfigurationsDlg : public ui::Configurations {
 public:
  RunConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                       Project* project, int selected);

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
  Project* project_;
  ride::UserProject user_backup_;
  GuiList<ride::UserProject, ProjectRunFunctions> feature_list_;
  bool has_applied_;
};

bool DoRunConfigurationsDlg(wxWindow* parent, MainWindow* mainwindow,
                            Project* project, int selected) {
  RunConfigurationsDlg dlg(parent, mainwindow, project, selected);
  dlg.ShowModal();

  return dlg.has_applied();
}

RunConfigurationsDlg::RunConfigurationsDlg(wxWindow* parent,
                                           MainWindow* mainwindow,
                                           Project* project, int selected)
    : ::ui::Configurations(parent, wxID_ANY),
      project_(project),
      user_backup_(project->user()),
      feature_list_(uiList, this),
      has_applied_(false) {
  AllToGui(true);
  SetTitle("Run Configuration Manager");

  feature_list_.Select(selected, project->user_ptr());
  feature_list_.Setup(uiListAdd, uiListRemove, uiListChange, uiListUp,
                      uiListDown);
}

void RunConfigurationsDlg::OnApply(wxCommandEvent& event) { Apply(); }

void RunConfigurationsDlg::OnCancel(wxCommandEvent& event) {
  project_->set_user(user_backup_);
  EndModal(wxCANCEL);
}

void RunConfigurationsDlg::OnOk(wxCommandEvent& event) {
  if (Apply()) {
    EndModal(wxOK);
  }
}

void RunConfigurationsDlg::AllToGui(bool togui) {
  feature_list_.ToGui(project_->user_ptr(), togui);
}

//////////////////////////////////////////////////////////////////////////

bool RunConfigurationsDlg::Apply() {
  AllToGui(false);

  user_backup_ = project_->user();

  has_applied_ = true;
  return true;
}

void RunConfigurationsDlg::OnAdd(wxCommandEvent& event) {
  if (false == feature_list_.Add(project_->user_ptr())) {
    return;
  }
  AllToGui(true);
}

void RunConfigurationsDlg::OnEdit(wxCommandEvent& event) {
  if (false == feature_list_.Edit(project_->user_ptr())) {
    return;
  }
  AllToGui(true);
}

void RunConfigurationsDlg::OnRemove(wxCommandEvent& event) {
  if (false == feature_list_.Remove(project_->user_ptr())) {
    return;
  }

  AllToGui(true);
}

void RunConfigurationsDlg::OnUp(wxCommandEvent& event) {
  if (false == feature_list_.Up(project_->user_ptr())) {
    return;
  }
  AllToGui(true);
}

void RunConfigurationsDlg::OnDown(wxCommandEvent& event) {
  if (false == feature_list_.Down(project_->user_ptr())) {
    return;
  }
  AllToGui(true);
}
