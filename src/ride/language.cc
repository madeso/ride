#include "ride/language.h"

#include <ride/wx.h>
#include <wx/stc/stc.h>

#include <cassert>

#include "ride_compiler_settings.h"	 // NOLINT this is how we include it

#include "ride/settings.h"
#include "ride/wxutils.h"
#include "ride/stcutils.h"
#include "ride/path.h"

wxString b2s01(bool b);

//////////////////////////////////////////////////////////////////////////

Language::Language()
	: language_name_(_("NULL"))
	, lexer_style_(wxSTC_LEX_NULL)
{
}

const wxString& Language::language_name() const
{
	return language_name_;
}

wxString PropTypeToString(int type)
{
	switch (type)
	{
	case wxSTC_TYPE_BOOLEAN: return _("bool    ");
	case wxSTC_TYPE_INTEGER: return _("int     ");
	case wxSTC_TYPE_STRING: return _("wxString");
	default: assert(0 && "Unknown property type"); return _("XXXXXXXX");
	}
}

bool Language::IsKeyword(int group, const wxString& word) const
{
	const auto it = keywords.find(group);
	if (it == keywords.end()) return false;
	const auto& keyword_list = it->second;
	return keyword_list.find(word) != keyword_list.end();
}

wxString keywords_to_string(const std::vector<wxString>& keywords_)
{
	wxString ret;
	bool first = true;
	for (const auto& keyword: keywords_)
	{
		if (first)
			ret = keyword;
		else
			ret += " " + keyword;
		first = false;
	}
	return ret;
}

wxString keywords_to_string(const std::set<wxString>& keywords_)
{
	wxString ret;
	bool first = true;
	for (const auto& keyword: keywords_)
	{
		if (first)
			ret = keyword;
		else
			ret += " " + keyword;
		first = false;
	}
	return ret;
}

class KeywordBuilder
{
public:

	KeywordBuilder& operator<<(const wxString& keyword)
	{
		keywords_.push_back(keyword);
		return *this;
	}

	const std::vector<wxString>& ToVector() const
	{
		return keywords_;
	}

	wxString ToString() const
	{
		return keywords_to_string(keywords_);
	}

	operator const wxString() const
	{
		return ToString();
	}

private:

	std::vector<wxString> keywords_;
};

struct PropsAndKeywords
{
#ifdef _DEBUG
	// todo(Gustav): move to local variables
	std::set<wxString> used_properties_;
	std::set<unsigned int> used_keywords_;
#endif

	void SetProperty(wxStyledTextCtrl* text, const wxString& name, const wxString& value)
	{
		text->SetProperty(name, value);
	#ifdef _DEBUG
		assert(used_properties_.find(name) == used_properties_.end());
		used_properties_.insert(name);
	#endif
	}

	
	void SetKeys(wxStyledTextCtrl* text, unsigned int id, const wxString& keywords)
	{
		text->SetKeyWords(id, keywords);
#ifdef _DEBUG
		assert(used_keywords_.find(id) == used_keywords_.end());
		used_keywords_.insert(id);
#endif
	}
		
	void WarnAboutProperties(wxStyledTextCtrl* text, const wxString& language_name_) const
	{
		const auto available_props_vec = Split(text->PropertyNames(), '\n');
		const auto available_props
			= std::set(available_props_vec.begin(), available_props_vec.end());

		for (const auto& prop_name: available_props)
		{
			if (used_properties_.find(prop_name) != used_properties_.end()) continue;
			const wxString desc = text->DescribeProperty(prop_name);
			const wxString value = text->GetProperty(prop_name);
			const wxString type = PropTypeToString(text->PropertyType(prop_name));
			wxLogWarning(
				_("Property for %s was not set: %s %s; // %s %s"),
				language_name_,
				type,
				prop_name,
				value,
				desc
			);
		}

		for (const auto& prop: used_properties_)
		{
			if (available_props.find(prop) == available_props.end())
			{
				wxLogWarning(
					_("Property %s for %s was set, but does not exist."), prop, language_name_
				);
			}
		}
	}

	void WarnAboutKeywords(const wxStyledTextCtrl* text, const wxString& language_name_) const
	{
		const auto available_keywords = Split(text->DescribeKeyWordSets(), '\n');
		for (unsigned int i = 0; i < available_keywords.size(); ++i)
		{
			if (used_keywords_.find(i) == used_keywords_.end())
			{
				wxLogWarning(
					_("Keyword %d for %s was not set: %s"), i, language_name_, available_keywords[i]
				);
			}
		}
	}
};



