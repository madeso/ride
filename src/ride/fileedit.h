#ifndef RIDE_FILEEDIT_H_
#define RIDE_FILEEDIT_H_

#include <optional>

#include <ride/wx.h>
#include <wx/stc/stc.h>
#include "ride/tab.h"
#include "ride/path.h"


class wxAuiNotebook;
class MainWindow;
class CompilerMessage;

class Project;
class OutputControl;

namespace ride
{
struct Settings;
struct Language;
struct Languages;
}

class FileEdit : public wxControl
{
public:

	FileEdit(
		wxAuiNotebook* anotebook, MainWindow* parent, const Fil& file, ride::Languages* languages
	);
	const std::optional<Fil>& filename() const;
	void SetSelection(int start_line, int start_index, int end_line, int end_index);
	void GetSelection(int* start_line, int* start_index, int* end_line, int* end_index);
	void AddCompilerMessage(const CompilerMessage& mess);
	void ClearCompilerMessages();
	wxString GetLanguageName();

	bool CanClose(bool canAbort);
	void UpdateTextControl();
	void ReloadFileIfNeeded();
	void FileHasBeenRenamed(const Fil& new_path);

	void Find(OutputControl* output, const std::optional<Dir>& project);
	void Replace(OutputControl* output, const std::optional<Dir>& project);
	void FindInFiles(OutputControl* output, const std::optional<Dir>& project);
	void ReplaceInFiles(OutputControl* output, const std::optional<Dir>& project);

	void SetFocus();
	void SetFocusFromKbd();
	bool AcceptsFocusRecursively() const;
	bool AcceptsFocus() const;
	bool AcceptsFocusFromKeyboard() const;

	wxStyledTextCtrl* GetStc();

	void UpdateStatusText();

public:

	bool Save();
	bool SaveAs();
	void Undo();
	void Redo();
	void Cut();
	void Copy();
	void Paste();
	void Duplicate();
	void Delete();
	void MatchBrace();
	void SelectBrace();
	void GotoLine();
	void Indent();
	void UnIndent();
	void SelectAll();
	void SelectLine();
	void ToUpper();
	void ToLower();
	void MoveLinesUp();
	void MoveLinesDown();
	void OpenInOnlineDocumentation();
	void ShowProperties();

	void ShowAutocomplete();

public:

	/** Event callback when a margin is clicked, used here for code folding */
	void OnMarginClick(wxStyledTextEvent& event);
	void OnCharAdded(wxStyledTextEvent& event);
	void OnUpdateUi(wxStyledTextEvent& event);
	void OnChanged(wxStyledTextEvent& event);
	void OnSelectionUpdated(wxCommandEvent& event);

	bool ProcessCharEvent(wxChar c);
	bool ProcessKey(wxKeyCode key, wxKeyModifier mod);

	~FileEdit();

private:

	void UpdateFilename();
	void UpdateTitle();
	bool SaveTo(const Fil& target);

	bool ShouldBeSaved();
	void HighlightCurrentWord();
	void UpdateBraceMatching();

	wxString CalculateDocumentName() const;
	void LoadFile();
	void UpdateFileTime();

private:

	Tab tab_;
	MainWindow* main_;
	wxStyledTextCtrl* text_;
	wxAuiNotebook* notebook_;
	std::optional<Fil> filename_; // if unset, this is a new file that isn't saved
	ride::Languages* languages_;

	const ride::Language* current_language_;

	int highlight_current_word_last_start_position_;
	int highlight_current_word_last_end_position_;
	wxDateTime last_modification_time_;

	bool inside_compiler_message_update = false;

private:

	void BindEvents();
};

#endif	// RIDE_FILEEDIT_H_
