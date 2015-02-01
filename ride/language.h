#ifndef RIDE_LANGUAGE_H
#define RIDE_LANGUAGE_H

#include <wx/wx.h>
#include <vector>
#include <set>

class wxStyledTextCtrl;
class Settings;

class Language {
public:
  // internal
  bool matchPattern(const wxString& file) const;

  // internal
  Language& operator()(const wxString& ext);

  void style(wxStyledTextCtrl* text, const Settings& settings);
protected:
  Language(const wxString& name, int style);
  virtual void dostyle(wxStyledTextCtrl* text, const Settings& settings) = 0;
  void SetProp(wxStyledTextCtrl* text, const wxString& name, const wxString& value);
  void SetKeys(wxStyledTextCtrl* text, int id, const wxString& keywords);
private:
  wxString language_name;
  int lexstyle;
  std::vector<wxString> patterns;
#ifdef _DEBUG
  std::set<wxString> props;
  std::set<int> used_keywords;
#endif
};

Language* DetermineLanguage(const wxString& filepath);

#endif  // RIDE_LANGUAGE_H