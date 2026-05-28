#include "ride/projectexplorer.h"

#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/textdlg.h>

#include <utility>
#include <vector>

#include "ride/resources/icons.h"

#include "ride/deletefolderdlg.h"
#include "ride/mainwindow.h"
#include "ride/wxutils.h"

enum
{
	ICON_FILE_NORMAL,
	ICON_FOLDER_NORMAL
};

ProjectExplorer::ProjectExplorer(MainWindow* main)
	: wxTreeCtrl(
		  main,
		  wxID_ANY,
		  wxDefaultPosition,
		  wxDefaultSize,
		  wxTR_HAS_BUTTONS | wxTR_TWIST_BUTTONS | wxTR_MULTIPLE | wxTR_LINES_AT_ROOT
			  | wxTR_EDIT_LABELS | wxBORDER_NONE
	  )
	, images_(16, 16)
	, main_(main)
	, last_highlighted_item_(nullptr)
{
	BindEvents();
	UpdateColors();

	wxTreeCtrl::SetImageList(&images_);

	images_.Add(wxIcon(file_normal_xpm));
	images_.Add(wxIcon(folder_normal_xpm));
}

void ProjectExplorer::UpdateColors()
{
	const auto* theme = main_->settings().find_current_theme();
	if (! theme) return;
	wxTreeCtrl::SetBackgroundColour(LOOKUP_COLOR(*theme, explorer_background).value_or(ride::BLACK));
	wxTreeCtrl::SetForegroundColour(LOOKUP_COLOR(*theme, explorer_foreground).value_or(ride::BLACK));
}

void ProjectExplorer::SetFolder(const Dir& folder)
{
	folder_ = folder;
	UpdateFolderStructure();
}

wxString GetRelativePath(const wxString& root, const wxString& f)
{
	if (f == "") return "";
	if(f.StartsWith(root))
	{
		wxString p = f.substr(root.Length());
		p.Replace("\\", "/");
		return p;
	}
	else
	{
		return f;
	}
}

void ProjectExplorer::HighlightOpenFile(const Fil& f)
{
	auto res = files_.files.find(f);
	const wxTreeItemId id = res != files_.files.end() ? res->second : wxTreeItemId();
	if (id.IsOk())
	{
		this->SetItemBold(id, true);
	}
	if (last_highlighted_item_.IsOk())
	{
		this->SetItemBold(last_highlighted_item_, false);
	}
	last_highlighted_item_ = id;
}

Dir SubFolder(const Dir& root, const wxString& sub_folder)
{
	return root.subdir(sub_folder);
}

/// Contains either a Dir or a Fil structure
// todo(Gustav) replace a better structure
class FileEntry : public wxTreeItemData
{
public:
	FileEntry(bool is_directory, const wxString& path)
		: is_directory_(is_directory)
		, path_(path)
	{
	}

	const wxString& path() const
	{
		return path_;
	}

	void set_path(const wxString& path)
	{
		path_ = path;
	}

	bool is_directory() const
	{
		return is_directory_;
	}

private:

	bool is_directory_;
	wxString path_;
};

typedef std::pair<wxTreeItemId, FileEntry*> TreeItemFileEntry;

TreeItemFileEntry GetTreeItemData(const ProjectExplorer* pe, wxTreeItemId selected)
{
	if (selected.IsOk() == false) return TreeItemFileEntry(nullptr, nullptr);
	wxTreeItemData* data = pe->GetItemData(selected);
	if (data)
	{
		FileEntry* entry = reinterpret_cast<FileEntry*>(data);
		return TreeItemFileEntry(selected, entry);
	}
	else
	{
		return TreeItemFileEntry(selected, nullptr);
	}
}

TreeItemFileEntry GetFocused(const ProjectExplorer* pe)
{
	wxTreeItemId selected = pe->GetFocusedItem();
	return GetTreeItemData(pe, selected);
}

FileEntry* GetFocusedFileEntry(const ProjectExplorer* pe)
{
	auto entry = GetFocused(pe);
	return entry.second;
}

std::optional<Fil> ProjectExplorer::GetSelectedFile() const
{
	FileEntry* file = GetFocusedFileEntry(this);
	if (file == nullptr) return std::nullopt;
	if(file->is_directory()) return std::nullopt;
	return Fil::from_full_path(file->path());
}


std::optional<Dir> ProjectExplorer::GetSelectedDir() const
{
	FileEntry* file = GetFocusedFileEntry(this);
	if (file == nullptr) return std::nullopt;
	if (file->is_directory() == false) return std::nullopt;
	return Dir::from_full_path(file->path());
}

