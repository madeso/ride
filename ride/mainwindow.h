#ifndef RIDE_MAINWINDOW_H
#define RIDE_MAINWINDOW_H


#include <wx/aui/aui.h>

#include "ride/language.h"
#include "ride/settings.h"
#include "ride/project.h"

class FileEdit;
class SettingsDlg;

class MainWindow: public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
  ~MainWindow();

  const ride::Settings& getSettings() const;
  void setSettings(const ride::Settings& settings);
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
  void OnNotebookPageClosed(wxAuiNotebookEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
  void updateTitle();
  void updateAllEdits();
  void createNotebook();
  FileEdit* getSelectedEditorNull();

  wxAuiManager aui;
  wxAuiNotebook* notebook;
  wxTextCtrl* output_window;

  ride::Settings settings;
  Project project;
  wxString title_;
};

#endif  // RIDE_MAINWINDOW_H