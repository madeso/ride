#include "ride/language.h"
#include "ride/settings.h"
#include <wx/stc/stc.h>
#include <cassert>

void SetStyle(wxStyledTextCtrl* text, int id, const Style& style);
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

void Language::style(wxStyledTextCtrl* text, const Settings& settings) {
#ifdef _DEBUG
  props.clear();
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

  int dog = 3;
#endif
}

void Language::SetProp(wxStyledTextCtrl* text, const wxString& name, const wxString& value) {
  text->SetProperty(name, value);
#ifdef _DEBUG
  assert(props.find(name) == props.end());
  props.insert(name);
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
  void dostyle(wxStyledTextCtrl* text, const Settings& settings) {
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
    SetProp(text, wxT("fold"), b2s01(settings.foldEnable));
    SetProp(text, wxT("fold.comment"), b2s01(settings.foldComment));
    SetProp(text, wxT("fold.compact"), b2s01(settings.foldCompact));
    SetProp(text, wxT("fold.preprocessor"), b2s01(settings.foldPreproc));
    SetProp(text, wxT("styling.within.preprocessor"), b2s01(settings.styling_within_preprocessor));
    SetProp(text, wxT("lexer.cpp.allow.dollars"), b2s01(settings.lexer_cpp_allow_dollars));
    SetProp(text, wxT("lexer.cpp.track.preprocessor"), b2s01(settings.lexer_cpp_track_preprocessor));
    SetProp(text, wxT("lexer.cpp.update.preprocessor"), b2s01(settings.lexer_cpp_update_preprocessor));
    SetProp(text, wxT("lexer.cpp.triplequoted.strings"), b2s01(settings.lexer_cpp_triplequoted_strings));
    SetProp(text, wxT("lexer.cpp.hashquoted.strings"), b2s01(settings.lexer_cpp_hashquoted_strings));
    SetProp(text, wxT("fold.cpp.syntax.based"), b2s01(settings.fold_cpp_syntax_based));
    SetProp(text, wxT("fold.cpp.comment.multiline"), b2s01(settings.fold_cpp_comment_multiline));
    SetProp(text, wxT("fold.cpp.comment.explicit"), b2s01(settings.fold_cpp_comment_explicit));
    SetProp(text, wxT("fold.cpp.explicit.anywhere"), b2s01(settings.fold_cpp_explicit_anywhere));
    SetProp(text, wxT("fold.at.else"), b2s01(settings.fold_at_else));
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
    text->SetKeyWords(0, CppWordlist1);
    text->SetKeyWords(1, CppWordlist2);
    text->SetKeyWords(2, CppWordlist3);
  }
} g_language_cpp;

class NullLanguage : public Language {
public:
  NullLanguage() : Language(_("NULL"), wxSTC_LEX_NULL) {
  }
  void dostyle(wxStyledTextCtrl* text, const Settings& settings) {
  }
} g_language_null;

#define wxSTC_MARKDOWN_DEFAULT 0
#define wxSTC_MARKDOWN_LINE_BEGIN 1
#define wxSTC_MARKDOWN_STRONG1 2
#define wxSTC_MARKDOWN_STRONG2 3
#define wxSTC_MARKDOWN_EM1 4
#define wxSTC_MARKDOWN_EM2 5
#define wxSTC_MARKDOWN_HEADER1 6
#define wxSTC_MARKDOWN_HEADER2 7
#define wxSTC_MARKDOWN_HEADER3 8
#define wxSTC_MARKDOWN_HEADER4 9
#define wxSTC_MARKDOWN_HEADER5 10
#define wxSTC_MARKDOWN_HEADER6 11
#define wxSTC_MARKDOWN_PRECHAR 12
#define wxSTC_MARKDOWN_ULIST_ITEM 13
#define wxSTC_MARKDOWN_OLIST_ITEM 14
#define wxSTC_MARKDOWN_BLOCKQUOTE 15
#define wxSTC_MARKDOWN_STRIKEOUT 16
#define wxSTC_MARKDOWN_HRULE 17
#define wxSTC_MARKDOWN_LINK 18
#define wxSTC_MARKDOWN_CODE 19
#define wxSTC_MARKDOWN_CODE2 20
#define wxSTC_MARKDOWN_CODEBK 21

