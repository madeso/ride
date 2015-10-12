///////////////////////////////////////////////////////////////////////////////
// Name:        bombs.h
// Purpose:     Bombs game
// Author:      P. Foggia 1996
// Modified by: Wlodzimierz Skiba (ABX) since 2003
// Created:     1996
// Copyright:   (c) 1996 P. Foggia
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DEMOS_BOMBS_BOMBS_H_
#define _WX_DEMOS_BOMBS_BOMBS_H_

#include "game.h"
#include "ride/menuevent.h"

class BombsCanvas;
class wxMenuBar;

void SetupBombMenu(wxMenuBar* menu);

class BombsFrame : public wxPanel
{
public:

  BombsFrame(wxWindow* parent, BombsGame *bombsGame);

    void NewGame(int level, bool query);

private:

    void OnNewGame(wxCommandEvent& event);
    void OnEasyGame(wxCommandEvent& event);
    void OnMediumGame(wxCommandEvent& event);
    void OnHardGame(wxCommandEvent& event);

    void OnEasyCorner(wxCommandEvent& event);

    void OnExit(wxCommandEvent& event);

    void OnAbout(wxCommandEvent& event);

    BombsGame *m_game;
    bool m_easyCorner;
    int m_lastLevel;

    // Subwindows for reference within the program.
    BombsCanvas *m_canvas;

    // DECLARE_EVENT_TABLE()
};

class BombsCanvas : public wxPanel
{
public:

    // Constructor and destructor

  BombsCanvas(BombsFrame *parent, BombsGame *game);

    void UpdateGridSize();

    wxSize GetGridSizeInPixels() const;

    virtual ~BombsCanvas();

private:

    void OnPaint(wxPaintEvent& event);
    void DrawField(wxDC *, int xc1, int yc1, int xc2, int yc2);
    void RefreshField(int xc1, int yc1, int xc2, int yc2);
    void Uncover(int x, int y);
    void OnMouseEvent(wxMouseEvent& event);
    void OnChar(wxKeyEvent& event);

    BombsGame *m_game;

    wxBitmap *m_bmp;

    // Cell size in pixels
    int m_cellWidth;
    int m_cellHeight;

    DECLARE_EVENT_TABLE()
};

/* The following sizes should probably be redefined */
/* dimensions of a scroll unit, in pixels */
#define X_UNIT 4
#define Y_UNIT 4

/* the dimensions of a cell, in scroll units are in
 * BombsCanvas::x_cell and y_cell
 */

#ifdef __WXWINCE__
#define BOMBS_FONT wxFont(12, wxSWISS, wxNORMAL, wxNORMAL)
#else
#define BOMBS_FONT wxFont(14, wxROMAN, wxNORMAL, wxNORMAL)
#endif

#endif // #ifndef _WX_DEMOS_BOMBS_BOMBS_H_