//////////////////////////////////////////////////////////////////////////


void Language::StyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) const
{
	PropsAndKeywords props_and_keywords;

	text->SetLexer(lexer_style_);

	auto* theme = settings.find_current_theme();
	if (theme)
	{
		for (const auto& [sci_id, style_name]: bindings)
		{
			const auto found_style = theme->GetStyle(style_name);
			if (! found_style.has_value())
			{
				wxLogWarning(
					_("Style %s for %s was not found."),
					style_name,
					language_name_
				);
				continue;
			}

			SetStyle(text, sci_id, *found_style, *theme);
		}
	}

	for (const auto& [name, value]: properties)
	{
		props_and_keywords.SetProperty(text, name, settings.LookupSetting(value));
	}
	for (const auto& [kwclass, kws]: keywords)
	{
		props_and_keywords.SetKeys(text, kwclass, keywords_to_string(kws));
	}

#ifdef _DEBUG
	props_and_keywords.WarnAboutProperties(text, language_name_);
	props_and_keywords.WarnAboutKeywords(text, language_name_);
#endif
}

void Language::AddExtension(const wxString& ext)
{
	file_patterns_.push_back(ext);
}

bool Language::MatchPattern(const Fil& file) const
{
	for (const auto& elem: file_patterns_)
	{
		if (file.ends_with(elem))
		{
			return true;
		}
	}
	return false;
}


