#ifndef RIDE_SWITCHERSTYLE_H_
#define RIDE_SWITCHERSTYLE_H_

#include <ride/wx.h>
#include <wx/html/htmlwin.h>

#include <vector>

#include "item.h"
#include "group.h"

namespace switcher
{

class Style
{
public:
	Style();

	int row_count_;
	int text_margin_x_;
	int text_margin_y_;

	wxColour background_color_;
	wxColour text_color_;
	wxColour selection_color_;
	wxColour selection_outline_color_;

	wxFont item_font_;

	int x_margin_;
	int y_margin_;

	int row_spacing_;
	int col_spacing_;

	int item_maxwidth_;
	int item_maxheight_;

	int dlg_main_border_;
	int dlg_item_border_;

	wxColor dialog_color_;
	wxColor base_color_;

	int min_width_;
	int min_height_;
};

}  //  namespace switcher

#endif	// RIDE_SWITCHERSTYLE_H_
