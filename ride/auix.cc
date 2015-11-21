// Copyright (2015) Gustav

#include "ride/auix.h"

#include <wx/renderer.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>
#include <wx/aui/auibook.h>

namespace {

// wxAuiBitmapFromBits() is a utility function that creates a
// masked bitmap from raw bits (XBM format)
wxBitmap wxAuiBitmapFromBits(const unsigned char bits[], int w, int h,
                             const wxColour& color) {
  wxImage img =
      wxBitmap(reinterpret_cast<const char*>(bits), w, h).ConvertToImage();
  img.Replace(0, 0, 0, 123, 123, 123);
  img.Replace(255, 255, 255, color.Red(), color.Green(), color.Blue());
  img.SetMaskColour(123, 123, 123);
  return wxBitmap(img);
}

wxString wxAuiChopText(wxDC& dc, const wxString& text, int max_size) {
  wxCoord x, y;

  // first check if the text fits with no problems
  dc.GetTextExtent(text, &x, &y);
  if (x <= max_size) return text;

  size_t i, len = text.Length();
  size_t last_good_length = 0;
  for (i = 0; i < len; ++i) {
    wxString s = text.Left(i);
    s += wxT("...");

    dc.GetTextExtent(s, &x, &y);
    if (x > max_size) break;

    last_good_length = i;
  }

  wxString ret = text.Left(last_good_length);
  ret += wxT("...");
  return ret;
}

static void IndentPressedBitmap(wxRect* rect, int button_state) {
  if (button_state == wxAUI_BUTTON_STATE_PRESSED) {
    rect->x++;
    rect->y++;
  }
}

wxColour CalculateBaseColor() {
  wxColor baseColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);

  // the baseColour is too pale to use as our base colour,
  // so darken it a bit --
  if ((255 - baseColour.Red()) + (255 - baseColour.Green()) +
          (255 - baseColour.Blue()) <
      60) {
    baseColour = baseColour.ChangeLightness(92);
  }

  return baseColour;
}

}  // namespace

class wxAuiCommandCapture : public wxEvtHandler {
 public:
  wxAuiCommandCapture() { m_lastId = 0; }
  int GetCommandId() const { return m_lastId; }

  bool ProcessEvent(wxEvent& evt) {  // NOLINT
    if (evt.GetEventType() == wxEVT_MENU) {
      m_lastId = evt.GetId();
      return true;
    }

    if (GetNextHandler()) return GetNextHandler()->ProcessEvent(evt);

    return false;
  }

 private:
  int m_lastId;
};

//////////////////////////////////////////////////////////////////////////

#if defined(__WXMAC__)
static const unsigned char close_bits[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFE, 0x03, 0xF8, 0x01, 0xF0, 0x19,
    0xF3, 0xB8, 0xE3, 0xF0, 0xE1, 0xE0, 0xE0, 0xF0, 0xE1, 0xB8, 0xE3,
    0x19, 0xF3, 0x01, 0xF0, 0x03, 0xF8, 0x0F, 0xFE, 0xFF, 0xFF};
