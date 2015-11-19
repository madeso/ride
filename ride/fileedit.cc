// Copyright (2015) Gustav

#include "ride/fileedit.h"

#include <ride/wx.h>

#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/numdlg.h>
#include <wx/tokenzr.h>
#include <wx/uri.h>
#include <wx/aui/aui.h>
#include <wx/stc/stc.h>

#include <algorithm>
#include <cassert>
#include <set>
#include <vector>
#include <string>

#include "ride_compiler_settings.h"  // NOLINT

#include "ride/resources/icons.h"

#include "ride/autocomplete.h"
#include "ride/cmdrunner.h"
#include "ride/compilermessage.h"
#include "ride/compileutils.h"
#include "ride/filepropertiesdlg.h"
#include "ride/finddlg.h"
#include "ride/mainwindow.h"
#include "ride/project.h"
#include "ride/stcutils.h"
#include "ride/wxutils.h"
#include "ride/findresultscontrol.h"

wxDEFINE_EVENT(EVENT_UPDATE_SELECTION, wxCommandEvent);

void FileEdit::Undo() {
  if (!text_->CanUndo()) return;
  text_->Undo();
  UpdateTitle();
}

wxString FileEdit::GetLanguageName() {
  if (current_language_)
    return current_language_->language_name();
  else
    return "<none>";
}

void FileEdit::Redo() {
  if (!text_->CanRedo()) return;
  text_->Redo();
  UpdateTitle();
}

void FileEdit::Cut() {
  if (text_->GetReadOnly() ||
      (text_->GetSelectionEnd() - text_->GetSelectionStart() <= 0))
    return;
  text_->Cut();
  UpdateTitle();
}

void FileEdit::Copy() {
  if (text_->GetSelectionEnd() - text_->GetSelectionStart() <= 0) return;
  text_->Copy();
}

void FileEdit::Paste() {
  if (!text_->CanPaste()) return;
  text_->Paste();
  UpdateTitle();
}

void FileEdit::Duplicate() {
  text_->SelectionDuplicate();
  UpdateTitle();
}

void FileEdit::Delete() {
  if (text_->GetReadOnly()) return;
  text_->Clear();
  UpdateTitle();
}

void FileEdit::Find(FindResultsControl* output, const wxString& project) {
  ShowFindDlg(main_, text_->GetSelectedText(), filename_, project, output,
              FindAction::Find, FindScope::File);
}

void FileEdit::Replace(FindResultsControl* output, const wxString& project) {
  ShowFindDlg(main_, text_->GetSelectedText(), filename_, project, output,
              FindAction::Replace, FindScope::File);
}

void FileEdit::FindInFiles(FindResultsControl* output,
                           const wxString& project) {
  ShowFindDlg(main_, text_->GetSelectedText(), filename_, project, output,
              FindAction::Find, FindScope::Project);
}

void FileEdit::ReplaceInFiles(FindResultsControl* output,
                              const wxString& project) {
  ShowFindDlg(main_, text_->GetSelectedText(), filename_, project, output,
              FindAction::Replace, FindScope::Project);
}

wxStyledTextCtrl* FileEdit::GetStc() { return text_; }

void FileEdit::MatchBrace() {
  int start_brace = text_->GetCurrentPos();
  int other_brace = text_->BraceMatch(start_brace);
  if (other_brace == -1) return;

  text_->SetSelection(other_brace, other_brace);
}

void FileEdit::SelectBrace() {
  int start_brace = text_->GetCurrentPos();
  int other_brace = text_->BraceMatch(start_brace);
  if (other_brace == -1) return;

  if (other_brace < start_brace) {
    std::swap(start_brace, other_brace);
  }

  assert(start_brace < other_brace);
  text_->SetSelection(start_brace, other_brace + 1);
}

void FileEdit::GotoLine() {
  const int total_plus_one = text_->GetLineCount() + 1;
  const wxString message =
      wxString::Format("Enter line number(1-%d)", total_plus_one);
  const auto new_line_one_based =
      wxGetNumberFromUser(message, wxEmptyString, "Goto line",
                          text_->GetCurrentLine() + 1, 1, total_plus_one);
  if (new_line_one_based == -1) return;
  const int pos = text_->PositionFromLine(new_line_one_based - 1);
  text_->SetSelection(pos, pos);
  text_->EnsureCaretVisible();
}

void FileEdit::Indent() {
  text_->CmdKeyExecute(wxSTC_CMD_TAB);
  UpdateTitle();
}

void FileEdit::UnIndent() {
  text_->CmdKeyExecute(wxSTC_CMD_BACKTAB);
  UpdateTitle();
}

void FileEdit::SelectAll() { text_->SetSelection(0, text_->GetTextLength()); }

void FileEdit::SelectLine() {
  int line_start = text_->PositionFromLine(text_->GetCurrentLine());
  int line_end = text_->PositionFromLine(text_->GetCurrentLine() + 1);
  text_->SetSelection(line_start, line_end);
}

void FileEdit::ToUpper() {
  text_->UpperCase();
  UpdateTitle();
}

void FileEdit::ToLower() {
  text_->LowerCase();
  UpdateTitle();
}

void FileEdit::MoveLinesUp() {
  text_->MoveSelectedLinesUp();
  UpdateTitle();
}

void FileEdit::MoveLinesDown() {
  text_->MoveSelectedLinesDown();
  UpdateTitle();
}

