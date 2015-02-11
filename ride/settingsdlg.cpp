#include "ride/wx.h"
#include "ride/settingsdlg.h"
#include "ride/mainwindow.h"
#include <wx/fontenum.h>

//////////////////////////////////////////////////////////////////////////

#define RETURN_COMBOBOX_VALUE(TYPE, VALUE) assert(ride::TYPE##_IsValid(VALUE)); return static_cast<ride::TYPE>(VALUE)
#define DIALOG_DATA(ROOT, FUN, UI, SETNAME) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_##FUN(ToData##SETNAME(UI)); } } while(false)
#define DIALOG_DATAX(ROOT, FUN, UI) do { if( togui ) { ToGui(ROOT.FUN(), UI); } else { ROOT.set_allocated_##FUN(Allocate(ToData(UI))); } } while(false)

template<typename T>
T* Allocate(const T& t) {
  return new T(t);
}

//////////////////////////////////////////////////////////////////////////

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
  if (value.length() == 0) return -1;
  assert(false && "Unable to get integer value");
  return -1;
}

//////////////////////////////////////////////////////////////////////////

class StyleLink {
public:
  StyleLink(const wxString& name)
    : name_(name)
  { }

  virtual ~StyleLink() {
  };

  const wxString& name() const {
    assert(this);
    return name_;
  }

  virtual const ride::Style get(const ride::FontsAndColors& color) = 0;

  virtual void set(ride::FontsAndColors& color, const ride::Style& style) = 0;

  // todo: add some form of group so we can easily group styles in the gui

private:
  wxString name_;
};

std::vector<StyleLink*> BuildStyleLinks() {
  std::vector<StyleLink*> ret;
#define DEF_STYLE(NAME, STYLE) \
  struct StyleLink##STYLE : public StyleLink { \
  StyleLink##STYLE() : StyleLink(NAME) {} \
  const ride::Style get(const ride::FontsAndColors& co){return co.has_ ## STYLE() ? co.STYLE() : ride::Style();}\
  void set(ride::FontsAndColors& co, const ride::Style& style){co.set_allocated_ ## STYLE(Allocate(style));} \
  };\
  static StyleLink##STYLE styleLink##STYLE;\
  ret.push_back(&styleLink##STYLE)

  DEF_STYLE("Default", default_style);
  DEF_STYLE("Brace light", bracelight_style);
  DEF_STYLE("Brace bad", bracebad_style);
  DEF_STYLE("Control char", controlchar_style);
  DEF_STYLE("Indent guide", indentguide_style);
  DEF_STYLE("Calltip stype", calltip_style);
#undef DEF_STYLE
  return ret;
}

const std::vector<StyleLink*>& StyleLinks() {
  static std::vector<StyleLink*> links = BuildStyleLinks();
  return links;
}

class FontLister : public wxFontEnumerator {
public:
  std::vector<wxString> fonts;
  virtual bool OnFacename(const wxString& font) {
    // The version of the font with the '@' is a version optimized for writing in the vertical direction.
    // https://groups.google.com/forum/#!topic/wx-users/3hjrhPlSULI
    if (font.StartsWith("@") == false) {
      fonts.push_back(font);
    }
    return true;
  }
};

void SettingsDlg::OnOnlyFixedSysChanged(wxCommandEvent& event) {
  updateFonts();
}

void SettingsDlg::updateFonts() {
  FontLister allfonts;

  const bool fixedSize = uiStyleOnlyFixedSize->GetValue();

  allfonts.EnumerateFacenames(wxFONTENCODING_SYSTEM, fixedSize);
  uiStyleTypeface->Clear();
  for (auto name : allfonts.fonts) {
    uiStyleTypeface->AppendString(name);
  }
}

void SettingsDlg::OnSelectedStyleChanged(wxCommandEvent& event) {
  allowStyleChange = false;
  styleToGui(true);
  allowStyleChange = true;
}

void SettingsDlg::OnStyleFontChanged(wxCommandEvent& event) {
  updateFontDisplay();
  styleChanged();
}

void SettingsDlg::OnStyleCheckChanged(wxCommandEvent& event) {
  styleChanged();
}

void SettingsDlg::OnStyleComboboxChanged(wxCommandEvent& event) {
  styleChanged();
}

void SettingsDlg::OnStyleColorChanged(wxColourPickerEvent& event) {
  styleChanged();
}

void SettingsDlg::styleChanged() {
  if (allowStyleChange == false) return;
  styleToGui(false);
  main->setSettings(edit);
  main->setSettings(edit); // update seems to lag behind one setSettings, this seems to fix that, weird... I should investigate this...
}

void SettingsDlg::styleToGui(bool togui) {
  const int selection = uiFontStyles->GetSelection();
  if (selection == wxNOT_FOUND) return;
  StyleLink* link = reinterpret_cast<StyleLink*>(uiFontStyles->GetClientData(selection));
  assert(link);
  if (link == NULL) return;

  ride::Style style;
  
  /*if (togui) {
    if (edit.has_fonts_and_colors()) {*/
      style = link->get( edit.fonts_and_colors() );
    /*}
  }*/

  DIALOG_DATA(style, use_foreground, uiStyleUseForeground,);
  DIALOG_DATA(style, use_background, uiStyleUseBackground,);
  DIALOG_DATAX(style, foreground, uiStyleForeground);
  DIALOG_DATAX(style, background, uiStyleBackground);

  if (togui == false) {
    ride::FontsAndColors color;
    if (edit.has_fonts_and_colors()) {
      color = edit.fonts_and_colors();
    }
    link->set(color, style);

    edit.set_allocated_fonts_and_colors(Allocate(color));
  }
}

void SettingsDlg::updateFontDisplay() {
  int selectedId = uiStyleTypeface->GetSelection();
  wxString faceName = selectedId > 0 ? uiStyleTypeface->GetString(selectedId) : "";
  wxFont font( wxFontInfo(12).FaceName(faceName) );
  uiStyleExample->SetFont(font);
}

//////////////////////////////////////////////////////////////////////////

SettingsDlg::SettingsDlg(wxWindow* parent, MainWindow* mainwindow) :
::ui::Settings(parent, wxID_ANY), main(mainwindow), allowApply(false), allowStyleChange(false)
{
  global = main->getSettings();
  edit = global;
  editToGui(true);
  allowApply = true;

  for (auto link: StyleLinks()) {
    uiFontStyles->Append(link->name(), link);
  }
  updateFonts();
}

void SettingsDlg::OnApply( wxCommandEvent& event )
{
  apply();
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

void SettingsDlg::OnCheckboxChanged(wxCommandEvent& event)
{
  assert(this);
  apply();
}

void SettingsDlg::OnComboboxChanged(wxCommandEvent& event)
{
  assert(this);
  apply();
}

void SettingsDlg::OnColorChanged(wxColourPickerEvent& event)
{
  assert(this);
  apply();
}

void SettingsDlg::OnEditChanged(wxCommandEvent& event)
{
  assert(this);
  apply();
}

void SettingsDlg::apply()
{
  if (allowApply == false) { return; }
  editToGui(false);
  main->setSettings(edit);
}

//////////////////////////////////////////////////////////////////////////

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