class MarkdownLanguage : public Language {
public:
  MarkdownLanguage() : Language(_("Markdown"), wxSTC_LEX_MARKDOWN) {
    (*this)
      (".md")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const Settings& settings) {
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
  }
} g_language_markdown;


#define wxSTC_PROPS_DEFAULT 0
#define wxSTC_PROPS_COMMENT 1
#define wxSTC_PROPS_SECTION 2
#define wxSTC_PROPS_ASSIGNMENT 3
#define wxSTC_PROPS_DEFVAL 4
#define wxSTC_PROPS_KEY 5


class PropertiesLanguage : public Language {
public:
  PropertiesLanguage() : Language(_("Properties"), wxSTC_LEX_PROPERTIES) {
    (*this)
      (".props")
      (".toml") // properties are kinda like toml
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    SetStyle(text, wxSTC_PROPS_DEFAULT, Style(font));
    SetStyle(text, wxSTC_PROPS_COMMENT, Style(font, wxColor(0, 255, 0) ));
    SetStyle(text, wxSTC_PROPS_SECTION, Style(font, wxColor(0, 0, 255)));
    SetStyle(text, wxSTC_PROPS_ASSIGNMENT, Style(font, wxColor(0, 0, 100)));
    SetStyle(text, wxSTC_PROPS_DEFVAL, Style(font));
    SetStyle(text, wxSTC_PROPS_KEY, Style(font, wxColor(70, 70, 70)));
  }
} g_language_properties;

// #define  5

class XmlLanguage : public Language {
public:
  XmlLanguage() : Language(_("XML"), wxSTC_LEX_XML) {
    (*this)
      (".xml")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

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
  void dostyle(wxStyledTextCtrl* text, const Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

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
  }
} g_language_cmake;

class LuaLanguage : public Language {
public:
  LuaLanguage() : Language(_("Lua"), wxSTC_LEX_LUA) {
    (*this)
      (".lua")
      ;
  }
  void dostyle(wxStyledTextCtrl* text, const Settings& settings) {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));
    
    SetStyle(text, wxSTC_LUA_DEFAULT, Style(font));
    SetStyle(text, wxSTC_LUA_COMMENT, Style(font, wxColor(0,200,0)));
    SetStyle(text, wxSTC_LUA_COMMENTLINE, Style(font, wxColor(0, 200, 0)));
    SetStyle(text, wxSTC_LUA_COMMENTDOC, Style(font, wxColor(0, 200, 0)));
    SetStyle(text, wxSTC_LUA_NUMBER, Style(font, wxColor(0, 0, 100)));
    SetStyle(text, wxSTC_LUA_WORD, Style(font));
    SetStyle(text, wxSTC_LUA_STRING, Style(font));
    SetStyle(text, wxSTC_LUA_CHARACTER, Style(font));
    SetStyle(text, wxSTC_LUA_LITERALSTRING, Style(font));
    SetStyle(text, wxSTC_LUA_PREPROCESSOR, Style(font, wxColor(0, 0, 200)));
    SetStyle(text, wxSTC_LUA_OPERATOR, Style(font, wxColor(0, 0, 100)));
    SetStyle(text, wxSTC_LUA_IDENTIFIER, Style(font));
    SetStyle(text, wxSTC_LUA_STRINGEOL, Style(font));
    SetStyle(text, wxSTC_LUA_WORD2, Style(font, wxColor(0, 0, 200)));
    SetStyle(text, wxSTC_LUA_WORD3, Style(font));
    SetStyle(text, wxSTC_LUA_WORD4, Style(font));
    SetStyle(text, wxSTC_LUA_WORD5, Style(font));
    SetStyle(text, wxSTC_LUA_WORD6, Style(font));
    SetStyle(text, wxSTC_LUA_WORD7, Style(font));
    SetStyle(text, wxSTC_LUA_WORD8, Style(font));
    SetStyle(text, wxSTC_LUA_LABEL, Style(font));
  }
} g_language_lua;

std::vector<Language*> BuildLanguageList() {
  std::vector<Language*> ret;
  ret.push_back(&g_language_cpp);
  ret.push_back(&g_language_markdown);
  ret.push_back(&g_language_properties);
  ret.push_back(&g_language_xml);
  ret.push_back(&g_language_cmake);
  ret.push_back(&g_language_lua);
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