void FileEdit::OpenInOnlineDocumentation() {
  const bool only_word_characters = true;

  const wxString selected_text = text_->GetSelectedText();

  const int pos = text_->GetCurrentPos();
  const int start_position =
      text_->WordStartPosition(pos, only_word_characters);
  const int end_position = text_->WordEndPosition(pos, only_word_characters);
  const wxString current_word = text_->GetRange(start_position, end_position);

  const wxString current_selection_or_word =
      selected_text.Length() == 0 ? current_word : selected_text;
  // todo: html encode the current_selection_or_word since if it's &dog it
  // should be probably be encoded as &amp;dog
  const wxString url_to_open = wxString::Format(
      "http://doc.rust-lang.org/std/?search=%s", current_selection_or_word);
  wxLaunchDefaultBrowser(wxURI(url_to_open).BuildURI());
}

void FileEdit::ShowProperties() { ShowFilePropertiesDlg(this, text_); }

//////////////////////////////////////////////////////////////////////////

const wxString& FileEdit::filename() const { return filename_; }

void FileEdit::FileHasBeenRenamed(const wxString& new_path) {
  filename_ = new_path;
  UpdateFilename();
  UpdateTitle();
}

int FromLineColToTextOffset(wxStyledTextCtrl* text, int line, int col) {
  if (line == -1) {
    return -1;
  }

  int from = text->PositionFromLine(line - 1);
  if (col > 0) {
    from += col - 1;
  }
  return from;
}

void FromTextOffsetToLineCol(wxStyledTextCtrl* text, int index, int* line,
                             int* col) {
  if (index == -1) {
    *line = -1;
    *col = -1;
  } else {
    *line = text->LineFromPosition(index);
    int start = text->PositionFromLine(*line - 1);
    *col = index - start + 1;
  }
}

void FileEdit::GetSelection(int* start_line, int* start_index, int* end_line,
                            int* end_index) {
  assert(start_line);
  assert(start_index);
  assert(end_line);
  assert(end_index);
  WXID from = 0;
  WXID to = 0;
  text_->GetSelection(&from, &to);
  FromTextOffsetToLineCol(text_, from, start_line, start_index);
  FromTextOffsetToLineCol(text_, to, end_line, end_index);
}

void FileEdit::SetSelection(int start_line, int start_index, int end_line,
                            int end_index) {
  int from = FromLineColToTextOffset(text_, start_line, start_index);
  int to = FromLineColToTextOffset(text_, end_line, end_index);

  if (from == -1) return;

  if (to != -1) {
    text_->SetSelection(from, to);
  } else {
    text_->SetSelection(from, from);
  }

  text_->EnsureCaretVisible();
}

void FileEdit::SetFocus() { text_->SetFocus(); }
void FileEdit::SetFocusFromKbd() { text_->SetFocus(); }

bool FileEdit::AcceptsFocusRecursively() const { return true; }

bool FileEdit::AcceptsFocus() const { return true; }
bool FileEdit::AcceptsFocusFromKeyboard() const { return true; }

void FileEdit::ClearCompilerMessages() {
  text_->AnnotationClearAll();
  text_->SetIndicatorCurrent(ID_INDICATOR_WARNING);
  text_->IndicatorClearRange(0, text_->GetLength());

  text_->SetIndicatorCurrent(ID_INDICATOR_ERROR);
  text_->IndicatorClearRange(0, text_->GetLength());
}

void FileEdit::AddCompilerMessage(const CompilerMessage& mess) {
  assert(filename_ == mess.file());
  const bool is_error = mess.type() == CompilerMessage::TYPE_ERROR;
  const bool is_warning = mess.type() == CompilerMessage::TYPE_WARNING;
  const bool is_note = mess.type() == CompilerMessage::TYPE_NOTE;

  if (main_->settings().show_compiler_messages_as_annotations() &&
      (is_error || is_warning || is_note)) {
    const int style =
        is_error ? STYLE_ANNOTATION_ERROR : STYLE_ANNOTATION_WARNING;
    // todo: setup note annotations!
    const int line = mess.start_line() - 1;
    const wxString type =
        is_error ? "Error: " : (is_warning ? "Warning: " : "");
    const wxString value = type + mess.message();
    const wxString old_text =
        text_->AnnotationGetText(line);  // todo: setup multicoloring/styling
    const wxString ann = old_text.IsEmpty() ? value : old_text + "\n" + value;
    text_->AnnotationSetText(line, ann);
    text_->AnnotationSetStyle(line, style);

    if (is_warning || is_error) {
      // only do indicators for errors and warnings, not for notes
      const bool on_single_line = mess.start_line() == mess.end_line();
      if (main_->settings().show_multiline_indicators() || on_single_line) {
        int from = FromLineColToTextOffset(text_, mess.start_line(),
                                           mess.start_index());
        int to =
            FromLineColToTextOffset(text_, mess.end_line(), mess.end_index());

        const int ind = is_error ? ID_INDICATOR_ERROR : ID_INDICATOR_WARNING;

        if (from >= 0 && to >= 0) {
          text_->SetIndicatorCurrent(ind);
          text_->IndicatorFillRange(from, to - from);
        }
      }
    }
  }
}

wxDateTime GetFileDetectionTime(const wxString file) {
  wxFileName file_name(file);
  return file_name.GetModificationTime();
}