struct Traversed
{
	std::vector<Fil> files;
	std::vector<Dir> dirs;
};

template<typename F>
void Traverse(wxString root_full_path, int flags, const wxString filespec, F&& callback)
{
	wxDir directory(root_full_path);
	directory.Open(root_full_path);

	wxString file_or_directory_name;
	bool cont = directory.GetFirst(&file_or_directory_name, filespec, flags);
	while (cont)
	{
		callback(file_or_directory_name);
		cont = directory.GetNext(&file_or_directory_name);
	}
	directory.Close();
}

Traversed TraverseFilesAndFolders(const Dir& root, const wxString filespec, const int flags)
{
	const wxString root_full_path = root.full_path();

	Traversed ret;

	Traverse(root_full_path, flags | wxDIR_DIRS, filespec, [&](const wxString& dir) {
		ret.dirs.emplace_back(root.subdir(dir));
	});
	Traverse(root_full_path, flags | wxDIR_FILES, filespec, [&](const wxString& file) {
		ret.files.emplace_back(root.file(file));
	});

	return ret;
}

wxString JoinPath(const wxFileName& root, const wxString& file_or_folder)
{
	// todo: is this really the correct way to do things?
	return root.GetFullPath() + file_or_folder;
}

wxTreeItemId TreeItemIdNull()
{
	wxTreeItemId ret;
	return ret;
}

void ListFilesAndFolders(
	FilesAndFolders* ret,
	const Dir& root,
	const wxString& filespec,
	const int flags,
	int depth
)
{
	ret->folders.insert({root, TreeItemIdNull()});

	const auto traversed = TraverseFilesAndFolders(root, filespec, flags);

	for (const auto& file: traversed.files)
	{
		ret->files.insert({file, TreeItemIdNull()});
	}

	for (const auto& dir: traversed.dirs)
	{
		ListFilesAndFolders(ret, dir, filespec, flags, depth + 1);
	}
}

FilesAndFolders ListFilesAndFolders(const Dir& root, const wxString& filespec, int flags)
{
	FilesAndFolders ff;
	ListFilesAndFolders(&ff, root, filespec, flags, 0);
	return ff;
}

std::vector<wxTreeItemId> ListChildren(wxTreeCtrl* tree, const wxTreeItemId& root)
{
	std::vector<wxTreeItemId> ret;
	wxTreeItemIdValue cookie;
	for (wxTreeItemId child = tree->GetFirstChild(root, cookie); child.IsOk();
		 child = tree->GetNextChild(root, cookie))
	{
		ret.push_back(child);
	}
	return ret;
}

FileEntry* GetFileEntryOrNull(wxTreeCtrl* tree, const wxTreeItemId& id)
{
	assert(id.IsOk());
	wxTreeItemData* data = tree->GetItemData(id);
	if (data == nullptr) return nullptr;
	FileEntry* entry = reinterpret_cast<FileEntry*>(data);
	return entry;
}

void ListTree(
	FilesAndFolders* ff,
	wxTreeCtrl* tree,
	const Dir& root,
	const wxTreeItemId& id,
	FileEntry* data
)
{
	assert(id.IsOk());
	assert(data);
	assert(data->is_directory());

	ff->folders.insert({Dir::from_full_path(data->path()), id});

	const std::vector<wxTreeItemId> children = ListChildren(tree, id);
	for (const auto& child: children)
	{
		FileEntry* entry = GetFileEntryOrNull(tree, child);
		if (entry->is_directory())
		{
			ListTree(ff, tree, root, child, entry);
		}
		else
		{
			ff->files.insert({Fil::from_full_path(entry->path()), child});
		}
	}
}

FilesAndFolders ListTree(wxTreeCtrl* tree, const Dir& root)
{
	FilesAndFolders ret;

	wxTreeItemId root_id = tree->GetRootItem();
	if (root_id.IsOk())
	{
		ListTree(&ret, tree, root, root_id, GetFileEntryOrNull(tree, root_id));
	}

	return ret;
}

wxString FindParentPath(const wxString& pp)
{
	auto p = pp;
	if (p.EndsWith('/')) p = p.substr(0, p.Length() - 1);
	auto i = p.find_last_of('/');
	if (i == -1) return "";
	wxString t = p.substr(0, i);
	return t + "/";
}

wxTreeItemId FindRoot(wxTreeCtrl* tree, const Dir& root, const Dir& path)
{
	// TODO(Gustav): Optimize
	FilesAndFolders ff = ListTree(tree, root);
	const auto root_path = path.parent();
	if (!root_path)
	{
		return tree->GetRootItem();
	}

	auto f = ff.folders.find(*root_path);
	assert(f != ff.folders.end());
	return f->second;
}

