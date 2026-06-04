#pragma once

#include <vector>
#include <set>
#include <memory>

#include "ride/settings.h"

class wxStyledTextCtrl;
struct Fil;


bool MatchPattern(const ride::Language& lang, const Fil& file);
bool IsKeyword(const ride::Language& lang, int group, const wxString& word);
void StyleDocument(const ride::Language& lang, wxStyledTextCtrl* text, const ride::Settings& settings);

const ride::Language* DetermineLanguage(const ride::Languages& lang, const Fil& filepath);
wxString GetFilePattern(const ride::Languages& lang);
