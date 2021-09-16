// Copyright (2015) Gustav

#include "ride/language.h"

#include <ride/wx.h>
#include <wx/stc/stc.h>

#include <cassert>

#include "ride_compiler_settings.h"  // NOLINT this is how we include it

#include "ride/settings.h"
#include "ride/wxutils.h"
#include "ride/stcutils.h"

wxString b2s01(bool b);

//////////////////////////////////////////////////////////////////////////

Language::Language(const wxString& name, int style)
    : language_name_(name), lexer_style_(style) {}

const wxString& Language::language_name() const { return language_name_; }

wxString PropTypeToString(int type) {
  switch (type) {
    case wxSTC_TYPE_BOOLEAN:
      return _("bool    ");
    case wxSTC_TYPE_INTEGER:
      return _("int     ");
    case wxSTC_TYPE_STRING:
      return _("wxString");
    default:
      assert(0 && "Unknown property type");
      return _("XXXXXXXX");
  }
}

bool Language::IsKeyword(const wxString word) const {
  for (const auto& keyword : keywords_) {
    if (keyword == word) {
      return true;
    }
  }

  return false;
}

class KeywordBuilder {
 public:
  KeywordBuilder& operator<<(const wxString keyword) {
    keywords_.push_back(keyword);
    return *this;
  }

  const std::vector<wxString>& ToVector() const { return keywords_; }
  wxString ToString() const {
    wxString ret;
    bool first = true;
    for (const auto& keyword : keywords_) {
      if (first)
        ret = keyword;
      else
        ret += " " + keyword;
      first = false;
    }
    return ret;
  }

  operator const wxString() const { return ToString(); }

 private:
  std::vector<wxString> keywords_;
};

void Language::StyleDocument(wxStyledTextCtrl* text,
                             const ride::Settings& settings) {
#ifdef _DEBUG
  used_properties_.clear();
  used_keywords_.clear();
#endif
  text->SetLexer(lexer_style_);
  DoStyleDocument(text, settings);
#ifdef _DEBUG
  const std::vector<wxString> available_props =
      Split(text->PropertyNames(), '\n');
  const std::vector<wxString> available_keywords =
      Split(text->DescribeKeyWordSets(), '\n');

  for (std::vector<wxString>::const_iterator p = available_props.begin();
       p != available_props.end(); ++p) {
    const wxString prop_name = *p;
    if (used_properties_.find(prop_name) == used_properties_.end()) {
      const wxString desc = text->DescribeProperty(prop_name);
      const wxString value = text->GetProperty(prop_name);
      const wxString type = PropTypeToString(text->PropertyType(prop_name));
      wxLogWarning(_("Property for %s was not set: %s %s; // %s %s"),
                   language_name_, type, prop_name, value, desc);
    }
  }

  for (unsigned int i = 0; i < available_keywords.size(); ++i) {
    if (used_keywords_.find(i) == used_keywords_.end()) {
      wxLogWarning(_("Keyword %d for %s was not set: %s"), i, language_name_,
                   available_keywords[i]);
    }
  }
#endif
}

void Language::SetProperty(wxStyledTextCtrl* text, const wxString& name,
                           const wxString& value) {
  text->SetProperty(name, value);
#ifdef _DEBUG
  assert(used_properties_.find(name) == used_properties_.end());
  used_properties_.insert(name);
#endif
}

void Language::SetKeys(wxStyledTextCtrl* text, unsigned int id,
                       const wxString& keywords) {
  text->SetKeyWords(id, keywords);
#ifdef _DEBUG
  assert(used_keywords_.find(id) == used_keywords_.end());
  used_keywords_.insert(id);
#endif
}

void Language::AddExtension(const wxString& ext) {
  file_patterns_.push_back(ext);
}

bool Language::MatchPattern(const wxString& file) const {
  for (const auto& elem : file_patterns_) {
    if (file.EndsWith(elem)) {
      return true;
    }
  }
  return false;
}

wxString Language::GetFilePattern() const {
  wxString patterns;

  for (const auto& elem : file_patterns_) {
    // if the pattern starts with a dot, assume it's a extension and we
    // need a star, if not we need to match the whole file
    const wxString patt = elem.StartsWith(".") ? "*" + elem : elem;

    // build a *.txt;*.pdf list
    if (patterns.IsEmpty()) {
      patterns = patt;
    } else {
      patterns += ";" + patt;
    }
  }

  return language_name_ + " files (" + patterns + ")|" + patterns;
}

const std::vector<wxString>& Language::GetKeywords() const { return keywords_; }

//////////////////////////////////////////////////////////////////////////