Language MakeCppLanguage()
{
	Language cpp;
	cpp.language_name_ = "C++";
	cpp.lexer_style_ = wxSTC_LEX_CPP;
	cpp.file_patterns_ = {".c", ".cc", ".cpp", ".cs", ".h", ".hh", ".hpp", ".hxx"};

	// primary
	cpp.keywords[0] = {"asm",		   "auto",		"bool",
					   "break",		   "case",		"catch",
					   "char",		   "class",		"const",
					   "const_cast",   "continue",	"default",
					   "delete",	   "do",		"double",
					   "dynamic_cast", "else",		"enum",
					   "explicit",	   "export",	"extern",
					   "false",		   "float",		"for",
					   "friend",	   "goto",		"if",
					   "inline",	   "int",		"long",
					   "mutable",	   "namespace", "new",
					   "operator",	   "private",	"protected",
					   "public",	   "register",	"reinterpret_cast",
					   "return",	   "short",		"signed",
					   "sizeof",	   "static",	"static_cast",
					   "struct",	   "switch",	"template",
					   "this",		   "throw",		"true",
					   "try",		   "typedef",	"typeid",
					   "typename",	   "union",		"unsigned",
					   "using",		   "virtual",	"void",
					   "volatile",	   "wchar_t",	"while"};

	// secondary
	cpp.keywords[1] = {"file"};

	// documentation
	cpp.keywords[2] = {
		"a", "addindex", "addtogroup", "anchor", "arg", "attention", "author", "b", "brief", "bug", "c", "class", "code",
		"date", "def", "defgroup", "deprecated", "dontinclude", "e", "em", "endcode", "endhtmlonly", "endif", "endlatexonly",
		"endlink", "endverbatim", "enum", "example", "exception", "f$", "f[", "f]", "file", "fn", "hideinitializer",
		"htmlinclude", "htmlonly", "if", "image", "include", "ingroup", "internal", "invariant", "interface", "latexonly",
		"li", "line", "link", "mainpage", "name", "namespace", "nosubgrouping", "note", "overload", "p", "page", "par",
		"param", "post", "pre", "ref", "relates", "remarks", "return", "retval", "sa", "section", "see", "showinitializer",
		"since", "skip", "skipline", "struct", "subsection", "test", "throw", "todo", "typedef", "union", "until", "var", "verbatim",
		"verbinclude", "version", "warning", "weakgroup", "$", "@", "\"\"", "&", "<", ">", "#", "{", "}"
	};

	// global classes and typedefs
	cpp.keywords[3] = {};

	// preprocessor defines
	cpp.keywords[4] = {};

	// Task marker and error marker keywords
	cpp.keywords[5] = {"todo", "error"};

	//  Set to 1 to allow verbatim strings to contain escape sequences.
	cpp.properties["lexer.cpp.verbatim.strings.allow.escapes"] = "1";
	//  Set to 1 to enable highlighting of back-quoted raw strings .
	cpp.properties["lexer.cpp.backquoted.strings"] = "1";
	//  Set to 1 to enable highlighting of escape sequences in strings
	cpp.properties["lexer.cpp.escape.sequence"] = "1";
	//  This option enables folding on a preprocessor #else or #endif line of an #if statement.
	cpp.properties["fold.cpp.preprocessor.at.else"] = "1";

	// todo(Gustav): introduce auto settings
	cpp.properties["fold"] = "auto.fold";

	cpp.properties["fold.comment"] = "1";
	cpp.properties["fold.compact"] = "1";
	cpp.properties["fold.preprocessor"] = "1";
	cpp.properties["styling.within.preprocessor"] = "0"; //  For C++ code determines whether all preprocessor code is styled in the preprocessor style (0, the default) or only from the initial # to the end of the command word(1).
	cpp.properties["lexer.cpp.allow.dollars"] = "0"; // Set to 0 to disallow the '$' character in identifiers with the cpp lexer.
	cpp.properties["lexer.cpp.track.preprocessor"] = "0"; // Set to 1 to interpret #if/#else/#endif to grey out code that is not active.
	cpp.properties["lexer.cpp.update.preprocessor"] = "0"; // Set to 1 to update preprocessor definitions when #define found.
	cpp.properties["lexer.cpp.triplequoted.strings"] = "0"; // Set to 1 to enable highlighting of triple-quoted std::strings.
	cpp.properties["lexer.cpp.hashquoted.strings"] = "0"; // Set to 1 to enable highlighting of hash-quoted std::strings.
	cpp.properties["fold.cpp.syntax.based"] = "0"; // Set this property to 0 to disable syntax based folding.
	cpp.properties["fold.cpp.comment.multiline"] = "1";	// Set this property to 0 to disable folding multi-line comments when fold.comment=1.;
	cpp.properties["fold.cpp.comment.explicit"] = "1"; // Set this property to 0 to disable folding explicit fold points when fold.comment=1.
	cpp.properties["fold.cpp.explicit.anywhere"] = "0"; // Set this property to 1 to enable explicit fold points anywhere, not just in line comments.
	cpp.properties["fold.at.else"] = "1"; // This option enables C++ folding on a "} else {" line of an if statement.
	cpp.properties["fold.cpp.explicit.start"] = _T("//{");
	cpp.properties["fold.cpp.explicit.end"] = _T("//}");

	// setup alabaster light style
	const wxString style_string = "string";
	const wxString style_default = "default";
	const wxString style_constant = "constant";
	const wxString style_comment = "comment";
	const wxString style_global = "global";
	
	// bind cpp
	cpp.bindings[wxSTC_C_DEFAULT] = style_default;
	cpp.bindings[wxSTC_C_STRINGEOL] = style_string;
	cpp.bindings[wxSTC_C_VERBATIM] = style_string;
	cpp.bindings[wxSTC_C_REGEX] = style_string;
	cpp.bindings[wxSTC_C_COMMENTLINEDOC] = style_comment;
	// cpp.bindings[wxSTC_C_WORD2] = style_keyword_types;
	cpp.bindings[wxSTC_C_COMMENTDOCKEYWORD] = style_comment;
	cpp.bindings[wxSTC_C_COMMENTDOCKEYWORDERROR] = style_comment;
	cpp.bindings[wxSTC_C_GLOBALCLASS] = style_global;
	cpp.bindings[wxSTC_C_STRINGRAW] = style_string;
	cpp.bindings[wxSTC_C_TRIPLEVERBATIM] = style_string;
	cpp.bindings[wxSTC_C_HASHQUOTEDSTRING] = style_string;
	// cpp.bindings[wxSTC_C_PREPROCESSORCOMMENT] = style_preprocessorcomment ;
	// cpp.bindings[wxSTC_C_DEFAULT] = default_style;
	cpp.bindings[wxSTC_C_COMMENT] = style_comment;
	cpp.bindings[wxSTC_C_COMMENTLINE] = style_comment;
	cpp.bindings[wxSTC_C_COMMENTDOC] = style_comment;
	cpp.bindings[wxSTC_C_NUMBER] = style_constant;
	// cpp.bindings[wxSTC_C_WORD] = style_keyword;
	cpp.bindings[wxSTC_C_STRING] = style_string;
	cpp.bindings[wxSTC_C_CHARACTER] = style_string;
	cpp.bindings[wxSTC_C_UUID] = style_string;
	cpp.bindings[wxSTC_C_PREPROCESSOR] = style_global;
	// cpp.bindings[wxSTC_C_OPERATOR] = style_operator;
	cpp.bindings[wxSTC_C_IDENTIFIER] = style_global;

	return cpp;
}

