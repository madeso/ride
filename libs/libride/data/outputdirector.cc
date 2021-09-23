// Copyright (2015) Gustav

#include "ride/outputdirector.h"

#include <ride/wx.h>

#include "ride/fileedit.h"
#include "ride/mainwindow.h"
#include "ride/outputcontrol.h"
#include "ride/wxutils.h"

OutputDirector::OutputDirector() : main_(nullptr), control_(nullptr) {}

void OutputDirector::Create(MainWindow* window, wxAuiManager* aui,
                            const wxString& name, const wxString& caption) {
  main_ = window;
  control_ = new OutputControl(
      window, static_cast<OutputControlFunctions>(OCF_RUN_COMPILER_MESSAGE |
                                                  OCF_SEARCH_COMPILER_MESSAGE));
  control_->UpdateStyle();
  control_->UpdateStyle();
  aui->AddPane(
      control_,
      wxAuiPaneInfo().Name(name).Caption(caption).Bottom().CloseButton(true));
}

void OutputDirector::Clear() {
  // todo: this probably needs to happen in the gui thread instead of here... or
  // does it?
  control_->ClearOutput();
  compiler_messages_.resize(0);

  for (const wxString file : files_) {
    FoundEdit edit = main_->GetEditFromFileName(file);
    if (edit) {
      edit.edit->ClearCompilerMessages();
    }
  }
}

void OutputDirector::Append(const wxString& str) {
  // todo: this probably needs to happen in the gui thread instead of here... or
  // does it?
  control_->WriteLine(str);

  CompilerMessage mess;
  if (CompilerMessage::Parse(CompilerMessage::SOURCE_RUSTC,
                             main_->root_folder(), str, &mess)) {
    AddCompilerMessage(mess);
  }
}

void OutputDirector::AddCompilerMessage(const CompilerMessage& mess) {
  compiler_messages_.push_back(mess);
  files_.push_back(mess.file());
  FoundEdit edit = main_->GetEditFromFileName(mess.file());
  if (edit) {
    edit.edit->AddCompilerMessage(mess);
  }
}

void OutputDirector::UpdateStyles() { control_->UpdateStyle(); }

void OutputDirector::AddAllCompilerMessages(FileEdit* file_edit) {
  for (const CompilerMessage message : compiler_messages_) {
    if (message.file() == file_edit->filename()) {
      file_edit->AddCompilerMessage(message);
    }
  }
}
