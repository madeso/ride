#include "ride/project.h"

#include <wx/choicdlg.h>
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/filename.h>

#include <string>
#include <vector>

#include "settings.proto.h"	 // NOLINT this is how we include it

#include "ride/wxutils.h"
#include "ride/mainwindow.h"
#include "ride/proto.h"
#include "ride/projectsettingsdlg.h"
#include "ride/enviroment.h"

Project::Project(MainWindow* output, const wxString& root_folder)
	: main_(output)
	, root_folder_(root_folder)
{
	if (root_folder_.IsEmpty() == false)
	{
		if (LoadProtoJson(&project_, GetProjectFile()) != "")
		{
		}
		if (project_.build_settings.size() == 0)
		{
			// add default build settings to the project
			ride::BuildSetting debug;
			debug.name = "Debug";
			debug.release = false;
			project_.build_settings.push_back(debug);

			ride::BuildSetting release;
			release.name = "Release";
			release.release = true;
			project_.build_settings.push_back(release);
		}

		if (LoadProtoJson(&user_, GetUserFile()) != "")
		{
		}

		if (user_.run.size() == 0)
		{
			user_.run.push_back({});
			SaveUser();
		}

		// validate project file
		GetCurrentBuildSetting();
	}
}

Project::~Project()
{
	Save();
}

const wxString& Project::root_folder() const
{
	return root_folder_;
}

bool Project::Save()
{
	if (root_folder_.IsEmpty()) return false;
	bool project_saved = SaveProtoJson(project_, GetProjectFile()) != "";
	bool user_saved = SaveUser();
	return project_saved && user_saved;
}

int Project::tabwidth() const
{
	return project_.tabWidth;
}

bool Project::usetabs() const
{
	return project_.useTabs;
}

void Project::set_tabwidth(int tabwidth)
{
	project_.tabWidth = tabwidth;
}

void Project::set_usetabs(bool usetabs)
{
	project_.useTabs = usetabs;
}

const ride::Project& Project::project() const
{
	return project_;
}

ride::Project* Project::project_ptr()
{
	return &project_;
}

void Project::set_project(const ride::Project& project)
{
	project_ = project;
}

const ride::UserProject& Project::user() const
{
	return user_;
}

ride::UserProject* Project::user_ptr()
{
	return &user_;
}

void Project::set_user(const ride::UserProject& user)
{
	user_ = user;
}

const wxString Project::GetCargoFile() const
{
	if (root_folder_.IsEmpty()) return "";
	wxFileName cargo(root_folder_, "cargo.toml");
	return cargo.GetFullPath();
}

const wxString Project::GetProjectFile() const
{
	if (root_folder_.IsEmpty()) return "";
	wxFileName cargo(root_folder_, "project.ride");
	return cargo.GetFullPath();
}

const wxString Project::GetUserFile() const
{
	if (root_folder_.IsEmpty()) return "";
	wxFileName cargo(root_folder_, "project.ride.user");
	return cargo.GetFullPath();
}

bool Project::IsPartOfProject(const wxString& filename)
{
	// todo: implement a better logic for checking if the file is part of the
	// project
	return true;
}

void Project::Settings()
{
	DoProjectSettingsDlg(main_, main_, this);
}

void Project::SelectActiveBuild()
{
	std::vector<wxString> names;
	names.reserve(project_.build_settings.size());
	for (const ride::BuildSetting& setting: project_.build_settings)
	{
		names.push_back(setting.name);
	}

	wxSingleChoiceDialog dlg(nullptr, "Select build", "Build", names.size(), &names[0]);
	dlg.SetSelection(user_.build_setting);
	const int dialog_result = dlg.ShowModal();

	if (dialog_result != wxID_OK) return;
	user_.build_setting = dlg.GetSelection();
	SaveUser();
	SetMainStatusbarText();
}

void Project::SelectActiveRun()
{
	std::vector<wxString> names;
	names.reserve(user_.run.size());
	for (const ride::RunSetting& setting: user_.run)
	{
		names.push_back(setting.name);
	}

	wxSingleChoiceDialog dlg(nullptr, "Select run", "Build", names.size(), &names[0]);
	dlg.SetSelection(user_.run_setting);
	const int dialog_result = dlg.ShowModal();

	if (dialog_result != wxID_OK) return;
	user_.run_setting = dlg.GetSelection();
	SaveUser();
	SetMainStatusbarText();
}

void Project::SaveAllFiles()
{
	main_->SaveAllChangedProjectFiles();
	Save();
}

