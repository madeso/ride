#ifndef RIDE_CREATENEWFILEDLG_H_
#define RIDE_CREATENEWFILEDLG_H_

#include "ride/wx.h"
#include "ride/path.h"

class CreateNewFileDlgHandler
{
public:

	CreateNewFileDlgHandler(
		wxWindow* parent, const Dir& project_folder, const Dir& fodler_hint
	);

	bool ShowModal();

	const std::optional<Fil> file_path() const;
	const wxString template_source() const;

private:

	wxWindow* parent_;
	Dir project_folder_;
	wxString fodler_hint_;

	std::optional<Fil> file_path_;
	wxString template_source_;
};

#endif	// RIDE_CREATENEWFILEDLG_H_