Languages::Languages()
{
	languages.emplace_back(MakeCppLanguage());
}

// todo(Gustav): implement cpp first, then the rest
#if 0
class CppLanguage : public Language
{
	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings)
	{
		DefaultStyleDocument(text, settings, this);
	}
};


class RustLanguage : public Language
{
public:

	RustLanguage()
		: Language(_("Rust"), wxSTC_LEX_RUST)
	{
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

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		SetStyle(text, wxSTC_RUST_DEFAULT, settings.current_theme.default_style, true);
		SetStyle(text, wxSTC_RUST_COMMENTBLOCK, settings.current_theme.style_comment);
		SetStyle(text, wxSTC_RUST_COMMENTLINE, settings.current_theme.style_commentline);
		SetStyle(text, wxSTC_RUST_COMMENTBLOCKDOC, settings.current_theme.style_commentdoc);
		SetStyle(text, wxSTC_RUST_COMMENTLINEDOC, settings.current_theme.style_commentdoc);
		SetStyle(text, wxSTC_RUST_NUMBER, settings.current_theme.style_number);
		SetStyle(text, wxSTC_RUST_WORD, settings.current_theme.style_keyword);
		/*
#define wxSTC_RUST_WORD2 7
#define wxSTC_RUST_WORD3 8
#define wxSTC_RUST_WORD4 9
#define wxSTC_RUST_WORD5 10
#define wxSTC_RUST_WORD6 11
#define wxSTC_RUST_WORD7 12
    */
		SetStyle(text, wxSTC_RUST_CHARACTER, settings.current_theme.style_character);
		SetStyle(text, wxSTC_RUST_STRING, settings.current_theme.style_string);
		SetStyle(text, wxSTC_RUST_STRINGR, settings.current_theme.style_verbatim);

		SetStyle(text, wxSTC_RUST_OPERATOR, settings.current_theme.style_operator);
		SetStyle(text, wxSTC_RUST_IDENTIFIER, settings.current_theme.style_identifier);
		SetStyle(text, wxSTC_RUST_LIFETIME, settings.current_theme.rust_lifetime);
		SetStyle(text, wxSTC_RUST_MACRO, settings.current_theme.style_preprocessor);
		SetStyle(text, wxSTC_RUST_LEXERROR, settings.current_theme.rust_lex_error);

		SetStyle(text, wxSTC_C_UUID, settings.current_theme.style_uuid);

		SetProperty(text, wxT("fold"), b2s01(settings.foldEnable));
		SetProperty(text, wxT("fold.comment"), b2s01(settings.foldComment));
		SetProperty(text, wxT("fold.compact"), b2s01(settings.foldCompact));
		SetProperty(text, wxT("fold.at.else"), b2s01(settings.fold_at_else));
		SetProperty(text, wxT("fold.rust.syntax.based"), b2s01(settings.fold_cpp_syntax_based));
		SetProperty(
			text, wxT("fold.rust.comment.multiline"), b2s01(settings.fold_cpp_comment_multiline)
		);
		SetProperty(
			text, wxT("fold.rust.comment.explicit"), b2s01(settings.fold_cpp_comment_explicit)
		);
		SetProperty(
			text, wxT("fold.rust.explicit.anywhere"), b2s01(settings.fold_cpp_explicit_anywhere)
		);
		SetProperty(text, wxT("fold.rust.explicit.start"), _T("//{"));
		SetProperty(text, wxT("fold.rust.explicit.end"), _T("//}"));
		SetProperty(text, wxT("lexer.rust.fold.at.else"), b2s01(settings.fold_at_else));

		// end of setstyle/setprop fallback

		const wxString keyword_types
			= "bool char f32 f64 i16 i32 i64 i8 isize slice str tuple u16 u32 u64 u8 "
			  "usize";

		SetKeys(text, 0, primary_keywords_);  // Primary keywords and identifiers
		SetKeys(text, 1, keyword_types);  // Built in types
		SetKeys(text, 2, "");  // Other keywords
		SetKeys(text, 3, "");
		SetKeys(text, 4, "");
		SetKeys(text, 5, "");
		SetKeys(text, 6, "");
	}
};

