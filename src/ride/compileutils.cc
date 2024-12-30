#include "ride/compileutils.h"

#include <ride/wx.h>

#include <wx/filename.h>

#include <vector>

#include "ride/cmdrunner.h"
#include "ride/compilermessage.h"
#include "ride/mainwindow.h"
#include "ride/wxutils.h"
#include "ride/enviroment.h"

void AddAllOutput(
	const wxString& result,
	const CompilerMessage::Source source,
	const wxString folder,
	MainWindow* main_window
)
{
	const std::vector<wxString> lines = Split(result, "\n");
	main_window->compiler_output().Clear();
	for (const wxString& line: lines)
	{
		CompilerMessage message;

		wxString theline = line;
		// if it looks like a protoc compiler message, transform it into a rustc
		// message
		if (CompilerMessage::Parse(CompilerMessage::SOURCE_PROTOC, folder, line, &message))
		{
			theline = message.ToStringRepresentation(CompilerMessage::SOURCE_RUSTC);
		}
		main_window->compiler_output().Append(theline);
	}
}

void CompileProtoFile(
	const ride::MachineSettings& machine, const wxString& full_path, MainWindow* main_window
)
{
	if (full_path.EndsWith(".proto") == false) return;

	const size_t index = full_path.find_last_of(wxFileName::GetPathSeparators());
	const wxString folder = full_path.SubString(0, index);
	const wxString file = full_path.substr(index + 1);
	const wxString cmd = wxString::Format((machine.protoc + " --rust_out . %s").c_str(), file);
	wxString result;
	const bool proto_compile_successful
		= CmdRunner::Run(folder, cmd, CollectRideSpecificEnviroment(machine), &result);

	AddAllOutput(result, CompilerMessage::SOURCE_PROTOC, folder, main_window);

	if (proto_compile_successful)
	{
		main_window->compiler_output().Append(
			wxString::Format("%s compiled without errors\n", full_path)
		);
	}
	else
	{
		main_window->compiler_output().Append(wxString::Format("%s failed to compile\n", full_path)
		);
		ShowError(main_window, "Protobuf failed to compile", "Compilation failed!");
	}
}
