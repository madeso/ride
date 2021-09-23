/////////////////////////////////////////////////////////////////////////////
// Name:        forty.cpp
// Purpose:     Forty Thieves patience game
// Author:      Chris Breeze
// Modified by:
// Created:     21/07/97
// Copyright:   (c) 1993-1998 Chris Breeze
// Licence:     wxWindows licence
//---------------------------------------------------------------------------
// Last modified: 22nd July 1998 - ported to wxWidgets 2.0
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "canvas.h"
#include "forty.h"
#include "card.h"

#if wxUSE_HTML
#include "wx/textfile.h"
#include "wx/html/htmlwin.h"
#endif

#include "wx/stockitem.h"

#include "ride/menuevent.h"

const wxColour BackgroundColour()
{
    return wxColour(0, 128, 0);
}

const wxBrush BackgroundBrush()
{
    return wxBrush(BackgroundColour(), wxSOLID);
}

const wxColour TextColour()
{
    return wxColour(*wxBLACK);
}

void FortyFrame::SetupFortyMenu(wxMenuBar* menuBar)
{
  m_menuBar = menuBar;

  const bool largecards = false;

  // Make a menu bar
  wxMenu* gameMenu = new wxMenu;
  gameMenu->Append(FORTY_NEW, "New game", wxT("Start a new game"));
  gameMenu->Append(wxID_UNDO, "Undo", wxT("Undo the last move"));
  gameMenu->Append(wxID_REDO, "Redo", wxT("Redo a move that has been undone"));

  wxMenu* optionsMenu = new wxMenu;
  optionsMenu->Append(RIGHT_BUTTON_UNDO,
    wxT("&Right button undo"),
    wxT("Enables/disables right mouse button undo and redo"),
    true
    );
  optionsMenu->Append(HELPING_HAND,
    wxT("&Helping hand"),
    wxT("Enables/disables hand cursor when a card can be moved"),
    true
    );
  optionsMenu->Append(LARGE_CARDS,
    wxT("&Large cards"),
    wxT("Enables/disables large cards for high resolution displays"),
    true
    );
  optionsMenu->Check(HELPING_HAND, true);
  optionsMenu->Check(RIGHT_BUTTON_UNDO, true);
  optionsMenu->Check(LARGE_CARDS, largecards ? true : false);
  gameMenu->AppendSubMenu(optionsMenu, wxT("&Options"));

  gameMenu->Append(FORTY_HELP_CONTENTS, wxT("&Help Contents"), wxT("Displays information about playing the game"));
  gameMenu->Append(FORTY_ABOUT, wxT("&About"), wxT("About Forty Thieves"));

  m_menuBar->Append(gameMenu, wxT("&Game"));
}

// My frame constructor
FortyFrame::FortyFrame(wxWindow* frame) :
wxPanel(frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER), parent_(frame)
{
  const bool largecards = false;
    if (largecards)
        Card::SetScale(1.3);

    wxSize size(100, 100);

    m_canvas = new FortyCanvas(this, wxDefaultPosition, size);

    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
    topsizer->Add( m_canvas, 1, wxEXPAND | wxALL, 0);
    SetSizer( topsizer );

    
    Bind(wxEVT_MENU,       &FortyFrame::NewGame,               this,        FORTY_NEW        );
    Bind(wxEVT_MENU,       &FortyFrame::About,                 this,      FORTY_ABOUT        );
    Bind(wxEVT_MENU,       &FortyFrame::Help,                  this,     FORTY_HELP_CONTENTS );
    Bind(wxEVT_MENU,       &FortyFrame::Undo,                  this,     FORTY_UNDO          );
    Bind(wxEVT_MENU,       &FortyFrame::Redo,                  this,     FORTY_REDO          );
    Bind(wxEVT_MENU,       &FortyFrame::ToggleRightButtonUndo, this,     RIGHT_BUTTON_UNDO);
    Bind(wxEVT_MENU,       &FortyFrame::ToggleHelpingHand,     this,     HELPING_HAND        );
    Bind(wxEVT_MENU,       &FortyFrame::ToggleCardSize,        this,     LARGE_CARDS            );

    topsizer->SetSizeHints( this );

    SetupSize(largecards);

    GetCanvas()->ShowPlayerDialog();
}