void ProjectExplorer::UpdateFolderStructure()
{
	if(!folder_)
	{
		// todo(Gustav): clear items
		return;
	}

	// todo(Gustav): add option to include wxDIR_HIDDEN
	const int flags = wxDIR_NO_FOLLOW; // walk files and folders
	const wxString filespec = "";

	const FilesAndFolders current = ListFilesAndFolders(*folder_, filespec, flags);
	const FilesAndFolders tree = ListTree(this, *folder_);

	this->Freeze();

	// add missing folders
	for (auto i: current.folders)
	{
		if (tree.folders.find(i.first) == tree.folders.end())
		{
			auto root = i.first == * folder_ ? this->GetRootItem() : FindRoot(this, *folder_, i.first);
			const auto absolute_path = i.first;
			const auto display_name = i.first.get_name();
			this->AppendItem(
				root,
				display_name,
				ICON_FOLDER_NORMAL,
				ICON_FOLDER_NORMAL,
				new FileEntry(true, absolute_path.full_path())
			);
		}
	}

	// add missing files
	const FilesAndFolders all_folders = ListTree(this, *folder_);
	for (auto i: current.files)
	{
		if (tree.files.find(i.first) == tree.files.end())
		{
			const auto parent_path = i.first.dir();
			auto parent = all_folders.folders.find(parent_path);
			assert(parent != all_folders.folders.end());
			const auto absolute_path = i.first;
			const auto display_name = i.first.get_name_with_extension();
			this->AppendItem(
				parent->second,
				display_name,
				ICON_FILE_NORMAL,
				ICON_FILE_NORMAL,
				new FileEntry(false, absolute_path.full_path())
			);
		}
	}

	// remove files
	const FilesAndFolders all_files = ListTree(this, *folder_);
	for (auto i: all_files.files)
	{
		if (current.files.find(i.first) == current.files.end())
		{
			if (i.second == last_highlighted_item_)
			{
				last_highlighted_item_.Unset();
			}
			this->Delete(i.second);
		}
	}

	// remove folders in reverse to remove child before parent
	for (auto i = all_files.folders.rbegin(); i != all_files.folders.rend(); ++i)
	{
		if (current.folders.find(i->first) == current.folders.end())
		{
			this->Delete(i->second);
		}
	}

	files_ = ListTree(this, *folder_);

	this->Thaw();
	if (tree.files.empty())
	{
		this->ExpandAll();
	}
}

std::vector<Fil> ProjectExplorer::GetFiles()
{
	std::vector<Fil> r;
	r.reserve(files_.files.size());
	for (auto i: files_.files)
	{
		r.push_back(i.first);
	}
	return r;
}

void OpenFile(TreeItemFileEntry tife, MainWindow* main)
{
	if (tife.second && false == tife.second->is_directory())
	{
		const wxFileName path{tife.second->path()};
		main->OpenFile(Fil{path});
	}
}

void ProjectExplorer::OnDoubleClick(wxMouseEvent& event)
{
	const auto focused = GetFocused(this);
	if (focused.first.IsOk() == false) return;
	this->Toggle(focused.first);
	OpenFile(focused, main_);
}

enum
{
	ID_FOLDER_COLLAPSE = wxID_HIGHEST,
	ID_FOLDER_EXPAND,

	ID_FOLDER_COLLAPSE_ALL_CHILDREN,
	ID_FOLDER_EXPAND_ALL_CHILDREN,

	ID_COLLAPSE_ALL,
	ID_EXPAND_ALL,

	ID_OPEN_FILE,
	ID_CREATE_NEW_FILE,
	ID_CREATE_NEW_FOLDER,
	ID_DELETE_FILE_OR_FOLDER,
	ID_RENAME,
	ID_OPEN_EXPLORER
};

