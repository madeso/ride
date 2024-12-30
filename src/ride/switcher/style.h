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

	void set_row_count(int rows);
	int row_count() const;

	void set_text_margin_x(int margin);

	int text_margin_x() const;

	void set_text_margin_y(int margin);

	int text_margin_y() const;

	void set_background_color(const wxColour& colour);
	const wxColour& background_color() const;

	void set_text_color(const wxColour& colour);
	const wxColour& text_color() const;

	void set_selection_color(const wxColour& colour);
	const wxColour& selection_color() const;

	void set_selection_outline_color(const wxColour& colour);
	const wxColour& selection_outline_color() const;

	void set_item_font(const wxFont& font);
	const wxFont& item_font() const;

	int x_margin() const;
	int y_margin() const;

	void set_x_margin(int x);
	void set_y_margin(int y);

	int row_spacing() const;
	int col_spacing() const;

	void set_row_spacing(int spacing);
	void set_col_spacing(int spacing);

	int item_maxwidth() const;
	int item_maxheight() const;

	void set_item_maxwidth(int max);
	void set_item_maxheight(int max);

	int dlg_main_border() const;
	int dlg_item_border() const;

	void set_dlg_main_border(int border);
	void set_dlg_item_border(int border);

	wxColor dialog_color() const;
	void set_dialog_color(wxColor color);

	wxColor base_color() const;
	void set_base_color(wxColor color);

	int min_width() const;
	int min_height() const;

	void set_min_width(int w);
	void set_min_height(int h);

protected:

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