class StyledTextCtrl : public wxStyledTextCtrl {
 public:
  explicit StyledTextCtrl(FileEdit* parent)
      : wxStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                         wxBORDER_NONE | wxVSCROLL),
        parent_(parent) {
    Bind(wxEVT_CHAR, &StyledTextCtrl::OnChar, this);
    Bind(wxEVT_KEY_DOWN, &StyledTextCtrl::OnKeyDown, this);
  }

  // stolen from wx widgets, simplified and adapted for us

  void OnChar(wxKeyEvent& evt) {  // NOLINT
    bool lastKeyDownConsumed = m_lastKeyDownConsumed;

    // On (some?) non-US PC keyboards the AltGr key is required to enter some
    // common characters.  It comes to us as both Alt and Ctrl down so we need
    // to let the char through in that case, otherwise if only ctrl or only
    // alt let's skip it.
    bool ctrl = evt.ControlDown();
#ifdef __WXMAC__
    // On the Mac the Alt key is just a modifier key (like Shift) so we need
    // to allow the char events to be processed when Alt is pressed.
    // todo  Should we check MetaDown instead in this case?
    bool alt = false;
#else
    bool alt = evt.AltDown();
#endif
    bool skip = ((ctrl || alt) && !(ctrl && alt));

    // apparently if we don't do this, Unicode keys pressed after non-char
    // ASCII ones (e.g. Enter, Tab) are not taken into account (patch 1615989)
    if (lastKeyDownConsumed && evt.GetUnicodeKey() > 255)
      lastKeyDownConsumed = false;

    if (!lastKeyDownConsumed && !skip) {
      int key = evt.GetUnicodeKey();
      bool keyOk = true;

      // if the unicode key code is not really a unicode character (it may
      // be a function key or etc., the platforms appear to always give us a
      // small value in this case) then fallback to the ascii key code but
      // don't do anything for function keys or etc.
      if (key <= 127) {
        key = evt.GetKeyCode();
        keyOk = (key <= 127);
      }
      if (keyOk) {
        ProcessCharEvent(evt);
        return;
      }
    }

    evt.Skip();
  }

  void ProcessCharEvent(wxKeyEvent& evt) {  // NOLINT
    assert(parent_);
    if (parent_->ProcessCharEvent(evt.GetUnicodeKey()) == false) {
      wxStyledTextCtrl::OnChar(evt);
    }
  }

  void OnKeyDown(wxKeyEvent& evt) {  // NOLINT
    assert(parent_);
    if (parent_->ProcessKey(static_cast<wxKeyCode>(evt.GetKeyCode()),
                            static_cast<wxKeyModifier>(evt.GetModifiers())) ==
        false) {
      // wxStyledTextCtrl::OnKeyDown(evt);
      evt.Skip();
    }
  }

 private:
  FileEdit* parent_;
};

void SetTextPosition(wxStyledTextCtrl* text, int next_position) {
  // text->SetCurrentPos(next_position);
  text->SetSelection(next_position, next_position);
  // BUG: the virtual horizontal position is kept, not reset to next_position
}

class UndoActionRaii {
 public:
  explicit UndoActionRaii(wxStyledTextCtrl* text) : text_(text) {
    text_->BeginUndoAction();
  }

  ~UndoActionRaii() { text_->EndUndoAction(); }

 private:
  wxStyledTextCtrl* text_;
};

enum class StringType { NONE, DOUBLE_QUOTE, SINGLE_QUOTE };

StringType IsStringAt(wxStyledTextCtrl* text, int pos,
                      bool* support_language = NULL) {
  const int lexer = text->GetLexer();
  const int style = text->GetStyleAt(pos);

  switch (lexer) {
    case wxSTC_LEX_CPP:
      if (support_language) *support_language = true;
      switch (style) {
        case wxSTC_C_STRING:
          return StringType::DOUBLE_QUOTE;
        case wxSTC_C_CHARACTER:
          return StringType::SINGLE_QUOTE;
      }
      break;
#ifdef wxSTC_LEX_RUST
    case wxSTC_LEX_RUST:
      if (support_language) *support_language = true;
      switch (style) {
        case wxSTC_RUST_STRING:
          return StringType::DOUBLE_QUOTE;
        case wxSTC_RUST_STRINGR:
          // TODO(Gustav): Fix raw string literals
          return StringType::DOUBLE_QUOTE;
        case wxSTC_RUST_CHARACTER:
          return StringType::SINGLE_QUOTE;
      }
      break;
#endif
  }

  return StringType::NONE;
}

