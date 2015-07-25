///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __UI_H__
#define __UI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/clrpicker.h>
#include <wx/gbsizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/valgen.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include "ride/textctrllist.h"
#include <wx/stc/stc.h>

///////////////////////////////////////////////////////////////////////////

namespace ui
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class Settings
	///////////////////////////////////////////////////////////////////////////////
	class Settings : public wxDialog 
	{
		private:
		
		protected:
			wxNotebook* m_notebook1;
			wxPanel* m_fonts;
			wxPanel* m_panel9;
			wxListBox* uiFontStyles;
			wxPanel* m_panel7;
			wxCheckBox* uiStyleUseTypeface;
			wxCheckBox* uiStyleOnlyFixedSize;
			wxListBox* uiStyleTypeface;
			wxCheckBox* uiStyleBold;
			wxCheckBox* uiStyleUseBold;
			wxCheckBox* uiStyleItalic;
			wxCheckBox* uiStyleUseItalic;
			wxCheckBox* uiStyleUnderline;
			wxCheckBox* uiStyleUseUnderline;
			wxTextCtrl* uiStyleSize;
			wxCheckBox* uiStyleUseSize;
			wxColourPickerCtrl* uiStyleForeground;
			wxCheckBox* uiStyleUseForeground;
			wxColourPickerCtrl* uiStyleBackground;
			wxCheckBox* uiStyleUseBackground;
			wxTextCtrl* uiStyleExample;
			wxPanel* m_indicators;
			wxListBox* uiIndicatorList;
			wxStaticText* uiIndicatorStyleLabel;
			wxComboBox* uiIndicatorStyle;
			wxStaticText* m_staticText11;
			wxColourPickerCtrl* uiIndicatorColor;
			wxCheckBox* uiIndicatorUnder;
			wxStaticText* m_staticText12;
			wxTextCtrl* uiIndicatorAlpha;
			wxStaticText* m_staticText13;
			wxTextCtrl* uiIndicatorOutlineAlpha;
			wxPanel* m_markers;
			wxListBox* uiMarkerList;
			wxStaticText* m_staticText7;
			wxComboBox* uiMarkerSymbol;
			wxStaticText* m_staticText9;
			wxColourPickerCtrl* uiMarkerForegroundColor;
			wxStaticText* m_staticText8;
			wxColourPickerCtrl* uiMarkerBackgroundColor;
			wxPanel* m_editor;
			wxNotebook* m_notebook2;
			wxPanel* m_panel10;
			wxCheckBox* uiShowLineNumbers;
			wxCheckBox* uiDisplayEOL;
			wxCheckBox* uiIndentGuide;
			wxComboBox* uiViewWhitespace;
			wxComboBox* uiWordwrap;
			wxComboBox* uiEdgeStyle;
			wxColourPickerCtrl* uiEdgeColor;
			wxTextCtrl* uiEdgeColumn;
			wxTextCtrl* uiTabWidth;
			wxCheckBox* uiUseTabs;
			wxCheckBox* uiTabIndents;
			wxCheckBox* uiBackspaceUnindents;
			wxCheckBox* uiAllowFolding;
			wxCheckBox* uiFoldLevelNumbers;
			wxCheckBox* uiFoldLineBeforeExpanded;
			wxCheckBox* uiFoldLineBeforeContracted;
			wxCheckBox* uiFoldLineAfterExpanded;
			wxCheckBox* uiFoldLineAfterContracted;
			wxCheckBox* uiEditCurrentLineOverdraw;
			wxColourPickerCtrl* uiEditCurrentLineColor;
			wxCheckBox* uiCurrentLineVisible;
			wxTextCtrl* uiEditCurrentLineAlpha;
			wxColourPickerCtrl* uiEditFoldHi;
			wxColourPickerCtrl* uiEditFoldLow;
			wxCheckBox* uiEditHighlightKeyword;
			wxComboBox* uiEditIndentation;
			wxCheckBox* uiEditAutocompleteCurlyBraces;
			wxCheckBox* uiEditAutocompleteParentheses;
			wxCheckBox* uiEditAutocompleteBrackets;
			wxCheckBox* uiEditShowMultilineIndicators;
			wxCheckBox* uiEditShowCompilerAnnotations;
			wxCheckBox* uiEditUseSelectionForeground;
			wxColourPickerCtrl* uiEditSelectionForeground;
			wxCheckBox* uiEditUseSelectionBackground;
			wxColourPickerCtrl* uiEditSelectionBackground;
			wxPanel* m_panel11;
			wxPanel* m_window;
			wxStdDialogButtonSizer* m_sdbSizer1;
			wxButton* m_sdbSizer1OK;
			wxButton* m_sdbSizer1Apply;
			wxButton* m_sdbSizer1Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnSelectedStyleChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnStyleCheckChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOnlyFixedSysChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnStyleFontChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnlyAllowNumberChars( wxKeyEvent& event ) { event.Skip(); }
			virtual void OnStyleTextChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnStyleColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnIndicatorListChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnIndicatorCombobox( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnIndicatorColor( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnIndicatorCheckbox( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnIndicatorText( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMarkerListChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMarkerComboChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMarkerColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnMarkerComboChanged( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnCheckboxChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnComboboxChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnEditChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			Settings( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 648,789 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~Settings();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class FileProperties
	///////////////////////////////////////////////////////////////////////////////
	class FileProperties : public wxDialog 
	{
		private:
		
		protected:
			wxStaticText* m_staticText1;
			wxStaticText* uiFileName;
			wxStaticText* m_staticText3;
			wxStaticText* uiLanguage;
			wxButton* m_button1;
			wxStaticText* uiLineEndings;
			wxButton* m_button2;
			wxStaticText* uiEncoding;
			wxStdDialogButtonSizer* m_sdbSizer3;
			wxButton* m_sdbSizer3OK;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnChangeLineEnding( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnChangeEncoding( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			FileProperties( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("File properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 455,197 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~FileProperties();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class CreateNewProject
	///////////////////////////////////////////////////////////////////////////////
	class CreateNewProject : public wxDialog 
	{
		private:
		
		protected:
			wxListCtrl* uiTemplates;
			wxStaticText* lblProjectName;
			wxTextCtrl* uiProjectName;
			wxStaticText* lblProjectFolder;
			wxTextCtrl* uiProjectfolder;
			wxButton* uiBrowseProjectFolder;
			wxComboBox* uiVcs;
			wxCheckBox* uiTravis;
			wxStaticText* lblTargetFolder;
			wxStaticText* uiTargetFolder;
			wxStdDialogButtonSizer* uiButtons;
			wxButton* uiButtonsOK;
			wxButton* uiButtonsCancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnProjectNameChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnProjectNameEnter( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnProjectFolderChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBrowseProjectFolder( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			CreateNewProject( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Create new project"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 540,349 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~CreateNewProject();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class CreateNewFile
	///////////////////////////////////////////////////////////////////////////////
	class CreateNewFile : public wxDialog 
	{
		private:
		
		protected:
			wxListCtrl* uiTemplates;
			wxStaticText* m_staticText18;
			wxTextCtrl* uiName;
			wxStaticText* m_staticText19;
			wxTextCtrl* uiPath;
			wxComboBox* uiReplaceAction;
			wxCheckBox* uiLowerCase;
			wxStaticText* m_staticText21;
			wxStaticText* uiSuggestedFilePath;
			wxStdDialogButtonSizer* m_sdbSizer4;
			wxButton* m_sdbSizer4OK;
			wxButton* m_sdbSizer4Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnTextChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnNameEnter( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnComboChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCheckChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			CreateNewFile( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Create new file..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 548,351 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~CreateNewFile();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class DeleteFolder
	///////////////////////////////////////////////////////////////////////////////
	class DeleteFolder : public wxDialog 
	{
		private:
		
		protected:
			wxStaticText* uiRemoveFileDescription;
			wxCheckBox* uiFull;
			wxCheckBox* uiRecursive;
			wxStdDialogButtonSizer* m_sdbSizer5;
			wxButton* m_sdbSizer5Yes;
			wxButton* m_sdbSizer5No;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnNo( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnYes( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			DeleteFolder( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
			~DeleteFolder();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class QuickOpen
	///////////////////////////////////////////////////////////////////////////////
	class QuickOpen : public wxDialog 
	{
		private:
		
		protected:
			wxListCtrl* uiFileList;
			TextCtrlList* uiFilterName;
			wxCheckBox* uiCaseSensitive;
			wxStdDialogButtonSizer* m_sdbSizer6;
			wxButton* m_sdbSizer6OK;
			wxButton* m_sdbSizer6Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnActivate( wxActivateEvent& event ) { event.Skip(); }
			virtual void OnContextSensitive( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			QuickOpen( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Open file in project..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 616,394 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~QuickOpen();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class Find
	///////////////////////////////////////////////////////////////////////////////
	class Find : public wxDialog 
	{
		private:
		
		protected:
			wxStaticText* m_staticText23;
			wxTextCtrl* uiFindText;
			wxStaticText* uiReplaceStatic;
			wxTextCtrl* uiReplaceText;
			wxStaticText* m_staticText24;
			wxComboBox* uiLookIn;
			wxCheckBox* uiIncludeSubFolders;
			wxCheckBox* uiKeepFilesOpen;
			wxCheckBox* uiMatchCase;
			wxCheckBox* uiMatchWholeWord;
			wxCheckBox* uiFindWordStart;
			wxComboBox* uiFindTarget;
			wxStaticText* m_staticText25;
			wxTextCtrl* uiFileTypes;
			wxStyledTextCtrl* m_scintilla1;
			wxStdDialogButtonSizer* m_sdbSizer7;
			wxButton* m_sdbSizer7OK;
			wxButton* m_sdbSizer7Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnEnter( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			Find( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Find..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 326,492 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~Find();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class ProjectSettings
	///////////////////////////////////////////////////////////////////////////////
	class ProjectSettings : public wxDialog 
	{
		private:
		
		protected:
			wxNotebook* m_notebook3;
			wxPanel* m_cargoPanel;
			wxStaticText* m_staticText27;
			wxTextCtrl* uiCargoName;
			wxStaticText* m_staticText28;
			wxTextCtrl* uiCargoVersion;
			wxStaticText* m_staticText29;
			wxListBox* uiCargoAuthors;
			wxStaticText* uiCargoLoadError;
			wxPanel* m_dependenciesPanel;
			wxListBox* uiCargoDependencies;
			wxPanel* m_editorPanel;
			wxPanel* m_buildPanel;
			wxPanel* m_runPanel;
			wxStdDialogButtonSizer* m_sdbSizer8;
			wxButton* m_sdbSizer8OK;
			wxButton* m_sdbSizer8Apply;
			wxButton* m_sdbSizer8Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			ProjectSettings( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 666,535 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~ProjectSettings();
		
	};
	
} // namespace ui

#endif //__UI_H__