void ProjectExplorer::OnContextMenu(wxContextMenuEvent& event)
{
	// const wxPoint mouse_point = GetContextEventPosition(event);
	// const wxPoint client_point = ScreenToClient(mouse_point);

	const auto selected = GetFocused(this);

	const bool is_folder = selected.second ? selected.second->is_directory() == true : false;
	const bool is_file = selected.second ? selected.second->is_directory() == false : false;

	wxMenu menu;
	AppendEnabled(menu, ID_OPEN_FILE, "Open file", is_file);
	AppendEnabled(menu, ID_RENAME, "Rename", is_file);
	menu.AppendSeparator();
	AppendEnabled(menu, ID_CREATE_NEW_FILE, "Create new file...", is_folder);
	AppendEnabled(menu, ID_CREATE_NEW_FOLDER, "Create new folder...", is_folder);
	AppendEnabled(menu, ID_OPEN_EXPLORER, "Open in explorer", is_folder);
	// open in shell
	menu.AppendSeparator();
	AppendEnabled(menu, ID_FOLDER_COLLAPSE, "Collapse", is_folder);
	AppendEnabled(menu, ID_FOLDER_COLLAPSE_ALL_CHILDREN, "Collapse children", is_folder);
	AppendEnabled(menu, ID_COLLAPSE_ALL, "Collapse all", true);
	menu.AppendSeparator();
	AppendEnabled(menu, ID_FOLDER_EXPAND, "Expand", is_folder);
	AppendEnabled(menu, ID_FOLDER_EXPAND_ALL_CHILDREN, "Expand children", is_folder);
	AppendEnabled(menu, ID_EXPAND_ALL, "Expand all", true);
	menu.AppendSeparator();
	AppendEnabled(menu, ID_DELETE_FILE_OR_FOLDER, "Delete", true);

	PopupMenu(&menu);
}

void ProjectExplorer::OnCreateNewFolder(wxCommandEvent& event)
{
	const auto path_of_selected = GetSelectedDir();
	if (!path_of_selected)
	{
		return;
	}

	wxTextEntryDialog dlg(this, "Please enter folder name:", "Folder name");
	if (dlg.ShowModal() != wxID_OK) return;

	const wxString folder_name = dlg.GetValue();
	const auto fn = path_of_selected->subdir(folder_name);
	if (fn.exist())
	{
		ShowWarning(this, "Entered folder exist", "Unable to create");
		return;
	}

	if (false == fn.create())
	{
		ShowError(this, "Unable to create folder", "Unable to create");
		return;
	}
	UpdateFolderStructure();
}

void ProjectExplorer::OnCreateNewFile(wxCommandEvent& event)
{
	CreateNewFile(folder_, main_, this);
}

void ProjectExplorer::OnFolderCollapse(wxCommandEvent& event)
{
	const auto selected = GetFocused(this);
	if (selected.first.IsOk() == false) return;
	this->Collapse(selected.first);
}

void ProjectExplorer::OnFolderExpand(wxCommandEvent& event)
{
	const auto selected = GetFocused(this);
	if (selected.first.IsOk() == false) return;
	this->Expand(selected.first);
}

void ProjectExplorer::OnFolderCollapseAllChildren(wxCommandEvent& event)
{
	const auto selected = GetFocused(this);
	if (selected.first.IsOk() == false) return;
	this->CollapseAllChildren(selected.first);
}

void ProjectExplorer::OnFolderExpandAllChildren(wxCommandEvent& event)
{
	const auto selected = GetFocused(this);
	if (selected.first.IsOk() == false) return;
	this->ExpandAllChildren(selected.first);
}

void ProjectExplorer::OnCollapseAll(wxCommandEvent& event)
{
	this->CollapseAll();
}

void ProjectExplorer::OnExpandAll(wxCommandEvent& event)
{
	this->ExpandAll();
}

void ProjectExplorer::OnOpenFile(wxCommandEvent& event)
{
	const auto selected = GetFocused(this);
	OpenFile(selected, main_);
}

void ProjectExplorer::OnDeleteFileOrFolder(wxCommandEvent& event)
{
	const auto selected = GetFocused(this);
	FileEntry* file = selected.second;
	if (file == nullptr) return;

	// if a dialog is shown, the file pointer is invalidated, since we rebuild the
	// whole tree structure and thus delete the entries
	// lets "fix" this for now by storing copies and setting our reference to null
	const wxString path = file->path();
	const bool is_directory = file->is_directory();
	file = nullptr;

	if (is_directory)
	{
		// we don't care about the value of these really, since the dialog should
		// set them
		// however, let's set them to a safe value to be sure
		bool full = false;
		bool recursive = false;
		if (false == ShowDeleteFolderDlg(this, path, &full, &recursive))
		{
			return;
		}
		int flags = 0;
		if (full)
		{
			flags |= wxPATH_RMDIR_FULL;
		}
		if (recursive)
		{
			flags |= wxPATH_RMDIR_RECURSIVE;
		}
		const bool removed = wxFileName::Rmdir(path, flags);
		if (false == removed)
		{
			// wxWidgets seems to display a dialog here for us...
		}
	}
	else
	{
		wxString mess = "Do you want to delete " + path;
		DialogResult ret = ShowYesNo(
			this, "Remove?", "Remove file", "Keep file", path, "Do you want to delete " + path
		);
		if (DialogResult::YES != ret)
		{
			return;
		}
		const bool file_removed = wxRemoveFile(path);
		if (false == file_removed)
		{
			ShowError(this, "Unable to remove file!", "Unable to remove");
		}
	}
	UpdateFolderStructure();
}

