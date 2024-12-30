///////////////////////////////////////////////////////////////////////////////
// Name:        bombs.cpp
// Purpose:     Bombs game
// Author:      P. Foggia 1996
// Modified by: Wlodzimierz Skiba (ABX) since 2003
// Created:     1996
// Copyright:   (c) 1996 P. Foggia
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif	//precompiled headers

#include "wx/stockitem.h"

#include "bombs.h"

#include <stdlib.h>

#ifndef __WXWINCE__
	#include <time.h>
#endif

#ifdef __WXWINCE__
STDAPI_(__int64) CeGetRandomSeed();
#endif

void SetupBombMenu(wxMenuBar* menuBar)
{
	wxMenu* menuFile = new wxMenu;
	wxMenu* menuLevel = new wxMenu;
	menuLevel->AppendRadioItem(bombsID_EASY, wxT("&Easy (10x10)"));
	menuLevel->AppendRadioItem(bombsID_MEDIUM, wxT("&Medium (15x15)"));
	menuLevel->AppendRadioItem(bombsID_HARD, wxT("&Hard (25x20)"));

	menuFile->Append(bombsID_NEW_GAME, wxT("&New game"));
	menuFile->Append(bombsID_LEVEL, wxT("&Level"), menuLevel, wxT("Starts a new game"));
	menuFile->AppendCheckItem(bombsID_EASYCORNER, wxT("&Easy corner"));

	menuBar->Append(menuFile, wxT("&Bombs"));
}

BombsFrame::BombsFrame(wxWindow* parent, BombsGame* game)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
{
	m_game = game;
	m_easyCorner = false;
	m_lastLevel = bombsID_EASY;

	// Create child subwindows.
	m_canvas = new BombsCanvas(this, m_game);

	// Ensure the subwindows get resized o.k.
	//  OnSize(width, height);

	// Centre frame on the screen.
	// Centre(wxBOTH);

	// Show the frame.
	// Show();


	// BEGIN_EVENT_TABLE(BombsFrame, wxFrame)
	Bind(wxEVT_MENU, &BombsFrame::OnNewGame, this, bombsID_NEW_GAME);
	Bind(wxEVT_MENU, &BombsFrame::OnEasyGame, this, bombsID_EASY);
	Bind(wxEVT_MENU, &BombsFrame::OnMediumGame, this, bombsID_MEDIUM);
	Bind(wxEVT_MENU, &BombsFrame::OnHardGame, this, bombsID_HARD);
	Bind(wxEVT_MENU, &BombsFrame::OnEasyCorner, this, bombsID_EASYCORNER);
	// Bind(wxEVT_MENU, &BombsFrame::OnExit, this      , wxID_EXIT);
	// Bind(wxEVT_MENU, &BombsFrame::OnAbout, this     , wxID_ABOUT);
	// END_EVENT_TABLE()

	NewGame(bombsID_EASY, false);
}

void BombsFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close();
}

void BombsFrame::NewGame(int level, bool query)
{
	if (query)
	{
		int ok = wxMessageBox(
			wxT("Start new game regardless previous board?"),
			wxT("Confirm"),
			wxYES_NO | wxICON_QUESTION,
			this
		);
		if (ok != wxYES) return;
	}

	int numHorzCells = 20, numVertCells = 20;
	m_lastLevel = level;

	switch (level)
	{
	case bombsID_EASY: numHorzCells = numVertCells = 10; break;

	case bombsID_MEDIUM: numHorzCells = numVertCells = 15; break;

	case bombsID_HARD:
		numHorzCells = 25;
		numVertCells = 20;
		break;

	default: wxFAIL_MSG(wxT("Invalid level")); break;
	}

	m_game->Init(numHorzCells, numVertCells, m_easyCorner);

	// GetMenuBar()->Check(level, true);

	m_canvas->UpdateGridSize();
	// SetClientSize(m_canvas->GetGridSizeInPixels());
}

void BombsFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(
		wxT("wxBombs (c) 1996 by P. Foggia\n<foggia@amalfi.dis.unina.it>"), wxT("About wxBombs")
	);
}

void BombsFrame::OnNewGame(wxCommandEvent& WXUNUSED(event))
{
	NewGame(m_lastLevel, true);
}

void BombsFrame::OnEasyGame(wxCommandEvent& WXUNUSED(event))
{
	NewGame(bombsID_EASY, true);
}

void BombsFrame::OnMediumGame(wxCommandEvent& WXUNUSED(event))
{
	NewGame(bombsID_MEDIUM, true);
}

void BombsFrame::OnHardGame(wxCommandEvent& WXUNUSED(event))
{
	NewGame(bombsID_HARD, true);
}

void BombsFrame::OnEasyCorner(wxCommandEvent& WXUNUSED(event))
{
	wxString msg;
	if (m_easyCorner)
		msg = wxT("enable");
	else
		msg = wxT("disable");

	msg = wxT("Do you really want to ") + msg
		+ wxT(" having\ntop left corner always empty for easier start?");

	int ok = wxMessageBox(msg, wxT("Confirm"), wxYES_NO | wxICON_QUESTION, this);

	if (ok != wxYES) return;

	m_easyCorner = ! m_easyCorner;

	NewGame(m_lastLevel, true);
}

BEGIN_EVENT_TABLE(BombsCanvas, wxPanel)
EVT_PAINT(BombsCanvas::OnPaint)
EVT_MOUSE_EVENTS(BombsCanvas::OnMouseEvent)
EVT_CHAR(BombsCanvas::OnChar)
END_EVENT_TABLE()

BombsCanvas::BombsCanvas(BombsFrame* parent, BombsGame* game)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
{
	m_game = game;
	int sx, sy;
	wxClientDC dc(this);
	wxFont font = BOMBS_FONT;
	dc.SetFont(font);

	wxCoord chw, chh;
	wxString buf = wxT("M");

	dc.GetTextExtent(buf, &chw, &chh);
	dc.SetFont(wxNullFont);

	dc.SetMapMode(wxMM_METRIC);

	int xcm = dc.LogicalToDeviceX(10);
	int ycm = dc.LogicalToDeviceY(10);
	// To have a square cell, there must be :
	//    sx*ycm == sy*xcm
	if (chw * ycm < chh * xcm)
	{
		sy = chh;
		sx = chh * xcm / ycm;
	}
	else
	{
		sx = chw;
		sy = chw * ycm / xcm;
	}

	m_cellWidth = (sx + 3 + X_UNIT) / X_UNIT;
	m_cellHeight = (sy + 3 + Y_UNIT) / Y_UNIT;
	dc.SetMapMode(wxMM_TEXT);
}

BombsCanvas::~BombsCanvas()
{
}

// Called when canvas needs to be repainted.
void BombsCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);

	const int numHorzCells = m_game->GetWidth();
	const int numVertCells = m_game->GetHeight();

	DrawField(&dc, 0, 0, numHorzCells - 1, numVertCells - 1);
}

void BombsCanvas::UpdateGridSize()
{
	wxSize sz = GetGridSizeInPixels();
	SetSize(sz);
	SetMinClientSize(sz);
	Refresh();
}

wxSize BombsCanvas::GetGridSizeInPixels() const
{
	return wxSize(
		m_cellWidth * X_UNIT * m_game->GetWidth() + 1,
		m_cellHeight * Y_UNIT * m_game->GetHeight() + 1
	);
}
