#include "ride/builtinthemes.h"

#include <string>
#include <optional>

typedef std::vector<ride::Theme> ThemeList;

std::pair<ride::Theme*, bool> GetOrCreateTheme(ThemeList* themes, const std::string& name)
{
	for (ride::Theme& t: *themes)
	{
		if (t.name == name) return {&t, false};
	}

	themes->push_back({});
	ride::Theme* temp = &(*themes->rbegin());
	temp->name = name;
	temp->can_remove = false;
	return {temp, true};
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
	std::optional<wxString> front,
	std::optional<wxString> back = std::nullopt,
	bool bold = false
)
{
	ride::Style style;

	style.foreground = front;
	style.background = back;
	style.bold = bold;

	return style;
}

template<typename T>
T* New(const T& t)
{
	return new T(t);
}

ride::Indicator Indicator(const wxString& c)
{
	ride::Indicator ind;
	ind.foreground = c;
	return ind;
}

//////////////////////////////////////////////////////////////////////////

class BasicThemeBuilder
{
public:
	ride::FontsAndColors* colors;

	explicit BasicThemeBuilder(ride::FontsAndColors* c)
		: colors(c)
	{
	}

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

	void Setup()
	{
		const wxString selection_foreground_name = "selection_foreground";
		const wxString selection_background_name = "selection_background";
		const wxString front_name = "front";
		const wxString bkg_name = "bkg";
		const wxString fold_hi_name = "fold_hi";
		const wxString fold_lo_name = "fold_lo";
		const wxString selected_line_name = "selected_line";
		const wxString comment_name = "comment";
		const wxString keyword_name = "keyword";
		const wxString error_name = "error";
		const wxString error_front_name = "error_front";
		const wxString warning_name = "warning";
		const wxString warning_front_name = "warning_front";
		const wxString search_hi_name = "search_hi";
		const wxString select_hi_name = "select_hi";
		const wxString edge_color_name = "edge_color";
		const wxString caret_color_name = "caret_color";
		const wxString switcher_background_name = "switcher_background";
		const wxString ui_background_name = "ui_background";
		const wxString title_front_name = "title_front";
		const wxString title_bkg_name = "title_bkg";

		colors->colors[selection_foreground_name] = selection_foreground_;
		colors->colors[selection_background_name] = selection_background_;
		colors->colors[front_name] = front_;
		colors->colors[bkg_name] = bkg_;
		colors->colors[fold_hi_name] = fold_hi_;
		colors->colors[fold_lo_name] = fold_lo_;
		colors->colors[selected_line_name] = selected_line_;
		colors->colors[comment_name] = comment_;
		colors->colors[keyword_name] = keyword_;
		colors->colors[error_name] = error_;
		colors->colors[error_front_name] = error_front_;
		colors->colors[warning_name] = warning_;
		colors->colors[warning_front_name] = warning_front_;
		colors->colors[search_hi_name] = search_hi_;
		colors->colors[select_hi_name] = select_hi_;
		colors->colors[edge_color_name] = edge_color_;
		colors->colors[caret_color_name] = caret_color_;
		colors->colors[switcher_background_name] = switcher_background_;
		colors->colors[ui_background_name] = ui_background_;
		colors->colors[title_front_name] = title_front_;
		colors->colors[title_bkg_name] = title_bkg_;

		colors->selection_foreground = selection_foreground_name;
		colors->selection_background = selection_background_name;
		colors->default_style = Style(front_name, bkg_name);
		colors->line_number_style = Style(std::nullopt, bkg_name);
		colors->fold_margin_hi = fold_hi_name;
		colors->fold_margin_low = fold_lo_name;
		colors->selected_line = selected_line_name;
		colors->explorer_foreground = front_name;
		colors->explorer_background = bkg_name;
		colors->marker_foreground = front_name;
		colors->marker_background = bkg_name;
		colors->indicator_error = Indicator(error_name);
		colors->indicator_warning = Indicator(warning_name);
		colors->indicator_search_highlight = Indicator(search_hi_name);
		colors->indicator_select_highlight = Indicator(select_hi_name);
		colors->annotation_error_style = Style(error_front_name, error_name);
		colors->annotation_warning_style = Style(warning_front_name, warning_name);
		colors->edgeColor = edge_color_name;
		colors->caret_foreground = caret_color_name;
		colors->switcher_background_color = switcher_background_name;
		colors->switcher_dialog_color = front_name;
		colors->switcher_base_color = switcher_background_name;
		colors->switcher_selection_color = selected_line_name;
		colors->switcher_selection_outline_color = selected_line_name;
		colors->switcher_text_color = front_name;
		colors->dock_background = ui_background_name;
		colors->dock_sash = ui_background_name;
		colors->dock_active_caption = title_bkg_name;
		colors->dock_active_caption_gradient = title_bkg_name;
		colors->dock_inactive_caption = title_bkg_name;
		colors->dock_inactive_caption_gradient = title_bkg_name;
		colors->dock_active_caption_text = title_front_name;
		colors->dock_inactive_caption_text = title_front_name;
		colors->dock_border = ui_background_name;
		colors->dock_gripper = ui_background_name;
		colors->tab_background = ui_background_name;
		colors->tab_border = ui_background_name;
		colors->tab_sash = front_name;
		colors->tab_active_tab = bkg_name;
		colors->tab_inactive_tab = bkg_name;
		colors->tab_active_border = front_name;
		colors->tab_inactive_border = front_name;
		colors->tab_active_text = front_name;
		colors->tab_inactive_text = front_name;
		colors->statusbar_shadow = ui_background_name;
		colors->statusbar_highlight = front_name;
		colors->statusbar_foreground = front_name;
		colors->statusbar_background = ui_background_name;
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
	BasicThemeBuilder(colors)
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
		.Setup();
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
	BasicThemeBuilder(colors)
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
		.Setup();
}

//////////////////////////////////////////////////////////////////////////

void AddBuiltInThemes(::ride::Settings* settings)
{
	ThemeList* themes = &settings->themes;

	const std::string default_name = "Ride (default)";
	const std::string solarized_dark_name = "Solarized (dark)";

	{
		auto [default_theme, created] = GetOrCreateTheme(themes, default_name);
		if (created)
		{
			SetupDefaultTheme(&default_theme->data);
		}
	}

	{
		auto [solarized_dark_theme, created] = GetOrCreateTheme(themes, solarized_dark_name);
		if (created)
		{
			SetupSolarizedDarkTheme(&solarized_dark_theme->data);
		}
	}

	if (settings->current_theme.empty())
	{
		// if the current settings is missing the fonts and colors
		// apply the default theme
		GetOrCreateTheme(themes, default_name);
		settings->current_theme = default_name;
	}
}
