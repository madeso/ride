// Copyright (2015) Gustav

#include "ride/settingsdlg.h"

#include <ride/wx.h>

#include <vector>
#include <algorithm>

#include "ride/generated/ui.h"

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/settings.h"
#include "ride/wxutils.h"
#include "ride/guilist.h"
#include "ride/proto.h"

//////////////////////////////////////////////////////////////////////////
// custom form functions

void ToGui(ride::Color data, wxColourPickerCtrl* gui) {
  gui->SetColour(C(data));
}
ride::Color ToData(wxColourPickerCtrl* gui) { return C(gui->GetColour()); }

void ToGui(ride::ViewWhitespace data, wxComboBox* gui) {
  gui->SetSelection(static_cast<int>(data));
}
ride::ViewWhitespace ToData_VW(wxComboBox* gui) {
  RETURN_COMBOBOX_VALUE(ViewWhitespace, gui->GetSelection());
}

void ToGui(ride::WrapMode data, wxComboBox* gui) {
  gui->SetSelection(static_cast<int>(data));
}
ride::WrapMode ToData_WM(wxComboBox* gui) {
  RETURN_COMBOBOX_VALUE(WrapMode, gui->GetSelection());
}

void ToGui(ride::EdgeStyle data, wxComboBox* gui) {
  gui->SetSelection(static_cast<int>(data));
}
ride::EdgeStyle ToData_ES(wxComboBox* gui) {
  RETURN_COMBOBOX_VALUE(EdgeStyle, gui->GetSelection());
}

void ToGui(ride::AutoIndentation data, wxComboBox* gui) {
  gui->SetSelection(static_cast<int>(data));
}
ride::AutoIndentation ToData_AI(wxComboBox* gui) {
  RETURN_COMBOBOX_VALUE(AutoIndentation, gui->GetSelection());
}

void ToGui(ride::MarkerSymbol data, wxComboBox* gui) {
  gui->SetSelection(static_cast<int>(data));
}
ride::MarkerSymbol ToData_MS(wxComboBox* gui) {
  RETURN_COMBOBOX_VALUE(MarkerSymbol, gui->GetSelection());
}

void ToGui(ride::IndicatorStyle data, wxComboBox* gui) {
  gui->SetSelection(static_cast<int>(data));
}
ride::IndicatorStyle ToData_IS(wxComboBox* gui) {
  RETURN_COMBOBOX_VALUE(IndicatorStyle, gui->GetSelection());
}

//////////////////////////////////////////////////////////////////////////

struct ThemeFunctions {
  static const wxString ADD_TEXT;
  static const wxString EDIT_TEXT;

  static int Size(ride::Settings* bs) { return bs->themes_size(); }

  static wxString GetDisplayString(ride::Settings* bs, int i) {
    return bs->themes(i).name();
  }

  static void SetDisplayString(ride::Settings* bs, int i,
                               const wxString& new_string) {
    bs->mutable_themes(i)->set_name(new_string);
  }

  static void Add(ride::Settings* bs, const wxString& name) {
    ride::Theme* theme = bs->add_themes();
    theme->set_name(name);
    theme->set_allocated_data(new ride::FontsAndColors(bs->fonts_and_colors()));
  }

  static void Remove(ride::Settings* bs, int i) {
    bs->mutable_themes()->DeleteSubrange(i, 1);
  }

  static void Swap(ride::Settings* bs, int selection, int next_index) {
    std::swap(*bs->mutable_themes(selection), *bs->mutable_themes(next_index));
  }
};
const wxString ThemeFunctions::ADD_TEXT = "Save current theme as ";
const wxString ThemeFunctions::EDIT_TEXT = "Please specify the new theme name";

//////////////////////////////////////////////////////////////////////////

class SettingsDlg : public ui::Settings {
 public:
  SettingsDlg(wxWindow* parent, MainWindow* mainwindow);

