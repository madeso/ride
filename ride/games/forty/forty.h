/////////////////////////////////////////////////////////////////////////////
// Name:        forty.h
// Purpose:     Forty Thieves patience game
// Author:      Chris Breeze
// Modified by:
// Created:     21/07/97
// Copyright:   (c) 1993-1998 Chris Breeze
// Licence:     wxWindows licence
//---------------------------------------------------------------------------
// Last modified: 22nd July 1998 - ported to wxWidgets 2.0
/////////////////////////////////////////////////////////////////////////////
#ifndef _FORTY_H_
#define _FORTY_H_

const wxColour BackgroundColour();
const wxBrush BackgroundBrush();
const wxColour TextColour();

class FortyCanvas;
class FortyFrame: public wxPanel
{
public:
    FortyFrame(wxWindow* frame);
    virtual ~FortyFrame(){};

    void SetupFortyMenu(wxMenuBar* menuBar);

    // Menu callbacks
    void NewGame(wxCommandEvent& event);
    void Exit(wxCommandEvent& event);
    void About(wxCommandEvent& event);
    void Help(wxCommandEvent& event);
    void Undo(wxCommandEvent& event);
    void Redo(wxCommandEvent& event);
    void ToggleRightButtonUndo(wxCommandEvent& event);
    void ToggleHelpingHand(wxCommandEvent& event);
    void ToggleCardSize(wxCommandEvent& event);

    FortyCanvas* GetCanvas() { return m_canvas; }

    void SetupSize(bool large);

private:
    wxMenuBar* m_menuBar;
    FortyCanvas* m_canvas;
};

//----------------------------------------------------------------------------
// stAboutDialog
//----------------------------------------------------------------------------

class FortyAboutDialog: public wxDialog
{
public:
    // constructors and destructors
    FortyAboutDialog( wxWindow *parent, wxWindowID id, const wxString &title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE );

    bool AddControls(wxWindow* parent);
};

#define ID_ABOUT_HTML_WINDOW    1000

#endif
