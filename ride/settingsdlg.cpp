#include "ride/wx.h"
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

#define RETURN_COMBOBOX_VALUE(TYPE, VALUE) assert(ride::TYPE##_IsValid(VALUE)); return static_cast<ride::TYPE>(VALUE)

void ToGui(ride::ViewWhitespace data, wxComboBox* gui)  {
  gui->SetSelection(static_cast<int>(data));
}
ride::ViewWhitespace ToData_VW(wxComboBox* gui)  {
  RETURN_COMBOBOX_VALUE(ViewWhitespace, gui->GetSelection());
}

void ToGui(ride::WrapMode data, wxComboBox* gui)  {
  gui->SetSelection(static_cast<int>(data));
}
ride::WrapMode ToData_WM(wxComboBox* gui)  {
  RETURN_COMBOBOX_VALUE(WrapMode, gui->GetSelection());
}

void ToGui(ride::EdgeStyle data, wxComboBox* gui)  {
  gui->SetSelection(static_cast<int>(data));
}
ride::EdgeStyle ToData_ES(wxComboBox* gui)  {
  RETURN_COMBOBOX_VALUE(EdgeStyle, gui->GetSelection());
}

void ToGui(google::protobuf::int32 data, wxTextCtrl* gui)  {
  wxString value = wxString::FromDouble(data, 0);
  gui->SetValue(value);
}
google::protobuf::int32 ToData(wxTextCtrl* gui)  {
  const wxString value = gui->GetValue();
  long ret = 0;
  if (true == value.ToLong(&ret)) {
    return ret;
  }
  assert(false && "Unable to get integer value");
  return -1;
}

template<typename T>
T* Allocate(const T& t) {
  return new T(t);
}

#define DIALOG_DATA(ROOT, FUN, UI, SETNAME) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_##FUN(ToData##SETNAME(UI)); } } while(false)
#define DIALOG_DATAX(ROOT, FUN, UI) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_allocated_##FUN(Allocate(ToData(UI))); } } while(false)

void SettingsDlg::editToGui(bool togui) {
  ride::FontsAndColors fonts_and_colors = edit.fonts_and_colors();
  ride::FoldFlags foldflags = edit.foldflags();

  DIALOG_DATA(edit, displayeolenable, uiDisplayEOL,);
  DIALOG_DATA(edit, linenumberenable, uiShowLineNumbers,);
  DIALOG_DATA(edit, indentguideenable, uiIndentGuide,);

  DIALOG_DATA(edit, tabwidth, uiTabWidth, );
  DIALOG_DATA(edit, edgecolumn, uiEdgeColumn, );

  DIALOG_DATA(edit, whitespace, uiViewWhitespace, _VW);
  DIALOG_DATA(edit, wordwrap, uiWordwrap, _WM);
  DIALOG_DATA(edit, edgestyle, uiEdgeStyle, _ES);

  DIALOG_DATA(edit, tabindents, uiTabIndents, );
  DIALOG_DATA(edit, usetabs, uiUseTabs, );
  DIALOG_DATA(edit, backspaceunindents, uiBackspaceUnindents, );

  DIALOG_DATA(edit, foldenable, uiAllowFolding, );
  
  DIALOG_DATA(foldflags, levelnumbers, uiFoldLevelNumbers, );
  DIALOG_DATA(foldflags, linebefore_expanded, uiFoldLineBeforeExpanded, );
  DIALOG_DATA(foldflags, linebefore_contracted, uiFoldLineBeforeContracted, );
  DIALOG_DATA(foldflags, lineafter_expanded, uiFoldLineAfterExpanded, );
  DIALOG_DATA(foldflags, lineafter_contracted, uiFoldLineAfterContracted, );

  DIALOG_DATAX(fonts_and_colors, edgecolor, uiEdgeColor);

  if (togui == false) {
    edit.set_allocated_fonts_and_colors(Allocate(fonts_and_colors));
    edit.set_allocated_foldflags(Allocate(foldflags));
  }
}