#elif defined(__WXGTK__)
static const unsigned char close_bits[] = {
    0xff, 0xff, 0xff, 0xff, 0x07, 0xf0, 0xfb, 0xef, 0xdb, 0xed, 0x8b,
    0xe8, 0x1b, 0xec, 0x3b, 0xee, 0x1b, 0xec, 0x8b, 0xe8, 0xdb, 0xed,
    0xfb, 0xef, 0x07, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#else
static const unsigned char close_bits[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xf3, 0xcf,
    0xf9, 0x9f, 0xfc, 0x3f, 0xfe, 0x3f, 0xfe, 0x9f, 0xfc, 0xcf, 0xf9,
    0xe7, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#endif

static const unsigned char left_bits[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x3f,
    0xfe, 0x1f, 0xfe, 0x0f, 0xfe, 0x1f, 0xfe, 0x3f, 0xfe, 0x7f, 0xfe,
    0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static const unsigned char right_bits[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0x9f, 0xff, 0x1f,
    0xff, 0x1f, 0xfe, 0x1f, 0xfc, 0x1f, 0xfe, 0x1f, 0xff, 0x9f, 0xff,
    0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static const unsigned char list_bits[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0f, 0xf8, 0xff, 0xff, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfe,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

//////////////////////////////////////////////////////////////////////////

AuiGenericTabArt::AuiGenericTabArt() {
  m_normalFont = *wxNORMAL_FONT;
  m_selectedFont = *wxNORMAL_FONT;
  m_selectedFont.SetWeight(wxFONTWEIGHT_BOLD);
  m_measuringFont = m_selectedFont;

  m_fixedTabWidth = 100;
  m_tabCtrlHeight = 0;

  const wxColor baseColour = CalculateBaseColor();
  const wxColor borderColor = baseColour.ChangeLightness(75);
  const wxColor black(0, 0, 0);

  backgroundColor_ = baseColour;
  activeTabBackground_ = baseColour;
  inactiveTabBackground_ = baseColour;
  activeBorderColor_ = borderColor;
  inactiveBorderColor_ = borderColor;
  activeTabText_ = black;
  inactiveTabText_ = black;

  m_activeCloseBmp = wxAuiBitmapFromBits(close_bits, 16, 16, *wxBLACK);
  m_disabledCloseBmp =
      wxAuiBitmapFromBits(close_bits, 16, 16, wxColour(128, 128, 128));

  m_activeLeftBmp = wxAuiBitmapFromBits(left_bits, 16, 16, *wxBLACK);
  m_disabledLeftBmp =
      wxAuiBitmapFromBits(left_bits, 16, 16, wxColour(128, 128, 128));

  m_activeRightBmp = wxAuiBitmapFromBits(right_bits, 16, 16, *wxBLACK);
  m_disabledRightBmp =
      wxAuiBitmapFromBits(right_bits, 16, 16, wxColour(128, 128, 128));

  m_activeWindowListBmp = wxAuiBitmapFromBits(list_bits, 16, 16, *wxBLACK);
  m_disabledWindowListBmp =
      wxAuiBitmapFromBits(list_bits, 16, 16, wxColour(128, 128, 128));

  m_flags = 0;
}

AuiGenericTabArt::~AuiGenericTabArt() {}

wxAuiTabArt* AuiGenericTabArt::Clone() { return new AuiGenericTabArt(*this); }

void AuiGenericTabArt::SetFlags(unsigned int flags) { m_flags = flags; }

void AuiGenericTabArt::SetSizingInfo(const wxSize& tab_ctrl_size,
                                     size_t tab_count) {
  m_fixedTabWidth = 100;

  int tot_width = static_cast<int>(tab_ctrl_size.x - GetIndentSize() - 4);

  if (m_flags & wxAUI_NB_CLOSE_BUTTON) tot_width -= m_activeCloseBmp.GetWidth();
  if (m_flags & wxAUI_NB_WINDOWLIST_BUTTON)
    tot_width -= m_activeWindowListBmp.GetWidth();

  if (tab_count > 0) {
    m_fixedTabWidth = tot_width / static_cast<int>(tab_count);
  }

  if (m_fixedTabWidth < 100) m_fixedTabWidth = 100;

  if (m_fixedTabWidth > tot_width / 2) m_fixedTabWidth = tot_width / 2;

  if (m_fixedTabWidth > 220) m_fixedTabWidth = 220;

  m_tabCtrlHeight = tab_ctrl_size.y;
}

void AuiGenericTabArt::DrawBorder(wxDC& dc, wxWindow* wnd, const wxRect& rect) {
  int i, border_width = GetBorderWidth(wnd);

  wxRect theRect(rect);
  for (i = 0; i < border_width; ++i) {
    dc.DrawRectangle(theRect.x, theRect.y, theRect.width, theRect.height);
    theRect.Deflate(1);
  }
}

void AuiGenericTabArt::DrawBackground(wxDC& dc,  // NOLINT
                                      wxWindow* wnd, const wxRect& rect) {
  // draw background
  wxRect r;

  if (m_flags & wxAUI_NB_BOTTOM)
    r = wxRect(rect.x, rect.y, rect.width + 2, rect.height);
  // TODO(unknown): else if (m_flags &wxAUI_NB_LEFT) {}
  // TODO(unknown): else if (m_flags &wxAUI_NB_RIGHT) {}
  else  // for wxAUI_NB_TOP
    r = wxRect(rect.x, rect.y, rect.width + 2, rect.height - 3);

  dc.SetPen(wxPen(backgroundColor_));
  dc.SetBrush(wxBrush(backgroundColor_));
  dc.DrawRectangle(r);

  // draw base lines

  dc.SetPen(wxPen(activeTabBackground_));
  dc.SetBrush(wxBrush(activeTabBackground_));

  int y = rect.GetHeight();
  int w = rect.GetWidth();

  int tabheight = 3;

  int ypos = -1;
  if (m_flags & wxAUI_NB_BOTTOM) {
    // dc.SetBrush(wxBrush(bottom_color));
    dc.DrawRectangle(-1, 0, w + 2, tabheight);
    ypos = tabheight;
  } else {
    // for wxAUI_NB_TOP
    // TODO(unknown): else if (m_flags &wxAUI_NB_LEFT) {}
    // TODO(unknown): else if (m_flags &wxAUI_NB_RIGHT) {}
    // dc.SetBrush(m_baseColourBrush);
    dc.DrawRectangle(-1, y - tabheight, w + 2, tabheight);
    ypos = y - tabheight;
  }

  dc.SetPen(wxPen(activeBorderColor_));
  ypos -= 1;
  dc.DrawLine(-1, ypos, w + 2, ypos);
}

// DrawTab() draws an individual tab.
//
// dc       - output dc
// in_rect  - rectangle the tab should be confined to
// caption  - tab's caption
// active   - whether or not the tab is active
// out_rect - actual output rectangle
// x_extent - the advance x; where the next tab should start

void AuiGenericTabArt::DrawTab(wxDC& dc, wxWindow* wnd,  // NOLINT
                               const wxAuiNotebookPage& page,
                               const wxRect& in_rect, int close_button_state,
                               wxRect* out_tab_rect, wxRect* out_button_rect,
                               int* x_extent) {
  wxCoord normal_textx, normal_texty;
  wxCoord selected_textx, selected_texty;
  wxCoord texty;

  // if the caption is empty, measure some temporary text
  wxString caption = page.caption;
  if (caption.empty()) caption = wxT("Xj");

  dc.SetFont(m_selectedFont);
  dc.GetTextExtent(caption, &selected_textx, &selected_texty);

  dc.SetFont(m_normalFont);
  dc.GetTextExtent(caption, &normal_textx, &normal_texty);

  // figure out the size of the tab
  wxSize tab_size = GetTabSize(dc, wnd, page.caption, page.bitmap, page.active,
                               close_button_state, x_extent);

  wxCoord tab_height = m_tabCtrlHeight - 3;
  wxCoord tab_width = tab_size.x;
  wxCoord tab_x = in_rect.x;
  wxCoord tab_y = in_rect.y + in_rect.height - tab_height;

  caption = page.caption;

  // select pen, brush and font for the tab to be drawn

  if (page.active) {
    dc.SetFont(m_selectedFont);
    texty = selected_texty;
  } else {
    dc.SetFont(m_normalFont);
    texty = normal_texty;
  }

  // create points that will make the tab outline

  int clip_width = tab_width;
  if (tab_x + clip_width > in_rect.x + in_rect.width)
    clip_width = (in_rect.x + in_rect.width) - tab_x;

  /*
  wxPoint clip_points[6];
  clip_points[0] = wxPoint(tab_x,              tab_y+tab_height-3);
  clip_points[1] = wxPoint(tab_x,              tab_y+2);
  clip_points[2] = wxPoint(tab_x+2,            tab_y);
  clip_points[3] = wxPoint(tab_x+clip_width-1, tab_y);
  clip_points[4] = wxPoint(tab_x+clip_width+1, tab_y+2);
  clip_points[5] = wxPoint(tab_x+clip_width+1, tab_y+tab_height-3);

  // FIXME: these ports don't provide wxRegion ctor from array of points
  #if !defined(__WXDFB__)
  // set the clipping region for the tab --
  wxRegion clipping_region(WXSIZEOF(clip_points), clip_points);
  dc.SetClippingRegion(clipping_region);
  #endif // !wxDFB && !wxCocoa
  */
  // since the above code above doesn't play well with WXDFB or WXCOCOA,
  // we'll just use a rectangle for the clipping region for now --
  dc.SetClippingRegion(tab_x, tab_y, clip_width + 1, tab_height - 3);

  wxPoint border_points[6];
  if (m_flags & wxAUI_NB_BOTTOM) {
    border_points[0] = wxPoint(tab_x, tab_y);
    border_points[1] = wxPoint(tab_x, tab_y + tab_height - 6);
    border_points[2] = wxPoint(tab_x + 2, tab_y + tab_height - 4);
    border_points[3] = wxPoint(tab_x + tab_width - 2, tab_y + tab_height - 4);
    border_points[4] = wxPoint(tab_x + tab_width, tab_y + tab_height - 6);
    border_points[5] = wxPoint(tab_x + tab_width, tab_y);
  } else {
    border_points[0] = wxPoint(tab_x, tab_y + tab_height - 4);
    border_points[1] = wxPoint(tab_x, tab_y + 2);
    border_points[2] = wxPoint(tab_x + 2, tab_y);
    border_points[3] = wxPoint(tab_x + tab_width - 2, tab_y);
    border_points[4] = wxPoint(tab_x + tab_width, tab_y + 2);
    border_points[5] = wxPoint(tab_x + tab_width, tab_y + tab_height - 4);
  }
  // TODO(unknown): else if (m_flags &wxAUI_NB_LEFT) {}
  // TODO(unknown): else if (m_flags &wxAUI_NB_RIGHT) {}

  int drawn_tab_yoff = border_points[1].y;
  int drawn_tab_height = border_points[0].y - border_points[1].y;

  if (page.active) {
    // draw active tab

    // draw base background color
    wxRect r(tab_x, tab_y, tab_width, tab_height);
    dc.SetPen(wxPen(activeTabBackground_));
    dc.SetBrush(wxBrush(activeTabBackground_));
    dc.DrawRectangle(r.x + 1, r.y + 1, r.width - 1, r.height - 4);

    // these two points help the rounded corners appear more antialiased
    dc.SetPen(wxPen(activeTabBackground_));
    dc.DrawPoint(r.x + 2, r.y + 1);
    dc.DrawPoint(r.x + r.width - 2, r.y + 1);
  } else {
    // draw inactive tab

    wxRect r(tab_x, tab_y, tab_width, tab_height);

    dc.SetPen(wxPen(inactiveTabBackground_));
    dc.SetBrush(wxBrush(inactiveTabBackground_));
    dc.DrawRectangle(r.x + 1, r.y + 1, r.width - 1, r.height - 4);

    // dc.DrawPoint(r.x + 2, r.y + 1);
    // dc.DrawPoint(r.x + r.width - 2, r.y + 1);
  }

  // draw tab outline
  if (page.active) {
    dc.SetPen(wxPen(activeBorderColor_));
  } else {
    dc.SetPen(wxPen(inactiveBorderColor_));
  }
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  dc.DrawPolygon(WXSIZEOF(border_points), border_points);

  // there are two horizontal grey lines at the bottom of the tab control,
  // this gets rid of the top one of those lines in the tab control
  if (page.active) {
    dc.SetPen(wxPen(activeTabBackground_));
    dc.DrawLine(border_points[0].x + 1, border_points[0].y, border_points[5].x,
                border_points[5].y);
  }

  int text_offset = tab_x + 8;
  int close_button_width = 0;
  if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN) {
    close_button_width = m_activeCloseBmp.GetWidth();
  }

  int bitmap_offset = 0;
  if (page.bitmap.IsOk()) {
    bitmap_offset = tab_x + 8;

    // draw bitmap
    dc.DrawBitmap(
        page.bitmap, bitmap_offset,
        drawn_tab_yoff + (drawn_tab_height / 2) - (page.bitmap.GetHeight() / 2),
        true);

    text_offset = bitmap_offset + page.bitmap.GetWidth();
    text_offset += 3;  // bitmap padding
  }

  wxString draw_text = wxAuiChopText(
      dc, caption, tab_width - (text_offset - tab_x) - close_button_width);

  // draw tab text
  if (page.active) {
    dc.SetTextForeground(activeTabText_);
  } else {
    dc.SetTextForeground(inactiveTabText_);
  }
  dc.DrawText(draw_text, text_offset,
              drawn_tab_yoff + (drawn_tab_height) / 2 - (texty / 2) - 1);

  // draw focus rectangle
  if (page.active && (wnd->FindFocus() == wnd)) {
    wxRect focusRectText(text_offset, (drawn_tab_yoff + (drawn_tab_height) / 2 -
                                       (texty / 2) - 1),
                         selected_textx, selected_texty);

    wxRect focusRect;
    wxRect focusRectBitmap;

    if (page.bitmap.IsOk())
      focusRectBitmap =
          wxRect(bitmap_offset, drawn_tab_yoff + (drawn_tab_height / 2) -
                                    (page.bitmap.GetHeight() / 2),
                 page.bitmap.GetWidth(), page.bitmap.GetHeight());

    if (page.bitmap.IsOk() && draw_text.IsEmpty())
      focusRect = focusRectBitmap;
    else if (!page.bitmap.IsOk() && !draw_text.IsEmpty())
      focusRect = focusRectText;
    else if (page.bitmap.IsOk() && !draw_text.IsEmpty())
      focusRect = focusRectText.Union(focusRectBitmap);

    focusRect.Inflate(2, 2);

    wxRendererNative::Get().DrawFocusRect(wnd, dc, focusRect, 0);
  }

  // draw close button if necessary
  if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN) {
    wxBitmap bmp = m_disabledCloseBmp;

    if (close_button_state == wxAUI_BUTTON_STATE_HOVER ||
        close_button_state == wxAUI_BUTTON_STATE_PRESSED) {
      bmp = m_activeCloseBmp;
    }

    int offsetY = tab_y - 1;
    if (m_flags & wxAUI_NB_BOTTOM) offsetY = 1;

    wxRect rect(tab_x + tab_width - close_button_width - 1,
                offsetY + (tab_height / 2) - (bmp.GetHeight() / 2),
                close_button_width, tab_height);

    IndentPressedBitmap(&rect, close_button_state);
    dc.DrawBitmap(bmp, rect.x, rect.y, true);

    *out_button_rect = rect;
  }

  *out_tab_rect = wxRect(tab_x, tab_y, tab_width, tab_height);

  dc.DestroyClippingRegion();
}

int AuiGenericTabArt::GetIndentSize() { return 5; }

int AuiGenericTabArt::GetBorderWidth(wxWindow* wnd) {
  wxAuiManager* mgr = wxAuiManager::GetManager(wnd);
  if (mgr) {
    wxAuiDockArt* art = mgr->GetArtProvider();
    if (art) return art->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE);
  }
  return 1;
}

int AuiGenericTabArt::GetAdditionalBorderSpace(wxWindow* wnd) { return 0; }

wxSize AuiGenericTabArt::GetTabSize(wxDC& dc,  // NOLINT
                                    wxWindow* wnd, const wxString& caption,
                                    const wxBitmap& bitmap, bool active,
                                    int close_button_state, int* x_extent) {
  wxCoord measured_textx, measured_texty, tmp;

  dc.SetFont(m_measuringFont);
  dc.GetTextExtent(caption, &measured_textx, &measured_texty);

  dc.GetTextExtent(wxT("ABCDEFXj"), &tmp, &measured_texty);

  // add padding around the text
  wxCoord tab_width = measured_textx;
  wxCoord tab_height = measured_texty;

  // if the close button is showing, add space for it
  if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    tab_width += m_activeCloseBmp.GetWidth() + 3;

  // if there's a bitmap, add space for it
  if (bitmap.IsOk()) {
    tab_width += bitmap.GetWidth();
    tab_width += 3;  // right side bitmap padding
    tab_height = wxMax(tab_height, bitmap.GetHeight());
  }

  // add padding
  tab_width += 16;
  tab_height += 10;

  if (m_flags & wxAUI_NB_TAB_FIXED_WIDTH) {
    tab_width = m_fixedTabWidth;
  }

  *x_extent = tab_width;

  return wxSize(tab_width, tab_height);
}

void AuiGenericTabArt::DrawButton(wxDC& dc, wxWindow* WXUNUSED(wnd),  // NOLINT
                                  const wxRect& in_rect, int bitmap_id,
                                  int button_state, int orientation,
                                  wxRect* out_rect) {
  wxBitmap bmp;
  wxRect rect;

  switch (bitmap_id) {
    case wxAUI_BUTTON_CLOSE:
      if (button_state & wxAUI_BUTTON_STATE_DISABLED)
        bmp = m_disabledCloseBmp;
      else
        bmp = m_activeCloseBmp;
      break;
    case wxAUI_BUTTON_LEFT:
      if (button_state & wxAUI_BUTTON_STATE_DISABLED)
        bmp = m_disabledLeftBmp;
      else
        bmp = m_activeLeftBmp;
      break;
    case wxAUI_BUTTON_RIGHT:
      if (button_state & wxAUI_BUTTON_STATE_DISABLED)
        bmp = m_disabledRightBmp;
      else
        bmp = m_activeRightBmp;
      break;
    case wxAUI_BUTTON_WINDOWLIST:
      if (button_state & wxAUI_BUTTON_STATE_DISABLED)
        bmp = m_disabledWindowListBmp;
      else
        bmp = m_activeWindowListBmp;
      break;
  }

  if (!bmp.IsOk()) return;

  rect = in_rect;

  if (orientation == wxLEFT) {
    rect.SetX(in_rect.x);
    rect.SetY(((in_rect.y + in_rect.height) / 2) - (bmp.GetHeight() / 2));
    rect.SetWidth(bmp.GetWidth());
    rect.SetHeight(bmp.GetHeight());
  } else {
    rect = wxRect(in_rect.x + in_rect.width - bmp.GetWidth(),
                  ((in_rect.y + in_rect.height) / 2) - (bmp.GetHeight() / 2),
                  bmp.GetWidth(), bmp.GetHeight());
  }

  IndentPressedBitmap(&rect, button_state);
  dc.DrawBitmap(bmp, rect.x, rect.y, true);

  *out_rect = rect;
}

int AuiGenericTabArt::ShowDropDown(wxWindow* wnd,
                                   const wxAuiNotebookPageArray& pages,
                                   int /*active_idx*/) {
  wxMenu menuPopup;

  size_t i, count = pages.GetCount();
  for (i = 0; i < count; ++i) {
    const wxAuiNotebookPage& page = pages.Item(i);
    wxString caption = page.caption;

    // if there is no caption, make it a space.  This will prevent
    // an assert in the menu code.
    if (caption.IsEmpty()) caption = wxT(" ");

    wxMenuItem* item = new wxMenuItem(NULL, 1000 + i, caption);
    if (page.bitmap.IsOk()) item->SetBitmap(page.bitmap);
    menuPopup.Append(item);
  }

  // find out where to put the popup menu of window items
  wxPoint pt = ::wxGetMousePosition();
  pt = wnd->ScreenToClient(pt);

  // find out the screen coordinate at the bottom of the tab ctrl
  wxRect cli_rect = wnd->GetClientRect();
  pt.y = cli_rect.y + cli_rect.height;

  wxAuiCommandCapture* cc = new wxAuiCommandCapture;
  wnd->PushEventHandler(cc);
  wnd->PopupMenu(&menuPopup, pt);
  int command = cc->GetCommandId();
  wnd->PopEventHandler(true);

  if (command >= 1000) return command - 1000;

  return -1;
}

int AuiGenericTabArt::GetBestTabCtrlSize(wxWindow* wnd,
                                         const wxAuiNotebookPageArray& pages,
                                         const wxSize& requiredBmp_size) {
  wxClientDC dc(wnd);
  dc.SetFont(m_measuringFont);

  // sometimes a standard bitmap size needs to be enforced, especially
  // if some tabs have bitmaps and others don't.  This is important because
  // it prevents the tab control from resizing when tabs are added.
  wxBitmap measureBmp;
  if (requiredBmp_size.IsFullySpecified()) {
    measureBmp.Create(requiredBmp_size.x, requiredBmp_size.y);
  }

  int max_y = 0;
  size_t i, page_count = pages.GetCount();
  for (i = 0; i < page_count; ++i) {
    wxAuiNotebookPage& page = pages.Item(i);

    wxBitmap bmp;
    if (measureBmp.IsOk())
      bmp = measureBmp;
    else
      bmp = page.bitmap;

    // we don't use the caption text because we don't
    // want tab heights to be different in the case
    // of a very short piece of text on one tab and a very
    // tall piece of text on another tab
    int x_ext = 0;
    wxSize s = GetTabSize(dc, wnd, wxT("ABCDEFGHIj"), bmp, true,
                          wxAUI_BUTTON_STATE_HIDDEN, &x_ext);

    max_y = wxMax(max_y, s.y);
  }

  return max_y + 2;
}

void AuiGenericTabArt::SetNormalFont(const wxFont& font) {
  m_normalFont = font;
}

void AuiGenericTabArt::SetSelectedFont(const wxFont& font) {
  m_selectedFont = font;
}

void AuiGenericTabArt::SetMeasuringFont(const wxFont& font) {
  m_measuringFont = font;
}

void AuiGenericTabArt::SetColour(const wxColour& colour) {
  wxColor border = colour.ChangeLightness(75);

  backgroundColor_ = colour;
  inactiveTabBackground_ = colour;
  activeBorderColor_ = border;
  inactiveBorderColor_ = border;
}

void AuiGenericTabArt::SetActiveColour(const wxColour& colour) {
  activeTabBackground_ = colour;
}

AuiGenericTabArt& AuiGenericTabArt::set_backgroundColor(const wxColor& c) {
  backgroundColor_ = c;
  return *this;
}
AuiGenericTabArt& AuiGenericTabArt::set_activeTabBackground(const wxColor& c) {
  activeTabBackground_ = c;
  return *this;
}
AuiGenericTabArt& AuiGenericTabArt::set_inactiveTabBackground(
    const wxColor& c) {
  inactiveTabBackground_ = c;
  return *this;
}
AuiGenericTabArt& AuiGenericTabArt::set_activeBorderColor(const wxColor& c) {
  activeBorderColor_ = c;
  return *this;
}
AuiGenericTabArt& AuiGenericTabArt::set_inactiveBorderColor(const wxColor& c) {
  inactiveBorderColor_ = c;
  return *this;
}
AuiGenericTabArt& AuiGenericTabArt::set_activeTabText(const wxColor& c) {
  activeTabText_ = c;
  return *this;
}
AuiGenericTabArt& AuiGenericTabArt::set_inactiveTabText(const wxColor& c) {
  inactiveTabText_ = c;
  return *this;
}

//////////////////////////////////////////////////////////////////////////

StatusBarGeneric::StatusBarGeneric(wxWindow* parent, wxWindowID winid,
                                   long style,  // NOLINT wx default
                                   const wxString& name) {
  wxFIELD_TEXT_MARGIN = 5;
  shadow_ = wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW);
  highlight_ = wxSystemSettings::GetColour(wxSYS_COLOUR_3DHILIGHT);
  Init();
  Create(parent, winid, style, name);
  SetThemeEnabled(false);
}

void StatusBarGeneric::InitColours() {
  m_mediumShadowPen = wxPen(shadow_);
  m_hilightPen = wxPen(highlight_);
}

void StatusBarGeneric::DrawField(wxDC& dc, int i, int textHeight) {  // NOLINT
  wxRect rect;
  GetFieldRect(i, rect);

  if (rect.GetWidth() <= 0)
    return;  // happens when the status bar is shrunk in a very small area!

  int style = m_panes[i].GetStyle();
  if (style == wxSB_RAISED || style == wxSB_SUNKEN) {
    // Draw border
    // For wxSB_SUNKEN: paint a grey background, plus 3-d border (one black
    // rectangle)
    // Inside this, left and top sides (dark grey). Bottom and right (white).
    // Reverse it for wxSB_RAISED

    dc.SetPen((style == wxSB_RAISED) ? m_mediumShadowPen : m_hilightPen);

    // Right and bottom lines
    dc.DrawLine(rect.x + rect.width, rect.y, rect.x + rect.width,
                rect.y + rect.height);
    dc.DrawLine(rect.x + rect.width, rect.y + rect.height, rect.x,
                rect.y + rect.height);

    dc.SetPen((style == wxSB_RAISED) ? m_hilightPen : m_mediumShadowPen);

    // Left and top lines
    dc.DrawLine(rect.x, rect.y + rect.height, rect.x, rect.y);
    dc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y);
  } else if (style == wxSB_NORMAL) {
    if (i != 0) {
      dc.SetPen(m_mediumShadowPen);
      dc.DrawLine(rect.x, rect.y + rect.height, rect.x, rect.y);
    }
  }

  DrawFieldText(dc, rect, i, textHeight);
}

