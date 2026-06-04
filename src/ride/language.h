#pragma once

#include <vector>
#include <set>
#include <memory>

#include "ride/settings.h"

class wxStyledTextCtrl;
struct Fil;

using KeywordList = std::set<wxString>;

class Language
{
public:
	Language();

	wxString language_name;
	wxString lexer_style;

	// file pattern could be both a extension '.txt' or that the file must be named 'CMakeLists.txt'
	std::vector<wxString> file_patterns;
	std::vector<KeywordList> keywords;
	std::unordered_map<wxString, wxString> properties;

	// language specific mapping
	std::unordered_map<wxString, wxString> bindings; // scintilla id to style name
};

bool MatchPattern(const Language& lang, const Fil& file);
bool IsKeyword(const Language& lang, int group, const wxString& word);
void StyleDocument(const Language& lang, wxStyledTextCtrl* text, const ride::Settings& settings);

class Languages
{
public:

	Languages();

	Language null_language;
	std::vector<Language> languages;
};

const Language* DetermineLanguage(const Languages& lang, const Fil& filepath);
wxString GetFilePattern(const Languages& lang);