void CommonDocumentStyle(wxStyledTextCtrl* text,
                         const ride::Settings& settings) {
  SetStyle(text, wxSTC_C_STRINGEOL,
           settings.fonts_and_colors().style_string_eol());
  SetStyle(text, wxSTC_C_VERBATIM,
           settings.fonts_and_colors().style_verbatim());
  SetStyle(text, wxSTC_C_REGEX, settings.fonts_and_colors().style_regex());
  SetStyle(text, wxSTC_C_COMMENTLINEDOC,
           settings.fonts_and_colors().style_commentlinedoc());
  SetStyle(text, wxSTC_C_WORD2,
           settings.fonts_and_colors().style_keyword_types());
  SetStyle(text, wxSTC_C_COMMENTDOCKEYWORD,
           settings.fonts_and_colors().style_commentdockeyword());
  SetStyle(text, wxSTC_C_COMMENTDOCKEYWORDERROR,
           settings.fonts_and_colors().style_commentdockeyworderror());
  SetStyle(text, wxSTC_C_GLOBALCLASS,
           settings.fonts_and_colors().style_globalclass());
  SetStyle(text, wxSTC_C_STRINGRAW,
           settings.fonts_and_colors().style_stringraw());
  SetStyle(text, wxSTC_C_TRIPLEVERBATIM,
           settings.fonts_and_colors().style_tripleverbatim());
  SetStyle(text, wxSTC_C_HASHQUOTEDSTRING,
           settings.fonts_and_colors().style_hashquotedstring());
  SetStyle(text, wxSTC_C_PREPROCESSORCOMMENT,
           settings.fonts_and_colors().style_preprocessorcomment());
}

void DefaultStyleDocument(wxStyledTextCtrl* text,
                          const ride::Settings& settings, Language* language) {
  SetStyle(text, wxSTC_C_DEFAULT, settings.fonts_and_colors().default_style(),
           true);
  SetStyle(text, wxSTC_C_COMMENT, settings.fonts_and_colors().style_comment());
  SetStyle(text, wxSTC_C_COMMENTLINE,
           settings.fonts_and_colors().style_commentline());
  SetStyle(text, wxSTC_C_COMMENTDOC,
           settings.fonts_and_colors().style_commentdoc());
  SetStyle(text, wxSTC_C_NUMBER, settings.fonts_and_colors().style_number());
  SetStyle(text, wxSTC_C_WORD, settings.fonts_and_colors().style_keyword());
  SetStyle(text, wxSTC_C_STRING, settings.fonts_and_colors().style_string());
  SetStyle(text, wxSTC_C_CHARACTER,
           settings.fonts_and_colors().style_character());
  SetStyle(text, wxSTC_C_UUID, settings.fonts_and_colors().style_uuid());
  SetStyle(text, wxSTC_C_PREPROCESSOR,
           settings.fonts_and_colors().style_preprocessor());
  SetStyle(text, wxSTC_C_OPERATOR,
           settings.fonts_and_colors().style_operator());
  SetStyle(text, wxSTC_C_IDENTIFIER,
           settings.fonts_and_colors().style_identifier());
  CommonDocumentStyle(text, settings);

  language->SetProperty(text, wxT("fold"), b2s01(settings.foldenable()));
  language->SetProperty(text, wxT("fold.comment"),
                        b2s01(settings.foldcomment()));
  language->SetProperty(text, wxT("fold.compact"),
                        b2s01(settings.foldcompact()));
  language->SetProperty(text, wxT("fold.preprocessor"),
                        b2s01(settings.foldpreproc()));
  language->SetProperty(text, wxT("styling.within.preprocessor"),
                        b2s01(settings.styling_within_preprocessor()));
  language->SetProperty(text, wxT("lexer.cpp.allow.dollars"),
                        b2s01(settings.lexer_cpp_allow_dollars()));
  language->SetProperty(text, wxT("lexer.cpp.track.preprocessor"),
                        b2s01(settings.lexer_cpp_track_preprocessor()));
  language->SetProperty(text, wxT("lexer.cpp.update.preprocessor"),
                        b2s01(settings.lexer_cpp_update_preprocessor()));
  language->SetProperty(text, wxT("lexer.cpp.triplequoted.strings"),
                        b2s01(settings.lexer_cpp_triplequoted_strings()));
  language->SetProperty(text, wxT("lexer.cpp.hashquoted.strings"),
                        b2s01(settings.lexer_cpp_hashquoted_strings()));
  language->SetProperty(text, wxT("fold.cpp.syntax.based"),
                        b2s01(settings.fold_cpp_syntax_based()));
  language->SetProperty(text, wxT("fold.cpp.comment.multiline"),
                        b2s01(settings.fold_cpp_comment_multiline()));
  language->SetProperty(text, wxT("fold.cpp.comment.explicit"),
                        b2s01(settings.fold_cpp_comment_explicit()));
  language->SetProperty(text, wxT("fold.cpp.explicit.anywhere"),
                        b2s01(settings.fold_cpp_explicit_anywhere()));
  language->SetProperty(text, wxT("fold.at.else"),
                        b2s01(settings.fold_at_else()));
  language->SetProperty(text, wxT("fold.cpp.explicit.start"), _T("//{"));
  language->SetProperty(text, wxT("fold.cpp.explicit.end"), _T("//}"));
}