void FortyFrame::SetupSize(bool largecards) {
  wxSize size(668, 510);

  if (largecards) {
    size = wxSize(1000, 750);
  }

  this->SetMinClientSize(size);
  if (parent_) {
    parent_->Layout();
  }
}

void
FortyFrame::NewGame(wxCommandEvent&)
{
    m_canvas->NewGame();
}

void
FortyFrame::Exit(wxCommandEvent&)
{
    Close(true);
}

void
FortyFrame::Help(wxCommandEvent& event)
{
  FortyAboutDialog dialog(this, wxID_ANY, wxT("Forty Thieves Instructions"));
  if (dialog.ShowModal() == wxID_OK)
  {
  }
}

void
FortyFrame::About(wxCommandEvent&)
{
        wxMessageBox(
            wxT("Forty Thieves\n\n")
            wxT("A free card game written with the wxWidgets toolkit\n")
            wxT("Author: Chris Breeze (c) 1992-2004\n")
            wxT("email: chris@breezesys.com"),
            wxT("About Forty Thieves"),
            wxOK|wxICON_INFORMATION, this
            );
}


void
FortyFrame::Undo(wxCommandEvent&)
{
    m_canvas->Undo();
}

void
FortyFrame::Redo(wxCommandEvent&)
{
    m_canvas->Redo();
}

void
FortyFrame::ToggleRightButtonUndo(wxCommandEvent& event)
{
    bool checked = m_menuBar->IsChecked(event.GetId());
    m_canvas->EnableRightButtonUndo(checked);
}

void
FortyFrame::ToggleHelpingHand(wxCommandEvent& event)
{
    bool checked = m_menuBar->IsChecked(event.GetId());
    m_canvas->EnableHelpingHand(checked);
}

void
FortyFrame::ToggleCardSize(wxCommandEvent& event)
{
        bool checked = m_menuBar->IsChecked(event.GetId());
        Card::SetScale(checked ? 1.3 : 1);
        m_canvas->LayoutGame();
        m_canvas->Refresh();
        SetupSize(checked);
}

//----------------------------------------------------------------------------
// stAboutDialog
//----------------------------------------------------------------------------

FortyAboutDialog::FortyAboutDialog( wxWindow *parent, wxWindowID id, const wxString &title,
    const wxPoint &position, const wxSize& size, long style ) :
    wxDialog( parent, id, title, position, size, style )
{
    AddControls(this);

    Centre(wxBOTH);
}

