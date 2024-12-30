#ifndef RIDE_PROJECTEXPLORER_H_
#define RIDE_PROJECTEXPLORER_H_

#include <ride/wx.h>
#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include <map>
#include <vector>

class wxFileName;
class MainWindow;

typedef std::map<wxString, wxTreeItemId> StringIdMap;

struct FilesAndFolders
{
	StringIdMap files;
	StringIdMap folders;
};

class ProjectExplorer : public wxTreeCtrl
{
public:

	explicit ProjectExplorer(MainWindow* main);

	void UpdateColors();

	void SetFolder(const wxString& folder);
	void UpdateFolderStructure();
	void HighlightOpenFile(const wxString& file_path);

	wxString GetPathOfSelected() const;
	wxString GetRelativePathOfSelected() const;

	std::vector<wxString> GetFiles();

private:

	void OnDoubleClick(wxMouseEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);

	void OnCreateNewFolder(wxCommandEvent& event);
	void OnCreateNewFile(wxCommandEvent& event);
	void OnFolderCollapse(wxCommandEvent& event);
	void OnFolderExpand(wxCommandEvent& event);
	void OnFolderCollapseAllChildren(wxCommandEvent& event);
	void OnFolderExpandAllChildren(wxCommandEvent& event);
	void OnCollapseAll(wxCommandEvent& event);
	void OnExpandAll(wxCommandEvent& event);
	void OnOpenFile(wxCommandEvent& event);
	void OnDeleteFileOrFolder(wxCommandEvent& event);
	void OnRename(wxCommandEvent& event);
	void OnOpenExplorer(wxCommandEvent& event);

	void OnEditLabelStart(wxTreeEvent& event);
	void OnEditLabelEnd(wxTreeEvent& event);

	void BindEvents();

private:

	wxImageList images_;
	MainWindow* main_;
	wxString folder_;
	wxTreeItemId last_highlighted_item_;

	FilesAndFolders files_;
};

#endif	// RIDE_PROJECTEXPLORER_H_