 protected:
  void OnApply(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Style tab
  void OnOnlyFixedSysChanged(wxCommandEvent& event);
  void OnSelectedStyleChanged(wxCommandEvent& event);
  void OnStyleFontChanged(wxCommandEvent& event);
  void OnStyleCheckChanged(wxCommandEvent& event);
  void OnStyleTextChanged(wxCommandEvent& event);
  void OnStyleColorChanged(wxColourPickerEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Indicator tab
  void OnIndicatorListChanged(wxCommandEvent& event);
  void OnIndicatorCombobox(wxCommandEvent& event);
  void OnIndicatorColor(wxColourPickerEvent& event);
  void OnIndicatorCheckbox(wxCommandEvent& event);
  void OnIndicatorText(wxCommandEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Marker tab
  void OnMarkerListChanged(wxCommandEvent& event);
  void OnMarkerComboChanged(wxCommandEvent& event);
  void OnMarkerColorChanged(wxColourPickerEvent& event);
  void OnMarkerComboChanged(wxColourPickerEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Edit tab
  void OnCheckboxChanged(wxCommandEvent& event);
  void OnComboboxChanged(wxCommandEvent& event);
  void OnColorChanged(wxColourPickerEvent& event);
  void OnEditChanged(wxCommandEvent& event);
  void OnlyAllowNumberChars(wxKeyEvent& event);

  //////////////////////////////////////////////////////////////////////////
  // Theme tab

  void OnThemeApplySelected(wxCommandEvent& event);
  void OnThemeExportSelected(wxCommandEvent& event);
  void OnThemeImport(wxCommandEvent& event);

  void OnAdd(wxCommandEvent& event);
  void OnEdit(wxCommandEvent& event);
  void OnRemove(wxCommandEvent& event);
  void OnUp(wxCommandEvent& event);
  void OnDown(wxCommandEvent& event);

 private:
  MainWindow* main_window_;
  ride::Settings global_settings_;
  ride::Settings current_settings_;

  void UpdateMain() {
    for (int i = 0; i < 2; ++i) {
      main_window_->set_settings(current_settings_);
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // Style tab
  bool allow_send_style_to_main_;
  void UpdateStyleEnable();
  void SendStyleToMain();
  void StyleToGui(bool togui);
  void UpdateStyleFonts();
  void StyleUpdateFontDisplay();
  void StyleSaveSelectedIndex();

  //////////////////////////////////////////////////////////////////////////
  // Indicator tab
  bool allow_send_indicator_to_main_;
  void SendIndicatorToMain();
  void IndicatorToGui(bool togui);

  //////////////////////////////////////////////////////////////////////////
  // Marker tab
  bool allow_send_marker_to_main_;
  void SendMarkerToMain();
  void MarkerToGui(bool togui);

  //////////////////////////////////////////////////////////////////////////
  // Edit tab
  bool allow_send_edit_to_main_;
  void SendEditToMain();
  void EditToGui(bool togui);

  //////////////////////////////////////////////////////////////////////////
  // Theme tab
  GuiList<ride::Settings, ThemeFunctions> theme_list_;
  void ThemeToGui(bool togui);
};

void ShowSettingsDlg(wxWindow* parent, MainWindow* mainwindow) {
  SettingsDlg dlg(parent, mainwindow);
  dlg.ShowModal();
}

//////////////////////////////////////////////////////////////////////////

class StyleLink {
 public:
  explicit StyleLink(const wxString& name) : name_(name) {}

  virtual ~StyleLink() {}

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

#define DEF_STYLE(NAME, STYLE)                                     \
  struct StyleLink##STYLE : public StyleLink {                     \
    StyleLink##STYLE() : StyleLink(NAME) {}                        \
    const ride::Style get(const ride::FontsAndColors& co) {        \
      return co.has_##STYLE() ? co.STYLE() : ride::Style();        \
    }                                                              \
    void set(ride::FontsAndColors& co, const ride::Style& style) { \
      co.set_allocated_##STYLE(Allocate(style));                   \
    }                                                              \
  };                                                               \
  static StyleLink##STYLE styleLink##STYLE;                        \
  ret.push_back(&styleLink##STYLE)

  DEF_STYLE("Default", default_style);
  DEF_STYLE("Line number", line_number_style);
  DEF_STYLE("Brace light", bracelight_style);
  DEF_STYLE("Brace bad", bracebad_style);
  DEF_STYLE("Control char", controlchar_style);
  DEF_STYLE("Indent guide", indentguide_style);
  DEF_STYLE("Calltip stype", calltip_style);

  DEF_STYLE("Annotation error", annotation_error_style);
  DEF_STYLE("Annotation warning", annotation_warning_style);

  DEF_STYLE("Comment", style_comment);
  DEF_STYLE("Commentline", style_commentline);
  DEF_STYLE("Commentdoc", style_commentdoc);
  DEF_STYLE("Number", style_number);
  DEF_STYLE("Keyword", style_keyword);
  DEF_STYLE("String", style_string);
  DEF_STYLE("Character", style_character);
  DEF_STYLE("UUID", style_uuid);
  DEF_STYLE("Preprocessor", style_preprocessor);
  DEF_STYLE("Operator", style_operator);
  DEF_STYLE("Identifier", style_identifier);
  DEF_STYLE("String EOL", style_string_eol);
  DEF_STYLE("Verbatim", style_verbatim);
  DEF_STYLE("Regex", style_regex);
  DEF_STYLE("Commentline doc", style_commentlinedoc);
  DEF_STYLE("Types", style_keyword_types);
  DEF_STYLE("Comment doc keyword", style_commentdockeyword);
  DEF_STYLE("Comment doc keyword error", style_commentdockeyworderror);
  DEF_STYLE("Global class", style_globalclass);
  DEF_STYLE("String raw", style_stringraw);
  DEF_STYLE("Triple verbatim", style_tripleverbatim);
  DEF_STYLE("Hash quoted string", style_hashquotedstring);
  DEF_STYLE("Preprocessor comment", style_preprocessorcomment);

  DEF_STYLE("Rust Lifetimg", rust_lifetime);
  DEF_STYLE("Rust Lexical error", rust_lex_error);

  DEF_STYLE("Markdown Line begin", markdown_line_begin);
  DEF_STYLE("Markdown Strong 1", markdown_strong1);
  DEF_STYLE("Markdown Strong 2", markdown_strong2);
  DEF_STYLE("Markdown EM 1", markdown_em1);
  DEF_STYLE("Markdown EM 2", markdown_em2);
  DEF_STYLE("Markdown Header 1", markdown_header1);
  DEF_STYLE("Markdown Header 2", markdown_header2);
  DEF_STYLE("Markdown Header 3", markdown_header3);
  DEF_STYLE("Markdown Header 4", markdown_header4);
  DEF_STYLE("Markdown Header 5", markdown_header5);
  DEF_STYLE("Markdown Header 6", markdown_header6);
  DEF_STYLE("Markdown Prechar", markdown_prechar);
  DEF_STYLE("Markdown UList item", markdown_ulist_item);
  DEF_STYLE("Markdown OList item", markdown_olist_item);
  DEF_STYLE("Markdown Blockquote", markdown_blockquote);
  DEF_STYLE("Markdown Strikeout", markdown_strikeout);
  DEF_STYLE("Markdown HRule", markdown_hrule);
  DEF_STYLE("Markdown Link", markdown_link);
  DEF_STYLE("Markdown Code 1", markdown_code);
  DEF_STYLE("Markdown Code 2", markdown_code2);
  DEF_STYLE("Markdown Code bk", markdown_codebk);

  DEF_STYLE("Props section", props_section);
  DEF_STYLE("Props assignment", props_assignment);
  DEF_STYLE("Props defval", props_defval);
  DEF_STYLE("Props key", props_key);

  DEF_STYLE("XML/HTML Tag", h_tag);
  DEF_STYLE("XML/HTML Tag unknown", h_tagunknown);
  DEF_STYLE("XML/HTML attribute", h_attribute);
  DEF_STYLE("XML/HTML attribute unknown", h_attributeunknown);
  DEF_STYLE("XML/HTML Number", h_number);
  DEF_STYLE("XML/HTML Double string", h_doublestring);
  DEF_STYLE("XML/HTML Single string", h_singlestring);
  DEF_STYLE("XML/HTML Other", h_other);
  DEF_STYLE("XML/HTML Entity", h_entity);
  DEF_STYLE("XML/HTML Tagend", h_tagend);
  DEF_STYLE("XML/HTML XML start", h_xmlstart);
  DEF_STYLE("XML/HTML XML end", h_xmlend);
  DEF_STYLE("XML/HTML Script", h_script);
  DEF_STYLE("XML/HTML ASP", h_asp);
  DEF_STYLE("XML/HTML ASP at", h_aspat);
  DEF_STYLE("XML/HTML CData", h_cdata);
  DEF_STYLE("XML/HTML Question", h_question);
  DEF_STYLE("XML/HTML Value", h_value);
  DEF_STYLE("XML/HTML XC Comment", h_xccomment);

  DEF_STYLE("CMake String DQ", cmake_stringdq);
  DEF_STYLE("CMake String LQ", cmake_stringlq);
  DEF_STYLE("CMake String RQ", cmake_stringrq);
  DEF_STYLE("CMake Commands", cmake_commands);
  DEF_STYLE("CMake Parameters", cmake_parameters);
  DEF_STYLE("CMake Variable", cmake_variable);
  DEF_STYLE("CMake Userdefined", cmake_userdefined);
  DEF_STYLE("CMake While def", cmake_whiledef);
  DEF_STYLE("CMake Foreach def", cmake_foreachdef);
  DEF_STYLE("CMake Ifdefine def", cmake_ifdefinedef);
  DEF_STYLE("CMake Macro def", cmake_macrodef);
  DEF_STYLE("CMake Stringvar", cmake_stringvar);
  DEF_STYLE("CMake Number", cmake_number);

  DEF_STYLE("Lua Keyword Label", lua_label);

  DEF_STYLE("YAML Number", yaml_number);
  DEF_STYLE("YAML Reference", yaml_reference);
  DEF_STYLE("YAML Document", yaml_document);
  DEF_STYLE("YAML Text", yaml_text);
  DEF_STYLE("YAML Error", yaml_error);
#undef DEF_STYLE
  return ret;
}

const std::vector<StyleLink*>& StyleLinks() {
  static std::vector<StyleLink*> links = BuildStyleLinks();
  return links;
}

void SettingsDlg::OnOnlyFixedSysChanged(wxCommandEvent& event) {
  UpdateStyleFonts();
}

void SettingsDlg::UpdateStyleFonts() {
  const std::vector<wxString> all_fonts =
      ListFonts(uiStyleOnlyFixedSize->GetValue());

  uiStyleTypeface->Clear();
  for (auto name : all_fonts) {
    uiStyleTypeface->AppendString(name);
  }
}

void SettingsDlg::OnSelectedStyleChanged(wxCommandEvent& event) {
  allow_send_style_to_main_ = false;
  StyleToGui(true);
  UpdateStyleEnable();
  allow_send_style_to_main_ = true;
}

void SettingsDlg::OnStyleFontChanged(wxCommandEvent& event) {
  StyleUpdateFontDisplay();
  SendStyleToMain();
}

void SettingsDlg::OnStyleCheckChanged(wxCommandEvent& event) {
  UpdateStyleEnable();
  SendStyleToMain();
}

void SettingsDlg::OnStyleTextChanged(wxCommandEvent& event) {
  SendStyleToMain();
}

void SettingsDlg::OnStyleColorChanged(wxColourPickerEvent& event) {
  SendStyleToMain();
}

void SettingsDlg::SendStyleToMain() {
  if (allow_send_style_to_main_ == false) return;
  StyleToGui(false);
  main_window_->set_settings(current_settings_);
  main_window_->set_settings(current_settings_);  // update seems to lag behind
                                                  // one setSettings, this seems
                                                  // to fix that, weird... I
                                                  // should investigate this...
}

void SettingsDlg::SendEditToMain() {
  if (allow_send_edit_to_main_ == false) {
    return;
  }
  EditToGui(false);
  main_window_->set_settings(current_settings_);
}

void UpdateCheckEnabled(wxCheckBox* check, wxWindow* slave) {
  slave->Enable(check->IsChecked());
}

void SettingsDlg::UpdateStyleEnable() {
  UpdateCheckEnabled(uiStyleUseBold, uiStyleBold);
  UpdateCheckEnabled(uiStyleUseUnderline, uiStyleUnderline);
  UpdateCheckEnabled(uiStyleUseItalic, uiStyleItalic);
  UpdateCheckEnabled(uiStyleUseSize, uiStyleSize);

  UpdateCheckEnabled(uiStyleUseForeground, uiStyleForeground);
  UpdateCheckEnabled(uiStyleUseBackground, uiStyleBackground);
  UpdateCheckEnabled(uiStyleUseTypeface, uiStyleUseTypeface);
}

void SettingsDlg::StyleToGui(bool togui) {
  const int selection = uiFontStyles->GetSelection();
  if (selection == wxNOT_FOUND) return;
  StyleLink* link =
      reinterpret_cast<StyleLink*>(uiFontStyles->GetClientData(selection));
  assert(link);
  if (link == NULL) return;

  ride::Style style = link->get(current_settings_.fonts_and_colors());

  DIALOG_DATA(style, use_bold, uiStyleUseBold, );
  DIALOG_DATA(style, bold, uiStyleBold, );
  DIALOG_DATA(style, use_italic, uiStyleUseItalic, );
  DIALOG_DATA(style, italic, uiStyleItalic, );
  DIALOG_DATA(style, use_underline, uiStyleUseUnderline, );
  DIALOG_DATA(style, underline, uiStyleUnderline, );

  DIALOG_DATA(style, use_typeface, uiStyleUseTypeface, );
  DIALOG_DATA(style, typeface, uiStyleTypeface, );

  DIALOG_DATA(style, use_font_size, uiStyleUseSize, );
  DIALOG_DATA(style, font_size, uiStyleSize, _I32);

  DIALOG_DATA(style, use_foreground, uiStyleUseForeground, );
  DIALOG_DATAX(style, foreground, uiStyleForeground);
  DIALOG_DATA(style, use_background, uiStyleUseBackground, );
  DIALOG_DATAX(style, background, uiStyleBackground);

  if (togui == false) {
    ride::FontsAndColors color = current_settings_.fonts_and_colors();
    link->set(color, style);

    current_settings_.set_allocated_fonts_and_colors(Allocate(color));
  }
}

void SettingsDlg::StyleUpdateFontDisplay() {
  int selected_typeface = uiStyleTypeface->GetSelection();
  wxString selected_facename =
      selected_typeface > 0 ? uiStyleTypeface->GetString(selected_typeface)
                            : "";
  wxFont font(wxFontInfo(12).FaceName(selected_facename));
  uiStyleExample->SetFont(font);
}

//////////////////////////////////////////////////////////////////////////

void SettingsDlg::OnIndicatorListChanged(wxCommandEvent& event) {
  allow_send_indicator_to_main_ = false;
  IndicatorToGui(true);
  allow_send_indicator_to_main_ = true;
}

void SettingsDlg::OnIndicatorCombobox(wxCommandEvent& event) {
  SendIndicatorToMain();
}

void SettingsDlg::OnIndicatorColor(wxColourPickerEvent& event) {
  SendIndicatorToMain();
}

void SettingsDlg::OnIndicatorCheckbox(wxCommandEvent& event) {
  SendIndicatorToMain();
}

void SettingsDlg::OnIndicatorText(wxCommandEvent& event) {
  SendIndicatorToMain();
}

void SettingsDlg::SendIndicatorToMain() {
  if (allow_send_indicator_to_main_ == false) {
    return;
  }
  IndicatorToGui(false);
  main_window_->set_settings(current_settings_);
}

class IndicatorLink {
 public:
  explicit IndicatorLink(const wxString& name) : name_(name) {}

  const wxString& name() const {
    assert(this);
    return name_;
  }

  virtual void IndicatorToGui(
      bool togui, ride::FontsAndColors* fonts_and_colors,
      ride::Settings& current_settings_, wxComboBox* uiIndicatorStyle,
      wxColourPickerCtrl* uiIndicatorColor, wxCheckBox* uiIndicatorUnder,
      wxTextCtrl* uiIndicatorAlpha, wxTextCtrl* uiIndicatorOutlineAlpha) = 0;

 private:
  wxString name_;
};

std::vector<IndicatorLink*> BuildIndicatorLinks() {
  std::vector<IndicatorLink*> ret;
#define DEF_INDICATOR_LINK(NAME, ID)                                       \
  struct IndicatorLink##ID : public IndicatorLink {                        \
    IndicatorLink##ID() : IndicatorLink(NAME) {}                           \
    void IndicatorToGui(bool togui, ride::FontsAndColors* col,             \
                        ride::Settings& set, wxComboBox* uiIndicatorStyle, \
                        wxColourPickerCtrl* uiIndicatorColor,              \
                        wxCheckBox* uiIndicatorUnder,                      \
                        wxTextCtrl* uiIndicatorAlpha,                      \
                        wxTextCtrl* uiIndicatorOutlineAlpha) {             \
      DIALOG_DATA(set, ID, uiIndicatorStyle, _IS);                         \
      ride::Indicator ind = col->ID();                                     \
                                                                           \
      DIALOG_DATAX(ind, foreground, uiIndicatorColor);                     \
      DIALOG_DATA(ind, under, uiIndicatorUnder, );                         \
      DIALOG_DATA(ind, alpha, uiIndicatorAlpha, _I32);                     \
      DIALOG_DATA(ind, outline_alpha, uiIndicatorOutlineAlpha, _I32);      \
                                                                           \
      if (togui == false) {                                                \
        col->set_allocated_##ID(Allocate(ind));                            \
      }                                                                    \
    }                                                                      \
  };                                                                       \
  static IndicatorLink##ID indicator_link_##ID;                            \
  ret.push_back(&indicator_link_##ID)

  DEF_INDICATOR_LINK("Error", indicator_error);
  DEF_INDICATOR_LINK("Warning", indicator_warning);
  DEF_INDICATOR_LINK("Search highlight", indicator_search_highlight);
  DEF_INDICATOR_LINK("Select highlight", indicator_select_highlight);

#undef DEF_INDICATOR_LINK
  return ret;
}

const std::vector<IndicatorLink*>& GetIndicatorLinks() {
  static std::vector<IndicatorLink*> indicator_links = BuildIndicatorLinks();
  return indicator_links;
}

void SettingsDlg::IndicatorToGui(bool togui) {
  ride::FontsAndColors fonts_and_colors = current_settings_.fonts_and_colors();

  int selected_item = uiIndicatorList->GetSelection();
  if (selected_item == -1) return;

  IndicatorLink* link = reinterpret_cast<IndicatorLink*>(
      uiIndicatorList->GetClientData(selected_item));
  assert(link);
  if (link == NULL) return;
  link->IndicatorToGui(togui, &fonts_and_colors, current_settings_,
                       uiIndicatorStyle, uiIndicatorColor, uiIndicatorUnder,
                       uiIndicatorAlpha, uiIndicatorOutlineAlpha);

  if (togui == false) {
    current_settings_.set_allocated_fonts_and_colors(
        Allocate(fonts_and_colors));
  }
}

//////////////////////////////////////////////////////////////////////////

void SettingsDlg::OnMarkerComboChanged(wxCommandEvent& event) {
  SendMarkerToMain();
}

void SettingsDlg::OnMarkerColorChanged(wxColourPickerEvent& event) {
  SendMarkerToMain();
}

void SettingsDlg::OnMarkerComboChanged(wxColourPickerEvent& event) {
  SendMarkerToMain();
}

class MarkerLink {
 public:
  explicit MarkerLink(const wxString& name) : name_(name) {}

  const wxString& name() const {
    assert(this);
    return name_;
  }

  virtual void MarkerToGui(bool togui, ride::FontsAndColors* ref,
                           ride::Settings& set, wxComboBox* sym,
                           wxColourPickerCtrl* fore,
                           wxColourPickerCtrl* back) = 0;

 private:
  wxString name_;
};

std::vector<MarkerLink*> BuildMarkerLinks() {
  std::vector<MarkerLink*> ret;
#define DEF_MARKER_LINK(NAME, ID)                                          \
  struct MarkerLink##ID : public MarkerLink {                              \
    MarkerLink##ID() : MarkerLink(NAME) {}                                 \
    void MarkerToGui(bool togui, ride::FontsAndColors* col,                \
                     ride::Settings& set, wxComboBox* sym,                 \
                     wxColourPickerCtrl* fore, wxColourPickerCtrl* back) { \
      DIALOG_DATA(set, ID, sym, _MS);                                      \
      DIALOG_DATAX((*col), ID##_foreground, fore);                         \
      DIALOG_DATAX((*col), ID##_background, back);                         \
    }                                                                      \
  };                                                                       \
  static MarkerLink##ID marker_link_##ID;                                  \
  ret.push_back(&marker_link_##ID)

  DEF_MARKER_LINK("Folder", folder);
  DEF_MARKER_LINK("Folder open", folderopen);
  DEF_MARKER_LINK("Folder open mid", folderopenmid);

  DEF_MARKER_LINK("Folder end", folderend);
  DEF_MARKER_LINK("Folder mid tail", foldermidtail);
  DEF_MARKER_LINK("Folder tail", foldertail);
  DEF_MARKER_LINK("Folder sub", foldersub);

#undef DEF_MARKER_LINK
  return ret;
}

const std::vector<MarkerLink*>& GetMarkerLinks() {
  static std::vector<MarkerLink*> marker_links = BuildMarkerLinks();
  return marker_links;
}

void SettingsDlg::OnMarkerListChanged(wxCommandEvent& event) {
  allow_send_marker_to_main_ = false;
  MarkerToGui(true);
  allow_send_marker_to_main_ = true;
}

void SettingsDlg::SendMarkerToMain() {
  if (allow_send_marker_to_main_ == false) {
    return;
  }
  MarkerToGui(false);
  main_window_->set_settings(current_settings_);
}

void SettingsDlg::MarkerToGui(bool togui) {
  ride::FontsAndColors fonts_and_colors = current_settings_.fonts_and_colors();

  int selected_item = uiMarkerList->GetSelection();
  if (selected_item == -1) return;

  MarkerLink* link =
      reinterpret_cast<MarkerLink*>(uiMarkerList->GetClientData(selected_item));
  assert(link);
  if (link == NULL) return;
  link->MarkerToGui(togui, &fonts_and_colors, current_settings_, uiMarkerSymbol,
                    uiMarkerForegroundColor, uiMarkerBackgroundColor);

  if (togui == false) {
    current_settings_.set_allocated_fonts_and_colors(
        Allocate(fonts_and_colors));
  }
}

//////////////////////////////////////////////////////////////////////////

int g_last_selected_font = 0;

SettingsDlg::SettingsDlg(wxWindow* parent, MainWindow* mainwindow)
    : ::ui::Settings(parent, wxID_ANY),
      main_window_(mainwindow),
      allow_send_edit_to_main_(false),
      allow_send_style_to_main_(false),
      theme_list_(uiThemeList, this) {
  global_settings_ = main_window_->settings();
  current_settings_ = global_settings_;
  theme_list_.Setup(uiThemeListAdd, uiThemeListRemove, uiThemeListChange,
                    uiThemeListUp, uiThemeListDown);
  EditToGui(true);
  allow_send_edit_to_main_ = true;
  allow_send_marker_to_main_ = true;
  allow_send_indicator_to_main_ = true;

  for (auto link : StyleLinks()) {
    uiFontStyles->Append(link->name(), link);
  }
  UpdateStyleFonts();
  allow_send_style_to_main_ = true;

  for (auto link : GetMarkerLinks()) {
    uiMarkerList->Append(link->name(), link);
  }

  for (auto link : GetIndicatorLinks()) {
    uiIndicatorList->Append(link->name(), link);
  }

  uiFontStyles->SetSelection(g_last_selected_font);
  uiFontStyles->EnsureVisible(g_last_selected_font);

  uiMarkerList->SetSelection(0);

  allow_send_style_to_main_ = false;
  allow_send_marker_to_main_ = false;
  allow_send_indicator_to_main_ = false;

  StyleToGui(true);
  MarkerToGui(true);
  ThemeToGui(true);
  UpdateStyleEnable();
}

void SettingsDlg::StyleSaveSelectedIndex() {
  g_last_selected_font = uiFontStyles->GetSelection();
  if (g_last_selected_font == -1) {
    g_last_selected_font = 0;
  }
}

void SettingsDlg::OnApply(wxCommandEvent& event) {
  StyleSaveSelectedIndex();
  UpdateMain();
}

void SettingsDlg::OnCancel(wxCommandEvent& event) {
  current_settings_ = global_settings_;
  main_window_->set_settings(global_settings_);
  UpdateMain();
  StyleSaveSelectedIndex();
  EndModal(wxCANCEL);
}

void SettingsDlg::OnOk(wxCommandEvent& event) {
  StyleSaveSelectedIndex();
  EditToGui(false);
  StyleToGui(false);
  MarkerToGui(false);
  main_window_->set_settings(current_settings_);
  if (false == SaveSettings(this, current_settings_)) {
    ShowError(this, "Failed to save settings", "Failed!");
  }
  EndModal(wxOK);
}

void SettingsDlg::OnCheckboxChanged(wxCommandEvent& event) {
  assert(this);
  SendEditToMain();
}

void SettingsDlg::OnComboboxChanged(wxCommandEvent& event) {
  assert(this);
  SendEditToMain();
}

void SettingsDlg::OnColorChanged(wxColourPickerEvent& event) {
  assert(this);
  SendEditToMain();
}

void SettingsDlg::OnEditChanged(wxCommandEvent& event) {
  assert(this);
  SendEditToMain();
}

//////////////////////////////////////////////////////////////////////////

void SettingsDlg::EditToGui(bool togui) {
  ride::FontsAndColors fonts_and_colors = current_settings_.fonts_and_colors();
  ride::FoldFlags foldflags = current_settings_.foldflags();

  DIALOG_DATA(current_settings_, displayeolenable, uiDisplayEOL, );
  DIALOG_DATA(current_settings_, linenumberenable, uiShowLineNumbers, );
  DIALOG_DATA(current_settings_, indentguideenable, uiIndentGuide, );

  DIALOG_DATA(current_settings_, tabwidth, uiTabWidth, _I32);
  DIALOG_DATA(current_settings_, edgecolumn, uiEdgeColumn, _I32);

  DIALOG_DATA(current_settings_, whitespace, uiViewWhitespace, _VW);
  DIALOG_DATA(current_settings_, wordwrap, uiWordwrap, _WM);
  DIALOG_DATA(current_settings_, edgestyle, uiEdgeStyle, _ES);
  DIALOG_DATA(current_settings_, auto_indentation, uiEditIndentation, _AI);

  DIALOG_DATA(current_settings_, tabindents, uiTabIndents, );
  DIALOG_DATA(current_settings_, usetabs, uiUseTabs, );
  DIALOG_DATA(current_settings_, backspaceunindents, uiBackspaceUnindents, );

  DIALOG_DATA(current_settings_, foldenable, uiAllowFolding, );

  DIALOG_DATA(foldflags, levelnumbers, uiFoldLevelNumbers, );
  DIALOG_DATA(foldflags, linebefore_expanded, uiFoldLineBeforeExpanded, );
  DIALOG_DATA(foldflags, linebefore_contracted, uiFoldLineBeforeContracted, );
  DIALOG_DATA(foldflags, lineafter_expanded, uiFoldLineAfterExpanded, );
  DIALOG_DATA(foldflags, lineafter_contracted, uiFoldLineAfterContracted, );

  DIALOG_DATAX(fonts_and_colors, edgecolor, uiEdgeColor);

  DIALOG_DATA(fonts_and_colors, use_selection_foreground,
              uiEditUseSelectionForeground, );
  DIALOG_DATAX(fonts_and_colors, selection_foreground,
               uiEditSelectionForeground);
  DIALOG_DATA(fonts_and_colors, use_selection_background,
              uiEditUseSelectionBackground, );
  DIALOG_DATAX(fonts_and_colors, selection_background,
               uiEditSelectionBackground);

  DIALOG_DATA(current_settings_, current_line_visible, uiCurrentLineVisible, );
  DIALOG_DATA(current_settings_, current_line_alpha, uiEditCurrentLineAlpha,
              _I32);
  DIALOG_DATA(current_settings_, current_line_overdraw,
              uiEditCurrentLineOverdraw, );
  DIALOG_DATAX(fonts_and_colors, selected_line, uiEditCurrentLineColor);

  DIALOG_DATAX(fonts_and_colors, fold_margin_hi, uiEditFoldHi);
  DIALOG_DATAX(fonts_and_colors, fold_margin_low, uiEditFoldLow);

  DIALOG_DATA(current_settings_, highlight_word_also_highlight_keywords,
              uiEditHighlightKeyword, );

  DIALOG_DATA(current_settings_, autocomplete_curly_braces,
              uiEditAutocompleteCurlyBraces, );
  DIALOG_DATA(current_settings_, autocomplete_parentheses,
              uiEditAutocompleteParentheses, );
  DIALOG_DATA(current_settings_, autocomplete_brackets,
              uiEditAutocompleteBrackets, );

  DIALOG_DATA(current_settings_, show_multiline_indicators,
              uiEditShowMultilineIndicators, );
  DIALOG_DATA(current_settings_, show_compiler_messages_as_annotations,
              uiEditShowCompilerAnnotations, );

  if (togui == false) {
    current_settings_.set_allocated_fonts_and_colors(
        Allocate(fonts_and_colors));
    current_settings_.set_allocated_foldflags(Allocate(foldflags));
  }
}

//////////////////////////////////////////////////////////////////////////

void SettingsDlg::ThemeToGui(bool togui) {
  theme_list_.ToGui(&current_settings_, togui);
}

void SettingsDlg::OnThemeApplySelected(wxCommandEvent& event) {
  int selected = uiThemeList->GetSelection();
  if (selected == -1) {
    return;
  }
  current_settings_.set_allocated_fonts_and_colors(
      new ride::FontsAndColors(current_settings_.themes(selected).data()));

  EditToGui(true);
  StyleToGui(true);
  IndicatorToGui(true);
  MarkerToGui(true);

  UpdateMain();
}

// import theme from file

void SettingsDlg::OnThemeExportSelected(wxCommandEvent& event) {
  int selected = uiThemeList->GetSelection();
  if (selected == -1) {
    return;
  }

  wxFileDialog saveFileDialog(this, _("Save theme"), "", "",
                              "Ride themes (*.ridetheme)|*.ridetheme",
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

  bool saved = SaveProtoBinary(current_settings_.themes(selected),
                               saveFileDialog.GetPath());
  if (saved == false) {
    ShowError(this, "Failed to save theme", "Failed to save");
  }
}

void SettingsDlg::OnThemeImport(wxCommandEvent& event) {
  wxFileDialog saveFileDialog(this, _("Open theme"), "", "",
                              "Ride themes (*.ridetheme)|*.ridetheme",
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

  ride::Theme theme;
  bool saved = LoadProtoBinary(&theme, saveFileDialog.GetPath());
  if (saved == false) {
    ShowError(this, "Failed to import theme", "Failed to import");
    return;
  }

  ride::Theme* new_theme = current_settings_.add_themes();
  new_theme->CopyFrom(theme);
  ThemeToGui(true);
}

void SettingsDlg::OnAdd(wxCommandEvent& event) {
  if (false == theme_list_.Add(&current_settings_)) {
    return;
  }
  ThemeToGui(true);
}

void SettingsDlg::OnEdit(wxCommandEvent& event) {
  if (false == theme_list_.Edit(&current_settings_)) {
    return;
  }
  ThemeToGui(true);
}

void SettingsDlg::OnRemove(wxCommandEvent& event) {
  if (false == theme_list_.Remove(&current_settings_)) {
    return;
  }
  ThemeToGui(true);
}

void SettingsDlg::OnUp(wxCommandEvent& event) {
  if (false == theme_list_.Up(&current_settings_)) {
    return;
  }
  ThemeToGui(true);
}

void SettingsDlg::OnDown(wxCommandEvent& event) {
  if (false == theme_list_.Down(&current_settings_)) {
    return;
  }
  ThemeToGui(true);
}

//////////////////////////////////////////////////////////////////////////

void SettingsDlg::OnlyAllowNumberChars(wxKeyEvent& event) {
  ::OnlyAllowNumberChars(event);
}
