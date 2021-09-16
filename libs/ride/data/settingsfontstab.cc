// Copyright (2015) Gustav

#include "ride/settingsfontstab.h"

#include <vector>
#include <algorithm>

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/settings.h"
#include "ride/wxutils.h"

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

SettingsFontsTab::SettingsFontsTab(wxWindow* parent, SettingsCommon* common)
    : ui::SettingsFontsPanel(parent), common_(common) {
  allow_send_style_to_main_ = false;
  for (auto link : StyleLinks()) {
    uiFontStyles->Append(link->name(), link);
  }
  UpdateStyleFonts();

  StyleToGui(true);
  allow_send_style_to_main_ = true;
}

void SettingsFontsTab::OnOnlyFixedSysChanged(wxCommandEvent& event) {
  UpdateStyleFonts();
}

void SettingsFontsTab::UpdateStyleFonts() {
  const std::vector<wxString> all_fonts =
      ListFonts(uiStyleOnlyFixedSize->GetValue());

  uiStyleTypeface->Clear();
  for (auto name : all_fonts) {
    uiStyleTypeface->AppendString(name);
  }
}

void SettingsFontsTab::OnSelectedStyleChanged(wxCommandEvent& event) {
  allow_send_style_to_main_ = false;
  StyleToGui(true);
  UpdateStyleEnable();
  allow_send_style_to_main_ = true;
}

void SettingsFontsTab::OnStyleFontChanged(wxCommandEvent& event) {
  StyleUpdateFontDisplay();
  SendStyleToMain();
}

void SettingsFontsTab::OnStyleCheckChanged(wxCommandEvent& event) {
  UpdateStyleEnable();
  SendStyleToMain();
}

void SettingsFontsTab::OnStyleTextChanged(wxCommandEvent& event) {
  SendStyleToMain();
}

void SettingsFontsTab::OnStyleColorChanged(wxColourPickerEvent& event) {
  SendStyleToMain();
}

void SettingsFontsTab::SendStyleToMain() {
  if (allow_send_style_to_main_ == false) return;
  StyleToGui(false);
  common_->UpdateMain();
}

void UpdateCheckEnabled(wxCheckBox* check, wxWindow* slave) {
  slave->Enable(check->IsChecked());
}

void SettingsFontsTab::UpdateStyleEnable() {
  UpdateCheckEnabled(uiStyleUseBold, uiStyleBold);
  UpdateCheckEnabled(uiStyleUseUnderline, uiStyleUnderline);
  UpdateCheckEnabled(uiStyleUseItalic, uiStyleItalic);
  UpdateCheckEnabled(uiStyleUseSize, uiStyleSize);

  UpdateCheckEnabled(uiStyleUseForeground, uiStyleForeground);
  UpdateCheckEnabled(uiStyleUseBackground, uiStyleBackground);
  UpdateCheckEnabled(uiStyleUseTypeface, uiStyleUseTypeface);
}

void SettingsFontsTab::StyleToGui(bool togui) {
  const int selection = uiFontStyles->GetSelection();
  if (selection == wxNOT_FOUND) return;
  StyleLink* link =
      reinterpret_cast<StyleLink*>(uiFontStyles->GetClientData(selection));
  assert(link);
  if (link == nullptr) return;

  ride::Style style = link->get(common_->current_settings().fonts_and_colors());

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
    ride::FontsAndColors color = common_->current_settings().fonts_and_colors();
    link->set(color, style);

    common_->current_settings_mod()->set_allocated_fonts_and_colors(
        Allocate(color));
  }
}

void SettingsFontsTab::StyleUpdateFontDisplay() {
  int selected_typeface = uiStyleTypeface->GetSelection();
  wxString selected_facename =
      selected_typeface > 0 ? uiStyleTypeface->GetString(selected_typeface)
                            : "";
  wxFont font(wxFontInfo(12).FaceName(selected_facename));
  uiStyleExample->SetFont(font);
}

void SettingsFontsTab::OnlyAllowNumberChars(wxKeyEvent& event) {
  ::OnlyAllowNumberChars(event);
}
