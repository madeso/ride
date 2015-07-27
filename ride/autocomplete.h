#ifndef RIDE_AUTOCOMPLETE_H
#define RIDE_AUTOCOMPLETE_H

#include "ride/wx.h"
class Language;

enum class ShowAutoCompleteAction {
  // no forcing at all
  NO_FORCE,
  // simple forcing
  FORCE_SIMPLE,
  // forcing with no auto-hide
  FORCE_KEEP
};

class wxStyledTextCtrl;

void SetupScintillaAutoCompleteImages(wxStyledTextCtrl* stc);
void Autocomplete(wxStyledTextCtrl* text, Language* current_language,
                  const wxString& filename, const wxString& root_folder,
                  wxWindow* self, ShowAutoCompleteAction action);

#endif  // RIDE_AUTOCOMPLETE_H