bool HandleParaEditCommon(int tab_width, ride::AutoComplete ac_setting,
                          wxChar c, wxStyledTextCtrl* text,
                          bool add_newlines_on_selection_encapsulation,
                          wxChar begin, wxChar end) {
  if (ac_setting != ride::AUTOCOMPLETE_PARA) return false;

  long selection_from = -1;  // NOLINT
  long selection_to = -1;    // NOLINT
  text->GetSelection(&selection_from, &selection_to);
  const bool has_selection = selection_from != selection_to;

  // const int caret = text->GetCurrentPos();
  const int caret = selection_from;

  // if the character is inside a string, abort the ac
  if (IsStringAt(text, caret) != StringType::NONE) return false;

  if (c == begin) {
    if (has_selection) {
      // Selection: When typing begin, replace selection with begin+end
      // as normal
      text->ReplaceSelection(wxString(begin) + wxString(end));
    } else {
      text->InsertText(caret, wxString(begin) + wxString(end));
    }
    SetTextPosition(text, caret + 1);
    return true;
  }

  if (c == end) {
    if (has_selection) {
      // Selection: when typing end, encapsulate the selection with
      // begin+selection+end and keep selection
      UndoActionRaii undo_action(text);
      auto line_start = text->LineFromPosition(selection_from);
      auto line_end = text->LineFromPosition(selection_to);

      auto position = selection_from;
      auto end_position = selection_to;
      text->InsertText(position, wxString(begin));
      position += 1;
      end_position += 1;

      auto final_start = selection_from + 1;
      auto final_end = end_position;

      if (add_newlines_on_selection_encapsulation) {
        if (line_start != line_end) {
          const wxString newline = "\n";
          text->InsertText(position, newline);
          const auto first_line_indent = text->GetLineIndentation(line_start);
          text->SetLineIndentation(line_start + 1, first_line_indent);
          end_position += first_line_indent + newline.length();
          for (auto line = line_start + 1; line <= line_end + 1; ++line) {
            const auto debug_line = text->GetLine(line);
            const auto indent = text->GetLineIndentation(line);
            text->SetLineIndentation(line, indent + tab_width);
            end_position += tab_width;
          }
          text->InsertText(end_position, newline);
          const auto debug_line0 = text->GetLine(line_end);
          const auto debug_line1 = text->GetLine(line_end + 1);
          const auto debug_line2 = text->GetLine(line_end + 2);
          text->SetLineIndentation(line_end + 2, first_line_indent);
          const auto debug_line22 = text->GetLine(line_end + 2);
          end_position += first_line_indent + newline.length();

          final_start += first_line_indent + tab_width + newline.length();
          final_end = end_position - (first_line_indent + newline.length());
        }
      }
      text->InsertText(end_position, wxString(end));
      const auto lline = text->LineFromPosition(end_position);
      const auto sstr = text->GetLine(lline);
      text->SetSelection(final_start, final_end);
      return true;
    } else {
      const int last_index = text->GetLength();
      int found = caret;
      do {
        found = text->FindText(found, last_index, wxString(c));
        if (found == -1) {
          return true;
        }
        ++found;  // move past the end

        // don't stop inside a string
      } while (IsStringAt(text, found - 1) != StringType::NONE);
      found = std::min(found, last_index);
      SetTextPosition(text, found);
      return true;
    }
  }

  return false;
}

bool HandleParaEditQuote(char c, ride::AutoComplete ac, wxStyledTextCtrl* text,
                         char quote_character, StringType string_type) {
  if (ac != ride::AutoComplete::AUTOCOMPLETE_PARA) return false;

  if (c == quote_character) {
    // TODO(Gustav): if there is a selection, encapsulate add concat and select
    // the content
    const int caret = text->GetCurrentPos();
    bool support_language = false;
    const StringType string = IsStringAt(text, caret, &support_language);
    if (support_language == false) {
      // if we don't support the language, abort the string process
      return false;
    }
    if (string == StringType::NONE) {
      // add string and move one forward
      text->InsertText(caret, wxString(2, c));
      SetTextPosition(text, caret + 1);
      return true;
    }
    if (string == string_type && c == quote_character) {
      // move past end delim if at the end, otherwise escape the char
      if (string == IsStringAt(text, caret + 1)) {
        // still the same string type, so add an escape and step 2
        text->InsertText(caret, wxString('\\') + wxString(c));
        SetTextPosition(text, caret + 2);
        return true;
      } else {
        // move past the character
        const wxString character = text->GetTextRange(caret, caret + 1);
        assert(character[0] == c);
        SetTextPosition(text, caret + 1);
        return true;
      }
    }
  }

  return false;
}

bool FileEdit::ProcessCharEvent(wxChar c) {
  assert(this);

  // TODO(Gustav): Add a disable-all-auto-completion-temporarily shortcut
  // suggestion: ctrl-shift-a

  const wxChar SINGLE_QUOTE = '\'';
  const wxChar DOUBLE_QUOTE = '\"';

  const int tab_width = main_->project()->tabwidth();

  if (HandleParaEditCommon(tab_width,
                           main_->settings().autocomplete_parentheses(), c,
                           text_, false, '(', ')'))
    return true;
  if (HandleParaEditCommon(tab_width,
                           main_->settings().autocomplete_curly_braces(), c,
                           text_, true, '{', '}'))
    return true;
  if (HandleParaEditCommon(tab_width, main_->settings().autocomplete_brackets(),
                           c, text_, false, '[', ']'))
    return true;

  if (HandleParaEditQuote(c, main_->settings().autocomplete_singlequote(),
                          text_, SINGLE_QUOTE, StringType::SINGLE_QUOTE))
    return true;

  if (HandleParaEditQuote(c, main_->settings().autocomplete_doublequote(),
                          text_, DOUBLE_QUOTE, StringType::DOUBLE_QUOTE))
    return true;

  return false;
}

enum class SelectionState { NONE, STREAM, RECT };

