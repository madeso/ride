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

	// internal
	// todo: move to protected/private
	bool MatchPattern(const Fil& file) const;

	const wxString& language_name() const;

	// internal
	// todo: move to protected/private
	void AddExtension(const wxString& ext);

	bool IsKeyword(int group, const wxString& word) const;

	void StyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) const;

	wxString language_name_;
	int lexer_style_;

	// file pattern could be both a extension '.txt' or that the file must be named 'CMakeLists.txt'
	std::vector<wxString> file_patterns_;
	std::vector<KeywordList> keywords;
	std::unordered_map<wxString, wxString> properties;

	// language specific mapping
	std::unordered_map<int, wxString> bindings; // scintilla id to style name
};

class Languages
{
public:

	Languages();

	Language* GetNullLanguage();
	const Language* DetermineLanguage(const Fil& filepath);
	wxString GetFilePattern();

	Language null_language;
	std::vector<Language> languages;
};

