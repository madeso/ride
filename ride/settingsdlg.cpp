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

void ToGui(ride::Color data, wxColourPickerCtrl* gui)  {
  gui->SetColour(C(data));
}
ride::Color ToData(wxColourPickerCtrl* gui)  {
  return C(gui->GetColour());
}

template<typename T>
T* Allocate(const T& t) {
  return new T(t);
}

#define DIALOG_DATA(ROOT, FUN, UI) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_##FUN(ToData(UI)); } } while(false)
#define DIALOG_DATAX(ROOT, FUN, UI) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_allocated_##FUN(Allocate(ToData(UI))); } } while(false)

void SettingsDlg::editToGui(bool togui) {
  ride::FontsAndColors fonts_and_colors = edit.fonts_and_colors();

  DIALOG_DATA(edit, displayeolenable, uiDisplayEOL);
  DIALOG_DATA(edit, linenumberenable, uiShowLineNumbers);
  DIALOG_DATA(edit, indentguideenable, uiIndentGuide);
  DIALOG_DATAX(fonts_and_colors, edgecolor, uiEdgeColor);

  if (togui == false) {
    edit.set_allocated_fonts_and_colors(Allocate(fonts_and_colors));
  }
}

