#include "ride/findcontrol.h"

#include "ride/compilermessage.h"
#include "ride/fileedit.h"
#include "ride/mainwindow.h"
#include "ride/wxutils.h"
#include <wx/clipbrd.h>

enum
{
  ID_FIRST = wxID_HIGHEST,

  ID_COPY_THIS_COMPILER_MESSAGE,
  ID_CLEAR_COMPILER_OUTPUT
};


FindControl::FindControl(MainWindow* main) : main_(main) {
}

void FindControl::UpdateStyle() {
  this->StyleClearAll();
  this->SetReadOnly(true);

  const ride::Settings& set = main_->settings();
  SetupScintillaCurrentLine(this, set);
  SetupScintillaDefaultStyles(this, set);
  this->SetEndAtLastLine(set.end_at_last_line());
}

void FindControl::OnContextMenu(wxContextMenuEvent& event) {
  const wxPoint mouse_point = GetContextEventPosition(event);
  const wxPoint client_point = ScreenToClient(mouse_point);
  context_positon_ = this->PositionFromPoint(client_point);

  const bool has_selected = this->GetSelectedText().IsEmpty() == false;
  const wxString line_content = GetContextLineContent();
  CompilerMessage compiler_message;
  const bool has_compiler_message = CompilerMessage::Parse(main_->root_folder(), line_content, &compiler_message);
  const wxString message = has_compiler_message ? ToShortString(compiler_message.message(), 45) : "<none>";

  wxMenu menu;
  AppendEnabled(menu, wxID_COPY, "Copy", has_selected);
  AppendEnabled(menu, wxID_SELECTALL, "Select all", true);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_COPY_THIS_COMPILER_MESSAGE, wxString::Format("Copy \"%s\" to clipboard", message), has_compiler_message);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_CLEAR_COMPILER_OUTPUT, "Clear output", true);

  PopupMenu(&menu);
}

void FindControl::OnCopyThisCompilerMessage(wxCommandEvent& event) {
  const wxString line_content = GetContextLineContent();

  CompilerMessage message;
  if (CompilerMessage::Parse(main_->root_folder(), line_content, &message)) {
    if (wxTheClipboard->Open()) {
      wxTheClipboard->SetData(new wxTextDataObject(message.message()));
      wxTheClipboard->Close();
    }
  }
  else {
    ShowWarning(this, "Unable to get compiler message data", "No compiler message data");
  }
}

void FindControl::OnClearCompilerOuput(wxCommandEvent& event) {
  ClearOutput(this);
}

void FindControl::OnSelectAll(wxCommandEvent& event) {
  this->SelectAll();
}

void FindControl::OnCopy(wxCommandEvent& event) {
  this->Copy();
}

const wxString FindControl::GetContextLineContent() {
  long line_number = 0;
  long col = 0;
  const long index = context_positon_;
  this->PositionToXY(index, &col, &line_number);
  if (line_number == -1) return wxEmptyString;
  const wxString line_content = GetLineText(line_number);
  return line_content;
}

void FindControl::OnDoubleClick(wxMouseEvent& event) {
  long line_number = 0;
  long col = 0;
  long index = this->GetInsertionPoint();
  this->PositionToXY(index, &col, &line_number);
  if (line_number == -1) return;
  wxString line_content = GetLineText(line_number);

  CompilerMessage message;
  if (CompilerMessage::Parse(main_->root_folder(), line_content, &message)) {
    main_->OpenCompilerMessage(message);
  }
}

wxBEGIN_EVENT_TABLE(FindControl, wxStyledTextCtrl)
  EVT_LEFT_DCLICK(FindControl::OnDoubleClick)
  EVT_CONTEXT_MENU(FindControl::OnContextMenu)

  EVT_MENU(ID_COPY_THIS_COMPILER_MESSAGE, FindControl::OnCopyThisCompilerMessage)
  EVT_MENU(ID_CLEAR_COMPILER_OUTPUT, FindControl::OnClearCompilerOuput)
  EVT_MENU(wxID_SELECTALL, FindControl::OnSelectAll)
  EVT_MENU(wxID_COPY, FindControl::OnCopy)
wxEND_EVENT_TABLE()

