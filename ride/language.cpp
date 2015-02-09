#include "ride/wx.h"
#include "ride/language.h"
#include "ride/settings.h"
#include <wx/stc/stc.h>
#include <cassert>

void SetStyle(wxStyledTextCtrl* text, int id, const ride::Style& style);

wxString b2s01(bool b);

std::vector<wxString> Split(const wxString& str, char c) {
  wxString temp = str;
  std::vector <wxString> ret;
  while (!temp.empty()) {
    wxString cur = temp.BeforeFirst(c);
    ret.push_back(cur);
    temp = temp.AfterFirst(c);
  }

  return ret;
}

//////////////////////////////////////////////////////////////////////////

Language::Language(const wxString& name, int style) : language_name(name), lexstyle(style) {
}

wxString PropTypeToString(int type) {
  switch (type)
  {
  case wxSTC_TYPE_BOOLEAN:
    return _("bool    ");
  case wxSTC_TYPE_INTEGER:
    return _("int     ");
  case wxSTC_TYPE_STRING:
    return _("wxString");
  default:
    return _("XXXXXXXX");
  }
}

void Language::style(wxStyledTextCtrl* text, const ride::Settings& settings) {
#ifdef _DEBUG
  props.clear();
  used_keywords.clear();
#endif
  text->SetLexer(lexstyle);
  dostyle(text, settings);
#ifdef _DEBUG
  const std::vector<wxString> available_props = Split(text->PropertyNames(), '\n');
  const std::vector<wxString> available_keywords = Split(text->DescribeKeyWordSets(), '\n');

  for (std::vector<wxString>::const_iterator p = available_props.begin(); p != available_props.end(); ++p) {
    const wxString prop_name = *p;
    if (props.find(prop_name) == props.end()) {
      const wxString desc = text->DescribeProperty(prop_name);
      const wxString value = text->GetProperty(prop_name);
      const wxString type = PropTypeToString(text->PropertyType(prop_name));
      wxLogWarning(_("Property for %s was not set: %s %s; // %s %s"), language_name, type, prop_name, value, desc);
    }
  }

  for (unsigned int i = 0; i < available_keywords.size(); ++i) {
    if (used_keywords.find(i) == used_keywords.end()) {
      wxLogWarning(_("Keyword %d for %s was not set: %s"), i, language_name, available_keywords[i]);
    }
  }
#endif
}

void Language::SetProp(wxStyledTextCtrl* text, const wxString& name, const wxString& value) {
  text->SetProperty(name, value);
#ifdef _DEBUG
  assert(props.find(name) == props.end());
  props.insert(name);
#endif
}

void Language::SetKeys(wxStyledTextCtrl* text, unsigned int id, const wxString& keywords) {
  text->SetKeyWords(id, keywords);
#ifdef _DEBUG
  assert(used_keywords.find(id) == used_keywords.end());
  used_keywords.insert(id);
#endif
}

Language& Language::operator()(const wxString& ext) {
  patterns.push_back(ext);
  return *this;
}

