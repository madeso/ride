#include "ride/outputcontrol.h"

#include "ride/mainwindow.h"
#include "settings.pb.h"
#include "ride/fileedit.h"
#include "ride/compilermessage.h"
#include "ride/wxutils.h"
#include <wx/uri.h>
#include <wx/clipbrd.h>

enum
{
  ID_FIRST = wxID_HIGHEST,
  
  ID_SEARCH_FOR_THIS_COMPILER_MESSAGE,
  ID_COPY_THIS_COMPILER_MESSAGE,
  ID_CLEAR_COMPILER_OUTPUT,
  ID_QUICK_OPEN
};

OutputControl::OutputControl(MainWindow* main) : main_(main) {
}

void OutputControl::UpdateStyle() {
  this->StyleClearAll();
  this->SetReadOnly(true);

  const ride::Settings& set = main_->settings();
  SetupScintillaCurrentLine(this, set);
  SetupScintillaDefaultStyles(this, set);
  this->SetEndAtLastLine(set.end_at_last_line());
}

void OutputControl::OnContextMenu(wxContextMenuEvent& event) {
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
  AppendEnabled(menu, ID_SEARCH_FOR_THIS_COMPILER_MESSAGE, wxString::Format("Search for \"%s\" online", message), has_compiler_message);
  AppendEnabled(menu, ID_COPY_THIS_COMPILER_MESSAGE, wxString::Format("Copy \"%s\" to clipboard", message), has_compiler_message);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_CLEAR_COMPILER_OUTPUT, "Clear output", true);
    


  PopupMenu(&menu);
}

void OutputControl::OnCopyThisCompilerMessage(wxCommandEvent& event) {
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

void OutputControl::OnClearCompilerOuput(wxCommandEvent& event) {
  ClearOutput(this);
}

void OutputControl::OnSelectAll(wxCommandEvent& event) {
  this->SelectAll();
}

void OutputControl::OnCopy(wxCommandEvent& event) {
  this->Copy();
}

const wxString OutputControl::GetContextLineContent() {
  long line_number = 0;
  long col = 0;
  const long index = context_positon_;
  this->PositionToXY(index, &col, &line_number);
  if (line_number == -1) return wxEmptyString;
  const wxString line_content = GetLineText(line_number);
  return line_content;
}

void OutputControl::OnSearchForThisCompilerMessage(wxCommandEvent& event) {
  const wxString line_content = GetContextLineContent();

  CompilerMessage message;
  if (CompilerMessage::Parse(main_->root_folder(), line_content, &message)) {
    wxString mess = message.message();
    mess.Replace("#", "%23");
    const wxString escaped_message = wxURI(mess).BuildURI();
    const wxString url_to_open = wxString::Format("http://www.google.com/search?q=%s", escaped_message);
    wxLaunchDefaultBrowser(url_to_open);
  }
  else {
    ShowWarning(this, "Unable to get compiler message data", "No compiler message data");
  }
}

void OutputControl::OnDoubleClick(wxMouseEvent& event) {
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

wxBEGIN_EVENT_TABLE(OutputControl, wxStyledTextCtrl)
  EVT_LEFT_DCLICK(OutputControl::OnDoubleClick)
  EVT_CONTEXT_MENU(OutputControl::OnContextMenu)
  EVT_MENU(ID_SEARCH_FOR_THIS_COMPILER_MESSAGE, OutputControl::OnSearchForThisCompilerMessage)

  EVT_MENU(ID_COPY_THIS_COMPILER_MESSAGE, OutputControl::OnCopyThisCompilerMessage)
  EVT_MENU(ID_CLEAR_COMPILER_OUTPUT, OutputControl::OnClearCompilerOuput)
  EVT_MENU(wxID_SELECTALL, OutputControl::OnSelectAll)
  EVT_MENU(wxID_COPY, OutputControl::OnCopy)
wxEND_EVENT_TABLE()
