// Copyright (2015) Gustav

#include "ride/deletefolderdlg.h"

#include <ride/wx.h>
#include "ride/generated/ui.h"

class DeleteFolderDlg : public ui::DeleteFolder
{
public:

	DeleteFolderDlg(wxWindow* parent, const wxString& message, const wxString& title);

	bool full() const;
	bool recursive() const;

	void set_full(bool new_full) const;
	void set_recursive(bool new_recursive) const;

protected:

	void OnNo(wxCommandEvent& event) override;
	void OnYes(wxCommandEvent& event) override;
};

namespace
{
static bool DefaultFull = true;
static bool DefaultRecursive = true;
}  // namespace

DeleteFolderDlg::DeleteFolderDlg(wxWindow* parent, const wxString& message, const wxString& title)
	: ui::DeleteFolder(parent, wxID_ANY, title)
{
	uiRemoveFileDescription->SetLabel(message);
	set_full(DefaultFull);
	set_recursive(DefaultRecursive);
}

bool DeleteFolderDlg::full() const
{
	return uiFull->GetValue();
}

bool DeleteFolderDlg::recursive() const
{
	return uiRecursive->GetValue();
}

void DeleteFolderDlg::set_full(bool new_full) const
{
	uiFull->SetValue(new_full);
}

void DeleteFolderDlg::set_recursive(bool new_recursive) const
{
	uiRecursive->SetValue(new_recursive);
}

void DeleteFolderDlg::OnNo(wxCommandEvent& event)
{
	EndModal(wxID_NO);
}

void DeleteFolderDlg::OnYes(wxCommandEvent& event)
{
	DefaultFull = full();
	DefaultRecursive = recursive();
	EndModal(wxID_YES);
}

bool ShowDeleteFolderDlg(wxWindow* parent, const wxString& folder, bool* full, bool* recursive)
{
	assert(full);
	assert(recursive);
	DeleteFolderDlg dlg(parent, "Are you sure you want to delete " + folder, "Delete?");
	if (wxID_YES != dlg.ShowModal()) return false;
	*full = dlg.full();
	*recursive = dlg.recursive();
	return true;
}