class CppLanguage : public Language {
 public:
  CppLanguage() : Language(_("C++"), wxSTC_LEX_CPP) {
    AddExtension(".c");
    AddExtension(".cc");
    AddExtension(".cpp");
    AddExtension(".cs");
    AddExtension(".h");
    AddExtension(".hh");
    AddExtension(".hpp");
    AddExtension(".hxx");
    const KeywordBuilder temp = KeywordBuilder() << "asm"
                                                 << "auto"
                                                 << "bool"
                                                 << "break"
                                                 << "case"
                                                 << "catch"
                                                 << "char"
                                                 << "class"
                                                 << "const"
                                                 << "const_cast"
                                                 << "continue"
                                                 << "default"
                                                 << "delete"
                                                 << "do"
                                                 << "double"
                                                 << "dynamistyle_cast"
                                                 << "else"
                                                 << "enum"
                                                 << "explicit"
                                                 << "export"
                                                 << "extern"
                                                 << "false"
                                                 << "float"
                                                 << "for"
                                                 << "friend"
                                                 << "goto"
                                                 << "if"
                                                 << "inline"
                                                 << "int"
                                                 << "long"
                                                 << "mutable"
                                                 << "namespace"
                                                 << "new"
                                                 << "operator"
                                                 << "private"
                                                 << "protected"
                                                 << "public"
                                                 << "register"
                                                 << "reinterpret_cast"
                                                 << "return"
                                                 << "short"
                                                 << "signed"
                                                 << "sizeof"
                                                 << "static"
                                                 << "statistyle_cast"
                                                 << "struct"
                                                 << "switch"
                                                 << "template"
                                                 << "this"
                                                 << "throw"
                                                 << "true"
                                                 << "try"
                                                 << "typedef"
                                                 << "typeid"
                                                 << "typename"
                                                 << "union"
                                                 << "unsigned"
                                                 << "using"
                                                 << "virtual"
                                                 << "void"
                                                 << "volatile"
                                                 << "wchar_t"
                                                 << "while";
    keywords_ = temp.ToVector();
    primary_keywords_ = temp.ToString();
  }

  wxString primary_keywords_;
  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
    DefaultStyleDocument(text, settings, this);

    const wxString CppWordlist2 = "file";
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
    SetKeys(text, 0, primary_keywords_);  // primary
    SetKeys(text, 1, CppWordlist2);       // secondary
    SetKeys(text, 2, CppWordlist3);       // documentation

    SetKeys(text, 3, "");  // global classes and typedefs
    SetKeys(text, 4, "");  // preprocessor defines
  }
};

#ifdef USE_CPP_AS_RUST
#ifdef wxSTC_LEX_RUST
#undef wxSTC_LEX_RUST
#endif
#define wxSTC_LEX_RUST wxSTC_LEX_CPP
#endif

