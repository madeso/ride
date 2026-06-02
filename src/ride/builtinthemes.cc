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

	BasicThemeBuilder& set_selection_foreground(const wxString& c)
	{
		selection_foreground_ = c;
		return *this;
	}

	BasicThemeBuilder& set_selection_background(const wxString& c)
	{
		selection_background_ = c;
		return *this;
	}

	BasicThemeBuilder& set_front(const wxString& c)
	{
		front_ = c;
		return *this;
	}

	BasicThemeBuilder& set_bkg(const wxString& c)
	{
		bkg_ = c;
		return *this;
	}

	BasicThemeBuilder& set_fold_hi(const wxString& c)
	{
		fold_hi_ = c;
		return *this;
	}

	BasicThemeBuilder& set_fold_lo(const wxString& c)
	{
		fold_lo_ = c;
		return *this;
	}

	BasicThemeBuilder& set_selected_line(const wxString& c)
	{
		selected_line_ = c;
		return *this;
	}

	BasicThemeBuilder& set_error(const wxString& c)
	{
		error_ = c;
		return *this;
	}

	BasicThemeBuilder& set_error_front(const wxString& c)
	{
		error_front_ = c;
		return *this;
	}

	BasicThemeBuilder& set_warning(const wxString& c)
	{
		warning_ = c;
		return *this;
	}

	BasicThemeBuilder& set_warning_front(const wxString& c)
	{
		warning_front_ = c;
		return *this;
	}

	BasicThemeBuilder& set_search_hi(const wxString& c)
	{
		search_hi_ = c;
		return *this;
	}

	BasicThemeBuilder& set_select_hi(const wxString& c)
	{
		select_hi_ = c;
		return *this;
	}

	BasicThemeBuilder& set_edge_color(const wxString& c)
	{
		edge_color_ = c;
		return *this;
	}

	BasicThemeBuilder& set_caret_color(const wxString& c)
	{
		caret_color_ = c;
		return *this;
	}

	BasicThemeBuilder& set_switcher_background(const wxString& c)
	{
		switcher_background_ = c;
		return *this;
	}

	BasicThemeBuilder& set_ui_background(const wxString& c)
	{
		ui_background_ = c;
		return *this;
	}

	BasicThemeBuilder& set_title_front(const wxString& c)
	{
		title_front_ = c;
		return *this;
	}

	BasicThemeBuilder& set_title_bkg(const wxString& c)
	{
		title_bkg_ = c;
		return *this;
	}

	BasicThemeBuilder& color(const wxString& alias, const ride::Color& col)
	{
		colors->colors[alias] = col;
		return *this;
	}

	BasicThemeBuilder& alias(const wxString& from, const wxString& to)
	{
		colors->alias[from] = to;
		return *this;
	}

	void Setup() const
	{
		const wxString front_name = "front";
		const wxString bkg_name = "bkg";
		const wxString selected_line_name = "selected_line";
		const wxString error_name = "error";
		const wxString warning_name = "warning";
		const wxString switcher_background_name = "switcher_background";
		const wxString ui_background_name = "ui_background";
		const wxString title_front_name = "title_front";
		const wxString title_bkg_name = "title_bkg";

		colors->alias[front_name] = front_;
		colors->alias[bkg_name] = bkg_;
		colors->alias[selected_line_name] = selected_line_;
		colors->alias[error_name] = error_;
		colors->alias[warning_name] = warning_;
		colors->alias[switcher_background_name] = switcher_background_;
		colors->alias[ui_background_name] = ui_background_;
		colors->alias[title_front_name] = title_front_;
		colors->alias[title_bkg_name] = title_bkg_;

		colors->selection_foreground = selection_foreground_;
		colors->selection_background = selection_background_;
		colors->default_style = Style(front_name, bkg_name);
		colors->line_number_style = Style(std::nullopt, bkg_name);
		colors->fold_margin_hi = fold_hi_;
		colors->fold_margin_low = fold_lo_;
		colors->selected_line = selected_line_name;
		colors->explorer_foreground = front_name;
		colors->explorer_background = bkg_name;
		colors->marker_foreground = front_name;
		colors->marker_background = bkg_name;
		colors->indicator_error = Indicator(error_name);
		colors->indicator_warning = Indicator(warning_name);
		colors->indicator_search_highlight = Indicator(search_hi_);
		colors->indicator_select_highlight = Indicator(select_hi_);
		colors->annotation_error_style = Style(error_front_, error_name);
		colors->annotation_warning_style = Style(warning_front_, warning_name);
		colors->edgeColor = edge_color_;
		colors->caret_foreground = caret_color_;
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

	wxString selection_foreground_;
	wxString selection_background_;
	wxString front_;
	wxString bkg_;
	wxString fold_hi_;
	wxString fold_lo_;
	wxString selected_line_;
	wxString error_;
	wxString error_front_;
	wxString warning_;
	wxString warning_front_;
	wxString search_hi_;
	wxString select_hi_;
	wxString edge_color_;
	wxString caret_color_;
	wxString switcher_background_;
	wxString ui_background_;
	wxString title_front_;
	wxString title_bkg_;
};

//////////////////////////////////////////////////////////////////////////

void AddBuiltInThemes(::ride::Settings* settings)
{
	ThemeList* themes = &settings->themes;

	const std::string default_name = "Ride (default)";

	if (settings->current_theme.empty())
	{
		// if the current settings is missing the fonts and colors
		// apply the default theme
		GetOrCreateTheme(themes, default_name);
		settings->current_theme = default_name;
	}
}
