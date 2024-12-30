// Copyright (2015) Gustav

#include "ride/outputcontrol.h"

#include <wx/clipbrd.h>
#include <wx/regex.h>
#include <wx/uri.h>

#include "settings.proto.h"  // NOLINT this is how we include it

#include "ride/cmdrunner.h"
#include "ride/compilermessage.h"
#include "ride/mainwindow.h"
#include "ride/stcutils.h"
#include "ride/wxutils.h"
#include "ride/enviroment.h"

enum {
  ID_FIRST = wxID_HIGHEST,

  ID_RUN_THIS_COMPILER_MESSAGE,
  ID_SEARCH_FOR_THIS_COMPILER_MESSAGE,
  ID_COPY_THIS_COMPILER_MESSAGE,
  ID_CLEAR_COMPILER_OUTPUT,
  ID_QUICK_OPEN,
  ID_SELECTALL,
  ID_COPY
};

namespace regex {
// src\main.rs:42:32: 42:58 help: run `rustc --explain E0243` to see a detailed
// explanation
const wxString GET_COMMAND_LINE = "run `(.*?)`";
}  // namespace regex

const wxRegEx& GetCommandLineRegex() {
  static wxRegEx ret(regex::GET_COMMAND_LINE, wxRE_ADVANCED);
  assert(ret.IsValid() && "Complex output regex failed to compile");
  return ret;
}

OutputControl::OutputControl(MainWindow* main, OutputControlFunctions functions)
    : wxControl(main, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                wxBORDER_NONE),
      main_(main),
      functions_(functions) {
  text_ = new wxStyledTextCtrl(
      this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
      wxBORDER_NONE | wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL);
  auto sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(text_, 1, wxEXPAND);
  SetSizer(sizer);
  BindEvents();
}

wxString GetCommandLine(const CompilerMessage& mess) {
  if (GetCommandLineRegex().Matches(mess.message())) {
    const size_t match_count = GetCommandLineRegex().GetMatchCount();
    assert(match_count == 2);
    const wxString cmd = GetCommandLineRegex().GetMatch(mess.message(), 1);
    return cmd;
  }

  return "";
}

void OutputControl::UpdateStyle() {
  text_->StyleClearAll();
  text_->SetReadOnly(true);

  const ride::Settings& set = main_->settings();
  SetupScintillaCurrentLine(text_, set);
  SetupScintillaDefaultStyles(text_, set);
  text_->SetEndAtLastLine(set.end_at_last_line);
}

void OutputControl::OnContextMenu(wxContextMenuEvent& event) {
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
  const wxString commandline =
      has_compiler_message ? GetCommandLine(compiler_message) : "";

  wxMenu menu;
  AppendEnabled(menu, ID_COPY, "Copy", has_selected);
  AppendEnabled(menu, ID_SELECTALL, "Select all", true);
  menu.AppendSeparator();
  if (functions_ & OCF_RUN_COMPILER_MESSAGE) {
    AppendEnabled(menu, ID_RUN_THIS_COMPILER_MESSAGE,
                  wxString::Format("Run '%s'", commandline.IsEmpty()
                                                   ? "<no command found>"
                                                   : commandline),
                  commandline.IsEmpty() == false);
  }
  if (functions_ & OCF_SEARCH_COMPILER_MESSAGE) {
    AppendEnabled(menu, ID_SEARCH_FOR_THIS_COMPILER_MESSAGE,
                  wxString::Format("Search for \"%s\" online", message),
                  has_compiler_message);
  }
  AppendEnabled(menu, ID_COPY_THIS_COMPILER_MESSAGE,
                wxString::Format("Copy \"%s\" to clipboard", message),
                has_compiler_message);
  menu.AppendSeparator();
  AppendEnabled(menu, ID_CLEAR_COMPILER_OUTPUT, "Clear output", true);

  PopupMenu(&menu);
}

const wxString ReplaceCmd(const ride::MachineSettings& machine,
                          const wxString& cmd) {
  if (cmd.StartsWith("rustc")) {
    return machine.rustc + cmd.Right(cmd.Length() - wxString("rustc").length());
  }

  if (cmd.StartsWith("cargo")) {
    return machine.cargo + cmd.Right(cmd.Length() - wxString("cargo").length());
  }

  if (cmd.StartsWith("protoc")) {
    return machine.protoc + cmd.Right(cmd.Length() - wxString("protoc").length());
  }

  return cmd;
}