class RustLanguage : public Language {
 public:
  RustLanguage() : Language(_("Rust"), wxSTC_LEX_RUST) {
    AddExtension(".rs");
    const KeywordBuilder temp = KeywordBuilder() << "abstract"
                                                 << "alignof"
                                                 << "as"
                                                 << "become"
                                                 << "box"
                                                 << "break"
                                                 << "const"
                                                 << "continue"
                                                 << "crate"
                                                 << "do"
                                                 << "else"
                                                 << "enum"
                                                 << "extern"
                                                 << "false"
                                                 << "final"
                                                 << "fn"
                                                 << "for"
                                                 << "if"
                                                 << "impl"
                                                 << "in"
                                                 << "let"
                                                 << "loop"
                                                 << "macro"
                                                 << "match"
                                                 << "mod"
                                                 << "move"
                                                 << "mut"
                                                 << "offsetof"
                                                 << "override"
                                                 << "priv"
                                                 << "pub"
                                                 << "pure"
                                                 << "ref"
                                                 << "return"
                                                 << "sizeof"
                                                 << "static"
                                                 << "self"
                                                 << "struct"
                                                 << "super"
                                                 << "true"
                                                 << "trait"
                                                 << "type"
                                                 << "typeof"
                                                 << "unsafe"
                                                 << "unsized"
                                                 << "use"
                                                 << "virtual"
                                                 << "where"
                                                 << "while"
                                                 << "yield";
    keywords_ = temp.ToVector();
    primary_keywords_ = temp.ToString();
  }
  wxString primary_keywords_;

  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
#ifndef USE_CPP_AS_RUST
    SetStyle(text, wxSTC_RUST_DEFAULT,
             settings.fonts_and_colors().default_style(), true);
    SetStyle(text, wxSTC_RUST_COMMENTBLOCK,
             settings.fonts_and_colors().style_comment());
    SetStyle(text, wxSTC_RUST_COMMENTLINE,
             settings.fonts_and_colors().style_commentline());
    SetStyle(text, wxSTC_RUST_COMMENTBLOCKDOC,
             settings.fonts_and_colors().style_commentdoc());
    SetStyle(text, wxSTC_RUST_COMMENTLINEDOC,
             settings.fonts_and_colors().style_commentdoc());
    SetStyle(text, wxSTC_RUST_NUMBER,
             settings.fonts_and_colors().style_number());
    SetStyle(text, wxSTC_RUST_WORD,
             settings.fonts_and_colors().style_keyword());
    /*
#define wxSTC_RUST_WORD2 7
#define wxSTC_RUST_WORD3 8
#define wxSTC_RUST_WORD4 9
#define wxSTC_RUST_WORD5 10
#define wxSTC_RUST_WORD6 11
#define wxSTC_RUST_WORD7 12
    */
    SetStyle(text, wxSTC_RUST_CHARACTER,
             settings.fonts_and_colors().style_character());
    SetStyle(text, wxSTC_RUST_STRING,
             settings.fonts_and_colors().style_string());
    SetStyle(text, wxSTC_RUST_STRINGR,
             settings.fonts_and_colors().style_verbatim());

    SetStyle(text, wxSTC_RUST_OPERATOR,
             settings.fonts_and_colors().style_operator());
    SetStyle(text, wxSTC_RUST_IDENTIFIER,
             settings.fonts_and_colors().style_identifier());
    SetStyle(text, wxSTC_RUST_LIFETIME,
             settings.fonts_and_colors().rust_lifetime());
    SetStyle(text, wxSTC_RUST_MACRO,
             settings.fonts_and_colors().style_preprocessor());
    SetStyle(text, wxSTC_RUST_LEXERROR,
             settings.fonts_and_colors().rust_lex_error());

    CommonDocumentStyle(text, settings);
    SetStyle(text, wxSTC_C_UUID, settings.fonts_and_colors().style_uuid());

    SetProperty(text, wxT("fold"), b2s01(settings.foldenable()));
    SetProperty(text, wxT("fold.comment"), b2s01(settings.foldcomment()));
    SetProperty(text, wxT("fold.compact"), b2s01(settings.foldcompact()));
    SetProperty(text, wxT("fold.at.else"), b2s01(settings.fold_at_else()));
    SetProperty(text, wxT("fold.rust.syntax.based"),
                b2s01(settings.fold_cpp_syntax_based()));
    SetProperty(text, wxT("fold.rust.comment.multiline"),
                b2s01(settings.fold_cpp_comment_multiline()));
    SetProperty(text, wxT("fold.rust.comment.explicit"),
                b2s01(settings.fold_cpp_comment_explicit()));
    SetProperty(text, wxT("fold.rust.explicit.anywhere"),
                b2s01(settings.fold_cpp_explicit_anywhere()));
    SetProperty(text, wxT("fold.rust.explicit.start"), _T("//{"));
    SetProperty(text, wxT("fold.rust.explicit.end"), _T("//}"));
    SetProperty(text, wxT("lexer.rust.fold.at.else"),
                b2s01(settings.fold_at_else()));
#else
    DefaultStyleDocument(text, settings, this);
#endif
    // end of setstyle/setprop fallback

    const wxString keyword_types =
        "bool char f32 f64 i16 i32 i64 i8 isize slice str tuple u16 u32 u64 u8 "
        "usize";

    SetKeys(text, 0, primary_keywords_);  // Primary keywords and identifiers
    SetKeys(text, 1, keyword_types);      // Built in types
    SetKeys(text, 2, "");                 // Other keywords
    SetKeys(text, 3, "");
    SetKeys(text, 4, "");

#ifndef USE_CPP_AS_RUST
    SetKeys(text, 5, "");
    SetKeys(text, 6, "");
#endif
  }
};

