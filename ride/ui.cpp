///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ride/wx.h"
#include "ui.h"

///////////////////////////////////////////////////////////////////////////
using namespace ui;

Settings::Settings( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_fonts = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->AddGrowableCol( 0 );
	fgSizer6->AddGrowableCol( 1 );
	fgSizer6->AddGrowableRow( 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_panel9 = new wxPanel( m_fonts, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel9, wxID_ANY, wxT("Styles") ), wxVERTICAL );
	
	uiFontStyles = new wxListBox( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sbSizer3->Add( uiFontStyles, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer5->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	
	m_panel9->SetSizer( bSizer5 );
	m_panel9->Layout();
	bSizer5->Fit( m_panel9 );
	fgSizer6->Add( m_panel9, 1, wxEXPAND | wxALL, 5 );
	
	m_panel7 = new wxPanel( m_fonts, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel7, wxID_ANY, wxT("Typefaces") ), wxVERTICAL );
	
	uiStyleTypeface = new wxListBox( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sbSizer2->Add( uiStyleTypeface, 1, wxALL|wxEXPAND, 5 );
	
	
	gbSizer2->Add( sbSizer2, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	uiStyleBold = new wxCheckBox( m_panel7, wxID_ANY, wxT("Bold"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer2->Add( uiStyleBold, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	uiStyleItalic = new wxCheckBox( m_panel7, wxID_ANY, wxT("Italic"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer2->Add( uiStyleItalic, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxALIGN_RIGHT|wxALL, 5 );
	
	uiStyleUnderline = new wxCheckBox( m_panel7, wxID_ANY, wxT("Underline"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer2->Add( uiStyleUnderline, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	uiStyleSize = new wxComboBox( m_panel7, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN );
	uiStyleSize->Append( wxT("8") );
	uiStyleSize->Append( wxT("10") );
	uiStyleSize->Append( wxT("12") );
	uiStyleSize->Append( wxT("14") );
	uiStyleSize->SetMinSize( wxSize( 100,-1 ) );
	
	gbSizer2->Add( uiStyleSize, wxGBPosition( 4, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	uiStyleForeground = new wxColourPickerCtrl( m_panel7, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gbSizer2->Add( uiStyleForeground, wxGBPosition( 5, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	uiStyleBackground = new wxColourPickerCtrl( m_panel7, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gbSizer2->Add( uiStyleBackground, wxGBPosition( 6, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_staticText1 = new wxStaticText( m_panel7, wxID_ANY, wxT("Font size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	gbSizer2->Add( m_staticText1, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticText2 = new wxStaticText( m_panel7, wxID_ANY, wxT("Foreground"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gbSizer2->Add( m_staticText2, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticText3 = new wxStaticText( m_panel7, wxID_ANY, wxT("Background"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	gbSizer2->Add( m_staticText3, wxGBPosition( 6, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 0 );
	
	m_panel7->SetSizer( gbSizer2 );
	m_panel7->Layout();
	gbSizer2->Fit( m_panel7 );
	fgSizer6->Add( m_panel7, 1, wxALL|wxEXPAND, 5 );
	
	
	m_fonts->SetSizer( fgSizer6 );
	m_fonts->Layout();
	fgSizer6->Fit( m_fonts );
	m_notebook1->AddPage( m_fonts, wxT("Fonts && Colors"), false );
	m_editor = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( m_editor, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel10 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	uiShowLineNumbers = new wxCheckBox( m_panel10, wxID_ANY, wxT("Show line numbers"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( uiShowLineNumbers, 0, wxALL, 5 );
	
	uiDisplayEOL = new wxCheckBox( m_panel10, wxID_ANY, wxT("Display EOL"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( uiDisplayEOL, 0, wxALL, 5 );
	
	uiIndentGuide = new wxCheckBox( m_panel10, wxID_ANY, wxT("Display indent guide"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( uiIndentGuide, 0, wxALL, 5 );
	
	uiViewWhitespace = new wxComboBox( m_panel10, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	uiViewWhitespace->Append( wxT("Hidden") );
	uiViewWhitespace->Append( wxT("Always") );
	uiViewWhitespace->Append( wxT("AfterIdent") );
	fgSizer4->Add( uiViewWhitespace, 0, wxALL, 5 );
	
	uiWordwrap = new wxComboBox( m_panel10, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	uiWordwrap->Append( wxT("None") );
	uiWordwrap->Append( wxT("Char") );
	uiWordwrap->Append( wxT("Word") );
	fgSizer4->Add( uiWordwrap, 0, wxALL, 5 );
	
	uiEdgeStyle = new wxComboBox( m_panel10, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	uiEdgeStyle->Append( wxT("None") );
	uiEdgeStyle->Append( wxT("Line") );
	uiEdgeStyle->Append( wxT("Background") );
	fgSizer4->Add( uiEdgeStyle, 0, wxALL, 5 );
	
	uiEdgeColor = new wxColourPickerCtrl( m_panel10, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer4->Add( uiEdgeColor, 0, wxALL, 5 );
	
	uiEdgeColumn = new wxTextCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( uiEdgeColumn, 0, wxALL, 5 );
	
	uiTabWidth = new wxTextCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( uiTabWidth, 0, wxALL, 5 );
	
	uiUseTabs = new wxCheckBox( m_panel10, wxID_ANY, wxT("Use tabs"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( uiUseTabs, 0, wxALL, 5 );
	
	uiTabIndents = new wxCheckBox( m_panel10, wxID_ANY, wxT("Tab indents"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( uiTabIndents, 0, wxALL, 5 );
	
	uiBackspaceUnindents = new wxCheckBox( m_panel10, wxID_ANY, wxT("Bacspace unindents"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( uiBackspaceUnindents, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizerFolding;
	sbSizerFolding = new wxStaticBoxSizer( new wxStaticBox( m_panel10, wxID_ANY, wxT("Folding") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	uiAllowFolding = new wxCheckBox( m_panel10, wxID_ANY, wxT("Allow folding"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( uiAllowFolding, 0, wxALL, 5 );
	
	uiFoldLevelNumbers = new wxCheckBox( m_panel10, wxID_ANY, wxT("Level numbers"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( uiFoldLevelNumbers, 0, wxALL, 5 );
	
	uiFoldLineBeforeExpanded = new wxCheckBox( m_panel10, wxID_ANY, wxT("Line before (expanded)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( uiFoldLineBeforeExpanded, 0, wxALL, 5 );
	
	uiFoldLineBeforeContracted = new wxCheckBox( m_panel10, wxID_ANY, wxT("Line before (contracted)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( uiFoldLineBeforeContracted, 0, wxALL, 5 );
	
	uiLineAfterExpanded = new wxCheckBox( m_panel10, wxID_ANY, wxT("Line after (expanded)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( uiLineAfterExpanded, 0, wxALL, 5 );
	
	uiLineAfterContracted = new wxCheckBox( m_panel10, wxID_ANY, wxT("Line After (contracted)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( uiLineAfterContracted, 0, wxALL, 5 );
	
	
	sbSizerFolding->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizerFolding, 1, wxEXPAND, 5 );
	
	
	m_panel10->SetSizer( fgSizer4 );
	m_panel10->Layout();
	fgSizer4->Fit( m_panel10 );
	m_notebook2->AddPage( m_panel10, wxT("General"), true );
	m_panel11 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook2->AddPage( m_panel11, wxT("C++"), false );
	
	bSizer2->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	m_editor->SetSizer( bSizer2 );
	m_editor->Layout();
	bSizer2->Fit( m_editor );
	m_notebook1->AddPage( m_editor, wxT("Editor && Feel"), true );
	m_window = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook1->AddPage( m_window, wxT("Window behaviour"), false );
	
	fgSizer1->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Apply = new wxButton( this, wxID_APPLY );
	m_sdbSizer1->AddButton( m_sdbSizer1Apply );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	m_sdbSizer1->SetMinSize( wxSize( -1,50 ) ); 
	
	fgSizer1->Add( m_sdbSizer1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	fgSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sdbSizer1Apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::OnApply ), NULL, this );
	m_sdbSizer1Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::OnCancel ), NULL, this );
	m_sdbSizer1OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::OnOk ), NULL, this );
}

Settings::~Settings()
{
	// Disconnect Events
	m_sdbSizer1Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::OnApply ), NULL, this );
	m_sdbSizer1Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::OnCancel ), NULL, this );
	m_sdbSizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Settings::OnOk ), NULL, this );
	
}
