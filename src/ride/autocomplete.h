#ifndef RIDE_AUTOCOMPLETE_H_
#define RIDE_AUTOCOMPLETE_H_

#include <ride/wx.h>

#include "ride/settings.h"

class Language;

enum class ShowAutoCompleteAction
{
	// no forcing at all
	NO_FORCE,
	// simple forcing
	FORCE_SIMPLE,
	// forcing with no auto-hide
	FORCE_KEEP
};

class wxStyledTextCtrl;

void SetupScintillaAutoCompleteImages(wxStyledTextCtrl* stc);
void Autocomplete(
	const ride::MachineSettings& settings,
	wxStyledTextCtrl* text,
	Language* current_language,
	const wxString& filename,
	const wxString& root_folder,
	wxWindow* self,
	ShowAutoCompleteAction action
);

#endif	// RIDE_AUTOCOMPLETE_H_