class ProtobufLanguage : public Language {
 public:
  ProtobufLanguage() : Language(_("Protobuf"), wxSTC_LEX_CPP) {
    AddExtension(".proto");
  }

  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
    DefaultStyleDocument(text, settings, this);

    SetKeys(text, 0, KeywordBuilder() << "message"
                                      << "required"
                                      << "optional"
                                      << "repeated"
                                      << "true"
                                      << "false"
                                      << "enum"
                                      << "import"
                                      << "group"
                                      << "extensions"
                                      << "to"
                                      << "max"
                                      << "oneof"
                                      << "package"
                                      << "service"
                                      << "rpc"
                                      << "returns");
    SetKeys(text, 1, KeywordBuilder() << "double"
                                      << "float"
                                      << "int32"
                                      << "int64"
                                      << "uint32"
                                      << "uint64"
                                      << "sin32"
                                      << "sint64"
                                      << "fixed32"
                                      << "fixed64"
                                      << "sfixed32"
                                      << "sfixed64"
                                      << "bool"
                                      << "string"
                                      << "bytes");  // secondary
    SetKeys(text, 2, "");                           // documentation

    SetKeys(text, 3, "");  // global classes and typedefs
    SetKeys(text, 4, "");  // preprocessor defines
  }
};

class MarkdownLanguage : public Language {
 public:
  MarkdownLanguage() : Language(_("Markdown"), wxSTC_LEX_MARKDOWN) {
    AddExtension(".md");
  }
  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
    SetStyle(text, wxSTC_MARKDOWN_DEFAULT,
             settings.fonts_and_colors().default_style(), true);
    SetStyle(text, wxSTC_MARKDOWN_LINE_BEGIN,
             settings.fonts_and_colors().markdown_line_begin());
    SetStyle(text, wxSTC_MARKDOWN_STRONG1,
             settings.fonts_and_colors().markdown_strong1());
    SetStyle(text, wxSTC_MARKDOWN_STRONG2,
             settings.fonts_and_colors().markdown_strong2());
    SetStyle(text, wxSTC_MARKDOWN_EM1,
             settings.fonts_and_colors().markdown_em1());
    SetStyle(text, wxSTC_MARKDOWN_EM2,
             settings.fonts_and_colors().markdown_em2());
    SetStyle(text, wxSTC_MARKDOWN_HEADER1,
             settings.fonts_and_colors().markdown_header1());
    SetStyle(text, wxSTC_MARKDOWN_HEADER2,
             settings.fonts_and_colors().markdown_header2());
    SetStyle(text, wxSTC_MARKDOWN_HEADER3,
             settings.fonts_and_colors().markdown_header3());
    SetStyle(text, wxSTC_MARKDOWN_HEADER4,
             settings.fonts_and_colors().markdown_header4());
    SetStyle(text, wxSTC_MARKDOWN_HEADER5,
             settings.fonts_and_colors().markdown_header5());
    SetStyle(text, wxSTC_MARKDOWN_HEADER6,
             settings.fonts_and_colors().markdown_header6());
    SetStyle(text, wxSTC_MARKDOWN_PRECHAR,
             settings.fonts_and_colors().markdown_prechar());
    SetStyle(text, wxSTC_MARKDOWN_ULIST_ITEM,
             settings.fonts_and_colors().markdown_ulist_item());
    SetStyle(text, wxSTC_MARKDOWN_OLIST_ITEM,
             settings.fonts_and_colors().markdown_olist_item());
    SetStyle(text, wxSTC_MARKDOWN_BLOCKQUOTE,
             settings.fonts_and_colors().markdown_blockquote());
    SetStyle(text, wxSTC_MARKDOWN_STRIKEOUT,
             settings.fonts_and_colors().markdown_strikeout());
    SetStyle(text, wxSTC_MARKDOWN_HRULE,
             settings.fonts_and_colors().markdown_hrule());
    SetStyle(text, wxSTC_MARKDOWN_LINK,
             settings.fonts_and_colors().markdown_link());
    SetStyle(text, wxSTC_MARKDOWN_CODE,
             settings.fonts_and_colors().markdown_code());
    SetStyle(text, wxSTC_MARKDOWN_CODE2,
             settings.fonts_and_colors().markdown_code2());
    SetStyle(text, wxSTC_MARKDOWN_CODEBK,
             settings.fonts_and_colors().markdown_codebk());
  }
};

