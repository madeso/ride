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

bool IsKeyword(const ride::Language& lang, int group, const wxString& word)
{
	if (group >= lang.keywords.size()) return false;
	const auto& keyword_list = lang.keywords[group];
	return keyword_list.contains(word);
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

struct LexerLookup
{
	int lexer;
	std::unordered_map<wxString, int> bindings;
};
using AllLexerLookups = std::unordered_map<wxString, LexerLookup>;

const AllLexerLookups& GetAllLexerLookups()
{
	static const AllLexerLookups all_lexer_lookups = []() -> AllLexerLookups
	{
		AllLexerLookups ret;

		ret["null"] = {
			.lexer = wxSTC_LEX_NULL,
		    .bindings = {
				{"default", wxSTC_STYLE_DEFAULT}
		    }
		};

		ret["cpp"] = {
			.lexer = wxSTC_LEX_CPP,
			.bindings = {
				{"default", wxSTC_C_DEFAULT},
				{"comment", wxSTC_C_COMMENT},
				{"commentline", wxSTC_C_COMMENTLINE},
				{"commentdoc", wxSTC_C_COMMENTDOC},
				{"number", wxSTC_C_NUMBER},
				{"word", wxSTC_C_WORD},
				{"string", wxSTC_C_STRING},
				{"character", wxSTC_C_CHARACTER},
				{"uuid", wxSTC_C_UUID},
				{"preprocessor", wxSTC_C_PREPROCESSOR},
				{"operator", wxSTC_C_OPERATOR },
				{"identifier", wxSTC_C_IDENTIFIER },
				{"stringeol", wxSTC_C_STRINGEOL },
				{"verbatim", wxSTC_C_VERBATIM },
				{"regex", wxSTC_C_REGEX },
				{"commentlinedoc", wxSTC_C_COMMENTLINEDOC },
				{"word2", wxSTC_C_WORD2 },
				{"commentdockeyword", wxSTC_C_COMMENTDOCKEYWORD },
				{"commentdockeyworderror", wxSTC_C_COMMENTDOCKEYWORDERROR },
				{"globalclass", wxSTC_C_GLOBALCLASS },
				{"stringraw", wxSTC_C_STRINGRAW },
				{"tripleverbatim", wxSTC_C_TRIPLEVERBATIM },
				{"hashquotedstring", wxSTC_C_HASHQUOTEDSTRING },
				{"preprocessorcomment", wxSTC_C_PREPROCESSORCOMMENT },
				{"preprocessorcommentdoc", wxSTC_C_PREPROCESSORCOMMENTDOC },
				{"userliteral", wxSTC_C_USERLITERAL },
				{"taskmarker", wxSTC_C_TASKMARKER },
				{"escapesequence", wxSTC_C_ESCAPESEQUENCE }
			}
		};
		return ret;
	}();
	return all_lexer_lookups;
}

const LexerLookup& GetLexerLookup(const wxString& lexer_style)
{
	const auto& lookups = GetAllLexerLookups();
	const auto it = lookups.find(lexer_style);
	if (it == lookups.end())
	{
		wxLogError(_("Unknown lexer style: %s"), lexer_style);
		static const LexerLookup empty_lookup = {.lexer = wxSTC_LEX_NULL, .bindings = {}};
		return empty_lookup;
	}
	return it->second;
}

void StyleDocument(const ride::Language& lang, wxStyledTextCtrl* text, const ride::Settings& settings)
{
	PropsAndKeywords props_and_keywords;

	const auto& lookup = GetLexerLookup(lang.lexer_style);
	text->SetLexer(lookup.lexer);

	auto* theme = settings.find_current_theme();
	if (theme)
	{
		for (const auto& [sci_name, style_name]: lang.bindings)
		{
			const auto found_style = theme->GetStyle(style_name);
			if (! found_style.has_value())
			{
				wxLogWarning(
					_("Style %s for %s was not found."),
					style_name,
					lang.language_name
				);
				continue;
			}

			const auto sci_id = lookup.bindings.find(sci_name);
			if (sci_id != lookup.bindings.end())
			{
				SetStyle(text, sci_id->second, *found_style, *theme);
			}
			else
			{
				wxLogWarning(
					_("Style %s for %s was set, but does not exist in lexer %s."),
					style_name,
					lang.language_name,
					lang.lexer_style
				);
			}
		}
	}

	for (const auto& [name, value]: lang.properties)
	{
		props_and_keywords.SetProperty(text, name, settings.LookupSetting(value));
	}
	for (unsigned int kwclass = 0; kwclass < lang.keywords.size(); kwclass+=1)
	{
		props_and_keywords.SetKeys(text, kwclass, keywords_to_string(lang.keywords[kwclass]));
	}

#ifdef _DEBUG
	props_and_keywords.WarnAboutProperties(text, lang.language_name);
	props_and_keywords.WarnAboutKeywords(text, lang.language_name);
#endif
}

bool MatchPattern(const ride::Language& lang, const Fil& file)
{
	for (const auto& elem: lang.file_patterns)
	{
		if (file.ends_with(elem))
		{
			return true;
		}
	}
	return false;
}

wxString GetFilePattern(const ride::Languages& self)
{
	wxString ret = "All files (*.*)|*.*";

	// need to loop from back to front to get the LanguageList in order for displaying
	// since we are adding 'back to front'
	for (auto l = self.languages.rbegin(); l != self.languages.rend(); ++l)
	{
		const ride::Language& lang = *l;

		wxString patterns;

		for (const auto& elem: lang.file_patterns)
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

		const auto filter_name = lang.language_name + " files (" + patterns + ")|" + patterns;
	

		ret = filter_name + "|" + ret;
	}
	return ret;
}

const ride::Language* DetermineLanguage(const ride::Languages& self, const Fil& filepath)
{
	for (const auto& lang: self.languages)
	{
		if (MatchPattern(lang, filepath))
		{
			return &lang;
		}
	}
	return &self.null_language;
}
