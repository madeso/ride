#ifndef RIDE_WXUTILS_H
#define RIDE_WXUTILS_H

#include <wx/gdicmn.h>
#include <wx/string.h>

class wxContextMenuEvent;
class wxMenu;

wxPoint GetContextEventPosition(const wxContextMenuEvent& event);
void AppendEnabled(wxMenu& menu, int id, const wxString& text, bool enabled);

#endif  // RIDE_WXUTILS_H