void GoHomeEnd(wxStyledTextCtrl* text_, bool right,
               ride::HomeEndStyle home_end_style,
               SelectionState selection_state) {
  // toggle between the first char on the line and the first non/whitespace or
  // go to the last char on the line
  switch (home_end_style) {
    case ride::HES_BASIC:
      if (right) {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->LineEnd();
            break;
          case SelectionState::STREAM:
            text_->LineEndExtend();
            break;
          case SelectionState::RECT:
            text_->LineEndRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      } else {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->Home();
            break;
          case SelectionState::STREAM:
            text_->HomeExtend();
            break;
          case SelectionState::RECT:
            text_->HomeRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      }
      break;
    case ride::HES_DISPLAY:
      if (right) {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->LineEndDisplay();
            break;
          case SelectionState::STREAM:
            text_->LineEndDisplayExtend();
            break;
          case SelectionState::RECT:
            text_->LineEndRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      } else {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->HomeDisplay();
            break;
          case SelectionState::STREAM:
            text_->HomeDisplayExtend();
            break;
          case SelectionState::RECT:
            text_->HomeRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      }
      break;
    case ride::HES_WRAP:
      if (right) {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->LineEndWrap();
            break;
          case SelectionState::STREAM:
            text_->LineEndWrapExtend();
            break;
          case SelectionState::RECT:
            text_->LineEndRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      } else {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->HomeWrap();
            break;
          case SelectionState::STREAM:
            text_->HomeWrapExtend();
            break;
          case SelectionState::RECT:
            text_->HomeRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      }
      break;
    case ride::HES_VCBASIC:
      if (right) {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->LineEnd();
            break;
          case SelectionState::STREAM:
            text_->LineEndExtend();
            break;
          case SelectionState::RECT:
            text_->LineEndRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      } else {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->VCHome();
            break;
          case SelectionState::STREAM:
            text_->VCHomeExtend();
            break;
          case SelectionState::RECT:
            text_->VCHomeRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      }
      break;
    case ride::HES_VCDISPLAY:
      if (right) {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->LineEndDisplay();
            break;
          case SelectionState::STREAM:
            text_->LineEndDisplayExtend();
            break;
          case SelectionState::RECT:
            text_->LineEndRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      } else {
        switch (selection_state) {
          case SelectionState::NONE:
#ifdef RIDE_WXWIDGETS_USE_NEW_GO_HOME_END
            text_->VCHomeDisplay();
#else
            text_->HomeDisplay();
#endif
            break;
          case SelectionState::STREAM:
#ifdef RIDE_WXWIDGETS_USE_NEW_GO_HOME_END
            text_->VCHomeDisplayExtend();
#else
            text_->HomeDisplayExtend();
#endif
            break;
          case SelectionState::RECT:
            text_->VCHomeRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      }
      break;
    case ride::HES_VCWRAP:
      if (right) {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->LineEndWrap();
            break;
          case SelectionState::STREAM:
            text_->LineEndWrapExtend();
            break;
          case SelectionState::RECT:
            text_->LineEndRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      } else {
        switch (selection_state) {
          case SelectionState::NONE:
            text_->VCHomeWrap();
            break;
          case SelectionState::STREAM:
            text_->VCHomeWrapExtend();
            break;
          case SelectionState::RECT:
            text_->VCHomeRectExtend();
            break;
          default:
            assert(false && "Unhandled selection state");
        }
      }
      break;
    case ride::HES_SCROLL:
      if (right) {
        text_->ScrollToEnd();
      } else {
        text_->ScrollToStart();
      }
      break;
  }
}

bool FileEdit::ProcessKey(wxKeyCode key, wxKeyModifier mod) {
  // standard left/right handled automagically

  const bool left_or_right = key == WXK_LEFT || key == WXK_RIGHT;
  const wxKeyModifier without_shift =
      static_cast<wxKeyModifier>(mod & ~wxMOD_SHIFT);
  const bool is_shift_down = (mod & wxMOD_SHIFT) == wxMOD_SHIFT;

  const SelectionState selection_state =
      is_shift_down ? (text_->SelectionIsRectangle() ? SelectionState::RECT
                                                     : SelectionState::STREAM)
                    : SelectionState::NONE;

  if (
#ifndef RIDE_OS_APPLE
      // win: ctrl + left/right
      without_shift == wxMOD_CONTROL && left_or_right
#else
      // osx: alt + left/right
      without_shift == wxMOD_ALT && left_or_right
#endif
      ) {
    // move one word, left or right
    const bool right = key == WXK_RIGHT;
    if (right) {
      switch (selection_state) {
        case SelectionState::NONE:
          text_->WordRight();
          break;
        case SelectionState::STREAM:
        case SelectionState::RECT:
          text_->WordRightExtend();
          break;
        default:
          assert(false && "Unhandled selection state");
      }
    } else {
      switch (selection_state) {
        case SelectionState::NONE:
          text_->WordLeft();
          break;
        case SelectionState::STREAM:
        case SelectionState::RECT:
          text_->WordLeftExtend();
          break;
        default:
          assert(false && "Unhandled selection state");
      }
    }
    return true;
  }

#ifndef RIDE_OS_APPLE
  // win: alt + left/right
  if (without_shift == wxMOD_ALT && left_or_right) {
    const bool right = key == WXK_RIGHT;
    // single step
    if (right) {
      switch (selection_state) {
        case SelectionState::NONE:
          text_->WordPartRight();
          break;
        case SelectionState::STREAM:
        case SelectionState::RECT:
          text_->WordPartRightExtend();
          break;
        default:
          assert(false && "Unhandled selection state");
      }
    } else {
      switch (selection_state) {
        case SelectionState::NONE:
          text_->WordPartLeft();
          break;
        case SelectionState::STREAM:
        case SelectionState::RECT:
          text_->WordPartLeftExtend();
          break;
        default:
          assert(false && "Unhandled selection state");
      }
    }
    return true;
  }
#endif
  // osx: fn + left/right
  // meta?

  if (
#ifndef RIDE_OS_APPLE
      // win: home/end
      key == WXK_HOME || key == WXK_END
#else
      // osx: cmd + left/right
      without_shift == wxMOD_CMD && left_or_right
#endif
      ) {
    const bool right = key == WXK_RIGHT || key == WXK_END;
    // TODO(Gustav): Expose home/end style in the gui
    GoHomeEnd(text_, right, main_->settings().home_end_style(),
              selection_state);
    return true;
  }

  return false;
}

FileEdit::FileEdit(wxAuiNotebook* anotebook, MainWindow* parent,
                   const wxString& file, Languages* languages)
    : wxControl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                wxBORDER_NONE),
      tab_(this),
      main_(parent),
      notebook_(anotebook),
      languages_(languages),
      current_language_(NULL),
      highlight_current_word_last_start_position_(-1),
      highlight_current_word_last_end_position_(-1) {
  assert(anotebook);
  assert(parent);
  assert(languages);
  assert(false == file.IsEmpty());
  this->SetClientData(&tab_);
  BindEvents();
  text_ = new StyledTextCtrl(this);

  filename_ = file;
  LoadFile();

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text_, 1, wxEXPAND);
  SetSizer(sizer);

  UpdateFilename();

  notebook_->AddPage(this, wxT(""), true);

  UpdateTitle();
}

