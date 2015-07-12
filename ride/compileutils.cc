#include "ride/compileutils.h"
#include "ride/wx.h"
#include "ride/wxutils.h"
#include "ride/mainwindow.h"
#include "ride/compilermessage.h"
#include "ride/cmdrunner.h"
#include "wx/filename.h"

void CompileProtoFile(const wxString& filename, MainWindow* main_window) {
  if (filename.EndsWith(".proto")) {
    // if we managed to successfully save a protobuf file, then
    // run the protobuf compiler automatically
    const size_t index = filename.find_last_of(wxFileName::GetPathSeparators());
    const wxString folder = filename.SubString(0, index);
    const wxString filename = filename.substr(index + 1);
    const wxString cmd = wxString::Format("protoc --rust_out . %s", filename);
    wxString result;
    const bool proto_compile_successful = CmdRunner::Run(folder, cmd, &result);
    if (proto_compile_successful) {
      ShowInfo(main_window, wxString::Format("%s compiled without errors", filename), "Compilation successful!");
    }
    else {
      const std::vector<wxString> lines = Split(result, "\n");
      // TODO: change a different error output pane...
      main_window->compiler_output().Clear();
      for (const wxString& line : lines) {
        CompilerMessage message;

        wxString theline = line;
        // if it looks like a protoc compiler message, transform it into a rustc message
        if (CompilerMessage::Parse(CompilerMessage::SOURCE_PROTOC, folder, line, &message)) {
          theline = message.ToStringRepresentation(CompilerMessage::SOURCE_RUSTC);
        }
        main_window->compiler_output().Append(theline);
      }
      ShowError(main_window, wxString::Format("%s failed to compile", filename), "Compilation failed!");
    }
  }
}

