#include "ride/language.h"
#include "ride/settings.h"
#include <wx/stc/stc.h>

void SetStyle(wxStyledTextCtrl* text, int id, const Style& style);
wxString b2s01(bool b);

Language::Language(int style) : lexstyle(style) {
}

void Language::style(wxStyledTextCtrl* text, const Settings& settings) {
#ifdef _DEBUG
  props.reserve(0);
#endif
  text->SetLexer(lexstyle);
  dostyle(text, settings);
#ifdef _DEBUG
  const wxString available_props = text->PropertyNames();
  const wxString available_keywords = text->DescribeKeyWordSets();
#endif
}

void Language::SetProp(wxStyledTextCtrl* text, const wxString& name, const wxString& value) {
  text->SetProperty(name, value);
#ifdef _DEBUG
  props.push_back(name);
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
  CppLanguage() : Language(wxSTC_LEX_CPP){
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
  NullLanguage() : Language(wxSTC_LEX_NULL) {
  }
  void dostyle(wxStyledTextCtrl* text, const Settings& settings) {
  }
} g_language_null;

std::vector<Language*> BuildLanguageList() {
  std::vector<Language*> ret;
  ret.push_back(&g_language_cpp);
  return ret;
}

const std::vector<Language*> LanguageList = BuildLanguageList();

Language* DetermineLanguage(const wxString& filepath) {
  for (std::vector<Language*>::const_iterator l = LanguageList.begin(); l != LanguageList.end(); ++l) {
    return *l;
  }
  return &g_language_null;
}
