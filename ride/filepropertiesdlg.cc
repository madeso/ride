#include "ride/wx.h"
#include "ride/filepropertiesdlg.h"
#include "ride/fileedit.h"

wxString GetEndingString(wxStyledTextCtrl* edit) {
  switch (edit->GetEOLMode()) {
  case wxSTC_EOL_CR: return wxT("CR (Unix)");
  case wxSTC_EOL_CRLF: return wxT("CRLF (Windows)");
  case wxSTC_EOL_LF: return wxT("CR (Macintosh)");
    default:
      return wxT("Unknown");
  }
}

wxString GetEncodingString(wxStyledTextCtrl* edit) {
  switch (edit->GetCodePage()) {
  case wxSTC_CP_UTF8                : return "UTF-8       ";
  case wxSTC_CHARSET_ANSI           : return "Ansi        ";
  case wxSTC_CHARSET_DEFAULT        : return "Default     ";
  case wxSTC_CHARSET_BALTIC         : return "Baltic      ";
  case wxSTC_CHARSET_CHINESEBIG5    : return "Chinesebig5 ";
  case wxSTC_CHARSET_EASTEUROPE     : return "Easteurope  ";
  case wxSTC_CHARSET_GB2312         : return "Gb2312      ";
  case wxSTC_CHARSET_GREEK          : return "Greek       ";
  case wxSTC_CHARSET_HANGUL         : return "Hangul      ";
  case wxSTC_CHARSET_MAC            : return "Mac         ";
  case wxSTC_CHARSET_OEM            : return "Oem         ";
  case wxSTC_CHARSET_RUSSIAN        : return "Russian     ";
  case wxSTC_CHARSET_CYRILLIC       : return "Cyrillic    ";
  case wxSTC_CHARSET_SHIFTJIS       : return "Shiftjis    ";
  case wxSTC_CHARSET_SYMBOL         : return "Symbol      ";
  case wxSTC_CHARSET_TURKISH        : return "Turkish     ";
  case wxSTC_CHARSET_JOHAB          : return "Johab       ";
  case wxSTC_CHARSET_HEBREW         : return "Hebrew      ";
  case wxSTC_CHARSET_ARABIC         : return "Arabic      ";
  case wxSTC_CHARSET_VIETNAMESE     : return "Vietnamese  ";
  case wxSTC_CHARSET_THAI           : return "Thai        ";
  case wxSTC_CHARSET_8859_15        : return "8859_15     ";
  default:
    return wxT("Unknown");
  }
}

FilePropertiesDlg::FilePropertiesDlg(FileEdit* parent, wxStyledTextCtrl* ctrl) : ::ui::FileProperties(parent, wxID_ANY), ctrl_(ctrl) {
  uiFileName->SetLabel(parent->filename());
  uiLanguage->SetLabelText(parent->GetLanguageName());
  uiEncoding->SetLabelText(GetEncodingString(ctrl_).Trim());
  uiLineEndings->SetLabelText(GetEndingString(ctrl_));
}