bool Language::matchPattern(const wxString& file) const {
  for (std::vector<wxString>::const_iterator p = patterns.begin(); p != patterns.end(); ++p) {
    if (file.EndsWith(*p)) {
      return true;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////

class CppLanguage : public Language {
public:
  CppLanguage() : Language(_("C++"), wxSTC_LEX_CPP){
    (*this)
      (".c")
      (".cc")
      (".cpp")
      (".cs")
      (".h")
      (".hh")
      (".hpp")
      (".hxx")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const ride::Settings& settings) {
    /*
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
    SetStyle(text, wxSTC_C_DEFAULT, Style(font));
    SetStyle(text, wxSTC_C_COMMENT, Style(font, wxColor(0, 255, 0)));
    SetStyle(text, wxSTC_C_COMMENTLINE, Style(font));
    SetStyle(text, wxSTC_C_COMMENTDOC, Style(font));
    SetStyle(text, wxSTC_C_NUMBER, Style(font));
    SetStyle(text, wxSTC_C_WORD, Style(font, wxColor(0, 0, 255)));
    SetStyle(text, wxSTC_C_STRING, Style(font));
    SetStyle(text, wxSTC_C_CHARACTER, Style(font));
    SetStyle(text, wxSTC_C_UUID, Style(font));
    SetStyle(text, wxSTC_C_PREPROCESSOR, Style(font));
    SetStyle(text, wxSTC_C_OPERATOR, Style(font));
    SetStyle(text, wxSTC_C_IDENTIFIER, Style(font));
    SetStyle(text, wxSTC_C_STRINGEOL, Style(font));
    SetStyle(text, wxSTC_C_VERBATIM, Style(font));
    SetStyle(text, wxSTC_C_REGEX, Style(font));
    SetStyle(text, wxSTC_C_COMMENTLINEDOC, Style(font));
    SetStyle(text, wxSTC_C_WORD2, Style(font));
    SetStyle(text, wxSTC_C_COMMENTDOCKEYWORD, Style(font));
    SetStyle(text, wxSTC_C_COMMENTDOCKEYWORDERROR, Style(font));
    SetStyle(text, wxSTC_C_GLOBALCLASS, Style(font));
    SetStyle(text, wxSTC_C_STRINGRAW, Style(font));
    SetStyle(text, wxSTC_C_TRIPLEVERBATIM, Style(font));
    SetStyle(text, wxSTC_C_HASHQUOTEDSTRING, Style(font));
    SetStyle(text, wxSTC_C_PREPROCESSORCOMMENT, Style(font));
    */
    SetProp(text, wxT("fold"), b2s01(settings.foldenable()));
    SetProp(text, wxT("fold.comment"), b2s01(settings.foldcomment()));
    SetProp(text, wxT("fold.compact"), b2s01(settings.foldcompact()));
    SetProp(text, wxT("fold.preprocessor"), b2s01(settings.foldpreproc()));
    SetProp(text, wxT("styling.within.preprocessor"), b2s01(settings.styling_within_preprocessor()));
    SetProp(text, wxT("lexer.cpp.allow.dollars"), b2s01(settings.lexer_cpp_allow_dollars()));
    SetProp(text, wxT("lexer.cpp.track.preprocessor"), b2s01(settings.lexer_cpp_track_preprocessor()));
    SetProp(text, wxT("lexer.cpp.update.preprocessor"), b2s01(settings.lexer_cpp_update_preprocessor()));
    SetProp(text, wxT("lexer.cpp.triplequoted.strings"), b2s01(settings.lexer_cpp_triplequoted_strings()));
    SetProp(text, wxT("lexer.cpp.hashquoted.strings"), b2s01(settings.lexer_cpp_hashquoted_strings()));
    SetProp(text, wxT("fold.cpp.syntax.based"), b2s01(settings.fold_cpp_syntax_based()));
    SetProp(text, wxT("fold.cpp.comment.multiline"), b2s01(settings.fold_cpp_comment_multiline()));
    SetProp(text, wxT("fold.cpp.comment.explicit"), b2s01(settings.fold_cpp_comment_explicit()));
    SetProp(text, wxT("fold.cpp.explicit.anywhere"), b2s01(settings.fold_cpp_explicit_anywhere()));
    SetProp(text, wxT("fold.at.else"), b2s01(settings.fold_at_else()));
    SetProp(text, wxT("fold.cpp.explicit.start"), _T("//{"));
    SetProp(text, wxT("fold.cpp.explicit.end"), _T("//}"));

    const wxString CppWordlist1 =
      "asm auto bool break case catch char class const const_cast "
      "continue default delete do double dynamic_cast else enum explicit "
      "export extern false float for friend goto if inline int long "
      "mutable namespace new operator private protected public register "
      "reinterpret_cast return short signed sizeof static static_cast "
      "struct switch template this throw true try typedef typeid "
      "typename union unsigned using virtual void volatile wchar_t "
      "while";
    const wxString CppWordlist2 =
      "file";
    const wxString CppWordlist3 =
      "a addindex addtogroup anchor arg attention author b brief bug c "
      "class code date def defgroup deprecated dontinclude e em endcode "
      "endhtmlonly endif endlatexonly endlink endverbatim enum example "
      "exception f$ f[ f] file fn hideinitializer htmlinclude "
      "htmlonly if image include ingroup internal invariant interface "
      "latexonly li line link mainpage name namespace nosubgrouping note "
      "overload p page par param post pre ref relates remarks return "
      "retval sa section see showinitializer since skip skipline struct "
      "subsection test throw todo typedef union until var verbatim "
      "verbinclude version warning weakgroup $ @ \"\" & < > # { }";
    SetKeys(text, 0, CppWordlist1);
    SetKeys(text, 1, CppWordlist2);
    SetKeys(text, 2, CppWordlist3);

    SetKeys(text, 3, ""); // global classes and typedefs
    SetKeys(text, 4, ""); // preprocessor defines
  }
} g_language_cpp;

class NullLanguage : public Language {
public:
  NullLanguage() : Language(_("NULL"), wxSTC_LEX_NULL) {
  }
  void dostyle(wxStyledTextCtrl* text, const ride::Settings& settings) {
  }
} g_language_null;

class MarkdownLanguage : public Language {
public:
  MarkdownLanguage() : Language(_("Markdown"), wxSTC_LEX_MARKDOWN) {
    (*this)
      (".md")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const ride::Settings& settings) {
    /*
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
    SetStyle(text, wxSTC_MARKDOWN_DEFAULT, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_LINE_BEGIN, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_STRONG1, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_STRONG2, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_EM1, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_EM2, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_HEADER1, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_HEADER2, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_HEADER3, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_HEADER4, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_HEADER5, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_HEADER6, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_PRECHAR, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_ULIST_ITEM, Style(font, wxColor(0, 255, 0)));
    SetStyle(text, wxSTC_MARKDOWN_OLIST_ITEM, Style(font, wxColor(0, 100, 0)));
    SetStyle(text, wxSTC_MARKDOWN_BLOCKQUOTE, Style(font, wxColor(100, 100, 100)));
    SetStyle(text, wxSTC_MARKDOWN_STRIKEOUT, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_HRULE, Style(font));
    SetStyle(text, wxSTC_MARKDOWN_LINK, Style(font, wxColor(0, 0, 255)));
    SetStyle(text, wxSTC_MARKDOWN_CODE, Style(font, wxColor(200, 0, 0)));
    SetStyle(text, wxSTC_MARKDOWN_CODE2, Style(font, wxColor(200, 0, 0)));
    SetStyle(text, wxSTC_MARKDOWN_CODEBK, Style(font));
    */
  }
} g_language_markdown;


class PropertiesLanguage : public Language {
public:
  PropertiesLanguage() : Language(_("Properties"), wxSTC_LEX_PROPERTIES) {
    (*this)
      (".props")
      (".toml") // properties are kinda like toml
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const ride::Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    /*
    SetStyle(text, wxSTC_PROPS_DEFAULT, Style(font));
    SetStyle(text, wxSTC_PROPS_COMMENT, Style(font, wxColor(0, 255, 0) ));
    SetStyle(text, wxSTC_PROPS_SECTION, Style(font, wxColor(0, 0, 255)));
    SetStyle(text, wxSTC_PROPS_ASSIGNMENT, Style(font, wxColor(0, 0, 100)));
    SetStyle(text, wxSTC_PROPS_DEFVAL, Style(font));
    SetStyle(text, wxSTC_PROPS_KEY, Style(font, wxColor(70, 70, 70)));
    */
  }
} g_language_properties;

class XmlLanguage : public Language {
public:
  XmlLanguage() : Language(_("XML"), wxSTC_LEX_XML) {
    (*this)
      (".xml")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const ride::Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    /*
    SetStyle(text, wxSTC_H_DEFAULT, Style(font));
    SetStyle(text, wxSTC_H_TAG, Style(font, wxColor(0, 0, 200)));
    SetStyle(text, wxSTC_H_TAGUNKNOWN, Style(font, wxColor(0, 0, 200)));
    SetStyle(text, wxSTC_H_ATTRIBUTE, Style(font, wxColor(100, 0, 0)));
    SetStyle(text, wxSTC_H_ATTRIBUTEUNKNOWN, Style(font, wxColor(100, 0, 0)));
    SetStyle(text, wxSTC_H_NUMBER, Style(font));
    SetStyle(text, wxSTC_H_DOUBLESTRING, Style(font));
    SetStyle(text, wxSTC_H_SINGLESTRING, Style(font));
    SetStyle(text, wxSTC_H_OTHER, Style(font));
    SetStyle(text, wxSTC_H_COMMENT, Style(font, wxColor(0, 200, 0) ));
    SetStyle(text, wxSTC_H_ENTITY, Style(font));
    SetStyle(text, wxSTC_H_TAGEND, Style(font));
    SetStyle(text, wxSTC_H_XMLSTART, Style(font));
    SetStyle(text, wxSTC_H_XMLEND, Style(font));
    SetStyle(text, wxSTC_H_SCRIPT, Style(font));
    SetStyle(text, wxSTC_H_ASP, Style(font));
    SetStyle(text, wxSTC_H_ASPAT, Style(font));
    SetStyle(text, wxSTC_H_CDATA, Style(font, wxColor(100, 100, 100)));
    SetStyle(text, wxSTC_H_QUESTION, Style(font));
    SetStyle(text, wxSTC_H_VALUE, Style(font));
    SetStyle(text, wxSTC_H_XCCOMMENT, Style(font));
    */

    SetKeys(text, 0, "html body a head b div ul li"); // HTML elements and attributes
    SetKeys(text, 1, "if while"); // JavaScript keywords
    SetKeys(text, 2, ""); // VBScript keywords
    SetKeys(text, 3, ""); // Python keywords
    SetKeys(text, 4, ""); // PHP keywords
    SetKeys(text, 5, ""); // SGML and DTD keywords
  }
} g_language_xml;

class CmakeLanguage : public Language {
public:
  CmakeLanguage() : Language(_("CMake"), wxSTC_LEX_CMAKE) {
    (*this)
      (".cmake")
      ("CMakeLists.txt")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const ride::Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    /*
    SetStyle(text, wxSTC_CMAKE_DEFAULT, Style(font));
    SetStyle(text, wxSTC_CMAKE_COMMENT, Style(font, wxColor(0,100,0) ));
    SetStyle(text, wxSTC_CMAKE_STRINGDQ, Style(font));
    SetStyle(text, wxSTC_CMAKE_STRINGLQ, Style(font));
    SetStyle(text, wxSTC_CMAKE_STRINGRQ, Style(font));
    SetStyle(text, wxSTC_CMAKE_COMMANDS, Style(font, wxColor(0, 0, 100)));
    SetStyle(text, wxSTC_CMAKE_PARAMETERS, Style(font));
    SetStyle(text, wxSTC_CMAKE_VARIABLE, Style(font, wxColor(100, 100, 100)));
    SetStyle(text, wxSTC_CMAKE_USERDEFINED, Style(font));
    SetStyle(text, wxSTC_CMAKE_WHILEDEF, Style(font));
    SetStyle(text, wxSTC_CMAKE_FOREACHDEF, Style(font));
    SetStyle(text, wxSTC_CMAKE_IFDEFINEDEF, Style(font));
    SetStyle(text, wxSTC_CMAKE_MACRODEF, Style(font));
    SetStyle(text, wxSTC_CMAKE_STRINGVAR, Style(font));
    SetStyle(text, wxSTC_CMAKE_NUMBER, Style(font));
    */

    SetKeys(text, 0, 
      "add_compile_options add_custom_command add_custom_target add_definitions add_dependencies add_executable add_library add_subdirectory "
      "add_test aux_source_directory break build_command cmake_host_system_information cmake_minimum_required cmake_policy configure_file "
      "create_test_sourcelist define_property elseif else enable_language enable_testing endforeach endfunction endif endmacro endwhile "
      "execute_process export file find_file find_library find_package find_path find_program fltk_wrap_ui foreach function get_cmake_property "
      "get_directory_property get_filename_component get_property get_source_file_property get_target_property get_test_property if include_directories "
      "include_external_msproject include_regular_expression include install link_directories list load_cache load_command macro mark_as_advanced math "
      "message option project qt_wrap_cpp qt_wrap_ui remove_definitions return separate_arguments set_directory_properties set_property set set_source_files_properties "
      "set_target_properties set_tests_properties site_name source_group string target_compile_definitions target_compile_options target_include_directories target_link_libraries "
      "try_compile try_run unset variable_watch while"
      ); // Commands
    SetKeys(text, 1, "CMAKE_ROOT CMAKE_SOURCE_DIR"); // Parameters
    SetKeys(text, 2, ""); // UserDefined
  }
} g_language_cmake;

class LuaLanguage : public Language {
public:
  LuaLanguage() : Language(_("Lua"), wxSTC_LEX_LUA) {
    (*this)
      (".lua")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const ride::Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
    
    /*
    SetStyle(text, wxSTC_LUA_DEFAULT, Style(font));
    SetStyle(text, wxSTC_LUA_COMMENT, Style(font, wxColor(0,200,0)));
    SetStyle(text, wxSTC_LUA_COMMENTLINE, Style(font, wxColor(0, 200, 0)));
    SetStyle(text, wxSTC_LUA_COMMENTDOC, Style(font, wxColor(0, 200, 0)));
    SetStyle(text, wxSTC_LUA_NUMBER, Style(font, wxColor(0, 0, 100)));
    SetStyle(text, wxSTC_LUA_WORD, Style(font, wxColor(0, 0, 200)));
    SetStyle(text, wxSTC_LUA_STRING, Style(font));
    SetStyle(text, wxSTC_LUA_CHARACTER, Style(font));
    SetStyle(text, wxSTC_LUA_LITERALSTRING, Style(font));
    SetStyle(text, wxSTC_LUA_PREPROCESSOR, Style(font, wxColor(0, 0, 200)));
    SetStyle(text, wxSTC_LUA_OPERATOR, Style(font, wxColor(0, 0, 100)));
    SetStyle(text, wxSTC_LUA_IDENTIFIER, Style(font));
    SetStyle(text, wxSTC_LUA_STRINGEOL, Style(font));
    SetStyle(text, wxSTC_LUA_WORD2, Style(font));
    SetStyle(text, wxSTC_LUA_WORD3, Style(font));
    SetStyle(text, wxSTC_LUA_WORD4, Style(font));
    SetStyle(text, wxSTC_LUA_WORD5, Style(font));
    SetStyle(text, wxSTC_LUA_WORD6, Style(font));
    SetStyle(text, wxSTC_LUA_WORD7, Style(font));
    SetStyle(text, wxSTC_LUA_WORD8, Style(font));
    SetStyle(text, wxSTC_LUA_LABEL, Style(font));
    */

    SetKeys(text, 0,
      "and break do else elseif "
      "end false for function if "
      "in local nil not or "
      "repeat return then true until while"
      );  // Keywords
    SetKeys(text, 1, "");  // Basic functions
    SetKeys(text, 2, "");  // String, (table) & math functions
    SetKeys(text, 3, "");  // (coroutines), I/O & system facilities
    SetKeys(text, 4, "");  // user1
    SetKeys(text, 5, "");  // user2
    SetKeys(text, 6, "");  // user3
    SetKeys(text, 7, "");  // user4
  }
} g_language_lua;

class YamlLanguage : public Language {
public:
  YamlLanguage() : Language(_("YAML"), wxSTC_LEX_YAML) {
    (*this)
      (".yaml")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const ride::Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    /*
    SetStyle(text, wxSTC_YAML_DEFAULT, Style(font));
    SetStyle(text, wxSTC_YAML_COMMENT, Style(font, wxColor(0,200,0)));
    SetStyle(text, wxSTC_YAML_IDENTIFIER, Style(font, wxColor(0, 0, 100)));
    SetStyle(text, wxSTC_YAML_KEYWORD, Style(font, wxColor(0, 0, 255)));
    SetStyle(text, wxSTC_YAML_NUMBER, Style(font));
    SetStyle(text, wxSTC_YAML_REFERENCE, Style(font));
    SetStyle(text, wxSTC_YAML_DOCUMENT, Style(font));
    SetStyle(text, wxSTC_YAML_TEXT, Style(font));
    SetStyle(text, wxSTC_YAML_ERROR, Style(font, wxColor(200, 0, 0)));
    SetStyle(text, wxSTC_YAML_OPERATOR, Style(font));
    */

    SetKeys(text, 0, ""); // keywords
  }
} g_language_yaml;

std::vector<Language*> BuildLanguageList() {
  std::vector<Language*> ret;
  ret.push_back(&g_language_cpp);
  ret.push_back(&g_language_markdown);
  ret.push_back(&g_language_properties);
  ret.push_back(&g_language_xml);
  ret.push_back(&g_language_cmake);
  ret.push_back(&g_language_lua);
  ret.push_back(&g_language_yaml);
  return ret;
}

const std::vector<Language*> LanguageList = BuildLanguageList();

Language* DetermineLanguage(const wxString& filepath) {
  for (std::vector<Language*>::const_iterator l = LanguageList.begin(); l != LanguageList.end(); ++l) {
    Language* lang = *l;
    if (lang->matchPattern(filepath)) {
      return lang;
    }
  }
  return &g_language_null;
}
