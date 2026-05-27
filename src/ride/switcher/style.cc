#include "style.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

namespace switcher
{

Style::Style()
	: row_count_(12)
	, text_margin_x_(4)
	, text_margin_y_(2)
	, background_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE))
	, text_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT))
	, selection_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT))
	, selection_outline_color_(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT))
	, item_font_(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT))
	, x_margin_(6)
	, y_margin_(6)
	, row_spacing_(1)
	, col_spacing_(1)
	, item_maxwidth_(300)
	, item_maxheight_(40)
	, dlg_main_border_(8)
	, dlg_item_border_(3)
	, dialog_color_(*wxBLACK)
{
}

}  // namespace switcher
