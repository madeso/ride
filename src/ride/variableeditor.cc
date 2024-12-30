#include "ride/variableeditor.h"

#include <wx/filedlg.h>
#include <wx/dirdlg.h>

#include <algorithm>
#include <string>

#include "ride/generated/ui.h"

class VariableEditorDlg : public ui::VariableEditor
{
public:

	VariableEditorDlg(wxWindow* parent, std::string* data);

protected:

	void OnTextChanged(wxCommandEvent& event) override;
	void OnFile(wxCommandEvent& event) override;
	void OnFolder(wxCommandEvent& event) override;
	void OnCustomDoubleClick(wxCommandEvent& event) override;
	void OnCancel(wxCommandEvent& event) override;
	void OnOk(wxCommandEvent& event) override;

protected:

	void AddText(const wxString& text);

private:

	std::string* data_;
};

bool DoVariableEditor(wxWindow* parent, std::string* text)
{
	std::string t = *text;
	VariableEditorDlg dlg(parent, text);
	if (dlg.ShowModal() == wxID_OK) return true;
	*text = t;
	return false;
}

VariableEditorDlg::VariableEditorDlg(wxWindow* parent, std::string* data)
	: ::ui::VariableEditor(parent, wxID_ANY)
	, data_(data)
{
	uiText->SetValue(*data);
	uiText->SelectAll();
	uiText->SetFocus();

	uiCustomVariables->Append("%built_application%");
	uiCustomVariables->Append("%cargo_folder%");
}

void VariableEditorDlg::OnTextChanged(wxCommandEvent& event)
{
	*data_ = uiText->GetValue();
}

void VariableEditorDlg::OnFile(wxCommandEvent& event)
{
	wxFileDialog sfd(this, _("Select file"), "", "", "All files (*.*)|*.*", wxFD_SAVE);
	if (sfd.ShowModal() == wxID_CANCEL) return;
	AddText(sfd.GetPath());
}

void VariableEditorDlg::AddText(const wxString& text)
{
	long from;	// NOLINT
	long to;  // NOLINT
	uiText->GetSelection(&from, &to);

	wxString value = uiText->GetValue();
	value.Remove(from, to - from);
	value.insert(from, text.c_str().AsChar(), text.length());
	uiText->SetValue(value);
}

void VariableEditorDlg::OnFolder(wxCommandEvent& event)
{
	wxDirDialog dlg(nullptr, "Select directory", "", wxDD_DEFAULT_STYLE);
	if (dlg.ShowModal() == wxID_CANCEL) return;
	AddText(dlg.GetPath());
}

void VariableEditorDlg::OnCustomDoubleClick(wxCommandEvent& event)
{
	int selected = uiCustomVariables->GetSelection();
	if (selected == -1) return;
	AddText(uiCustomVariables->GetString(selected));
}

void VariableEditorDlg::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void VariableEditorDlg::OnOk(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}
