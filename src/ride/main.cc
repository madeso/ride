#include <ride/wx.h>

#include "settings.proto.h"	 // NOLINT this is how we include it

#include "ride/mainwindow.h"

class MyApp : public wxApp
{
public:

	bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	const wxString DISPLAY_NAME = "Ride";

	/*
	#include <unistd.h>
	#include <cstdlib>

	int main(int argc, char** argv) {
		if (daemon(0, 0) == -1) {
			perror("daemon failed");
			return EXIT_FAILURE;
		}
	*/

	std::vector<wxFileName> files_to_open;
	std::vector<wxFileName> dirs_to_open;
	bool ok = true;
	for (int i = 1; i < argc; ++i)
	{
		const auto argument = argv[i];

		wxString flag;
		if(argument.StartsWith("/", &flag) || argument.StartsWith("/", &flag))
		{
			wxPrintf("Unknown flag at %d: %s\n", i, argument);
			ok = false;
		}
		else
		{
			wxFileName file {argument};
			file.Normalize(wxPATH_NORM_ABSOLUTE | wxPATH_NORM_DOTS);
			if(file.FileExists() || file.DirExists())
			{
				if(file.FileExists())
				{
					files_to_open.emplace_back(file);
				}
				else
				{
					dirs_to_open.emplace_back(file);
				}
			}
			else
			{
				wxPrintf("Neither a valid file or dir at %d: %s\n", i, argument);
				ok = false;
			}
		}
    }

	if(ok == false)
	{
		return false;
	}

	using OptionalFileName = std::optional<wxFileName>;
	const OptionalFileName project_to_open
		= dirs_to_open.size() == 1
		? OptionalFileName{dirs_to_open[0]}
		: std::nullopt
		;
	if(dirs_to_open.size() > 1)
	{
		wxPrintf("ERROR: too many directories specified!\n");
		for(const auto& dir: dirs_to_open)
		{
			wxPrintf(" * was asked to open: %s\n", dir.GetPath());
		}
		return false;
	}

	for(const auto& f: files_to_open)
	{
		wxPrintf("Will open file: %s\n", f.GetFullPath());
	}
	if(project_to_open)
	{
		wxPrintf("Will open project: %s\n", project_to_open->GetPathWithSep());
	}

	SetAppName("ride");
	SetAppDisplayName(DISPLAY_NAME);
	SetVendorName("madeso");
	SetVendorDisplayName("madeso");

	auto main_window = new MainWindow(DISPLAY_NAME, files_to_open, project_to_open, wxPoint(50, 50), wxSize(450, 340));
	main_window->Show(true);
	return true;
}