class ProtobufLanguage : public Language
{
public:

	ProtobufLanguage()
		: Language(_("Protobuf"), wxSTC_LEX_CPP)
	{
		AddExtension(".proto");
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		DefaultStyleDocument(text, settings, this);

		SetKeys(
			text,
			0,
			KeywordBuilder() << "message"
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
							 << "returns"
		);
		SetKeys(
			text,
			1,
			KeywordBuilder() << "double"
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
							 << "bytes"
		);	// secondary
		SetKeys(text, 2, "");  // documentation

		SetKeys(text, 3, "");  // global classes and typedefs
		SetKeys(text, 4, "");  // preprocessor defines
	}
};

class MarkdownLanguage : public Language
{
public:

	MarkdownLanguage()
		: Language(_("Markdown"), wxSTC_LEX_MARKDOWN)
	{
		AddExtension(".md");
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		SetStyle(text, wxSTC_MARKDOWN_DEFAULT, settings.current_theme.default_style, true);
		SetStyle(text, wxSTC_MARKDOWN_LINE_BEGIN, settings.current_theme.markdown_line_begin);
		SetStyle(text, wxSTC_MARKDOWN_STRONG1, settings.current_theme.markdown_strong1);
		SetStyle(text, wxSTC_MARKDOWN_STRONG2, settings.current_theme.markdown_strong2);
		SetStyle(text, wxSTC_MARKDOWN_EM1, settings.current_theme.markdown_em1);
		SetStyle(text, wxSTC_MARKDOWN_EM2, settings.current_theme.markdown_em2);
		SetStyle(text, wxSTC_MARKDOWN_HEADER1, settings.current_theme.markdown_header1);
		SetStyle(text, wxSTC_MARKDOWN_HEADER2, settings.current_theme.markdown_header2);
		SetStyle(text, wxSTC_MARKDOWN_HEADER3, settings.current_theme.markdown_header3);
		SetStyle(text, wxSTC_MARKDOWN_HEADER4, settings.current_theme.markdown_header4);
		SetStyle(text, wxSTC_MARKDOWN_HEADER5, settings.current_theme.markdown_header5);
		SetStyle(text, wxSTC_MARKDOWN_HEADER6, settings.current_theme.markdown_header6);
		SetStyle(text, wxSTC_MARKDOWN_PRECHAR, settings.current_theme.markdown_prechar);
		SetStyle(text, wxSTC_MARKDOWN_ULIST_ITEM, settings.current_theme.markdown_ulist_item);
		SetStyle(text, wxSTC_MARKDOWN_OLIST_ITEM, settings.current_theme.markdown_olist_item);
		SetStyle(text, wxSTC_MARKDOWN_BLOCKQUOTE, settings.current_theme.markdown_blockquote);
		SetStyle(text, wxSTC_MARKDOWN_STRIKEOUT, settings.current_theme.markdown_strikeout);
		SetStyle(text, wxSTC_MARKDOWN_HRULE, settings.current_theme.markdown_hrule);
		SetStyle(text, wxSTC_MARKDOWN_LINK, settings.current_theme.markdown_link);
		SetStyle(text, wxSTC_MARKDOWN_CODE, settings.current_theme.markdown_code);
		SetStyle(text, wxSTC_MARKDOWN_CODE2, settings.current_theme.markdown_code2);
		SetStyle(text, wxSTC_MARKDOWN_CODEBK, settings.current_theme.markdown_codebk);
	}
};

class PropertiesLanguage : public Language
{
public:

	PropertiesLanguage()
		: Language(_("Properties"), wxSTC_LEX_PROPERTIES)
	{
		AddExtension(".props");
		AddExtension(".toml");	// properties are kinda like toml
			// todo: implement a proper toml lexer/language
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

		SetStyle(text, wxSTC_PROPS_DEFAULT, settings.current_theme.default_style, true);
		SetStyle(text, wxSTC_PROPS_COMMENT, settings.current_theme.style_comment);
		SetStyle(text, wxSTC_PROPS_SECTION, settings.current_theme.props_section);
		SetStyle(text, wxSTC_PROPS_ASSIGNMENT, settings.current_theme.props_assignment);
		SetStyle(text, wxSTC_PROPS_DEFVAL, settings.current_theme.props_defval);
		SetStyle(text, wxSTC_PROPS_KEY, settings.current_theme.props_key);
	}
};

