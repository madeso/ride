#ifndef RIDE_MAINWINDOW_H
#define RIDE_MAINWINDOW_H


#include <wx/aui/aui.h>

#include "ride/language.h"
#include "ride/settings.h"

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

  void OnUndo(wxCommandEvent& event);
  void OnRedo(wxCommandEvent& event);
  void OnCut(wxCommandEvent& event);
  void OnCopy(wxCommandEvent& event);
  void OnPaste(wxCommandEvent& event);
  void OnDuplicate(wxCommandEvent& event);
  void OnDelete(wxCommandEvent& event);
  void OnFind(wxCommandEvent& event);
  void OnReplace(wxCommandEvent& event);
  void OnMatchBrace(wxCommandEvent& event);
  void OnSelectBrace(wxCommandEvent& event);
  void OnGotoLine(wxCommandEvent& event);
  void OnIndent(wxCommandEvent& event);
  void OnUnIndent(wxCommandEvent& event);
  void OnSelectAll(wxCommandEvent& event);
  void OnSelectLine(wxCommandEvent& event);
  void OnToUpper(wxCommandEvent& event);
  void OnToLower(wxCommandEvent& event);
  void OnMoveLinesUp(wxCommandEvent& event);
  void OnMoveLinesDown(wxCommandEvent& event);

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
  void updateAllEdits();
  void createNotebook();
  FileEdit* getSelectedEditorNull();

  wxAuiManager aui;
  wxAuiNotebook* notebook;

  ride::Settings settings;
};

#endif  // RIDE_MAINWINDOW_H