wxString BuildCommandLine(const ride::MachineSettings& machine, const ride::BuildSetting& build)
{
	wxString cmd = machine.cargo + " build";

	if (build.release)
	{
		cmd += " --release";
	}

	if (build.features.size() > 0)
	{
		cmd += " --features";
		for (const std::string& f: build.features)
		{
			cmd += " " + f;
		}
	}

	if (build.default_features == false)
	{
		cmd += " --no-default-features";
	}

	if (build.target.empty() == false)
	{
		cmd += " --target " + build.target;
	}

	if (build.verbose)
	{
		cmd += " --verbose";
	}

	if (build.custom_arguments.empty() == false)
	{
		cmd += " " + build.custom_arguments;
	}

	return cmd;
}

void Project::Build(bool origin_main)
{
	if (origin_main)
	{
		CleanOutput();
		SaveAllFiles();
	}

	const ride::BuildSetting& build = GetCurrentBuildSetting();
	const wxString cmd = BuildCommandLine(main_->machine(), build);

	RunCmd(cmd);
}

void Project::Clean(bool origin_main)
{
	if (origin_main)
	{
		CleanOutput();
		SaveAllFiles();
	}

	// todo: expand commandline with arguments
	RunCmd(main_->machine().cargo + " clean");
}

void Project::Rebuild(bool origin_main)
{
	if (origin_main)
	{
		CleanOutput();
		SaveAllFiles();
	}
	Clean(false);
	Build(false);
}

void Project::Doc(bool origin_main)
{
	if (origin_main)
	{
		CleanOutput();
		SaveAllFiles();
	}

	// todo: expand commandline with arguments
	RunCmd(main_->machine().cargo + " doc");
}

void Project::Run(bool origin_main)
{
	if (origin_main)
	{
		CleanOutput();
		SaveAllFiles();
	}

	Build(false);
	// todo: run the application
}

void Project::Test(bool origin_main)
{
	if (origin_main)
	{
		CleanOutput();
		SaveAllFiles();
	}

	// todo: expand commandline with arguments
	RunCmd(main_->machine().cargo + " test");
}

void Project::Bench(bool origin_main)
{
	if (origin_main)
	{
		CleanOutput();
		SaveAllFiles();
	}

	// todo: expand commandline with arguments
	RunCmd(main_->machine().cargo + " bench");
}

void Project::Update(bool origin_main)
{
	if (origin_main)
	{
		CleanOutput();
		SaveAllFiles();
	}

	// todo: expand commandline with arguments
	RunCmd(main_->machine().cargo + " update");
}

//////////////////////////////////////////////////////////////////////////

void Project::CleanOutput()
{
	main_->build_output().Clear();
}

void Project::Append(const wxString& str)
{
	main_->build_output().Append(str);
}

void Project::RunCmd(const wxString& cmd)
{
	if (root_folder_.IsEmpty())
	{
		ShowInfo(
			main_, "No project open, you need to open a cargo project first!", "No project open!"
		);
		return;
	}

	MultiRunner::RunCmd(Command(root_folder_, cmd, CollectRideSpecificEnviroment(main_->machine()))
	);
}

bool Project::SaveUser()
{
	return SaveProtoJson(user_, GetUserFile()) != "";
}

int Project::GetSelectedBuildIndex()
{
	if (project_.build_settings.size() <= 0) return -1;

	if (user_.build_setting < 0 || user_.build_setting >= project_.build_settings.size())
	{
		user_.build_setting = 0;
		SaveUser();
		SetMainStatusbarText();
	}

	return user_.build_setting;
}

const ride::BuildSetting& Project::GetCurrentBuildSetting()
{
	int index = GetSelectedBuildIndex();
	if (index == -1)
		return ride::BuildSetting{};
	else
		return project_.build_settings[index];
}

int Project::GetSelectedRunIndex()
{
	if (user_.run.size() <= 0) return -1;

	if (user_.run_setting < 0 || user_.run_setting >= user_.run.size())
	{
		user_.run_setting = 0;
		SaveUser();
		SetMainStatusbarText();
	}

	return user_.run_setting;
}

const ride::RunSetting& Project::GetCurrentRunSetting()
{
	int index = GetSelectedRunIndex();
	if (index == -1)
		return ride::RunSetting{};
	else
		return user_.run[index];
}

void Project::SetMainStatusbarText()
{
	main_->SetStatusBarText(GetCurrentBuildSetting().name, STATUSBAR_BUILD_CONF);
	main_->SetStatusBarText(GetCurrentRunSetting().name, STATUSBAR_RUN_CONF);
}