class PropertiesLanguage : public Language {
 public:
  PropertiesLanguage() : Language(_("Properties"), wxSTC_LEX_PROPERTIES) {
    AddExtension(".props");
    AddExtension(".toml");  // properties are kinda like toml
                            // todo: implement a proper toml lexer/language
  }
  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    SetStyle(text, wxSTC_PROPS_DEFAULT,
             settings.fonts_and_colors().default_style(), true);
    SetStyle(text, wxSTC_PROPS_COMMENT,
             settings.fonts_and_colors().style_comment());
    SetStyle(text, wxSTC_PROPS_SECTION,
             settings.fonts_and_colors().props_section());
    SetStyle(text, wxSTC_PROPS_ASSIGNMENT,
             settings.fonts_and_colors().props_assignment());
    SetStyle(text, wxSTC_PROPS_DEFVAL,
             settings.fonts_and_colors().props_defval());
    SetStyle(text, wxSTC_PROPS_KEY, settings.fonts_and_colors().props_key());
  }
};

class XmlLanguage : public Language {
 public:
  XmlLanguage() : Language(_("XML"), wxSTC_LEX_XML) { AddExtension(".xml"); }
  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    SetStyle(text, wxSTC_H_DEFAULT, settings.fonts_and_colors().default_style(),
             true);
    SetStyle(text, wxSTC_H_TAG, settings.fonts_and_colors().h_tag());
    SetStyle(text, wxSTC_H_TAGUNKNOWN,
             settings.fonts_and_colors().h_tagunknown());
    SetStyle(text, wxSTC_H_ATTRIBUTE,
             settings.fonts_and_colors().h_attribute());
    SetStyle(text, wxSTC_H_ATTRIBUTEUNKNOWN,
             settings.fonts_and_colors().h_attributeunknown());
    SetStyle(text, wxSTC_H_NUMBER, settings.fonts_and_colors().h_number());
    SetStyle(text, wxSTC_H_DOUBLESTRING,
             settings.fonts_and_colors().h_doublestring());
    SetStyle(text, wxSTC_H_SINGLESTRING,
             settings.fonts_and_colors().h_singlestring());
    SetStyle(text, wxSTC_H_OTHER, settings.fonts_and_colors().h_other());
    SetStyle(text, wxSTC_H_COMMENT,
             settings.fonts_and_colors().style_comment());
    SetStyle(text, wxSTC_H_ENTITY, settings.fonts_and_colors().h_entity());
    SetStyle(text, wxSTC_H_TAGEND, settings.fonts_and_colors().h_tagend());
    SetStyle(text, wxSTC_H_XMLSTART, settings.fonts_and_colors().h_xmlstart());
    SetStyle(text, wxSTC_H_XMLEND, settings.fonts_and_colors().h_xmlend());
    SetStyle(text, wxSTC_H_SCRIPT, settings.fonts_and_colors().h_script());
    SetStyle(text, wxSTC_H_ASP, settings.fonts_and_colors().h_asp());
    SetStyle(text, wxSTC_H_ASPAT, settings.fonts_and_colors().h_aspat());
    SetStyle(text, wxSTC_H_CDATA, settings.fonts_and_colors().h_cdata());
    SetStyle(text, wxSTC_H_QUESTION, settings.fonts_and_colors().h_question());
    SetStyle(text, wxSTC_H_VALUE, settings.fonts_and_colors().h_value());
    SetStyle(text, wxSTC_H_XCCOMMENT,
             settings.fonts_and_colors().h_xccomment());

    SetKeys(text, 0,
            "html body a head b div ul li");  // HTML elements and attributes
    SetKeys(text, 1, "if while");             // JavaScript keywords
    SetKeys(text, 2, "");                     // VBScript keywords
    SetKeys(text, 3, "");                     // Python keywords
    SetKeys(text, 4, "");                     // PHP keywords
    SetKeys(text, 5, "");                     // SGML and DTD keywords
  }
};

