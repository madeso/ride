// Copyright (2015) Gustav

#include "ride/wxutils.h"

#include <ride/wx.h>

#include <wx/listctrl.h>
#include <wx/tokenzr.h>
#include <wx/fontenum.h>

#include <wx/stc/stc.h>

#include "ride_compiler_settings.h"  // NOLINT this is how we include it

void WriteLine(wxStyledTextCtrl* stc, const wxString& str) {
  const int current_pos = stc->GetCurrentPos();
  const int current_line = stc->LineFromPosition(current_pos);
  const int line_count = stc->GetLineCount();
  stc->SetReadOnly(false);
  stc->AppendText(str);
  stc->AppendText(wxT("\n"));
  stc->SetReadOnly(true);
  if (current_line >= line_count - 1) {
    const int new_line = stc->GetLineCount();
    const int new_pos = stc->PositionFromLine(new_line);
    stc->SetSelection(new_pos, new_pos);
    stc->EnsureCaretVisible();
  }
}

void ClearOutput(wxStyledTextCtrl* stc) {
  stc->SetReadOnly(false);
  stc->SetText(wxEmptyString);
  stc->SetReadOnly(true);
}

wxPoint GetContextEventPosition(const wxContextMenuEvent& event) {
  wxPoint ret = event.GetPosition();
  // according to the documentation:
  // http://docs.wxwidgets.org/trunk/classwx_context_menu_event.html#a291e3437b4bf913128ea14e511d161cb
  if (ret == wxDefaultPosition) {
    return wxGetMousePosition();
  } else {
    return ret;
  }
}

void AppendEnabled(wxMenu& menu, int id, const wxString& text, bool enabled) {
  menu.Append(id, text);
  menu.Enable(id, enabled);
}

std::vector<wxString> Split(const wxString& str, const wxString& c) {
  std::vector<wxString> ret;
  wxStringTokenizer tokenizer(str, c);
  while (tokenizer.HasMoreTokens()) {
    wxString token = tokenizer.GetNextToken();
    ret.push_back(token);
  }
  return ret;
}

std::vector<wxString> RemoveEmptyStrings(const std::vector<wxString>& v) {
  std::vector<wxString> ret;
  ret.reserve(v.size());
  for (const wxString& s : v) {
    if (s.IsEmpty())
      continue;
    else
      ret.push_back(s);
  }
  return ret;
}

