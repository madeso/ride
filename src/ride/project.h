#ifndef RIDE_PROJECT_H_
#define RIDE_PROJECT_H_

#include <ride/wx.h>
#include "settings.proto.h"	 // NOLINT this is how we include it
#include "ride/runner.h"
#include "wx/filename.h"

class MainWindow;
struct Fil;

class Project : public MultiRunner
{
public:

	Project(MainWindow* output, const std::optional<Dir>& root_folder);
	~Project();
	const std::optional<Dir>& root_folder() const;

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

	const std::optional<Fil> GetCargoFile() const;
	const std::optional<Fil> GetProjectFile() const;
	const std::optional<Fil> GetUserFile() const;
	Fil GetSessionsFile() const;

	bool IsPartOfProject(const Fil& filename);

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

	const ride::BuildSetting& GetCurrentBuildSetting();
	const ride::RunSetting& GetCurrentRunSetting();

private:

	void SaveAllFiles();
	void CleanOutput();
	void RunCmd(const wxString& cmd, const wxString& folder);

	bool SaveUser();
	int GetSelectedBuildIndex();

	int GetSelectedRunIndex();

private:

	MainWindow* main_;
	std::optional<Dir> root_folder_;

	ride::Project project_;
	ride::UserProject user_;
};

#endif	// RIDE_PROJECT_H_
