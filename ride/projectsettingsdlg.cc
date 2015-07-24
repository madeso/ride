#include "ride/projectsettingsdlg.h"

#include "ride/generated/ui.h"
#include "ride/project.h"
#include "ride/mainwindow.h"
#include <wx/fontenum.h>
#include "ride/wxutils.h"

class ProjectSettingsDlg : public ui::ProjectSettings
{
public:
  ProjectSettingsDlg(wxWindow* parent, MainWindow* mainwindow, Project* project);

protected:
  void OnApply(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);

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
}

void ProjectSettingsDlg::OnApply( wxCommandEvent& event )
{
}

void ProjectSettingsDlg::OnCancel( wxCommandEvent& event )
{
  EndModal(wxCANCEL);
}

void ProjectSettingsDlg::OnOk( wxCommandEvent& event )
{
  EndModal(wxOK);
}