class XmlLanguage : public Language
{
public:

	XmlLanguage()
		: Language(_("XML"), wxSTC_LEX_XML)
	{
		AddExtension(".xml");
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

		SetStyle(text, wxSTC_H_DEFAULT, settings.current_theme.default_style, true);
		SetStyle(text, wxSTC_H_TAG, settings.current_theme.h_tag);
		SetStyle(text, wxSTC_H_TAGUNKNOWN, settings.current_theme.h_tagunknown);
		SetStyle(text, wxSTC_H_ATTRIBUTE, settings.current_theme.h_attribute);
		SetStyle(text, wxSTC_H_ATTRIBUTEUNKNOWN, settings.current_theme.h_attributeunknown);
		SetStyle(text, wxSTC_H_NUMBER, settings.current_theme.h_number);
		SetStyle(text, wxSTC_H_DOUBLESTRING, settings.current_theme.h_doublestring);
		SetStyle(text, wxSTC_H_SINGLESTRING, settings.current_theme.h_singlestring);
		SetStyle(text, wxSTC_H_OTHER, settings.current_theme.h_other);
		SetStyle(text, wxSTC_H_COMMENT, settings.current_theme.style_comment);
		SetStyle(text, wxSTC_H_ENTITY, settings.current_theme.h_entity);
		SetStyle(text, wxSTC_H_TAGEND, settings.current_theme.h_tagend);
		SetStyle(text, wxSTC_H_XMLSTART, settings.current_theme.h_xmlstart);
		SetStyle(text, wxSTC_H_XMLEND, settings.current_theme.h_xmlend);
		SetStyle(text, wxSTC_H_SCRIPT, settings.current_theme.h_script);
		SetStyle(text, wxSTC_H_ASP, settings.current_theme.h_asp);
		SetStyle(text, wxSTC_H_ASPAT, settings.current_theme.h_aspat);
		SetStyle(text, wxSTC_H_CDATA, settings.current_theme.h_cdata);
		SetStyle(text, wxSTC_H_QUESTION, settings.current_theme.h_question);
		SetStyle(text, wxSTC_H_VALUE, settings.current_theme.h_value);
		SetStyle(text, wxSTC_H_XCCOMMENT, settings.current_theme.h_xccomment);

		SetKeys(text, 0,
				"html body a head b div ul li");  // HTML elements and attributes
		SetKeys(text, 1, "if while");  // JavaScript keywords
		SetKeys(text, 2, "");  // VBScript keywords
		SetKeys(text, 3, "");  // Python keywords
		SetKeys(text, 4, "");  // PHP keywords
		SetKeys(text, 5, "");  // SGML and DTD keywords
	}
};

class CmakeLanguage : public Language
{
public:

	CmakeLanguage()
		: Language(_("CMake"), wxSTC_LEX_CMAKE)
	{
		AddExtension(".cmake");
		AddExtension("CMakeLists.txt");
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

		SetStyle(text, wxSTC_CMAKE_DEFAULT, settings.current_theme.default_style, true);
		SetStyle(text, wxSTC_CMAKE_COMMENT, settings.current_theme.style_comment);
		SetStyle(text, wxSTC_CMAKE_STRINGDQ, settings.current_theme.cmake_stringdq);
		SetStyle(text, wxSTC_CMAKE_STRINGLQ, settings.current_theme.cmake_stringlq);
		SetStyle(text, wxSTC_CMAKE_STRINGRQ, settings.current_theme.cmake_stringrq);
		SetStyle(text, wxSTC_CMAKE_COMMANDS, settings.current_theme.cmake_commands);
		SetStyle(text, wxSTC_CMAKE_PARAMETERS, settings.current_theme.cmake_parameters);
		SetStyle(text, wxSTC_CMAKE_VARIABLE, settings.current_theme.cmake_variable);
		SetStyle(text, wxSTC_CMAKE_USERDEFINED, settings.current_theme.cmake_userdefined);
		SetStyle(text, wxSTC_CMAKE_WHILEDEF, settings.current_theme.cmake_whiledef);
		SetStyle(text, wxSTC_CMAKE_FOREACHDEF, settings.current_theme.cmake_foreachdef);
		SetStyle(text, wxSTC_CMAKE_IFDEFINEDEF, settings.current_theme.cmake_ifdefinedef);
		SetStyle(text, wxSTC_CMAKE_MACRODEF, settings.current_theme.cmake_macrodef);
		SetStyle(text, wxSTC_CMAKE_STRINGVAR, settings.current_theme.cmake_stringvar);
		SetStyle(text, wxSTC_CMAKE_NUMBER, settings.current_theme.cmake_number);

		SetKeys(
			text,
			0,
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
			"try_compile try_run unset variable_watch while"
		);	// Commands
		SetKeys(text, 1, "CMAKE_ROOT CMAKE_SOURCE_DIR");  // Parameters
		SetKeys(text, 2, "");  // UserDefined
	}
};

