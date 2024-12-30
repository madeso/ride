// Copyright (2015) Gustav

#include "ride/builtinthemes.h"

#include <string>
#include <optional>

typedef std::vector<ride::Theme> ThemeList;

ride::Theme* GetOrCreateTheme(ThemeList* themes, const std::string& name)
{
	for (ride::Theme& t: *themes)
	{
		if (t.name == name) return &t;
	}

	themes->push_back({});
	ride::Theme* temp = &(*themes->rbegin());
	temp->name = name;
	temp->can_remove = false;
	return temp;
}

ride::Color Color(int r, int g, int b)
{
	ride::Color c;

	c.r = r;
	c.g = g;
	c.b = b;

	return c;
}

ride::Color Color(int c)
{
	return Color(c, c, c);
}

ride::Style Style(
	std::optional<ride::Color> front,
	std::optional<ride::Color> back = std::nullopt,
	bool bold = false
)
{
	ride::Style style;

	if (front)
	{
		style.use_foreground = true;
		style.foreground = *front;
	}

	if (back)
	{
		style.use_background = true;
		style.background = *back;
	}

	if (bold)
	{
		style.use_bold = true;
		style.bold = true;
	}

	return style;
}

template<typename T>
T* New(const T& t)
{
	return new T(t);
}

ride::Indicator Indicator(const ride::Color& c)
{
	ride::Indicator ind;
	ind.foreground = c;
	return ind;
}

//////////////////////////////////////////////////////////////////////////

class BasicThemeBuilder
{
public:

	BasicThemeBuilder& set_selection_foreground(const ride::Color& c)
	{
		selection_foreground_ = c;
		return *this;
	}

	BasicThemeBuilder& set_selection_background(const ride::Color& c)
	{
		selection_background_ = c;
		return *this;
	}

	BasicThemeBuilder& set_front(const ride::Color& c)
	{
		front_ = c;
		return *this;
	}

	BasicThemeBuilder& set_bkg(const ride::Color& c)
	{
		bkg_ = c;
		return *this;
	}

	BasicThemeBuilder& set_fold_hi(const ride::Color& c)
	{
		fold_hi_ = c;
		return *this;
	}

	BasicThemeBuilder& set_fold_lo(const ride::Color& c)
	{
		fold_lo_ = c;
		return *this;
	}

	BasicThemeBuilder& set_selected_line(const ride::Color& c)
	{
		selected_line_ = c;
		return *this;
	}

	BasicThemeBuilder& set_comment(const ride::Color& c)
	{
		comment_ = c;
		return *this;
	}

	BasicThemeBuilder& set_keyword(const ride::Color& c)
	{
		keyword_ = c;
		return *this;
	}

	BasicThemeBuilder& set_error(const ride::Color& c)
	{
		error_ = c;
		return *this;
	}

	BasicThemeBuilder& set_error_front(const ride::Color& c)
	{
		error_front_ = c;
		return *this;
	}

	BasicThemeBuilder& set_warning(const ride::Color& c)
	{
		warning_ = c;
		return *this;
	}

	BasicThemeBuilder& set_warning_front(const ride::Color& c)
	{
		warning_front_ = c;
		return *this;
	}

	BasicThemeBuilder& set_search_hi(const ride::Color& c)
	{
		search_hi_ = c;
		return *this;
	}

	BasicThemeBuilder& set_select_hi(const ride::Color& c)
	{
		select_hi_ = c;
		return *this;
	}

	BasicThemeBuilder& set_edge_color(const ride::Color& c)
	{
		edge_color_ = c;
		return *this;
	}

	BasicThemeBuilder& set_caret_color(const ride::Color& c)
	{
		caret_color_ = c;
		return *this;
	}

	BasicThemeBuilder& set_switcher_background(const ride::Color& c)
	{
		switcher_background_ = c;
		return *this;
	}

	BasicThemeBuilder& set_ui_background(const ride::Color& c)
	{
		ui_background_ = c;
		return *this;
	}

	BasicThemeBuilder& set_title_front(const ride::Color& c)
	{
		title_front_ = c;
		return *this;
	}

	BasicThemeBuilder& set_title_bkg(const ride::Color& c)
	{
		title_bkg_ = c;
		return *this;
	}

	const ride::Color& selection_foreground()
	{
		return selection_foreground_;
	}

	const ride::Color& selection_background()
	{
		return selection_background_;
	}