class CmakeLanguage : public Language {
 public:
  CmakeLanguage() : Language(_("CMake"), wxSTC_LEX_CMAKE) {
    AddExtension(".cmake");
    AddExtension("CMakeLists.txt");
  }
  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    SetStyle(text, wxSTC_CMAKE_DEFAULT,
             settings.fonts_and_colors().default_style(), true);
    SetStyle(text, wxSTC_CMAKE_COMMENT,
             settings.fonts_and_colors().style_comment());
    SetStyle(text, wxSTC_CMAKE_STRINGDQ,
             settings.fonts_and_colors().cmake_stringdq());
    SetStyle(text, wxSTC_CMAKE_STRINGLQ,
             settings.fonts_and_colors().cmake_stringlq());
    SetStyle(text, wxSTC_CMAKE_STRINGRQ,
             settings.fonts_and_colors().cmake_stringrq());
    SetStyle(text, wxSTC_CMAKE_COMMANDS,
             settings.fonts_and_colors().cmake_commands());
    SetStyle(text, wxSTC_CMAKE_PARAMETERS,
             settings.fonts_and_colors().cmake_parameters());
    SetStyle(text, wxSTC_CMAKE_VARIABLE,
             settings.fonts_and_colors().cmake_variable());
    SetStyle(text, wxSTC_CMAKE_USERDEFINED,
             settings.fonts_and_colors().cmake_userdefined());
    SetStyle(text, wxSTC_CMAKE_WHILEDEF,
             settings.fonts_and_colors().cmake_whiledef());
    SetStyle(text, wxSTC_CMAKE_FOREACHDEF,
             settings.fonts_and_colors().cmake_foreachdef());
    SetStyle(text, wxSTC_CMAKE_IFDEFINEDEF,
             settings.fonts_and_colors().cmake_ifdefinedef());
    SetStyle(text, wxSTC_CMAKE_MACRODEF,
             settings.fonts_and_colors().cmake_macrodef());
    SetStyle(text, wxSTC_CMAKE_STRINGVAR,
             settings.fonts_and_colors().cmake_stringvar());
    SetStyle(text, wxSTC_CMAKE_NUMBER,
             settings.fonts_and_colors().cmake_number());

    SetKeys(text, 0,
            "add_compile_options add_custom_command add_custom_target "
            "add_definitions add_dependencies add_executable add_library "
            "add_subdirectory "
            "add_test aux_source_directory break build_command "
            "cmake_host_system_information cmake_minimum_required cmake_policy "
            "configure_file "
            "create_test_sourcelist define_property elseif else "
            "enable_language enable_testing endforeach endfunction endif "
            "endmacro endwhile "
            "execute_process export file find_file find_library find_package "
            "find_path find_program fltk_wrap_ui foreach function "
            "get_cmake_property "
            "get_directory_property get_filename_component get_property "
            "get_source_file_property get_target_property get_test_property if "
            "include_directories "
            "include_external_msproject include_regular_expression include "
            "install link_directories list load_cache load_command macro "
            "mark_as_advanced math "
            "message option project qt_wrap_cpp qt_wrap_ui remove_definitions "
            "return separate_arguments set_directory_properties set_property "
            "set set_source_files_properties "
            "set_target_properties set_tests_properties site_name source_group "
            "string target_compile_definitions target_compile_options "
            "target_include_directories target_link_libraries "
            "try_compile try_run unset variable_watch while");  // Commands
    SetKeys(text, 1, "CMAKE_ROOT CMAKE_SOURCE_DIR");            // Parameters
    SetKeys(text, 2, "");                                       // UserDefined
  }
};

class LuaLanguage : public Language {
 public:
  LuaLanguage() : Language(_("Lua"), wxSTC_LEX_LUA) { AddExtension(".lua"); }
  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    SetStyle(text, wxSTC_LUA_DEFAULT,
             settings.fonts_and_colors().default_style(), true);
    SetStyle(text, wxSTC_LUA_COMMENT,
             settings.fonts_and_colors().style_comment());
    SetStyle(text, wxSTC_LUA_COMMENTLINE,
             settings.fonts_and_colors().style_commentline());
    SetStyle(text, wxSTC_LUA_COMMENTDOC,
             settings.fonts_and_colors().style_commentdoc());
    SetStyle(text, wxSTC_LUA_NUMBER,
             settings.fonts_and_colors().style_number());
    SetStyle(text, wxSTC_LUA_WORD, settings.fonts_and_colors().style_keyword());
    SetStyle(text, wxSTC_LUA_STRING,
             settings.fonts_and_colors().style_string());
    SetStyle(text, wxSTC_LUA_CHARACTER,
             settings.fonts_and_colors().style_character());
    SetStyle(text, wxSTC_LUA_LITERALSTRING,
             settings.fonts_and_colors().style_verbatim());
    SetStyle(text, wxSTC_LUA_PREPROCESSOR,
             settings.fonts_and_colors().style_preprocessor());
    SetStyle(text, wxSTC_LUA_OPERATOR,
             settings.fonts_and_colors().style_operator());
    SetStyle(text, wxSTC_LUA_IDENTIFIER,
             settings.fonts_and_colors().style_identifier());
    SetStyle(text, wxSTC_LUA_STRINGEOL,
             settings.fonts_and_colors().style_string_eol());
    // SetStyle(text, wxSTC_LUA_WORD2, settings.fonts_and_colors().lua_word2());
    // SetStyle(text, wxSTC_LUA_WORD3, settings.fonts_and_colors().lua_word3());
    // SetStyle(text, wxSTC_LUA_WORD4, settings.fonts_and_colors().lua_word4());
    // SetStyle(text, wxSTC_LUA_WORD5, settings.fonts_and_colors().lua_word5());
    // SetStyle(text, wxSTC_LUA_WORD6, settings.fonts_and_colors().lua_word6());
    // SetStyle(text, wxSTC_LUA_WORD7, settings.fonts_and_colors().lua_word7());
    // SetStyle(text, wxSTC_LUA_WORD8, settings.fonts_and_colors().lua_word8());
    SetStyle(text, wxSTC_LUA_LABEL, settings.fonts_and_colors().lua_label());