void OutputControl::OnRunThisCompilerMessage(wxCommandEvent& event) {
  const wxString line_content = GetContextLineContent();

  CompilerMessage message;
  if (CompilerMessage::Parse(CompilerMessage::SOURCE_RUSTC,
                             main_->root_folder(), line_content, &message)) {
    const wxString cmd = ReplaceCmd(main_->machine(), GetCommandLine(message));
    wxString output;
    CmdRunner::Run(main_->root_folder(), cmd,
                   CollectRideSpecificEnviroment(main_->machine()), &output);
    ShowInfo(this, output, "Command result");
  }
}

void OutputControl::OnCopyThisCompilerMessage(wxCommandEvent& event) {
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

void OutputControl::OnClearCompilerOuput(wxCommandEvent& event) {
  ::ClearOutput(text_);
}

void OutputControl::OnSelectAll(wxCommandEvent& event) { text_->SelectAll(); }

void OutputControl::OnCopy(wxCommandEvent& event) { text_->Copy(); }

const wxString OutputControl::GetContextLineContent() {
  WXID line_number = 0;
  WXID col = 0;
  const WXID index = context_positon_;
  text_->PositionToXY(index, &col, &line_number);
  if (line_number == -1) return wxEmptyString;
  const wxString line_content = text_->GetLineText(line_number);
  return line_content;
}

void OutputControl::ClearOutput() { ::ClearOutput(text_); }
void OutputControl::WriteLine(const wxString& str) { ::WriteLine(text_, str); }

void OutputControl::OnSearchForThisCompilerMessage(wxCommandEvent& event) {
  const wxString line_content = GetContextLineContent();

  CompilerMessage message;
  if (CompilerMessage::Parse(CompilerMessage::SOURCE_RUSTC,
                             main_->root_folder(), line_content, &message)) {
    wxString mess = message.message();
    mess.Replace("#", "%23");
    const wxString escaped_message = wxURI(mess).BuildURI();
    const wxString url_to_open =
        wxString::Format("http://www.google.com/search?q=%s", escaped_message);
    wxLaunchDefaultBrowser(url_to_open);
  } else {
    ShowWarning(this, "Unable to get compiler message data",
                "No compiler message data");
  }
}

void OutputControl::OnDoubleClick(wxMouseEvent& event) {
  WXID line_number = 0;
  WXID col = 0;
  WXID index = text_->GetInsertionPoint();
  text_->PositionToXY(index, &col, &line_number);
  if (line_number == -1) return;
  wxString line_content = text_->GetLineText(line_number);

  CompilerMessage message;
  if (CompilerMessage::Parse(CompilerMessage::SOURCE_RUSTC,
                             main_->root_folder(), line_content, &message)) {
    main_->OpenCompilerMessage(message);
  }
}

void OutputControl::BindEvents() {
  Bind(wxEVT_LEFT_DCLICK, &OutputControl::OnDoubleClick, this);
  Bind(wxEVT_CONTEXT_MENU, &OutputControl::OnContextMenu, this);
  Bind(wxEVT_MENU, &OutputControl::OnSearchForThisCompilerMessage, this,
       ID_SEARCH_FOR_THIS_COMPILER_MESSAGE);
  Bind(wxEVT_MENU, &OutputControl::OnRunThisCompilerMessage, this,
       ID_RUN_THIS_COMPILER_MESSAGE);
  Bind(wxEVT_MENU, &OutputControl::OnCopyThisCompilerMessage, this,
       ID_COPY_THIS_COMPILER_MESSAGE);
  Bind(wxEVT_MENU, &OutputControl::OnClearCompilerOuput, this,
       ID_CLEAR_COMPILER_OUTPUT);
  Bind(wxEVT_MENU, &OutputControl::OnSelectAll, this, ID_SELECTALL);
  Bind(wxEVT_MENU, &OutputControl::OnCopy, this, ID_COPY);
}
