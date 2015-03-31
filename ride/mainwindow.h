#ifndef RIDE_MAINWINDOW_H
#define RIDE_MAINWINDOW_H


#include <wx/aui/aui.h>

#include "ride/language.h"
#include "ride/settings.h"
#include "ride/project.h"

class FileEdit;
class SettingsDlg;
class CompilerMessage;
class OutputControl;
class FindControl;
class ProjectExplorer;

struct FoundEdit {
  FoundEdit(size_t i, FileEdit* e) : index(i), edit(e) { }

  // has a edit been found?
  operator bool() const { return edit != NULL; }

  static FoundEdit NOT_FOUND;

  size_t index;
  FileEdit* edit;
};

void CreateNewFile(const wxString& project_root, MainWindow* main, ProjectExplorer* project_explorer);

class MainWindow: public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
  ~MainWindow();

  const ride::Settings& settings() const;
  void set_settings(const ride::Settings& settings);

  void Clear();
  void Append(const wxString& str);

  void OpenCompilerMessage(const CompilerMessage& message);
  void FileHasBeenRenamed(const wxString& old_path, const wxString& new_path);

  void SaveAllChangedProjectFiles();
  void ReloadFilesIfNeeded();

  FileEdit* OpenFile(const wxString& file, int start_line = -1, int start_index = -1, int end_line = -1, int end_index = -1);
  FileEdit* GetFile(const wxString& file);

private:
  void OnActivated(wxActivateEvent& event);

	void OnFileOpen(wxCommandEvent& event);
  void OnFileSave(wxCommandEvent& event);
  void OnFileSaveAs(wxCommandEvent& event);

  void OnEditUndo(wxCommandEvent& event);
  void OnEditRedo(wxCommandEvent& event);
  void OnEditCut(wxCommandEvent& event);
  void OnEditCopy(wxCommandEvent& event);
  void OnEditPaste(wxCommandEvent& event);
  void OnEditDuplicate(wxCommandEvent& event);
  void OnEditDelete(wxCommandEvent& event);
  void OnEditFind(wxCommandEvent& event);
  void OnEditReplace(wxCommandEvent& event);
  void OnEditMatchBrace(wxCommandEvent& event);
  void OnEditSelectBrace(wxCommandEvent& event);
  void OnEditGotoLine(wxCommandEvent& event);
  void OnEditIndent(wxCommandEvent& event);
  void OnEditUnIndent(wxCommandEvent& event);
  void OnEditSelectAll(wxCommandEvent& event);
  void OnEditSelectLine(wxCommandEvent& event);
  void OnEditToUpper(wxCommandEvent& event);
  void OnEditToLower(wxCommandEvent& event);
  void OnEditMoveLinesUp(wxCommandEvent& event);
  void OnEditMoveLinesDown(wxCommandEvent& event);
  void OnEditOpenInOnlineDocumentation(wxCommandEvent& event);
  void OnEditShowProperties(wxCommandEvent& event);

  void OnProjectNew      (wxCommandEvent& event);
  void OnProjectOpen     (wxCommandEvent& event);
  void OnProjectSettings (wxCommandEvent& event);
  void OnProjectBuild    (wxCommandEvent& event);
  void OnProjectClean    (wxCommandEvent& event);
  void OnProjectRebuild  (wxCommandEvent& event);
  void OnProjectDoc      (wxCommandEvent& event);
  void OnProjectRun      (wxCommandEvent& event);
  void OnProjectTest     (wxCommandEvent& event);
  void OnProjectBench    (wxCommandEvent& event);
  void OnProjectUpdate   (wxCommandEvent& event);
  void OnProjectFileNew  (wxCommandEvent& event);
  void OnProjectQuickOpen(wxCommandEvent& event);
  void OnProjectFindInFiles(wxCommandEvent& event);
  void OnProjectReplaceInFiles(wxCommandEvent& event);


  void OnViewRestoreWindows(wxCommandEvent& event);
  void OnViewSaveLayout(wxCommandEvent& event);
  void OnViewLoadLayout(wxCommandEvent& event);
  void OnViewShowFindResult(wxCommandEvent& event);
  void OnViewShowOutput(wxCommandEvent& event);
  void OnViewShowProject(wxCommandEvent& event);

	void OnFileExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
  void OnFileShowSettings(wxCommandEvent& event);

  void OnClose(wxCloseEvent& event);

  void OnNotebookPageClose(wxAuiNotebookEvent& evt);
  void OnNotebookPageChanged(wxAuiNotebookEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
  void SaveSession();
  void RestoreSession();
  FoundEdit GetEditFromFileName(const wxString& file);
  void AddCompilerMessage(const CompilerMessage& mess);
  bool OpenProject(const wxString project_file);

  void UpdateTitle();
  void UpdateAllEdits();
  void CreateNotebook();
  FileEdit* GetSelectedEditorNull();

  wxAuiManager aui_;
  wxAuiNotebook* notebook_;
  OutputControl* output_window_;
  FindControl* findres_window_;
  ProjectExplorer* project_explorer_;

  ride::Settings settings_;
  Project project_;
  wxString app_name_;
  std::vector<CompilerMessage> compiler_messages_;

  wxString windows_locations_;

  wxMenuItem* menuItemViewFind_;
  wxMenuItem* menuItemViewProject_;
  wxMenuItem* menuItemViewOutput_;

  void UpdateMenuItemView();
};

#endif  // RIDE_MAINWINDOW_H