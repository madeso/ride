#ifndef RIDE_MAINWINDOW_H
#define RIDE_MAINWINDOW_H


#include <wx/aui/aui.h>

#include "ride/language.h"
#include "ride/settings.h"
#include "ride/project.h"

class FileEdit;
class SettingsDlg;
class CompilerMessage;

struct FoundEdit {
  FoundEdit(size_t i, FileEdit* e) : index(i), edit(e) { }

  // has a edit been found?
  operator bool() const { return edit != NULL; }

  static FoundEdit NOT_FOUND;

  size_t index;
  FileEdit* edit;
};

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

private:
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
  void OnEditShowProperties(wxCommandEvent& event);

  void OnProjectNew     (wxCommandEvent& event);
  void OnProjectOpen    (wxCommandEvent& event);
  void OnProjectSettings(wxCommandEvent& event);
  void OnProjectBuild   (wxCommandEvent& event);
  void OnProjectClean   (wxCommandEvent& event);
  void OnProjectRebuild (wxCommandEvent& event);
  void OnProjectDoc     (wxCommandEvent& event);
  void OnProjectRun     (wxCommandEvent& event);
  void OnProjectTest    (wxCommandEvent& event);
  void OnProjectBench   (wxCommandEvent& event);
  void OnProjectUpdate  (wxCommandEvent& event);

	void OnFileExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
  void OnFileShowSettings(wxCommandEvent& event);

  void OnClose(wxCloseEvent& event);

  void OnNotebookPageClose(wxAuiNotebookEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
  void OpenFile(const wxString& file, int start_line, int start_index, int end_line, int end_index);
  FoundEdit GetEditFromFileName(const wxString& file);
  void AddCompilerMessage(const CompilerMessage& mess);

  void UpdateTitle();
  void UpdateAllEdits();
  void CreateNotebook();
  FileEdit* GetSelectedEditorNull();

  wxAuiManager aui_;
  wxAuiNotebook* notebook_;
  wxTextCtrl* output_window_;

  ride::Settings settings_;
  Project project_;
  wxString app_name_;
  std::vector<CompilerMessage> compiler_messages_;
};

#endif  // RIDE_MAINWINDOW_H