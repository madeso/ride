///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __UI_H__
#define __UI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/stc/stc.h>
#include <wx/statbox.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/valgen.h>
#include <wx/notebook.h>
#include <wx/valtext.h>
#include <wx/clrpicker.h>

///////////////////////////////////////////////////////////////////////////

namespace ui
{
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
			
			DeleteFolder( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Delete folder?"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
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
			wxTextCtrl* uiFilterName;
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
			wxPanel* m_featuresPanel;
			wxListBox* uiCargoFeatures;
			wxPanel* m_editorPanel;
			wxStaticText* m_staticText32;
			wxStaticText* m_staticText31;
			wxTextCtrl*  uiEditorTabWidth;
			wxCheckBox* uiEditorUseTabs;
			wxPanel*    m_buildPanel;
			wxStaticText* m_staticText33;
			wxComboBox* uiBuildConfiguration;
			wxButton* uiBuildManageConfigurations;
			wxCheckBox* uiBuildConfigurationRelease;
			wxCheckBox* uiBuildConfigurationDefaultFeatures;
			wxCheckBox*  uiBuildConfigurationVerbose;
			wxStaticText* m_staticText37;
			wxTextCtrl* uiBuildConfigurationTarget;
			wxButton* uiBuildConfigurationTargetHelp;
			wxStaticText* m_staticText34;
			wxTextCtrl* uiBuildConfigurationCustomArgs;
			wxButton* uiBuildConfigurationCustomArgsHelp;
			wxStaticText* m_staticText38;
			wxListBox* uiBuildFeatures;
			wxButton* uiBuildFeatureAdd;
			wxButton* uiBuildFeatureEdit;
			wxButton* uiBuildFeatureRemove;
			wxButton* uiBuildFeatureUp;
			wxButton* uiBuildFeatureDown;
			wxStaticText* m_staticText48;
			wxTextCtrl* uiBuildCommandLine;
			wxPanel* m_runPanel;
			wxStaticText* m_staticText49;
			wxComboBox* uiRunConfigurations;
			wxButton* uiRunManageConfigurations;
			wxStaticText* m_staticText50;
			wxTextCtrl* uiRunApplication;
			wxButton* uiRunApplicationCmd;
			wxStaticText* m_staticText51;
			wxTextCtrl* uiRunArguments;
			wxButton* uiRunArgumentsCmd;
			wxStaticText* m_staticText52;
			wxTextCtrl* uiRunFolder;
			wxButton* uiRunFolderCmd;
			wxStaticText* m_staticText53;
			wxTextCtrl* uiCmdBeforeLaunch;
			wxButton* uiCmdBeforeLaunchCmd;
			wxCheckBox* uiRunWaitForExit;
			wxStdDialogButtonSizer* m_sdbSizer8;
			wxButton* m_sdbSizer8OK;
			wxButton* m_sdbSizer8Apply;
			wxButton* m_sdbSizer8Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnlyAllowNumberChars( wxKeyEvent& event ) { event.Skip(); }
			virtual void OnTabWdithChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnEditorUseTabsClicked( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildConfiguration( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildConfigurationModify( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildCheckbox( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildText( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildTargetHelp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildCustomArgHelp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildFeatureAdd( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildFeatureEdit( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildFeatureRemove( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildFeatureUp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnBuildFeatureDown( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRunConfiguration( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRunConfigurationModify( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRunText( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRunApplication( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRunArguments( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRunFolder( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCmdBeforeLaunch( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRunCheck( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			ProjectSettings( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Project settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 666,535 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~ProjectSettings();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class Configurations
	///////////////////////////////////////////////////////////////////////////////
	class Configurations : public wxDialog 
	{
		private:
		
		protected:
			wxListBox* uiList;
			wxButton* uiListAdd;
			wxButton*  uiListChange;
			wxButton* uiListRemove;
			wxButton* uiListUp;
			wxButton* uiListDown;
			wxStdDialogButtonSizer* m_sdbSizer9;
			wxButton* m_sdbSizer9OK;
			wxButton* m_sdbSizer9Apply;
			wxButton* m_sdbSizer9Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnAdd( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnEdit( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRemove( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnUp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnDown( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			Configurations( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Configurations"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 463,349 ), long style = wxDEFAULT_DIALOG_STYLE ); 
			~Configurations();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class VariableEditor
	///////////////////////////////////////////////////////////////////////////////
	class VariableEditor : public wxDialog 
	{
		private:
		
		protected:
			wxTextCtrl* uiText;
			wxStaticText* m_staticText54;
			wxButton* m_button22;
			wxButton* m_button23;
			wxListBox* uiCustomVariables;
			wxStdDialogButtonSizer* m_sdbSizer10;
			wxButton* m_sdbSizer10OK;
			wxButton* m_sdbSizer10Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnTextChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFile( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnFolder( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCustomDoubleClick( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			VariableEditor( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 434,250 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
			~VariableEditor();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class SettingsFontsPanel
	///////////////////////////////////////////////////////////////////////////////
	class SettingsFontsPanel : public wxPanel 
	{
		private:
		
		protected:
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
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnSelectedStyleChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnStyleCheckChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOnlyFixedSysChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnStyleFontChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnlyAllowNumberChars( wxKeyEvent& event ) { event.Skip(); }
			virtual void OnStyleTextChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnStyleColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
			
		
		public:
			
			SettingsFontsPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 476,363 ), long style = wxTAB_TRAVERSAL ); 
			~SettingsFontsPanel();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class SettingsEnviromentPanel
	///////////////////////////////////////////////////////////////////////////////
	class SettingsEnviromentPanel : public wxPanel 
	{
		private:
		
		protected:
			wxStaticText* m_staticText103;
			wxTextCtrl* uiCargoPath;
			wxButton* m_button32;
			wxStaticText* uiCargoStatus;
			wxStaticText* m_staticText104;
			wxTextCtrl* uiRustcPath;
			wxButton* m_button33;
			wxStaticText* uiRustcStatus;
			wxStaticText* m_staticText105;
			wxTextCtrl* uiRacerPath;
			wxButton* m_button34;
			wxStaticText* uiRacerStatus;
			wxStaticText* m_staticText106;
			wxTextCtrl* uiProtocPath;
			wxButton* m_button35;
			wxStaticText* uiProtocStatus;
			wxButton* m_button36;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnCargo( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRustc( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRacer( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnProtoc( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRefresh( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			SettingsEnviromentPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
			~SettingsEnviromentPanel();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class SettingsIndicatorsPanel
	///////////////////////////////////////////////////////////////////////////////
	class SettingsIndicatorsPanel : public wxPanel 
	{
		private:
		
		protected:
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
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnIndicatorListChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnIndicatorCombobox( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnIndicatorColor( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnIndicatorCheckbox( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnlyAllowNumberChars( wxKeyEvent& event ) { event.Skip(); }
			virtual void OnIndicatorText( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			SettingsIndicatorsPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
			~SettingsIndicatorsPanel();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class SettingsMarkersPanel
	///////////////////////////////////////////////////////////////////////////////
	class SettingsMarkersPanel : public wxPanel 
	{
		private:
		
		protected:
			wxListBox* uiMarkerList;
			wxStaticText* m_staticText7;
			wxComboBox* uiMarkerSymbol;
			wxStaticText* m_staticText9;
			wxColourPickerCtrl* uiMarkerForegroundColor;
			wxStaticText* m_staticText8;
			wxColourPickerCtrl* uiMarkerBackgroundColor;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnMarkerListChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMarkerComboChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnMarkerColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnMarkerComboChanged( wxColourPickerEvent& event ) { event.Skip(); }
			
		
		public:
			
			SettingsMarkersPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
			~SettingsMarkersPanel();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class SettingsEditorPanel
	///////////////////////////////////////////////////////////////////////////////
	class SettingsEditorPanel : public wxPanel 
	{
		private:
		
		protected:
			wxNotebook* m_notebook2;
			wxPanel* m_panel28;
			wxCheckBox* uiEditShowCompilerAnnotations;
			wxCheckBox* uiEditShowMultilineIndicators;
			wxCheckBox* uiEditHighlightKeyword;
			wxCheckBox* uiShowLineNumbers;
			wxCheckBox* uiTabIndents;
			wxCheckBox* uiBackspaceUnindents;
			wxPanel* m_panel26;
			wxStaticText* m_staticText98;
			wxComboBox* uiEditAutocompleteCurlyBraces;
			wxStaticText* m_staticText99;
			wxComboBox* uiEditAutocompleteParentheses ;
			wxStaticText* m_staticText100;
			wxComboBox* uiEditAutocompleteBrackets;
			wxStaticText* m_staticText101;
			wxComboBox* uiEditAutocompleteSingleQuote;
			wxStaticText* m_staticText102;
			wxComboBox* uiEditAutocompleteDoubleQuote;
			wxCheckBox* uiIndentGuide;
			wxStaticText* m_staticText44;
			wxComboBox* uiEditIndentation;
			wxPanel* m_panel23;
			wxCheckBox* uiUseTabs;
			wxStaticText* m_staticText41;
			wxTextCtrl* uiTabWidth;
			wxPanel* m_panel10;
			wxCheckBox* uiAllowFolding;
			wxCheckBox* uiFoldLevelNumbers;
			wxCheckBox* uiFoldLineBeforeExpanded;
			wxCheckBox* uiFoldLineBeforeContracted;
			wxCheckBox* uiFoldLineAfterExpanded;
			wxCheckBox* uiFoldLineAfterContracted;
			wxStaticText* m_staticText42;
			wxColourPickerCtrl* uiEditFoldHi;
			wxStaticText* m_staticText43;
			wxColourPickerCtrl* uiEditFoldLow;
			wxPanel* m_panel11;
			wxCheckBox* uiCurrentLineVisible;
			wxCheckBox* uiEditCurrentLineOverdraw;
			wxStaticText* m_staticText39;
			wxColourPickerCtrl* uiEditCurrentLineColor;
			wxStaticText* m_staticText40;
			wxTextCtrl* uiEditCurrentLineAlpha;
			wxCheckBox* uiEditUseSelectionForeground;
			wxColourPickerCtrl* uiEditSelectionForeground;
			wxCheckBox* uiEditUseSelectionBackground;
			wxColourPickerCtrl* uiEditSelectionBackground;
			wxStaticText* m_staticText47;
			wxComboBox* uiEdgeStyle;
			wxStaticText* m_staticText48;
			wxColourPickerCtrl* uiEdgeColor;
			wxStaticText* m_staticText49;
			wxTextCtrl* uiEdgeColumn;
			wxPanel* m_panel27;
			wxCheckBox* uiDisplayEOL;
			wxStaticText* m_staticText45;
			wxComboBox* uiViewWhitespace;
			wxStaticText* m_staticText46;
			wxComboBox* uiWordwrap;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnCheckboxChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnComboboxChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnlyAllowNumberChars( wxKeyEvent& event ) { event.Skip(); }
			virtual void OnEditChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
			
		
		public:
			
			SettingsEditorPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 460,411 ), long style = wxTAB_TRAVERSAL ); 
			~SettingsEditorPanel();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class SettingsWindowPanel
	///////////////////////////////////////////////////////////////////////////////
	class SettingsWindowPanel : public wxPanel 
	{
		private:
		
		protected:
			wxNotebook* m_notebook4;
			wxPanel* m_panel17;
			wxStaticText* m_staticText55;
			wxColourPickerCtrl* uiWindowDockCommonBackground;
			wxStaticText* m_staticText56;
			wxColourPickerCtrl* uiWindowDockCommonSash;
			wxStaticText* m_staticText57;
			wxColourPickerCtrl* uiWindowDockCommonBorder;
			wxStaticText* m_staticText58;
			wxColourPickerCtrl* uiWindowDockCommonGripper;
			wxStaticText* m_staticText59;
			wxColourPickerCtrl* uiWindowDockActiveCaption;
			wxStaticText* m_staticText60;
			wxColourPickerCtrl* uiWindowDockActiveGradient;
			wxStaticText* m_staticText61;
			wxColourPickerCtrl* uiWindowDockActiveText;
			wxStaticText* m_staticText62;
			wxColourPickerCtrl* uiWindowDockInactiveCaption;
			wxStaticText* m_staticText63;
			wxColourPickerCtrl* uiWindowDockInactiveGradient;
			wxStaticText* m_staticText64;
			wxColourPickerCtrl* uiWindowDockInactiveText;
			wxPanel* m_panel18;
			wxStaticText* m_staticText65;
			wxColourPickerCtrl* uiWindowTabCommonBackground;
			wxStaticText* m_staticText77;
			wxColourPickerCtrl* uiWindowTabCommonBorder;
			wxStaticText* m_staticText78;
			wxColourPickerCtrl* uiWindowTabCommonSash;
			wxStaticText* m_staticText66;
			wxColourPickerCtrl* uiWindowTabActiveTab;
			wxStaticText* m_staticText67;
			wxColourPickerCtrl* uiWindowTabActiveBorder;
			wxStaticText* m_staticText68;
			wxColourPickerCtrl* uiWindowTabActiveText;
			wxStaticText* m_staticText69;
			wxColourPickerCtrl* uiWindowTabInactiveTab;
			wxStaticText* m_staticText70;
			wxColourPickerCtrl* uiWindowTabInactiveBorder;
			wxStaticText* m_staticText71;
			wxColourPickerCtrl* uiWindowTabInactiveText;
			wxPanel* m_panel19;
			wxStaticText* m_staticText72;
			wxComboBox* uiWindowStatusbarStyle;
			wxStaticText* m_staticText73;
			wxColourPickerCtrl* uiWindowStatusbarForeground;
			wxStaticText* m_staticText74;
			wxColourPickerCtrl* uiWindowStatusbarBackground;
			wxStaticText* m_staticText75;
			wxColourPickerCtrl* uiWindowStatusbarShadow;
			wxStaticText* m_staticText76;
			wxColourPickerCtrl* uiWindowStatusbarHighlight;
			wxPanel* m_panel20;
			wxStaticText* m_staticText94;
			wxColourPickerCtrl* uiSwitcherItemBackground;
			wxStaticText* m_staticText95;
			wxColourPickerCtrl* uiSwitcherItemText;
			wxStaticText* m_staticText82;
			wxColourPickerCtrl* uiSwitcherSelection;
			wxStaticText* m_staticText83;
			wxColourPickerCtrl* uiSwitcherSelectionOutline;
			wxStaticText* m_staticText84;
			wxColourPickerCtrl* uiSwitcherDialog;
			wxStaticText* m_staticText85;
			wxColourPickerCtrl* uiSwitcherBackground;
			wxStaticText* m_staticText80;
			wxTextCtrl* uiSwitcherTextMarginX;
			wxStaticText* m_staticText79;
			wxTextCtrl* uiSwitcherTextMarginY;
			wxStaticText* m_staticText81;
			wxTextCtrl* uiSwitcherXMargin;
			wxStaticText* m_staticText86;
			wxTextCtrl* uiSwitcherYMargin;
			wxStaticText* m_staticText87;
			wxTextCtrl* uiSwitcherRowSpacing;
			wxStaticText* m_staticText88;
			wxTextCtrl* uiSwitcherColSpacing;
			wxStaticText* m_staticText89;
			wxTextCtrl* uiSwitcherMainBorder;
			wxStaticText* m_staticText90;
			wxTextCtrl* uiSwitcherItemBorder;
			wxCheckBox* uiSwitcherVsFocus;
			wxStaticText* m_staticText91;
			wxTextCtrl* uiSwitcherItemsPerRow;
			wxStaticText* m_staticText92;
			wxTextCtrl* uiSwitcherItemMaxWidth;
			wxStaticText* m_staticText93;
			wxTextCtrl* uiSwitcherItemMaxHeight;
			wxStaticText* m_staticText96;
			wxTextCtrl* uiSwitcherMinWidth;
			wxStaticText* m_staticText97;
			wxTextCtrl* uiSwitcherMinHeight;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnWindowColorChange( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnWindowComboChange( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnlyAllowNumberChars( wxKeyEvent& event ) { event.Skip(); }
			virtual void OnWindowText( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnWindowCheck( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			SettingsWindowPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 486,554 ), long style = wxTAB_TRAVERSAL ); 
			~SettingsWindowPanel();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class SettingsThemesPanel
	///////////////////////////////////////////////////////////////////////////////
	class SettingsThemesPanel : public wxPanel 
	{
		private:
		
		protected:
			wxButton* m_button40;
			wxButton* m_button42;
			wxButton* m_button41;
			wxListBox* uiThemeList;
			wxButton* uiThemeListAdd;
			wxButton*  uiThemeListChange;
			wxButton* uiThemeListRemove;
			wxButton* uiThemeListUp;
			wxButton* uiThemeListDown;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnThemeApplySelected( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnThemeImport( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnThemeExportSelected( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnAdd( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnEdit( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnRemove( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnUp( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnDown( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			SettingsThemesPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
			~SettingsThemesPanel();
		
	};
	
} // namespace ui

#endif //__UI_H__
