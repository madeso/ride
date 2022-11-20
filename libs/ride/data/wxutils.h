// Copyright (2015) Gustav

#ifndef RIDE_WXUTILS_H_
#define RIDE_WXUTILS_H_

#include <wx/gdicmn.h>
#include <wx/msgdlg.h>
#include <wx/string.h>

#include <vector>

#include "ride/wxid.h"

class wxContextMenuEvent;
class wxMenu;
class wxWindow;
class wxListCtrl;
class wxStyledTextCtrl;

void WriteLine(wxStyledTextCtrl* stc, const wxString& str);
void ClearOutput(wxStyledTextCtrl* stc);

wxPoint GetContextEventPosition(const wxContextMenuEvent& event);
void AppendEnabled(wxMenu& menu, int id, const wxString& text, bool enabled);
std::vector<wxString> Split(const wxString& str, const wxString& c);
std::vector<wxString> RemoveEmptyStrings(const std::vector<wxString>& v);

void SetSelection(wxListCtrl* list, WXID item, bool select);
std::vector<WXID> GetSelection(wxListCtrl* listctrl);
void ClearSelection(wxListCtrl* listctrl);

enum class DialogResult { YES, NO, CANCEL };

DialogResult ShowYesNo(wxWindow* self, const wxString& caption,
                       const wxMessageDialogBase::ButtonLabel& yes_button,
                       const wxMessageDialogBase::ButtonLabel& no_button,
                       const wxString& title_ok, const wxString title_error);

DialogResult ShowYesNoCancel(
    wxWindow* self, const wxString& caption,
    const wxMessageDialogBase::ButtonLabel& yes_button,
    const wxMessageDialogBase::ButtonLabel& no_button,
    const wxMessageDialogBase::ButtonLabel& cancel_button,
    const wxString& title_ok, const wxString title_error);

void ShowInfo(wxWindow* self, const wxString& message, const wxString& title);
void ShowWarning(wxWindow* self, const wxString& message,
                 const wxString& title);
void ShowError(wxWindow* self, const wxString& message, const wxString& title);

wxString ToShortString(const wxString& str, int max_length);

int FindStcText(wxStyledTextCtrl* stc, int minPos, int maxPos,
                const wxString& text, int flags, int* findEnd);

std::vector<wxString> ListFonts(bool show_only_fixed_size);

#endif  // RIDE_WXUTILS_H_