	const ride::Color& front()
	{
		return front_;
	}

	const ride::Color& bkg()
	{
		return bkg_;
	}

	const ride::Color& fold_hi()
	{
		return fold_hi_;
	}

	const ride::Color& fold_lo()
	{
		return fold_lo_;
	}

	const ride::Color& selected_line()
	{
		return selected_line_;
	}

	const ride::Color& comment()
	{
		return comment_;
	}

	const ride::Color& keyword()
	{
		return keyword_;
	}

	const ride::Color& error()
	{
		return error_;
	}

	const ride::Color& error_front()
	{
		return error_front_;
	}

	const ride::Color& warning()
	{
		return warning_;
	}

	const ride::Color& warning_front()
	{
		return warning_front_;
	}

	const ride::Color& search_hi()
	{
		return search_hi_;
	}

	const ride::Color& select_hi()
	{
		return select_hi_;
	}

	const ride::Color& edge_color()
	{
		return edge_color_;
	}

	const ride::Color& caret_color()
	{
		return caret_color_;
	}

	void Setup(ride::FontsAndColors* colors)
	{
		colors->use_selection_background = true;
		colors->use_selection_foreground = true;
		colors->selection_foreground = selection_foreground_;
		colors->selection_background = selection_background_;

		colors->default_style = Style(front_, bkg_);
		colors->line_number_style = Style(std::nullopt, bkg_);
		colors->fold_margin_hi = fold_hi_;
		colors->fold_margin_low = fold_lo_;

		colors->selected_line = selected_line_;	 // yelw

		colors->style_comment = Style(comment_);
		colors->style_commentline = Style(comment_);
		colors->style_commentdoc = Style(comment_);
		colors->style_commentlinedoc = Style(comment_);
		colors->style_keyword = Style(keyword_, std::nullopt, true);

		colors->folderend_foreground = front_;
		colors->folderopenmid_foreground = front_;
		colors->foldermidtail_foreground = front_;
		colors->foldertail_foreground = front_;
		colors->foldersub_foreground = front_;
		colors->folder_foreground = front_;
		colors->folderopen_foreground = front_;

		colors->folderend_background = bkg_;
		colors->folderopenmid_background = bkg_;
		colors->foldermidtail_background = bkg_;
		colors->foldertail_background = bkg_;
		colors->foldersub_background = bkg_;
		colors->folder_background = bkg_;
		colors->folderopen_background = bkg_;

		colors->props_key = Style(keyword_);
		colors->props_section = Style(std::nullopt, std::nullopt, true);

		colors->indicator_error = Indicator(error_);
		colors->indicator_warning = Indicator(warning_);
		colors->indicator_search_highlight = Indicator(search_hi_);
		colors->indicator_select_highlight = Indicator(select_hi_);

		colors->annotation_error_style = Style(error_front_, error_);
		colors->annotation_warning_style = Style(warning_front_, warning_);

		colors->edgeColor = edge_color_;

		colors->caret_foreground = caret_color_;

		colors->switcher_background_color = switcher_background_;
		colors->switcher_dialog_color = front_;
		colors->switcher_base_color = switcher_background_;
		colors->switcher_selection_color = selected_line_;
		colors->switcher_selection_outline_color = selected_line_;
		colors->switcher_text_color = front_;

		colors->dock_background = ui_background_;
		colors->dock_sash = ui_background_;
		colors->dock_active_caption = title_bkg_;
		colors->dock_active_caption_gradient = title_bkg_;
		colors->dock_inactive_caption = title_bkg_;
		colors->dock_inactive_caption_gradient = title_bkg_;
		colors->dock_active_caption_text = title_front_;
		colors->dock_inactive_caption_text = title_front_;
		colors->dock_border = ui_background_;
		colors->dock_gripper = ui_background_;
		colors->tab_background = ui_background_;
		colors->tab_border = ui_background_;
		colors->tab_sash = front_;
		colors->tab_active_tab = bkg_;
		colors->tab_inactive_tab = bkg_;
		colors->tab_active_border = front_;
		colors->tab_inactive_border = front_;
		colors->tab_active_text = front_;
		colors->tab_inactive_text = front_;

		colors->statusbar_shadow = ui_background_;
		colors->statusbar_highlight = front_;
		colors->statusbar_foreground = front_;
		colors->statusbar_background = ui_background_;
	}

private:

	ride::Color selection_foreground_;
	ride::Color selection_background_;
	ride::Color front_;
	ride::Color bkg_;
	ride::Color fold_hi_;
	ride::Color fold_lo_;
	ride::Color selected_line_;
	ride::Color comment_;
	ride::Color keyword_;
	ride::Color error_;
	ride::Color error_front_;
	ride::Color warning_;
	ride::Color warning_front_;
	ride::Color search_hi_;
	ride::Color select_hi_;
	ride::Color edge_color_;
	ride::Color caret_color_;
	ride::Color switcher_background_;
	ride::Color ui_background_;
	ride::Color title_front_;
	ride::Color title_bkg_;
};

//////////////////////////////////////////////////////////////////////////

void SetupDefaultTheme(ride::FontsAndColors* colors)
{
	BasicThemeBuilder()
		.set_selection_foreground(Color(255))
		.set_selection_background(Color(0))
		.set_front(Color(0))
		.set_bkg(Color(224))
		.set_fold_hi(Color(192))
		.set_fold_lo(Color(224))
		.set_selected_line(Color(255, 255, 0))	// yellow
		.set_comment(Color(128, 64, 0))
		.set_keyword(Color(0, 0, 255))
		.set_error(Color(255, 60, 60))
		.set_error_front(Color(0))
		.set_warning(Color(0, 255, 0))
		.set_warning_front(Color(0))
		.set_search_hi(Color(200))
		.set_select_hi(Color(180))
		.set_edge_color(Color(0))
		.set_caret_color(Color(0))
		.set_switcher_background(Color(200))
		.set_ui_background(Color(180))
		.set_title_front(Color(200))
		.set_title_bkg(Color(100))
		.Setup(colors);
}

//////////////////////////////////////////////////////////////////////////

// solarized colors from http://ethanschoonover.com/solarized
namespace solarized
{
const ride::Color base03 = Color(0, 43, 54);
const ride::Color base02 = Color(7, 54, 66);
const ride::Color base01 = Color(88, 110, 117);
const ride::Color base00 = Color(101, 123, 131);
const ride::Color base0 = Color(131, 148, 150);
const ride::Color base1 = Color(147, 161, 161);
const ride::Color base2 = Color(238, 232, 213);
const ride::Color base3 = Color(253, 246, 227);
const ride::Color yellow = Color(181, 137, 0);
const ride::Color orange = Color(203, 75, 22);
const ride::Color red = Color(220, 50, 47);
const ride::Color magenta = Color(211, 54, 130);
const ride::Color violet = Color(108, 113, 196);
const ride::Color blue = Color(38, 139, 210);
const ride::Color cyan = Color(42, 161, 152);
const ride::Color green = Color(133, 153, 0);
}  // namespace solarized

void SetupSolarizedDarkTheme(ride::FontsAndColors* colors)
{
	using namespace solarized;	// NOLINT
	BasicThemeBuilder()
		.set_selection_foreground(base1)
		.set_selection_background(base00)
		.set_front(base0)
		.set_bkg(base03)
		.set_fold_hi(base02)
		.set_fold_lo(base02)
		.set_selected_line(base02)
		.set_comment(base01)
		.set_keyword(red)
		.set_error(base1)
		.set_error_front(base03)
		.set_warning(base00)
		.set_warning_front(base03)
		.set_search_hi(base01)
		.set_select_hi(base02)
		.set_edge_color(base01)
		.set_caret_color(base00)
		.set_switcher_background(base01)
		.set_ui_background(base01)
		.set_title_front(base03)
		.set_title_bkg(base1)
		.Setup(colors);
}

//////////////////////////////////////////////////////////////////////////

void AddBuiltInThemes(::ride::Settings* settings)
{
	ThemeList* themes = &settings->themes;

	ride::Theme* default_theme = GetOrCreateTheme(themes, "Ride (default)");
	SetupDefaultTheme(&default_theme->data);

	ride::Theme* solarized_dark_theme = GetOrCreateTheme(themes, "Solarized (dark)");
	SetupSolarizedDarkTheme(&solarized_dark_theme->data);

	if (false == settings->has_fonts_and_colors)
	{
		// if the current settings is missing the fonts and colors
		// apply the default theme
		settings->fonts_and_colors = ride::FontsAndColors(default_theme->data);
		settings->has_fonts_and_colors = true;
	}
}
