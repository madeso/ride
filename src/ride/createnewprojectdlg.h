#pragma once

#include "ride/wx.h"
#include "ride/path.h"
#include "ride/settings.h"

class CreateNewProjectDlgHandler
{
public:

	explicit CreateNewProjectDlgHandler(wxWindow* parent);

	bool ShowModal(const ride::MachineSettings& machine);

	Dir project_folder() const;
	wxString project_name() const;

	Dir target() const;
	wxString cargo_command_line() const;

private:

	wxWindow* parent_;

	Dir project_folder_;
	wxString project_name_;
	std::optional<Dir> target_;
	wxString cargo_command_line_;
};
