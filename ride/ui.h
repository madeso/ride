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
#include <wx/combobox.h>
#include <wx/clrpicker.h>
#include <wx/stattext.h>
#include <wx/gbsizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/valgen.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dialog.h>

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
			wxCheckBox* uiOnlyFixedSize;
			wxListBox* uiStyleTypeface;
			wxCheckBox* uiStyleBold;
			wxCheckBox* uiStyleItalic;
			wxCheckBox* uiStyleUnderline;
			wxComboBox* uiStyleSize;
			wxColourPickerCtrl* uiStyleForeground;
			wxColourPickerCtrl* uiStyleBackground;
			wxStaticText* m_staticText1;
			wxStaticText* m_staticText2;
			wxStaticText* m_staticText3;
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
			wxPanel* m_panel11;
			wxPanel* m_window;
			wxStdDialogButtonSizer* m_sdbSizer1;
			wxButton* m_sdbSizer1OK;
			wxButton* m_sdbSizer1Apply;
			wxButton* m_sdbSizer1Cancel;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnOnlyFixedSysChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCheckboxChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnComboboxChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
			virtual void OnEditChanged( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
			virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			Settings( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
			~Settings();
		
	};
	
} // namespace ui

#endif //__UI_H__