class LuaLanguage : public Language
{
public:

	LuaLanguage()
		: Language(_("Lua"), wxSTC_LEX_LUA)
	{
		AddExtension(".lua");
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

		SetStyle(text, wxSTC_LUA_DEFAULT, settings.current_theme.default_style, true);
		SetStyle(text, wxSTC_LUA_COMMENT, settings.current_theme.style_comment);
		SetStyle(text, wxSTC_LUA_COMMENTLINE, settings.current_theme.style_commentline);
		SetStyle(text, wxSTC_LUA_COMMENTDOC, settings.current_theme.style_commentdoc);
		SetStyle(text, wxSTC_LUA_NUMBER, settings.current_theme.style_number);
		SetStyle(text, wxSTC_LUA_WORD, settings.current_theme.style_keyword);
		SetStyle(text, wxSTC_LUA_STRING, settings.current_theme.style_string);
		SetStyle(text, wxSTC_LUA_CHARACTER, settings.current_theme.style_character);
		SetStyle(text, wxSTC_LUA_LITERALSTRING, settings.current_theme.style_verbatim);
		SetStyle(text, wxSTC_LUA_PREPROCESSOR, settings.current_theme.style_preprocessor);
		SetStyle(text, wxSTC_LUA_OPERATOR, settings.current_theme.style_operator);
		SetStyle(text, wxSTC_LUA_IDENTIFIER, settings.current_theme.style_identifier);
		SetStyle(text, wxSTC_LUA_STRINGEOL, settings.current_theme.style_string_eol);
		// SetStyle(text, wxSTC_LUA_WORD2, settings.current_theme.lua_word2());
		// SetStyle(text, wxSTC_LUA_WORD3, settings.current_theme.lua_word3());
		// SetStyle(text, wxSTC_LUA_WORD4, settings.current_theme.lua_word4());
		// SetStyle(text, wxSTC_LUA_WORD5, settings.current_theme.lua_word5());
		// SetStyle(text, wxSTC_LUA_WORD6, settings.current_theme.lua_word6());
		// SetStyle(text, wxSTC_LUA_WORD7, settings.current_theme.lua_word7());
		// SetStyle(text, wxSTC_LUA_WORD8, settings.current_theme.lua_word8());
		SetStyle(text, wxSTC_LUA_LABEL, settings.current_theme.lua_label);

		SetKeys(
			text,
			0,
			"and break do else elseif "
			"end false for function if "
			"in local nil not or "
			"repeat return then true until while"
		);	// Keywords
		SetKeys(text, 1, "");  // Basic functions
		SetKeys(text, 2, "");  // String, (table) & math functions
		SetKeys(text, 3, "");  // (coroutines), I/O & system facilities
		SetKeys(text, 4, "");  // user1
		SetKeys(text, 5, "");  // user2
		SetKeys(text, 6, "");  // user3
		SetKeys(text, 7, "");  // user4
	}
};

class YamlLanguage : public Language
{
public:

