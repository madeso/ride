#include "ride/settingsdlg.h"
#include "ride/mainwindow.h"

SettingsDlg::SettingsDlg(wxWindow* parent, MainWindow* mainwindow) :
::ui::Settings(parent, wxID_ANY), main(mainwindow)
{
  global = main->getSettings();
  edit = global;
  editToGui(true);
}

void SettingsDlg::OnApply( wxCommandEvent& event )
{
  editToGui(false);
  main->setSettings(edit);
}

void SettingsDlg::OnCancel( wxCommandEvent& event )
{
  main->setSettings(global);
  EndModal(wxCANCEL);
}

void SettingsDlg::OnOk( wxCommandEvent& event )
{
  editToGui(false);
  main->setSettings(edit);
  if (false == SaveSettings(edit)) {
    wxMessageBox("Failed to save settings", "Failed!", wxOK | wxICON_ERROR);
  }
  EndModal(wxOK);
}

void ToGui(bool data, wxCheckBox* gui)  {
  gui->SetValue(data);
}
bool ToData(wxCheckBox* gui)  {
  return gui->GetValue();
}

#define DIALOG_DATA(ROOT, FUN, UI) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_##FUN(ToData(UI)); } } while(false)

void SettingsDlg::editToGui(bool togui) {
  DIALOG_DATA(edit, displayeolenable, uiDisplayEOL);
  DIALOG_DATA(edit, linenumberenable, uiShowLineNumbers);
  DIALOG_DATA(edit, indentguideenable, uiIndentGuide);
}

