#include "ride/wx.h"
#include "ride/filepropertiesdlg.h"
#include "ride/fileedit.h"
#include "ride/wxutils.h"

#include <map>
#include <vector>

//////////////////////////////////////////////////////////////////////////

#include "ride/generated/ui.h"
class FilePropertiesDlg : public ui::FileProperties
{
public:
  FilePropertiesDlg(FileEdit* parent, wxStyledTextCtrl* ctrl);

protected:
  void OnChangeEncoding(wxCommandEvent& event);
  void OnChangeLineEnding(wxCommandEvent& event);

private:
  void UpdateGui();

private:
  wxStyledTextCtrl* ctrl_;
};

void ShowFilePropertiesDlg(FileEdit* parent, wxStyledTextCtrl* ctrl)
{
  FilePropertiesDlg dlg(parent, ctrl);
  dlg.ShowModal();
}

//////////////////////////////////////////////////////////////////////////

class StringIntConverter {
private:
  typedef std::vector<int> Ints;
  typedef std::map<int, wxString> IntsToStrings;
public:
  StringIntConverter& Add(int i, const wxString& str) {
    strings_.Add(str);
    ints_.push_back(i);
    intstostrings_.insert(IntsToStrings::value_type(i, str));
    return *this;
  }

  const wxArrayString& strings() const {
    return strings_;
  }

  int GetInt(int index) const {
    return ints_[index];
  }

  size_t GetIndex(size_t i) const {
    for (size_t index = 0; index < ints_.size(); ++index) {
      if (ints_[index] == i) return index;
    }
    return 0;
  }

  wxString ToString(int i) const {
    IntsToStrings::const_iterator ret = intstostrings_.find(i);
    if (ret == intstostrings_.end()) return "Unknown";
    else return ret->second;
  }

private:
  wxArrayString strings_;
  Ints ints_;
  IntsToStrings intstostrings_;
};

const StringIntConverter& EOLString() {
  static StringIntConverter sic = StringIntConverter()
    .Add(wxSTC_EOL_CR, wxT("CR (Unix)"))
    .Add(wxSTC_EOL_CRLF, wxT("CRLF (Windows)"))
    .Add(wxSTC_EOL_LF, wxT("CR (Macintosh)"));
  return sic;
}

const StringIntConverter& CodePageString() {
  static StringIntConverter sic = StringIntConverter()
    .Add(wxSTC_CP_UTF8, "UTF-8")
    .Add(wxSTC_CHARSET_ANSI, "Ansi")
    .Add(wxSTC_CHARSET_DEFAULT, "Default")
    .Add(wxSTC_CHARSET_BALTIC, "Baltic")
    .Add(wxSTC_CHARSET_CHINESEBIG5, "Chinesebig5")
    .Add(wxSTC_CHARSET_EASTEUROPE, "Easteurope")
    .Add(wxSTC_CHARSET_GB2312, "Gb2312")
    .Add(wxSTC_CHARSET_GREEK, "Greek")
    .Add(wxSTC_CHARSET_HANGUL, "Hangul")
    .Add(wxSTC_CHARSET_MAC, "Mac")
    .Add(wxSTC_CHARSET_OEM, "Oem")
    .Add(wxSTC_CHARSET_RUSSIAN, "Russian")
    .Add(wxSTC_CHARSET_CYRILLIC, "Cyrillic")
    .Add(wxSTC_CHARSET_SHIFTJIS, "Shiftjis")
    .Add(wxSTC_CHARSET_SYMBOL, "Symbol")
    .Add(wxSTC_CHARSET_TURKISH, "Turkish")
    .Add(wxSTC_CHARSET_JOHAB, "Johab")
    .Add(wxSTC_CHARSET_HEBREW, "Hebrew")
    .Add(wxSTC_CHARSET_ARABIC, "Arabic")
    .Add(wxSTC_CHARSET_VIETNAMESE, "Vietnamese")
    .Add(wxSTC_CHARSET_THAI, "Thai")
    .Add(wxSTC_CHARSET_8859_15, "8859_15");
  return sic;
}

FilePropertiesDlg::FilePropertiesDlg(FileEdit* parent, wxStyledTextCtrl* ctrl) : ::ui::FileProperties(parent, wxID_ANY), ctrl_(ctrl) {
  uiFileName->SetLabel(parent->filename());
  uiLanguage->SetLabelText(parent->GetLanguageName());
  UpdateGui();
}

void FilePropertiesDlg::UpdateGui() {
  uiEncoding->SetLabelText(CodePageString().ToString(ctrl_->GetCodePage()));
  uiLineEndings->SetLabelText(EOLString().ToString(ctrl_->GetEOLMode()));
}

// remove the encoding part?
void FilePropertiesDlg::OnChangeEncoding(wxCommandEvent& event) {
#ifdef wxUSE_UNICODE
  ShowInfo(this, "Unable to change encoding in unicode build", "Unicode only");
#else
  wxSingleChoiceDialog dlg(this, "Select new text encoding", "Text encoding", CodePageString().strings());
  if (dlg.ShowModal() != wxID_OK) return;
  const int selected = dlg.GetSelection();
  const int new_encoding = CodePageString().GetInt(selected);
  
  ctrl_->SetCodePage(new_encoding);
  UpdateGui();
#endif
}

void FilePropertiesDlg::OnChangeLineEnding(wxCommandEvent& event) {
  wxSingleChoiceDialog dlg(this, "Select new line ending", "Line ending?", EOLString().strings());
  dlg.SetSelection(EOLString().GetIndex(ctrl_->GetEOLMode()));
  if (dlg.ShowModal() != wxID_OK) return;
  const int selected = dlg.GetSelection();
  const int new_ending = EOLString().GetInt(selected);

  ctrl_->SetEOLMode(new_ending);
  ctrl_->ConvertEOLs(new_ending);
  UpdateGui();
}