void SetSelection(wxListCtrl* list, WXID item, bool select) {
  if (select) {
    list->SetItemState(item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
  } else {
    list->SetItemState(item, 0, wxLIST_STATE_SELECTED);
  }
}

class ListCtrlIterator {
 public:
  ListCtrlIterator(wxListCtrl* list, int geom, int state, WXID index)
      : list_(list), geom_(geom), state_(state), index_(index) {}

  void operator++() { next(); }
  void operator++(int unused) { next(); }
  void next() { index_ = list_->GetNextItem(index_, geom_, state_); }

  WXID operator*() { return index_; }

  bool operator!=(const ListCtrlIterator& o) {
    return !(list_ == o.list_ && geom_ == o.geom_ && state_ == o.state_ &&
             index_ == o.index_);
  }

 private:
  wxListCtrl* list_;
  int geom_;
  int state_;
  WXID index_;
};

class IterateOverListCtrl {
 public:
  explicit IterateOverListCtrl(wxListCtrl* list, int geom, int state)
      : list_(list), geom_(geom), state_(state) {}

  ListCtrlIterator begin() {
    ListCtrlIterator it(list_, geom_, state_, -1);
    it.next();
    return it;
  }
  ListCtrlIterator end() { return ListCtrlIterator(list_, geom_, state_, -1); }

 private:
  wxListCtrl* list_;
  int geom_;
  int state_;
};

std::vector<WXID> GetSelection(wxListCtrl* listctrl) {
  std::vector<WXID> ret;
  IterateOverListCtrl it(listctrl, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  for (WXID item : it) {
    ret.push_back(item);
  }
  return ret;
}

void ClearSelection(wxListCtrl* listctrl) {
  IterateOverListCtrl it(listctrl, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  for (WXID item : it) {
    SetSelection(listctrl, item, false);
  }
}

DialogResult CDL(int dl) {
  switch (dl) {
    case wxID_YES:
      return DialogResult::YES;
    case wxID_NO:
      return DialogResult::NO;
    case wxID_CANCEL:
      return DialogResult::CANCEL;
    default:
      assert(0 && "Invalid dialog result");
      return DialogResult::CANCEL;
  }
}

DialogResult ShowYesNo(wxWindow* self, const wxString& caption,
                       const wxMessageDialogBase::ButtonLabel& yes_button,
                       const wxMessageDialogBase::ButtonLabel& no_button,
                       const wxString& title_ok, const wxString title_error) {
  wxMessageDialog dlg(self, _(""), caption, wxYES_NO | wxICON_QUESTION);
  const bool label_change_ok = dlg.SetYesNoLabels(yes_button, no_button);
  dlg.SetMessage(label_change_ok ? title_ok : title_error);
  return CDL(dlg.ShowModal());
}

DialogResult ShowYesNoCancel(
    wxWindow* self, const wxString& caption,
    const wxMessageDialogBase::ButtonLabel& yes_button,
    const wxMessageDialogBase::ButtonLabel& no_button,
    const wxMessageDialogBase::ButtonLabel& cancel_button,
    const wxString& title_ok, const wxString title_error) {
  wxMessageDialog dlg(self, _(""), caption,
                      wxYES_NO | wxCANCEL | wxICON_QUESTION);
  const bool label_change_ok =
      dlg.SetYesNoCancelLabels(yes_button, no_button, cancel_button);
  dlg.SetMessage(label_change_ok ? title_ok : title_error);
  return CDL(dlg.ShowModal());
}

void ShowInfo(wxWindow* self, const wxString& message, const wxString& title) {
  wxMessageBox(message, title, wxICON_INFORMATION | wxOK | wxCENTRE, self);
}

void ShowWarning(wxWindow* self, const wxString& message,
                 const wxString& title) {
  wxMessageBox(message, title, wxICON_WARNING | wxOK | wxCENTRE, self);
}

void ShowError(wxWindow* self, const wxString& message, const wxString& title) {
  wxMessageBox(message, title, wxICON_ERROR | wxOK | wxCENTRE, self);
}

wxString ToShortString(const wxString& str, int max_length) {
  const wxString dots = wxT("...");
  if (str.length() > max_length + dots.length()) {
    return str.Left(max_length) + dots;
  } else {
    return str;
  }
}

int FindStcText(wxStyledTextCtrl* stc, int minPos, int maxPos,
                const wxString& text, int flags, int* findEnd) {
  assert(stc);
#ifdef USE_WXWIDGETS_LEGACY_FINDTEXT
  const int ret = stc->FindText(minPos, maxPos, text, flags);
  if (findEnd) {
    *findEnd = ret;
  }
  return ret;
#else
  return stc->FindText(minPos, maxPos, text, flags, findEnd);
#endif
}

class FontLister : public wxFontEnumerator {
 public:
  std::vector<wxString> fonts;
  bool OnFacename(const wxString& font) override {
    // The version of the font with the '@' is a version optimized for writing
    // in the vertical direction.
    // https://groups.google.com/forum/#!topic/wx-users/3hjrhPlSULI
    if (font.StartsWith("@") == false) {
      fonts.push_back(font);
    }
    return true;
  }
};

std::vector<wxString> ListFonts(bool show_only_fixed_size) {
  FontLister all_fonts;
  all_fonts.EnumerateFacenames(wxFONTENCODING_SYSTEM, show_only_fixed_size);
  return all_fonts.fonts;
}