    SetKeys(text, 0,
            "and break do else elseif "
            "end false for function if "
            "in local nil not or "
            "repeat return then true until while");  // Keywords
    SetKeys(text, 1, "");                            // Basic functions
    SetKeys(text, 2, "");  // String, (table) & math functions
    SetKeys(text, 3, "");  // (coroutines), I/O & system facilities
    SetKeys(text, 4, "");  // user1
    SetKeys(text, 5, "");  // user2
    SetKeys(text, 6, "");  // user3
    SetKeys(text, 7, "");  // user4
  }
};

class YamlLanguage : public Language {
 public:
  YamlLanguage() : Language(_("YAML"), wxSTC_LEX_YAML) {
    AddExtension(".yaml");
  }
  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {
    wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

    SetStyle(text, wxSTC_YAML_DEFAULT,
             settings.fonts_and_colors().default_style(), true);
    SetStyle(text, wxSTC_YAML_COMMENT,
             settings.fonts_and_colors().style_comment());
    SetStyle(text, wxSTC_YAML_IDENTIFIER,
             settings.fonts_and_colors().style_identifier());
    SetStyle(text, wxSTC_YAML_KEYWORD,
             settings.fonts_and_colors().style_keyword());
    SetStyle(text, wxSTC_YAML_NUMBER,
             settings.fonts_and_colors().yaml_number());
    SetStyle(text, wxSTC_YAML_REFERENCE,
             settings.fonts_and_colors().yaml_reference());
    SetStyle(text, wxSTC_YAML_DOCUMENT,
             settings.fonts_and_colors().yaml_document());
    SetStyle(text, wxSTC_YAML_TEXT, settings.fonts_and_colors().yaml_text());
    SetStyle(text, wxSTC_YAML_ERROR, settings.fonts_and_colors().yaml_error());
    SetStyle(text, wxSTC_YAML_OPERATOR,
             settings.fonts_and_colors().style_operator());

    SetKeys(text, 0, "");  // keywords
  }
};

class NullLanguage : public Language {
 public:
  NullLanguage() : Language(_("NULL"), wxSTC_LEX_NULL) {}
  void DoStyleDocument(wxStyledTextCtrl* text,
                       const ride::Settings& settings) override {}
};

struct Languages::LanguagesPimpl {
  LanguagesPimpl() : LanguageList(BuildLanguageList()) {}

  std::vector<Language*> BuildLanguageList() {
    std::vector<Language*> ret;
    ret.push_back(&language_rust_);
    ret.push_back(&language_protobuf_);
    ret.push_back(&language_properties_);
    ret.push_back(&language_cpp_);
    ret.push_back(&language_markdown_);
    ret.push_back(&language_xml_);
    ret.push_back(&language_cmake_);
    ret.push_back(&language_lua_);
    ret.push_back(&language_yaml_);
    return ret;
  }

  CppLanguage language_cpp_;
  RustLanguage language_rust_;
  ProtobufLanguage language_protobuf_;
  NullLanguage language_null_;
  MarkdownLanguage language_markdown_;
  PropertiesLanguage language_properties_;
  XmlLanguage language_xml_;
  CmakeLanguage language_cmake_;
  LuaLanguage language_lua_;
  YamlLanguage language_yaml_;

  const std::vector<Language*> LanguageList;
};

Languages::Languages() : pimpl_(new Languages::LanguagesPimpl()) {}
Languages::~Languages() {}

wxString Languages::GetFilePattern() {
  wxString ret = "All files (*.*)|*.*";

  // need to loop from back to front to get the LanguageList in order for
  // display
  // since we are adding 'back to front'
  for (auto l = pimpl_->LanguageList.rbegin(); l != pimpl_->LanguageList.rend();
       ++l) {
    Language* lang = *l;
    ret = lang->GetFilePattern() + "|" + ret;
  }
  return ret;
}

Language* Languages::DetermineLanguage(const wxString& filepath) {
  for (auto lang : pimpl_->LanguageList) {
    if (lang->MatchPattern(filepath)) {
      return lang;
    }
  }
  return &pimpl_->language_null_;
}

Language* Languages::GetNullLanguage() { return &pimpl_->language_null_; }