void FileEdit::LoadFile() {
  text_->LoadFile(filename_);
  UpdateFileTime();
}

void FileEdit::UpdateFileTime() {
  last_modification_time_ = GetFileDetectionTime(filename_);
}

class TrueFalse {
 public:
  explicit TrueFalse(bool* capture) : capture_(capture) {
    assert(capture_);
    assert(*capture_ == false);
    *capture_ = true;
  }
  ~TrueFalse() {
    assert(capture_);
    assert(*capture_ == true);
    *capture_ = false;
  }

  bool* capture_;
};

void FileEdit::ReloadFileIfNeeded() {
  // basic check for infinite activation->question loop
  static bool inside = false;
  if (inside) return;
  TrueFalse inside_capture(&inside);

  const bool exist = wxFileName(filename_).FileExists();
  if (exist == false) {
    if (DialogResult::YES ==
        ShowYesNo(this, "Close file", "Close file", "Keep open",
                  filename_ + " has been removed",
                  filename_ + " has been removed, close it?")) {
      size_t index = notebook_->GetPageIndex(this);
      const bool file_closed = notebook_->DeletePage(index);
      if (false == file_closed) {
        ShowError(this, "Unable to close file", "Error");
      }
    }
    return;
  }

  wxDateTime latest_file_time = GetFileDetectionTime(filename_);
  if (last_modification_time_ != latest_file_time) {
    // ask to reload or not?
    if (ShowYesNo(this, "File modified!", "Reload the file", "Keep my changes",
                  wxString::Format(
                      "%s\nThis file has been modified by another program.",
                      filename_),
                  wxString::Format(
                      "%s\nThis file has been modified by another program.\nDo "
                      "you want to reload it?",
                      filename_)) == DialogResult::YES) {
      LoadFile();
      UpdateTitle();
    } else {
      // if the user said "keep changes", we will remember that modification and
      // not ask again for this "change" by remembering the modification time.
      UpdateFileTime();
    }
  }
}

bool FileEdit::Save() {
  if (ShouldBeSaved() == false) return true;
  const bool save_successful =
      filename_.IsEmpty() ? SaveAs() : SaveTo(filename_);

  // compile proto file
  if (save_successful) {
    // if we managed to successfully save a protobuf file, then
    // run the protobuf compiler automatically
    CompileProtoFile(main_->machine(), filename_, main_);
  }
  return save_successful;
}

bool FileEdit::SaveAs() {
  wxFileDialog saveFileDialog(this, _("Save file"), "", "",
                              languages_->GetFilePattern(),
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL) return false;
  return SaveTo(saveFileDialog.GetPath());
}

bool FileEdit::SaveTo(const wxString& target) {
  if (false == text_->SaveFile(target)) {
    return false;
  }
  filename_ = target;
  UpdateFilename();
  UpdateTitle();
  UpdateFileTime();
  return true;
}

wxString b2s01(bool b) {
  if (b)
    return _("1");
  else
    return _("0");
}

wxString FileEdit::CalculateDocumentName() const {
  if (filename_.IsEmpty()) {
    return "Untitled";
  } else {
    wxFileName fn(filename_);
    return fn.GetFullName();
  }
}

void FileEdit::ShowAutocomplete() {
  Autocomplete(main_->machine(), text_, current_language_, filename_,
               main_->root_folder(), this, ShowAutoCompleteAction::FORCE_KEEP);
}

void FileEdit::UpdateTextControl() {
  const ride::Settings& set = main_->settings();

  Project* project = main_->project();

  SetupScintilla(text_, set, current_language_,
                 project->IsPartOfProject(filename_) ? project : NULL);
  SetupScintillaAutoCompleteImages(text_);
}