void ProjectExplorer::OnEditLabelStart(wxTreeEvent& event)
{
	if (event.IsEditCancelled()) return;
	auto data = GetTreeItemData(this, event.GetItem());
	FileEntry* file = data.second;
	if (file == nullptr)
	{
		event.Veto();
		return;
	}
	if (file->is_directory())
	{
		// directory edit isn't allowed
		event.Veto();
		return;
	}
}

void ProjectExplorer::OnEditLabelEnd(wxTreeEvent& event)
{
	if (event.IsEditCancelled()) return;
	auto data = GetTreeItemData(this, event.GetItem());
	FileEntry* file = data.second;
	if (file == nullptr)
	{
		event.Veto();
		return;
	}
	const wxString text = event.GetLabel();	 // this->GetItemText(data.first);

	const wxString old_path = file->path();
	if (file->is_directory())
	{
		wxFileName new_name(old_path);
		new_name.RemoveLastDir();
		const bool appended = new_name.AppendDir(text);
		if (appended == false)
		{
			ShowError(this, "Unable to change directory path", "Unable to rename");
			event.Veto();
			return;
		}
		const wxString new_path = new_name.GetFullPath();

		// todo: rename directory!
		event.Veto();
		return;
	}
	else
	{
		const wxFileName old_name{old_path};
		wxFileName new_name{old_path};
		new_name.SetFullName(text);
		const wxString new_path = new_name.GetFullPath();
		const bool renamed_file = wxRenameFile(old_path, new_path, false);
		if (renamed_file == false)
		{
			event.Veto();
			ShowError(this, "Unable to change file name", "Unable to rename");
			return;
		}
		main_->FileHasBeenRenamed(Fil{old_name}, Fil{new_name});
		file->set_path(new_path);
		return;
	}
}

void ProjectExplorer::OnRename(wxCommandEvent& event)
{
	const auto data = GetFocused(this);
	this->EditLabel(data.first);
}

void ProjectExplorer::OnOpenExplorer(wxCommandEvent& event)
{
	const auto data = GetFocused(this);
	if (data.second != nullptr)
	{
		wxLaunchDefaultApplication(data.second->path());
	}
}

void ProjectExplorer::BindEvents()
{
	Bind(wxEVT_LEFT_DCLICK, &ProjectExplorer::OnDoubleClick, this);
	Bind(wxEVT_CONTEXT_MENU, &ProjectExplorer::OnContextMenu, this);

	Bind(wxEVT_MENU, &ProjectExplorer::OnCreateNewFile, this, ID_CREATE_NEW_FILE);
	Bind(wxEVT_MENU, &ProjectExplorer::OnCreateNewFolder, this, ID_CREATE_NEW_FOLDER);
	Bind(wxEVT_MENU, &ProjectExplorer::OnFolderCollapse, this, ID_FOLDER_COLLAPSE);
	Bind(wxEVT_MENU, &ProjectExplorer::OnFolderExpand, this, ID_FOLDER_EXPAND);
	Bind(
		wxEVT_MENU,
		&ProjectExplorer::OnFolderCollapseAllChildren,
		this,
		ID_FOLDER_COLLAPSE_ALL_CHILDREN
	);
	Bind(
		wxEVT_MENU, &ProjectExplorer::OnFolderExpandAllChildren, this, ID_FOLDER_EXPAND_ALL_CHILDREN
	);
	Bind(wxEVT_MENU, &ProjectExplorer::OnCollapseAll, this, ID_COLLAPSE_ALL);
	Bind(wxEVT_MENU, &ProjectExplorer::OnExpandAll, this, ID_EXPAND_ALL);
	Bind(wxEVT_MENU, &ProjectExplorer::OnOpenFile, this, ID_OPEN_FILE);
	Bind(wxEVT_MENU, &ProjectExplorer::OnDeleteFileOrFolder, this, ID_DELETE_FILE_OR_FOLDER);
	Bind(wxEVT_MENU, &ProjectExplorer::OnRename, this, ID_RENAME);
	Bind(wxEVT_MENU, &ProjectExplorer::OnOpenExplorer, this, ID_OPEN_EXPLORER);

	Bind(wxEVT_TREE_BEGIN_LABEL_EDIT, &ProjectExplorer::OnEditLabelStart, this);
	Bind(wxEVT_TREE_END_LABEL_EDIT, &ProjectExplorer::OnEditLabelEnd, this);
}
