#ifndef RIDE_AUTOCOMPLETE_H
#define RIDE_AUTOCOMPLETE_H

#include "ride/wx.h"
class Language;

enum class ShowAutoCompleteAction {
  NO_FORCE // no forcing at all
  ,FORCE_SIMPLE // simple forcing
  ,FORCE_KEEP // forcing with no auto-hide
};

class wxStyledTextCtrl;

void SetupScintillaAutoCompleteImages(wxStyledTextCtrl* stc);
void Autocomplete(wxStyledTextCtrl* text, Language* current_language, const wxString& filename, const wxString& root_folder, wxWindow* self, ShowAutoCompleteAction action);

#endif  // RIDE_AUTOCOMPLETE_H