void FileEdit::UpdateFilename() {
  UpdateStatusText();
  if (filename_.IsEmpty() == false) {
    size_t index = notebook_->GetPageIndex(this);
    notebook_->SetPageToolTip(index, filename_);

    wxFileName fname(filename_);
    current_language_ = languages_->DetermineLanguage(fname.GetFullName());
    UpdateTextControl();
    UpdateTextControl();  // update colors again, doing it twice seems to be
                          // needed to apply the colors
  }
}

void FileEdit::UpdateTitle() {
  size_t index = notebook_->GetPageIndex(this);
  const wxString modified_star = text_->IsModified() ? "*" : "";
  notebook_->SetPageText(index, CalculateDocumentName() + modified_star);
}

bool FileEdit::ShouldBeSaved() {
  return text_->IsModified() || filename_.IsEmpty();
}

bool FileEdit::CanClose(bool can_abort) {
  if (ShouldBeSaved()) {
    const wxString caption = "Save file?";
    const wxMessageDialogBase::ButtonLabel yes_button = _("&Save it");
    const wxMessageDialogBase::ButtonLabel no_button = _("&Discard changes");
    const wxMessageDialogBase::ButtonLabel cancel_button = _("&Abort");
    const wxString title_ok =
        "\"" + CalculateDocumentName() + "\" has changed since last time...";
    const wxString title_error = "\"" + CalculateDocumentName() +
                                 "\" has changed since last time, save it?";

    const DialogResult answer =
        can_abort ? ShowYesNoCancel(this, caption, yes_button, no_button,
                                    cancel_button, title_ok, title_error)
                  : ShowYesNo(this, caption, yes_button, no_button, title_ok,
                              title_error);

    if (answer == DialogResult::YES) {
      return Save();
    } else if (answer == DialogResult::NO) {
      return true;
    } else {
      assert(answer == DialogResult::CANCEL);
      return false;
    }
  }
  return true;
}

FileEdit::~FileEdit() { text_->SetClientData(NULL); }

void FileEdit::BindEvents() {
  Bind(wxEVT_STC_MARGINCLICK, &FileEdit::OnMarginClick, this);
  Bind(wxEVT_STC_CHARADDED, &FileEdit::OnCharAdded, this);
  Bind(wxEVT_STC_UPDATEUI, &FileEdit::OnUpdateUi, this);
  Bind(wxEVT_STC_CHANGE, &FileEdit::OnChanged, this);
  Bind(EVENT_UPDATE_SELECTION, &FileEdit::OnSelectionUpdated, this);
}

