#ifndef RIDE_PROJECT_H_
#define RIDE_PROJECT_H_

#include <ride/wx.h>
#include "settings.proto.h"	 // NOLINT this is how we include it
#include "ride/runner.h"
#include "wx/filename.h"

class MainWindow;

class Project : public MultiRunner
{
public:

	Project(MainWindow* output, const wxString& root_folder);
	~Project();
	const wxString& root_folder() const;

	bool Save();

	int tabwidth() const;
	bool usetabs() const;

	void set_tabwidth(int tabwidth);
	void set_usetabs(bool usetabs);

	const ride::Project& project() const;
	ride::Project* project_ptr();
	void set_project(const ride::Project& project);

	const ride::UserProject& user() const;
	ride::UserProject* user_ptr();
	void set_user(const ride::UserProject& user);

	const wxString GetCargoFile() const;
	const wxString GetProjectFile() const;
	const wxString GetUserFile() const;
	wxFileName GetSessionsFile() const;

	bool IsPartOfProject(const wxString& filename);

	void Settings();
	void SelectActiveBuild();
	void SelectActiveRun();
	void SetMainStatusbarText();

	void Build(bool origin_main = true);
	void Clean(bool origin_main = true);
	void Rebuild(bool origin_main = true);
	void Run(bool origin_main = true);

public:

	void Append(const wxString& str);

private:

	void SaveAllFiles();
	void CleanOutput();
	void RunCmd(const wxString& cmd);

	bool SaveUser();
	int GetSelectedBuildIndex();
	const ride::BuildSetting& GetCurrentBuildSetting();

	int GetSelectedRunIndex();
	const ride::RunSetting& GetCurrentRunSetting();

private:

	MainWindow* main_;
	wxString root_folder_;

	ride::Project project_;
	ride::UserProject user_;
};

wxString BuildCommandLine(const ride::MachineSettings& machine, const ride::BuildSetting& build);

#endif	// RIDE_PROJECT_H_
