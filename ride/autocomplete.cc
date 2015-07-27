// Copyright (2015) Gustav

#include "ride/autocomplete.h"

#include "ride/wx.h"
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/uri.h>
#include <wx/numdlg.h>
#include <wx/tokenzr.h>

#include <vector>
#include <set>
#include <cassert>

#include "ride/compilermessage.h"
#include "ride/filepropertiesdlg.h"
#include "ride/mainwindow.h"
#include "ride/fileedit.h"
#include "ride/wxutils.h"
#include "ride/finddlg.h"
#include "ride/cmdrunner.h"

#include "ride/resources/icons.h"
#include "ride/stcutils.h"

enum AutoIcon {
  AI_None = -1,
  AI_Snippet = 0,
  AI_Keyword,
  AI_Function,
  AI_Crate,
  AI_Let,
  AI_Struct,
  AI_StructField,
  AI_Module,
  AI_FnArg
};

class WordEntry {
 public:
  explicit WordEntry(const wxString& aname, AutoIcon aicon = AI_None)
      : name(aname), icon(aicon) {}

  wxString name;
  AutoIcon icon;

  const wxString GetName() const {
    if (icon == AI_None) {
      return name;
    } else {
      const wxString ret = wxString::Format("%s?%d", name, icon);
      return ret;
    }
  }
};
bool operator<(const WordEntry& lhs, const WordEntry& rhs) {
  if (lhs.name == rhs.name) {
    return lhs.icon < rhs.icon;
  }
  return lhs.name < rhs.name;
}

class WordEntryList {
 public:
  const wxString start_string_;
  const bool start_string_is_empty_;
  const bool ignore_case_;
  std::set<WordEntry> list_;

  WordEntryList(const wxString& start, bool ignore_case)
      : start_string_(ignore_case ? wxString(start).MakeLower() : start),
        start_string_is_empty_(start.IsEmpty()),
        ignore_case_(ignore_case) {}

  void Add(const WordEntry& entry) {
    if (ShouldAdd(entry)) {
      ForceAdd(entry);
    }
  }

  void ForceAdd(const WordEntry& entry) {
    if (list_.find(entry) == list_.end()) {
      list_.insert(entry);
    }
  }

  wxString ToString() const {
    wxString ret;
    for (const WordEntry& tok : list_) {
      if (ret.IsEmpty()) {
        ret = tok.GetName();
      } else {
        ret += ";" + tok.GetName();
      }
    }

    return ret;
  }

  bool ShouldAdd(const WordEntry& element) const {
    if (start_string_is_empty_) {
      return true;
    }
    const wxString element_name =
        ignore_case_ ? wxString(element.name).MakeLower() : element.name;
    const bool add = element_name.StartsWith(start_string_);
    return add;
  }

  bool IsEmpty() const { return list_.empty(); }
};

void RegisterImage(wxStyledTextCtrl* t, AutoIcon icon, const char** xpm) {
  wxBitmap bitmap(xpm, wxBITMAP_TYPE_XPM);
  wxImage img = bitmap.ConvertToImage();
  // t->RegisterImage(icon, bitmap);
  const int w = bitmap.GetWidth();
  const int h = bitmap.GetHeight();
  t->RGBAImageSetWidth(w);
  t->RGBAImageSetHeight(h);
  wxImage mask;
  if (bitmap.GetMask()) {
    mask = bitmap.GetMask()->GetBitmap().ConvertToImage();
  }
  std::unique_ptr<unsigned char[]> pixels(new unsigned char[w * h * 4]);
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const int index = (y * h + x) * 4;
      pixels[index + 0] = img.GetRed(x, y);
      pixels[index + 1] = img.GetGreen(x, y);
      pixels[index + 2] = img.GetBlue(x, y);
      pixels[index + 3] = img.HasAlpha()
                              ? img.GetAlpha(x, y)
                              : (bitmap.GetMask() ? mask.GetRed(x, y) : 255);
    }
  }

  // Register image kills wxWidgets, so we have to manually convert to rgba and
  // use that instead
  t->RegisterRGBAImage(icon, pixels.get());
}

void SetupScintillaAutoCompleteImages(wxStyledTextCtrl* t) {
  t->ClearRegisteredImages();
  RegisterImage(t, AI_Snippet, icon_ac_snippet_xpm);
  RegisterImage(t, AI_Keyword, icon_ac_keyword_xpm);

  RegisterImage(t, AI_Function, icon_ac_function_xpm);
  RegisterImage(t, AI_Crate, icon_ac_crate_xpm);
  RegisterImage(t, AI_Let, icon_ac_let_xpm);
  RegisterImage(t, AI_Struct, icon_ac_struct_xpm);
  RegisterImage(t, AI_StructField, icon_ac_structfield_xpm);
  RegisterImage(t, AI_Module, icon_ac_module_xpm);
  RegisterImage(t, AI_FnArg, icon_ac_fnarg_xpm);
}

void AddLocalVariables(WordEntryList* wordlist, wxStyledTextCtrl* text) {
  assert(wordlist);
  assert(text);

  int lines_left = 30;

  // start on -1 since we don't want to autocomplete local variables
  const int start_line = text->GetCurrentLine() - 1;

  for (int current_line = start_line; current_line >= 0; current_line -= 1) {
    lines_left -= 1;
    if (lines_left < 0) return;
    const wxString line = text->GetLine(current_line).Trim(false);
    if (line.StartsWith("let ")) {
      const int equal = line.First('=');
      if (equal == -1) continue;
      const wxString temp = line.SubString(0, equal - 1).Trim();
      const int space = temp.find_last_of(' ');
      if (space == -1) continue;
      const wxString varname = temp.Right(temp.length() - space - 1);
      wordlist->Add(WordEntry(varname));
      continue;
    }
    if (line.StartsWith("pub ")) return;
    if (line.StartsWith("fn ")) return;
    if (line.StartsWith("struct ")) return;
    if (line.StartsWith("impl ")) return;
  }
}