void StatusBarGeneric::DrawFieldText(wxDC& dc, const wxRect& rect, int i,
                                     int textHeight) {
  wxString text(GetStatusText(i));
  if (text.empty()) return;  // optimization

  int xpos = rect.x + wxFIELD_TEXT_MARGIN,
      maxWidth = rect.width - 2 * wxFIELD_TEXT_MARGIN,
      ypos = static_cast<int>(((rect.height - textHeight) / 2) + rect.y + 0.5);

  if (ShowsSizeGrip()) {
    // don't write text over the size grip:
    // NOTE: overloading DoGetClientSize() and GetClientAreaOrigin() wouldn't
    //       work because the adjustment needs to be done only when drawing
    //       the field text and not also when drawing the background, the
    //       size grip itself, etc
    if ((GetLayoutDirection() == wxLayout_RightToLeft && i == 0) ||
        (GetLayoutDirection() != wxLayout_RightToLeft &&
         i == static_cast<int>(m_panes.GetCount() - 1))) {
      const wxRect& gripRc = GetSizeGripRect();

      // NOTE: we don't need any special treatment wrt to the layout direction
      //       since DrawText() will automatically adjust the origin of the
      //       text accordingly to the layout in use

      maxWidth -= gripRc.width;
    }
  }

  // eventually ellipsize the text so that it fits the field width

  wxEllipsizeMode ellmode = static_cast<wxEllipsizeMode>(-1);
  if (HasFlag(wxSTB_ELLIPSIZE_START))
    ellmode = wxELLIPSIZE_START;
  else if (HasFlag(wxSTB_ELLIPSIZE_MIDDLE))
    ellmode = wxELLIPSIZE_MIDDLE;
  else if (HasFlag(wxSTB_ELLIPSIZE_END))
    ellmode = wxELLIPSIZE_END;

  if (ellmode == static_cast<wxEllipsizeMode>(-1)) {
    // if we have the wxSTB_SHOW_TIPS we must set the ellipsized flag even if
    // we don't ellipsize the text but just truncate it
    if (HasFlag(wxSTB_SHOW_TIPS))
      SetEllipsizedFlag(i, dc.GetTextExtent(text).GetWidth() > maxWidth);

    dc.SetClippingRegion(rect);
  } else {
    text = wxControl::Ellipsize(text, dc, ellmode, maxWidth,
                                wxELLIPSIZE_FLAGS_EXPAND_TABS);
    // Ellipsize() will do something only if necessary

    // update the ellipsization status for this pane; this is used later to
    // decide whether a tooltip should be shown or not for this pane
    // (if we have wxSTB_SHOW_TIPS)
    SetEllipsizedFlag(i, text != GetStatusText(i));
  }

#if defined(__WXGTK__) || defined(__WXMAC__)
  xpos++;
  ypos++;
#endif

  // draw the text
  dc.DrawText(text, xpos, ypos);

  if (ellmode == static_cast<wxEllipsizeMode>(-1)) dc.DestroyClippingRegion();
}
