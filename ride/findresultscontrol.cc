// Copyright (2015) Gustav

#include "ride/findresultscontrol.h"

#include <wx/clipbrd.h>

#include "ride/compilermessage.h"
#include "ride/stcutils.h"
#include "ride/mainwindow.h"
#include "ride/wxid.h"
#include "ride/wxutils.h"

enum {
  ID_FIRST = wxID_HIGHEST,

  ID_COPY_THIS_COMPILER_MESSAGE,
  ID_CLEAR_COMPILER_OUTPUT
};

FindResultsControl::FindResultsControl(MainWindow* main)
    : wxControl(main, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                wxBORDER_NONE),
      main_(main) {
  text_ = new wxStyledTextCtrl(
      this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
      wxBORDER_NONE | wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL);
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text_, 1, wxEXPAND);
  SetSizer(sizer);
  BindEvents();
}

void FindResultsControl::BindEvents() {
  Bind(wxEVT_LEFT_DCLICK, &FindResultsControl::OnDoubleClick, this);
  Bind(wxEVT_CONTEXT_MENU, &FindResultsControl::OnContextMenu, this);

  Bind(wxEVT_MENU, &FindResultsControl::OnCopyThisCompilerMessage, this,
       ID_COPY_THIS_COMPILER_MESSAGE);
  Bind(wxEVT_MENU, &FindResultsControl::OnClearCompilerOuput, this,
       ID_CLEAR_COMPILER_OUTPUT);
  Bind(wxEVT_MENU, &FindResultsControl::OnSelectAll, this, wxID_SELECTALL);
  Bind(wxEVT_MENU, &FindResultsControl::OnCopy, this, wxID_COPY);
}

void FindResultsControl::UpdateStyle() {
  text_->StyleClearAll();
  text_->SetReadOnly(true);

  const ride::Settings& set = main_->settings();
  SetupScintillaCurrentLine(text_, set);
  SetupScintillaDefaultStyles(text_, set);
  text_->SetEndAtLastLine(set.end_at_last_line());
}

void FindResultsControl::OnContextMenu(wxContextMenuEvent& event) {
  const wxPoint mouse_point = GetContextEventPosition(event);
  const wxPoint client_point = ScreenToClient(mouse_point);
  context_positon_ = text_->PositionFromPoint(client_point);

  const bool has_selected = text_->GetSelectedText().IsEmpty() == false;
  const wxString line_content = GetContextLineContent();
  CompilerMessage compiler_message;
  const bool has_compiler_message = CompilerMessage::Parse(
      CompilerMessage::SOURCE_RUSTC, main_->root_folder(), line_content,
      &compiler_message);
  const wxString message = has_compiler_message
                               ? ToShortString(compiler_message.message(), 45)
                               : "<none>";

  wxMenu menu;
  AppendEnabled(menu, wxID_COPY, "Copy", has_selected);
  AppendEnabled(menu, wxID_SELECTALL, "Select all", true);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_COPY_THIS_COMPILER_MESSAGE,
                wxString::Format("Copy \"%s\" to clipboard", message),
                has_compiler_message);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_CLEAR_COMPILER_OUTPUT, "Clear output", true);

  PopupMenu(&menu);
}

void FindResultsControl::OnCopyThisCompilerMessage(wxCommandEvent& event) {
  const wxString line_content = GetContextLineContent();

  CompilerMessage message;
  if (CompilerMessage::Parse(CompilerMessage::SOURCE_RUSTC,
                             main_->root_folder(), line_content, &message)) {
    if (wxTheClipboard->Open()) {
      wxTheClipboard->SetData(new wxTextDataObject(message.message()));
      wxTheClipboard->Close();
    }
  } else {
    ShowWarning(this, "Unable to get compiler message data",
                "No compiler message data");
  }
}

void FindResultsControl::OnClearCompilerOuput(wxCommandEvent& event) {
  ::ClearOutput(text_);
}

void FindResultsControl::OnSelectAll(wxCommandEvent& event) {
  text_->SelectAll();
}

void FindResultsControl::OnCopy(wxCommandEvent& event) { text_->Copy(); }

const wxString FindResultsControl::GetContextLineContent() {
  WXID line_number = 0;
  WXID col = 0;
  const WXID index = context_positon_;
  text_->PositionToXY(index, &col, &line_number);
  if (line_number == -1) return wxEmptyString;
  const wxString line_content = text_->GetLineText(line_number);
  return line_content;
}

void FindResultsControl::ClearOutput() { ::ClearOutput(text_); }

void FindResultsControl::WriteLine(const wxString& mess) {
  ::WriteLine(text_, mess);
}

void FindResultsControl::OnDoubleClick(wxMouseEvent& event) {
  WXID line_number = 0;
  WXID col = 0;
  auto index = text_->GetInsertionPoint();
  text_->PositionToXY(index, &col, &line_number);
  if (line_number == -1) return;
  wxString line_content = text_->GetLineText(line_number);

  CompilerMessage message;
  if (CompilerMessage::Parse(CompilerMessage::SOURCE_RUSTC,
                             main_->root_folder(), line_content, &message)) {
    main_->OpenCompilerMessage(message);
  }
}
