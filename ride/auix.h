// Copyright (2015) Gustav

#ifndef RIDE_AUIX_H_
#define RIDE_AUIX_H_

#include <wx/aui/tabart.h>
#include "ride/wx.h"

class AuiGenericTabArt : public wxAuiTabArt {
 public:
  AuiGenericTabArt();
  virtual ~AuiGenericTabArt();

  wxAuiTabArt* Clone() wxOVERRIDE;
  void SetFlags(unsigned int flags) wxOVERRIDE;
  void SetSizingInfo(const wxSize& tabCtrlSize, size_t tabCount) wxOVERRIDE;

  void SetNormalFont(const wxFont& font) wxOVERRIDE;
  void SetSelectedFont(const wxFont& font) wxOVERRIDE;
  void SetMeasuringFont(const wxFont& font) wxOVERRIDE;
  void SetColour(const wxColour& colour) wxOVERRIDE;
  void SetActiveColour(const wxColour& colour) wxOVERRIDE;

  void DrawBorder(wxDC& dc, wxWindow* wnd, const wxRect& rect) wxOVERRIDE;

  void DrawBackground(wxDC& dc, wxWindow* wnd, const wxRect& rect) wxOVERRIDE;

  void DrawTab(wxDC& dc, wxWindow* wnd, const wxAuiNotebookPage& pane,
               const wxRect& inRect, int closeButtonState, wxRect* outTabRect,
               wxRect* outButtonRect, int* xExtent) wxOVERRIDE;

  void DrawButton(wxDC& dc, wxWindow* wnd, const wxRect& inRect, int bitmapId,
                  int buttonState, int orientation, wxRect* outRect) wxOVERRIDE;

  int GetIndentSize() wxOVERRIDE;

  int GetBorderWidth(wxWindow* wnd) wxOVERRIDE;

  int GetAdditionalBorderSpace(wxWindow* wnd) wxOVERRIDE;

  wxSize GetTabSize(wxDC& dc, wxWindow* wnd, const wxString& caption,
                    const wxBitmap& bitmap, bool active, int closeButtonState,
                    int* xExtent) wxOVERRIDE;

  int ShowDropDown(wxWindow* wnd, const wxAuiNotebookPageArray& items,
                   int activeIdx) wxOVERRIDE;

  int GetBestTabCtrlSize(wxWindow* wnd, const wxAuiNotebookPageArray& pages,
                         const wxSize& requiredBmpSize) wxOVERRIDE;

 protected:
  wxFont m_normalFont;
  wxFont m_selectedFont;
  wxFont m_measuringFont;
  wxColour m_baseColour;
  wxPen m_baseColourPen;
  wxPen m_borderPen;
  wxBrush m_baseColourBrush;
  wxColour m_activeColour;
  wxBitmap m_activeCloseBmp;
  wxBitmap m_disabledCloseBmp;
  wxBitmap m_activeLeftBmp;
  wxBitmap m_disabledLeftBmp;
  wxBitmap m_activeRightBmp;
  wxBitmap m_disabledRightBmp;
  wxBitmap m_activeWindowListBmp;
  wxBitmap m_disabledWindowListBmp;

  int m_fixedTabWidth;
  int m_tabCtrlHeight;
  unsigned int m_flags;
};

#endif  // RIDE_AUIX_H_
