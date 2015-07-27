#ifndef RIDE_LANGUAGE_H
#define RIDE_LANGUAGE_H

#include <vector>
#include <set>

#include "ride/settings.h"

class wxStyledTextCtrl;

class Language {
 public:
  // internal
  // todo: move to protected/private
  bool MatchPattern(const wxString& file) const;

  const wxString& language_name() const;

  // internal
  // todo: move to protected/private
  void AddExtension(const wxString& ext);

  bool IsKeyword(const wxString word) const;

  void StyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings);
  wxString GetFilePattern() const;

  const std::vector<wxString>& GetKeywords() const;

 protected:
  Language(const wxString& name, int style);
  virtual void DoStyleDocument(wxStyledTextCtrl* text,
                               const ride::Settings& settings) = 0;
  void SetProp(wxStyledTextCtrl* text, const wxString& name,
               const wxString& value);
  void SetKeys(wxStyledTextCtrl* text, unsigned int id,
               const wxString& keywords);

  std::vector<wxString> keywords_;

 private:
  wxString language_name_;
  int lexer_style_;
  std::vector<wxString> file_patterns_;
#ifdef _DEBUG
  std::set<wxString> used_properties_;
  std::set<unsigned int> used_keywords_;
#endif
};

Language* NullLanguage();
Language* DetermineLanguage(const wxString& filepath);
wxString GetFilePattern();

#endif  // RIDE_LANGUAGE_H