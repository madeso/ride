#ifndef RIDE_MAINWINDOW_H
#define RIDE_MAINWINDOW_H

#include <wx/wx.h>
#include <wx/aui/aui.h>

#include "ride/language.h"
#include "ride/settings.h"
#include "ride/style.h"

class FileEdit;

class MainWindow: public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
  ~MainWindow();

  const Settings& getSettings() const;
  const std::vector<LanguageInfo>& getLanguages() const;
  const std::vector<StyleInfo>& getStyles() const;

private:
	void OnOpen(wxCommandEvent& event);
  void OnSave(wxCommandEvent& event);
  void OnSaveAs(wxCommandEvent& event);

	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

  void OnClose(wxCloseEvent& event);

  void OnNotebookPageClose(wxAuiNotebookEvent& evt);
  void OnNotebookPageClosed(wxAuiNotebookEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
  void createNotebook();
  FileEdit* getSelectedEditorNull();

  wxAuiManager aui;
  wxAuiNotebook* notebook;

  Settings settings;
  std::vector<LanguageInfo> languages;
  std::vector<StyleInfo> styles;
};

#endif  // RIDE_MAINWINDOW_H