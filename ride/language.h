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

  // internal
  // todo: move to protected/private
  Language& operator()(const wxString& ext);

  void StyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings);
  wxString GetFilePattern() const;
protected:
  Language(const wxString& name, int style);
  virtual void DoStyleDocument(wxStyledTextCtrl* text, const ride::Settings& settings) = 0;
  void SetProp(wxStyledTextCtrl* text, const wxString& name, const wxString& value);
  void SetKeys(wxStyledTextCtrl* text, unsigned int id, const wxString& keywords);
private:
  wxString language_name_;
  int lexer_style_;
  std::vector<wxString> file_patterns_;
#ifdef _DEBUG
  std::set<wxString> used_properties_;
  std::set<unsigned int> used_keywords_;
#endif
};

Language* DetermineLanguage(const wxString& filepath);
wxString GetFilePattern();

#endif  // RIDE_LANGUAGE_H