#pragma once

#include <vector>
#include <set>
#include <memory>

#include "ride/settings.h"

class wxStyledTextCtrl;
struct Fil;

using KeywordList = std::vector<wxString>;

class Style;

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

	bool IsKeyword(const wxString word) const;

	void WarnAboutProperties(wxStyledTextCtrl* text) const;
	void WarnAboutKeywords(const wxStyledTextCtrl* text) const;
	void StyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings);

	const std::vector<wxString>& GetKeywords() const;

	void SetProperty(wxStyledTextCtrl* text, const wxString& name, const wxString& value);


	
	void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings);
	void SetKeys(wxStyledTextCtrl* text, unsigned int id, const wxString& keywords);

	wxString language_name_;
	int lexer_style_;

	// file pattern could be both a extension '.txt' or that the file must be named 'CMakeLists.txt'
	std::vector<wxString> file_patterns_;
	std::unordered_map<int, KeywordList> keywords;
	std::unordered_map<wxString, wxString> properties;
	std::unordered_map<int, std::optional<Style>> styles;
#ifdef _DEBUG
	// todo(Gustav): move to local variables
	std::set<wxString> used_properties_;
	std::set<unsigned int> used_keywords_;
#endif
};

class Languages
{
public:
	Language* GetNullLanguage();
	const Language* DetermineLanguage(const Fil& filepath);
	wxString GetFilePattern();

	Language null_language;
	std::vector<Language> languages;
};

