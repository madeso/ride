#pragma once

#include <ride/wx.h>
#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include <map>
#include <vector>

#include "ride/path.h"

class wxFileName;
class MainWindow;

struct FilesAndFolders
{
	std::map<Fil, wxTreeItemId> files;
	std::map<Dir, wxTreeItemId> folders;
};

class ProjectExplorer : public wxTreeCtrl
{
public:

	explicit ProjectExplorer(MainWindow* main);

	void UpdateColors();

	void SetFolder(const Dir& folder);
	void UpdateFolderStructure();
	void HighlightOpenFile(const Fil& file_path);

	std::optional<Fil> GetPathOfSelected() const;

	std::vector<Fil> GetFiles();

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
	std::optional<Dir> folder_;
	wxTreeItemId last_highlighted_item_;

	FilesAndFolders files_;
};