wxString GetIndentationAsString(wxStyledTextCtrl* text, int line) {
  const int indent = text->GetLineIndentation(line);
  if (indent == 0) return "";
  if (text->GetUseTabs()) {
    return wxString(indent, '\t');
  } else {
    return wxString(indent, ' ');
  }
}

AutoIcon ParseRacerType(const wxString& aname) {
  const wxString name = wxString(aname).MakeLower();
  if (name == "function") {
    return AI_Function;
  }
  if (name == "crate") {
    return AI_Crate;
  }
  if (name == "let") {
    return AI_Let;
  }
  if (name == "struct") {
    return AI_Struct;
  }
  if (name == "structfield") {
    return AI_StructField;
  }
  if (name == "module") {
    return AI_Module;
  }
  if (name == "fnarg") {
    return AI_FnArg;
  }

  return AI_None;
}

wxString RunRacer(WordEntryList& wordlist, const wxString& filename_,
                  wxStyledTextCtrl* text_, const wxString& root_folder) {
  // save temp file
  wxFileName target(filename_);
  target.SetExt("racertmp");
  const wxString path = target.GetFullPath();
  {
    wxFile f(path, wxFile::OpenMode::write);
    f.Write(text_->GetText());
  }
  // run racer
  wxString output;
  const int linenum = text_->GetCurrentLine() + 1;
  const int charnum =
      text_->GetCurrentPos() - text_->PositionFromLine(text_->GetCurrentLine());
  const wxString cmd =
      wxString::Format("racer complete %d %d \"%s\"", linenum, charnum, path);
  CmdRunner::Run(root_folder, cmd, &output);
  // parse output
  const std::vector<wxString> o = Split(output, "\n");
  for (const wxString& l : o) {
    const wxString MATCH = "MATCH ";
    if (l.StartsWith(MATCH)) {
      const std::vector<wxString> args = Split(l.substr(MATCH.length()), ",");
      wordlist.ForceAdd(WordEntry(args[0], ParseRacerType(args[4])));
    }
  }
  // delete temp file
  wxRemoveFile(path);

  return output;
}

void Autocomplete(wxStyledTextCtrl* text, Language* current_language,
                  const wxString& filename, const wxString& root_folder,
                  wxWindow* self, ShowAutoCompleteAction action) {
  const bool ignore_case = true;
  const int word_wait_chars = 3;
  const bool racer = true;

  // only autocomplete rust files
  if (filename.EndsWith(".rs") == false) {
    return;
  }

  const int pos = text->GetCurrentPos();
  const int start_position = text->WordStartPosition(pos, true);
  const wxString word =
      text->GetRange(start_position, pos).Trim(true).Trim(false);
  const int length = word.Length();  //  pos - start_position;
  assert(length >= 0);

  const bool is_space_before =
      text->GetRange(start_position - 1, start_position)
          .Trim(true)
          .Trim(false)
          .IsEmpty();

  if (action != ShowAutoCompleteAction::NO_FORCE ||
      (text->AutoCompActive() == false && length >= word_wait_chars)) {
    WordEntryList wordlist(word, ignore_case);

    if (is_space_before) {
      if (current_language) {
        const auto kw = current_language->GetKeywords();
        for (const wxString& k : kw) {
          wordlist.Add(WordEntry(k, AI_Keyword));
        }
      }
    }

    if (racer == false) {
      if (is_space_before) {
        AddLocalVariables(&wordlist, text);
      }
    }

    if (is_space_before) {
      // TODO allow user to save theese templates
      wordlist.Add(WordEntry(wxString(80, '/'), AI_Snippet));
      const wxString indent =
          GetIndentationAsString(text, text->GetCurrentLine());
      wordlist.Add(
          WordEntry("/**\n" + indent + " * \n" + indent + " **/", AI_Snippet));
      wordlist.Add(WordEntry("/// \n" + indent + "/// \n" + indent + "/// ",
                             AI_Snippet));
    }

    wxString racer_output;
    if (racer) {
      racer_output = RunRacer(wordlist, filename, text, root_folder);
    }

    // setting it here instead of when spawning eats the entered '.' but
    // displays the AC instead of autocompleteing directly
    text->AutoCompSetFillUps("()<>.:;{}[] ");
    if (wordlist.IsEmpty()) {
      if (action == ShowAutoCompleteAction::FORCE_KEEP) {
        const wxString message = wxString::Format(
            "No autocomplete suggestions found, racer output was:\n%s",
            racer_output);
        ShowInfo(self, message, "No autocomplete suggestions found!");
      }
    } else {
      text->AutoCompSetAutoHide(action != ShowAutoCompleteAction::FORCE_KEEP);
      text->AutoCompSetIgnoreCase(ignore_case);
      text->AutoCompSetSeparator(';');
      const wxString wordliststr = wordlist.ToString();
      text->AutoCompShow(length, wordliststr);
      text->AutoCompSetFillUps("");
    }
  }
}
