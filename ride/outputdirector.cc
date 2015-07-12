#include "ride/wxutils.h"
#include "ride/wx.h"
#include "ride/outputdirector.h"

#include "ride/outputcontrol.h"
#include "ride/mainwindow.h"
#include "ride/fileedit.h"

OutputDirector::OutputDirector()
  : control_(NULL), main_(NULL) {
}

void OutputDirector::Create(MainWindow* window, wxAuiManager& aui, const wxString& name, const wxString& caption) {
  main_ = window;
  control_ = new OutputControl(window);
  control_->Create(window, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxHSCROLL);
  control_->UpdateStyle();
  control_->UpdateStyle();
  aui.AddPane(control_, wxAuiPaneInfo().Name(name).Caption(caption).Bottom().CloseButton(true));
}

void OutputDirector::Clear() {
  // todo: this probably needs to happen in the gui thread instead of here... or does it?
  ClearOutput(control_);
  compiler_messages_.resize(0);

  for (const wxString file : files_) {
    FoundEdit edit = main_->GetEditFromFileName(file);
    if (edit) {
      edit.edit->ClearCompilerMessages();
    }
  }
}

void OutputDirector::Append(const wxString& str) {
  // todo: this probably needs to happen in the gui thread instead of here... or does it?
  WriteLine(control_, str);

  CompilerMessage mess;
  if (CompilerMessage::Parse(CompilerMessage::SOURCE_RUSTC, main_->root_folder(), str, &mess)) {
    AddCompilerMessage(mess);
  }
}

void OutputDirector::AddCompilerMessage(const CompilerMessage& mess) {
  compiler_messages_.push_back(mess);
  files_.push_back(mess.file());
  FoundEdit edit = main_->GetEditFromFileName(mess.file());
  if ( edit ) {
    edit.edit->AddCompilerMessage(mess);
  }
}

void OutputDirector::UpdateStyles() {
  control_->UpdateStyle();
}

void OutputDirector::AddAllCompilerMessages(FileEdit* file_edit) {
  for (const CompilerMessage message : compiler_messages_) {
    if (message.file() == file_edit->filename()) {
      file_edit->AddCompilerMessage(message);
    }
  }
}
