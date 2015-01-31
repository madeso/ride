#ifndef RIDE_MAINWINDOW_H
#define RIDE_MAINWINDOW_H

#include <wx/wx.h>
#include <wx/aui/aui.h>

class FileEdit;

class MainWindow: public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
  ~MainWindow();

private:
	void OnOpen(wxCommandEvent& event);
  void OnSave(wxCommandEvent& event);
  void OnSaveAs(wxCommandEvent& event);

	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

  void OnNotebookPageClose(wxAuiNotebookEvent& evt);
  void OnNotebookPageClosed(wxAuiNotebookEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
  void createNotebook();
  FileEdit* getSelectedEditorNull();

  wxAuiManager aui;
  wxAuiNotebook* notebook;
};

#endif  // RIDE_MAINWINDOW_H