bool FortyAboutDialog::AddControls(wxWindow* parent)
{
#if wxUSE_HTML
  const wxString htmlText =                                                               
"<html>                                                               "
"<body bgcolor=\"#FFFFFF\" TEXT=\"#000000\" VLINK=\"#000000\"         "
" LINK=\"#000000\" ALINK=\"#000000\">                                 "
"<font color=\"#000000\">                                             "
"<center>                                                             "
"<font size=+1><b>Forty Thieves</b></font> <p>a game of patience</p>  "
"<p>(c)Chris Breeze, 1998 - 2002</p>                                  "
"</center>                                                            "
"<p>Forty Thieves is a patience game played with two full packs of    "
"cards. At the start of the game forty cards are dealt on the         "
"eight 'bases' along the top of the window. The object of the         "
"game is to place all the cards onto the eight 'foundations'. The     "
"foundations are built starting with the ace and adding cards of      "
"the same suit up to the king. Cards are dealt from the pack and      "
"placed on the discard pile. Cards may be moved from the discard      "
"pile or one of the bases to a base or a foundation. Only one         "
"card can be moved at a time. Cards can only be placed on a base      "
"if the top card of the base is of the same suit and is one           "
"higher in pip value or the base is empty e.g. the eight of           "
"spades can only be placed on top of the nine of spades.</p>          "
"                                                                     "
"<p>When the mouse cursor is over a card which can be moved it        "
"changes to the 'hand' cursor. The card can then be moved by          "
"double clicking the left button.</p>                                 "
"                                                                     "
"<p>The mouse cursor also changes to a hand when a card is dragged    "
"by placing the cursor over the card and holding down the left        "
"button. This feature can be enabled and disabled by selecting        "
"the 'Helping hand' option from the Edit menu.</p>                    "
"                                                                     "
"<p>The 'foundations' are the eight piles of cards down the left      "
"side of the window. When the game starts these piles are empty.      "
"The object of the game is to place all the cards on the              "
"foundations. An ace can be placed on any empty foundation. Other     "
"cards can only be placed on a foundation if the top card is of       "
"the same suit and is one lower in pip value e.g. the three of        "
"clubs can be placed on the four of clubs.</p>                        "
"                                                                     "
"<p>The 'bases' are the ten piles of cards along the top of the       "
"window. At the start of the game four cards are dealt face up on     "
"each of the bases. A card can be added to a base if the base is      "
"empty or if the top card is of the same suit and is one higher       "
"in pip value e.g. the queen of hearts can be placed on the king      "
"of hearts The top card of a base can be moved onto another base      "
"or a foundation.</p>                                                 "
"                                                                     "
"<p>Cards can be only moved one at a time.The top card of the pack    "
"can be dealt onto discard pile by placing the mouse cursor over      "
"the pack and pressing the left button. The number of cards           "
"remaining is displayed to the right of the pack.</p>                 "
"                                                                     "
"<p>Cards can be moved from the discard pile or the bases either by   "
"'double-clicking' or by dragging. If the left button is              "
"double-clicked when the mouse cursor is over a card and it can       "
"move to another pile, it will do so. This is a quick way of          "
"moving cards when their destination is unambiguous.</p>              "
"                                                                     "
"<p>A card can be dragged by placing the mouse cursor over the card   "
"and holding down the left button. The card will follow the mouse     "
"cursor until the left button is released. If the card is over a      "
"pile on which it can be placed it will be added to that pile,        "
"otherwise it will be returned to the pile from which it was          "
"dragged.</p>                                                         "
"                                                                     "
"<p>One point is scored for every card that is placed on a            "
"foundation. Since there are two packs of 52 cards the maximum        "
"score is 104. A record is kept of the number of games played,        "
"the number of games won, the current score and the average           "
"score. This information is displayed at bottom right of the          "
"window and is stored on disk between games. A game is deemed to      "
"have started if the cards have been dealt and any card has been      "
"moved. If the game is abandoned before it is finished (i.e. by       "
"starting a new game or closing window) it counts as a lost game.</p> "
"                                                                     "
"<p>All moves are recorded and can be undone.To undo a move select    "
"the undo menu item from the Game menu. A quicker way of undoing      "
"is to press the right mouse button (it doesn't matter where the      "
"mouse cursor is). Right button undo can be enabled and disabled      "
"by selecting the 'Right button undo' option from the Game menu.      "
"Pressing the right mouse button with the control key pressed         "
"re-does a previously undone move.</p>                                "
"                                                                     "
"<p>An empty base or two is very useful as it gives the opportunity   "
"to unscramble other bases. Try not to build onto kings which         "
"obscure valuable cards as it will be difficult to get to them        "
"later. The undo facility is very useful for going back and using     "
"'hindsight'.</p>                                                     "
"                                                                     "
"<p>Don't be put off if you can't win every game. I reckon winning 1  "
"in 10 is pretty good (winning 1 in 3 is excellent).</p>              "
"                                                                     "
"</font>                                                              "
"                                                                     "
"</body>                                                              "
"</html>                                                              "
    ;

    wxSize htmlSize(400, 290);

    // Note: in later versions of wxWin this will be fixed so wxRAISED_BORDER
    // does the right thing. Meanwhile, this is a workaround.
#ifdef __WXMSW__
    long borderStyle = wxDOUBLE_BORDER;
#else
    long borderStyle = wxRAISED_BORDER;
#endif

    wxHtmlWindow* html = new wxHtmlWindow(this, ID_ABOUT_HTML_WINDOW, wxDefaultPosition, htmlSize, borderStyle);
    html -> SetBorders(10);
    html -> SetPage(htmlText);

    //// Start of sizer-based control creation

    wxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxWindow *item1 = parent->FindWindow( ID_ABOUT_HTML_WINDOW );
    wxASSERT( item1 );
    item0->Add( item1, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *item2 = new wxButton( parent, wxID_CLOSE );
    item2->SetDefault();
    item2->SetFocus();
    SetAffirmativeId(wxID_CLOSE);

    item0->Add( item2, 0, wxALIGN_RIGHT|wxALL, 5 );

    parent->SetSizer( item0 );
    parent->Layout();
    item0->Fit( parent );
    item0->SetSizeHints( parent );
#endif

    return true;
}
