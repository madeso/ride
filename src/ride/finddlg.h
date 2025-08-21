#pragma once

#include <ride/wx.h>

class FileEdit;
struct Dir;
struct Fil;
class OutputControl;
class MainWindow;

enum class FindAction
{
	Find,
	Replace
};

enum class FindScope
{
	File,
	Project
};

bool ShowFindDlg(
	MainWindow* parent,
	const wxString& current_selection,
	const std::optional<Fil>& current_file,
	FileEdit* file_edit,
	const std::optional<Dir>& root_folder,
	OutputControl* output,
	FindAction find_action,
	FindScope find_scope
);