	YamlLanguage()
		: Language(_("YAML"), wxSTC_LEX_YAML)
	{
		AddExtension(".yaml");
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_TELETYPE));

		SetStyle(text, wxSTC_YAML_DEFAULT, settings.current_theme.default_style, true);
		SetStyle(text, wxSTC_YAML_COMMENT, settings.current_theme.style_comment);
		SetStyle(text, wxSTC_YAML_IDENTIFIER, settings.current_theme.style_identifier);
		SetStyle(text, wxSTC_YAML_KEYWORD, settings.current_theme.style_keyword);
		SetStyle(text, wxSTC_YAML_NUMBER, settings.current_theme.yaml_number);
		SetStyle(text, wxSTC_YAML_REFERENCE, settings.current_theme.yaml_reference);
		SetStyle(text, wxSTC_YAML_DOCUMENT, settings.current_theme.yaml_document);
		SetStyle(text, wxSTC_YAML_TEXT, settings.current_theme.yaml_text);
		SetStyle(text, wxSTC_YAML_ERROR, settings.current_theme.yaml_error);
		SetStyle(text, wxSTC_YAML_OPERATOR, settings.current_theme.style_operator);

		SetKeys(text, 0, "");  // keywords
	}
};

class JsonLanguage : public Language
{
public:

	JsonLanguage()
		: Language(_("JSON"), wxSTC_LEX_JSON)
	{
		AddExtension(".json");
		AddExtension(".jsonc");
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
		const auto& unknown = settings.current_theme.default_style;

		SetProperty(text, "lexer.json.escape.sequence", "1"); //  Set to 1 to enable highlighting of escape sequences in strings
		SetProperty(text, "lexer.json.allow.comments", "1"); // 1 Set to 1 to enable highlighting of line/block comments in JSON
		SetProperty(text, "fold", settings.foldEnable ? "1" : "0");
		SetProperty(text, "fold.compact", settings.foldCompact ? "1" : "0");
		
		SetStyle(text, wxSTC_JSON_DEFAULT, settings.current_theme.default_style, true);
		SetStyle(text, wxSTC_JSON_BLOCKCOMMENT, settings.current_theme.style_comment);
		SetStyle(text, wxSTC_JSON_LINECOMMENT, settings.current_theme.style_comment);
		SetStyle(text, wxSTC_JSON_COMPACTIRI , unknown);
		SetStyle(text, wxSTC_JSON_ERROR, settings.current_theme.yaml_error);
		SetStyle(text, wxSTC_JSON_ESCAPESEQUENCE, unknown);
		SetStyle(text, wxSTC_JSON_KEYWORD, settings.current_theme.style_keyword);
		SetStyle(text, wxSTC_JSON_LDKEYWORD, settings.current_theme.style_keyword);
		SetStyle(text, wxSTC_JSON_NUMBER, settings.current_theme.style_number);
		SetStyle(text, wxSTC_JSON_OPERATOR, settings.current_theme.style_operator);
		SetStyle(text, wxSTC_JSON_PROPERTYNAME, settings.current_theme.style_identifier);
		SetStyle(text, wxSTC_JSON_STRING, settings.current_theme.style_string);
		SetStyle(text, wxSTC_JSON_STRINGEOL, settings.current_theme.style_string_eol);
		SetStyle(text, wxSTC_JSON_URI, unknown);

		SetKeys(text, 0, "");  // keywords
		SetKeys(text, 1, "");  // ld keywords
	}
};

class NullLanguage : public Language
{
public:

	NullLanguage()
		: Language(_("NULL"), wxSTC_LEX_NULL)
	{
	}

	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) override
	{
	}
};
#endif

wxString Languages::GetFilePattern()
{
	wxString ret = "All files (*.*)|*.*";

	// need to loop from back to front to get the LanguageList in order for displaying
	// since we are adding 'back to front'
	for (auto l = languages.rbegin(); l != languages.rend(); ++l)
	{
		const Language& lang = *l;

		wxString patterns;

		for (const auto& elem: lang.file_patterns_)
		{
			// if the pattern starts with a dot, assume it's a extension and we
			// need a star, if not we need to match the whole file
			const wxString patt = elem.StartsWith(".") ? "*" + elem : elem;

			// build a *.txt;*.pdf list
			if (patterns.IsEmpty())
			{
				patterns = patt;
			}
			else
			{
				patterns += ";" + patt;
			}
		}

		const auto filter_name = lang.language_name_ + " files (" + patterns + ")|" + patterns;
	

		ret = filter_name + "|" + ret;
	}
	return ret;
}

const Language* Languages::DetermineLanguage(const Fil& filepath)
{
	for (const auto& lang: languages)
	{
		if (lang.MatchPattern(filepath))
		{
			return &lang;
		}
	}
	return &null_language;
}

Language* Languages::GetNullLanguage()
{
	return &null_language;
}