void FileEdit::OnMarginClick(wxStyledTextEvent& event) {
  if (event.GetMargin() == ID_MARGIN_FOLDING) {
    int clicked_line = text_->LineFromPosition(event.GetPosition());
    int fold_level = text_->GetFoldLevel(clicked_line);

    if ((fold_level & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
      text_->ToggleFold(clicked_line);
    }
  }
}

int CalculateIndentationChange(const wxString& str) {
  // todo: move indent calculation to language...
  int change = 0;
  for (size_t i = 0; i < str.Length(); ++i) {
    const wxUniChar c = str.at(i);
    if (c == '{') {
      change += 1;
    }
    if (c == '}') {
      // handles cases like {} <newline here>
      // but not } <newline here>
      if (change > 0) {
        change -= 1;
      }
    }

    // decrease is below...
  }
  return change;
}

void FileEdit::OnCharAdded(wxStyledTextEvent& event) {
  int entered_character =
      event.GetKey();  // the key seems to be the char that was added
  event.Skip();

  const wxString character_before_entered = text_->GetTextRange(
      text_->GetCurrentPos() - 2, text_->GetCurrentPos() - 1);

  const bool force =
      (character_before_entered == ":" && entered_character == ':') ||
      (character_before_entered != "." && entered_character == '.');

  Autocomplete(main_->machine(), text_, current_language_, filename_,
               main_->root_folder(), this,
               force ? ShowAutoCompleteAction::FORCE_SIMPLE
                     : ShowAutoCompleteAction::NO_FORCE);

  if (entered_character == '\n' || entered_character == '\r') {
    // fixing the line margin width since we may need to expand it
    // going from line 99 to 100
    SetupLineMargin(text_, main_->settings());

    // auto-indenting
    // loosely based on http://www.scintilla.org/ScintillaUsage.html and
    // https://groups.google.com/forum/#!topic/scintilla-interest/vTwXwIBswSM
    const int current_line = text_->GetCurrentLine();
    const int line_start = text_->PositionFromLine(text_->GetCurrentLine() - 1);
    const int line_end = text_->PositionFromLine(text_->GetCurrentLine());
    const int next_end = text_->PositionFromLine(text_->GetCurrentLine() + 1);
    const wxString previous_line_content =
        text_->GetTextRange(line_start, line_end);
    const wxString current_line_content =
        text_->GetTextRange(line_end, next_end);

    const int indent_change =
        main_->settings().auto_indentation() == ride::AUTOINDENTATION_SMART
            ? CalculateIndentationChange(previous_line_content)
            : 0;
    const int indent_change_in_spaces =
        indent_change * main_->settings().tabwidth();

    const int smart_indent = indent_change_in_spaces;

    const int indentation_in_spaces =
        current_line > 0
            ? std::max(
                  0, text_->GetLineIndentation(current_line - 1) + smart_indent)
            : smart_indent;

    const int indentation_in_tabs =
        indentation_in_spaces / main_->settings().tabwidth();

    // if we use tabs, divide the number of character by the char width to get
    // the actual width
    const int indentation_in_chars = main_->settings().usetabs()
                                         ? indentation_in_tabs
                                         : indentation_in_spaces;

    // adjust to remove weird spaces from indentation settings
    const int indentation_in_spaces_ajdusted =
        indentation_in_tabs * main_->settings().tabwidth();

    if (main_->settings().auto_indentation() != ride::AUTOINDENTATION_NONE) {
      if (indentation_in_spaces_ajdusted != 0) {
        text_->SetLineIndentation(current_line, indentation_in_spaces_ajdusted);
        text_->GotoPos(text_->PositionFromLine(current_line) +
                       indentation_in_chars);
      }

      if (current_line_content.StartsWith("}")) {
        // handle case like { USER_ENTER_NEWLINE_HERE }
        // add a newline after this and...
        text_->InsertText(text_->GetCurrentPos(), "\n");

        // ...set that newline - 1 indentation of the current row
        text_->SetLineIndentation(
            current_line + 1,
            indentation_in_spaces_ajdusted - main_->settings().tabwidth());
      }
    }
  }
}

void FileEdit::HighlightCurrentWord() {
  const int current_position = text_->GetCurrentPos();

  // highlight current word
  const bool only_word_characters = true;
  const int current_start_position =
      text_->WordStartPosition(current_position, only_word_characters);
  const int current_end_position =
      text_->WordEndPosition(current_position, only_word_characters);

  if (current_start_position != highlight_current_word_last_start_position_ ||
      current_end_position != highlight_current_word_last_end_position_) {
    text_->SetIndicatorCurrent(ID_INDICATOR_SELECT_HIGHLIGHT);

    // clear old highlight
    if (highlight_current_word_last_start_position_ != -1 &&
        highlight_current_word_last_end_position_ != -1 &&
        highlight_current_word_last_start_position_ !=
            highlight_current_word_last_end_position_) {
      text_->IndicatorClearRange(0, text_->GetLength());
    }

    if (current_start_position != -1 && current_end_position != -1 &&
        current_start_position != current_end_position) {
      const int length = current_end_position - current_start_position;
      assert(length > 0);

      const wxString current_text =
          text_->GetRange(current_start_position, current_end_position);

      if (current_text == "") {
        return;
      }

      const bool highlight_keyword =
          main_->settings().highlight_word_also_highlight_keywords();
      const bool is_keyword =
          highlight_keyword
              ? false
              : (current_language_ ? current_language_->IsKeyword(current_text)
                                   : false);

      if (is_keyword == false) {
        // search through the entire document for this text and highlight it
        int search_point = 0;
        while (true) {
          const int flags = wxSTC_FIND_WHOLEWORD | wxSTC_FIND_MATCHCASE;
          int match_position =
              FindStcText(text_, search_point, text_->GetLength(), current_text,
                          flags, NULL);
          if (match_position == -1) {
            break;
          }
          bool highlight_match = false;
          if (match_position == current_start_position) {
            // todo: set highlight_this to true depending on the the setting to
            // highlight the current word or not...
          } else {
            highlight_match = true;
          }

          if (highlight_match) {
            text_->IndicatorFillRange(match_position, length);
          }
          search_point = match_position + length;
        }
      }
    }
    highlight_current_word_last_start_position_ = current_start_position;
    highlight_current_word_last_end_position_ = current_end_position;
  }
}

void FileEdit::UpdateBraceMatching() {
  const int pos = text_->GetCurrentPos();
  int otherBrace = text_->BraceMatch(pos);
  if (otherBrace != -1) {
    text_->BraceHighlight(pos, otherBrace);
  } else {
    text_->BraceHighlight(-1, -1);
  }
}

void FileEdit::OnUpdateUi(wxStyledTextEvent& event) {
  const int type = event.GetUpdated();

  if (type & wxSTC_UPDATE_SELECTION) {
    wxCommandEvent event(EVENT_UPDATE_SELECTION);
    wxPostEvent(this, event);
  }
}

void FileEdit::OnSelectionUpdated(wxCommandEvent& event) {
  HighlightCurrentWord();
  UpdateBraceMatching();
  UpdateStatusText();
}

void FileEdit::OnChanged(wxStyledTextEvent& event) {
  UpdateTitle();
  HighlightCurrentWord();
  UpdateBraceMatching();
}

void FileEdit::UpdateStatusText() {
  main_->SetStatusText(
      filename_,
      STATUSBAR_GENERAL);  // change to only display the last 127 characters?
  const auto line = text_->GetCurrentLine();
  const auto cp = text_->GetCurrentPos();
  main_->SetStatusText(wxString::Format("Ln %d", line + 1), STATUSBAR_LINE);
  main_->SetStatusText(text_->GetOvertype() ? "OVR" : "INS", STATUSBAR_INS);
  main_->SetStatusText(wxString::Format("Col %d", text_->GetColumn(cp)),
                       STATUSBAR_COL);
  main_->SetStatusText(
      wxString::Format("Ch %d", cp - text_->PositionFromLine(line)),
      STATUSBAR_CH);
}
