#ifndef RIDE_LANGUAGE_H
#define RIDE_LANGUAGE_H

#include <wx/wx.h>
#include <vector>

class wxStyledTextCtrl;
class Settings;

class Language {
public:
  bool matchPattern(const wxString& file) const;
  virtual void style(wxStyledTextCtrl* text, const Settings& settings) = 0;
protected:
  std::vector<wxString> patterns;
};

Language* DetermineLanguage(const wxString& filepath);

#endif  // RIDE_LANGUAGE_H