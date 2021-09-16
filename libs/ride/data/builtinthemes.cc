// Copyright (2015) Gustav

#include "ride/builtinthemes.h"

#include <string>

typedef google::protobuf::RepeatedPtrField<ride::Theme> ThemeList;

ride::Theme* GetOrCreateTheme(ThemeList* themes, const std::string& name) {
  for (ride::Theme& t : *themes) {
    if (t.name() == name) return &t;
  }

  ride::Theme* temp = themes->Add();
  temp->set_name(name);
  temp->set_can_remove(false);
  return temp;
}

ride::Color Color(google::protobuf::int32 r, google::protobuf::int32 g,
                  google::protobuf::int32 b) {
  ride::Color c;

  c.set_r(r);
  c.set_g(g);
  c.set_b(b);

  return c;
}

ride::Color Color(google::protobuf::int32 c) { return Color(c, c, c); }

ride::Style Style(ride::Color* front, ride::Color* back = nullptr,
                  bool bold = false) {
  ride::Style style;

  if (front) {
    style.set_use_foreground(true);
    style.set_allocated_foreground(front);
  }

  if (back) {
    style.set_use_background(true);
    style.set_allocated_background(back);
  }

  if (bold) {
    style.set_use_bold(true);
    style.set_bold(true);
  }

  return style;
}

template <typename T>
T* New(const T& t) {
  return new T(t);
}

ride::Indicator Indicator(const ride::Color& c) {
  ride::Indicator ind;
  ind.set_allocated_foreground(New(c));
  return ind;
}

//////////////////////////////////////////////////////////////////////////

class BasicThemeBuilder {
 public:
  BasicThemeBuilder& set_selection_foreground(const ride::Color& c) {
    selection_foreground_ = c;
    return *this;
  }
  BasicThemeBuilder& set_selection_background(const ride::Color& c) {
    selection_background_ = c;
    return *this;
  }
  BasicThemeBuilder& set_front(const ride::Color& c) {
    front_ = c;
    return *this;
  }
  BasicThemeBuilder& set_bkg(const ride::Color& c) {
    bkg_ = c;
    return *this;
  }
  BasicThemeBuilder& set_fold_hi(const ride::Color& c) {
    fold_hi_ = c;
    return *this;
  }
  BasicThemeBuilder& set_fold_lo(const ride::Color& c) {
    fold_lo_ = c;
    return *this;
  }
  BasicThemeBuilder& set_selected_line(const ride::Color& c) {
    selected_line_ = c;
    return *this;
  }
  BasicThemeBuilder& set_comment(const ride::Color& c) {
    comment_ = c;
    return *this;
  }
  BasicThemeBuilder& set_keyword(const ride::Color& c) {
    keyword_ = c;
    return *this;
  }
  BasicThemeBuilder& set_error(const ride::Color& c) {
    error_ = c;
    return *this;
  }
  BasicThemeBuilder& set_error_front(const ride::Color& c) {
    error_front_ = c;
    return *this;
  }
  BasicThemeBuilder& set_warning(const ride::Color& c) {
    warning_ = c;
    return *this;
  }
  BasicThemeBuilder& set_warning_front(const ride::Color& c) {
    warning_front_ = c;
    return *this;
  }
  BasicThemeBuilder& set_search_hi(const ride::Color& c) {
    search_hi_ = c;
    return *this;
  }
  BasicThemeBuilder& set_select_hi(const ride::Color& c) {
    select_hi_ = c;
    return *this;
  }
  BasicThemeBuilder& set_edge_color(const ride::Color& c) {
    edge_color_ = c;
    return *this;
  }
  BasicThemeBuilder& set_caret_color(const ride::Color& c) {
    caret_color_ = c;
    return *this;
  }

  BasicThemeBuilder& set_switcher_background(const ride::Color& c) {
    switcher_background_ = c;
    return *this;
  }

  BasicThemeBuilder& set_ui_background(const ride::Color& c) {
    ui_background_ = c;
    return *this;
  }

  BasicThemeBuilder& set_title_front(const ride::Color& c) {
    title_front_ = c;
    return *this;
  }
  BasicThemeBuilder& set_title_bkg(const ride::Color& c) {
    title_bkg_ = c;
    return *this;
  }

  const ride::Color& selection_foreground() { return selection_foreground_; }
  const ride::Color& selection_background() { return selection_background_; }
  const ride::Color& front() { return front_; }
  const ride::Color& bkg() { return bkg_; }
  const ride::Color& fold_hi() { return fold_hi_; }
  const ride::Color& fold_lo() { return fold_lo_; }
  const ride::Color& selected_line() { return selected_line_; }
  const ride::Color& comment() { return comment_; }
  const ride::Color& keyword() { return keyword_; }
  const ride::Color& error() { return error_; }
  const ride::Color& error_front() { return error_front_; }
  const ride::Color& warning() { return warning_; }
  const ride::Color& warning_front() { return warning_front_; }
  const ride::Color& search_hi() { return search_hi_; }
  const ride::Color& select_hi() { return select_hi_; }
  const ride::Color& edge_color() { return edge_color_; }
  const ride::Color& caret_color() { return caret_color_; }

  void Setup(ride::FontsAndColors* colors) {
    colors->set_use_selection_background(true);
    colors->set_use_selection_foreground(true);
    colors->set_allocated_selection_foreground(New(selection_foreground_));
    colors->set_allocated_selection_background(New(selection_background_));

    colors->set_allocated_default_style(New(Style(New(front_), New(bkg_))));
    colors->set_allocated_line_number_style(New(Style(nullptr, New(bkg_))));
    colors->set_allocated_fold_margin_hi(New(fold_hi_));
    colors->set_allocated_fold_margin_low(New(fold_lo_));

    colors->set_allocated_selected_line(New(selected_line_));  // yellow

    colors->set_allocated_style_comment(New(Style(New(comment_))));
    colors->set_allocated_style_commentline(New(Style(New(comment_))));
    colors->set_allocated_style_commentdoc(New(Style(New(comment_))));
    colors->set_allocated_style_commentlinedoc(New(Style(New(comment_))));
    colors->set_allocated_style_keyword(
        New(Style(New(keyword_), nullptr, true)));

    colors->set_allocated_folderend_foreground(New(front_));
    colors->set_allocated_folderopenmid_foreground(New(front_));
    colors->set_allocated_foldermidtail_foreground(New(front_));
    colors->set_allocated_foldertail_foreground(New(front_));
    colors->set_allocated_foldersub_foreground(New(front_));
    colors->set_allocated_folder_foreground(New(front_));
    colors->set_allocated_folderopen_foreground(New(front_));

    colors->set_allocated_folderend_background(New(bkg_));
    colors->set_allocated_folderopenmid_background(New(bkg_));
    colors->set_allocated_foldermidtail_background(New(bkg_));
    colors->set_allocated_foldertail_background(New(bkg_));
    colors->set_allocated_foldersub_background(New(bkg_));
    colors->set_allocated_folder_background(New(bkg_));
    colors->set_allocated_folderopen_background(New(bkg_));

    colors->set_allocated_props_key(New(Style(New(keyword_))));
    colors->set_allocated_props_section(New(Style(nullptr, nullptr, true)));

    colors->set_allocated_indicator_error(New(Indicator(error_)));
    colors->set_allocated_indicator_warning(New(Indicator(warning_)));
    colors->set_allocated_indicator_search_highlight(
        New(Indicator(search_hi_)));
    colors->set_allocated_indicator_select_highlight(
        New(Indicator(select_hi_)));

    colors->set_allocated_annotation_error_style(
        New(Style(New(error_front_), New(error_))));
    colors->set_allocated_annotation_warning_style(
        New(Style(New(warning_front_), New(warning_))));

    colors->set_allocated_edgecolor(New(edge_color_));

    colors->set_allocated_caret_foreground(New(caret_color_));

    colors->set_allocated_switcher_background_color(New(switcher_background_));
    colors->set_allocated_switcher_dialog_color(New(front_));
    colors->set_allocated_switcher_base_color(New(switcher_background_));
    colors->set_allocated_switcher_selection_color(New(selected_line_));
    colors->set_allocated_switcher_selection_outline_color(New(selected_line_));
    colors->set_allocated_switcher_text_color(New(front_));

    colors->set_allocated_dock_background(New(ui_background_));
    colors->set_allocated_dock_sash(New(ui_background_));
    colors->set_allocated_dock_active_caption(New(title_bkg_));
    colors->set_allocated_dock_active_caption_gradient(New(title_bkg_));
    colors->set_allocated_dock_inactive_caption(New(title_bkg_));
    colors->set_allocated_dock_inactive_caption_gradient(New(title_bkg_));
    colors->set_allocated_dock_active_caption_text(New(title_front_));
    colors->set_allocated_dock_inactive_caption_text(New(title_front_));
    colors->set_allocated_dock_border(New(ui_background_));
    colors->set_allocated_dock_gripper(New(ui_background_));
    colors->set_allocated_tab_background(New(ui_background_));
    colors->set_allocated_tab_border(New(ui_background_));
    colors->set_allocated_tab_sash(New(front_));
    colors->set_allocated_tab_active_tab(New(bkg_));
    colors->set_allocated_tab_inactive_tab(New(bkg_));
    colors->set_allocated_tab_active_border(New(front_));
    colors->set_allocated_tab_inactive_border(New(front_));
    colors->set_allocated_tab_active_text(New(front_));
    colors->set_allocated_tab_inactive_text(New(front_));

    colors->set_allocated_statusbar_shadow(New(ui_background_));
    colors->set_allocated_statusbar_highlight(New(front_));
    colors->set_allocated_statusbar_foreground(New(front_));
    colors->set_allocated_statusbar_background(New(ui_background_));
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

void SetupDefaultTheme(ride::FontsAndColors* colors) {
  BasicThemeBuilder()
      .set_selection_foreground(Color(255))
      .set_selection_background(Color(0))
      .set_front(Color(0))
      .set_bkg(Color(224))
      .set_fold_hi(Color(192))
      .set_fold_lo(Color(224))
      .set_selected_line(Color(255, 255, 0))  // yellow
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
namespace solarized {
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

void SetupSolarizedDarkTheme(ride::FontsAndColors* colors) {
  using namespace solarized;  // NOLINT
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

void AddBuiltInThemes(::ride::Settings* settings) {
  ThemeList* themes = settings->mutable_themes();

  ride::Theme* default_theme = GetOrCreateTheme(themes, "Ride (default)");
  SetupDefaultTheme(default_theme->mutable_data());

  ride::Theme* solarized_dark_theme =
      GetOrCreateTheme(themes, "Solarized (dark)");
  SetupSolarizedDarkTheme(solarized_dark_theme->mutable_data());

  if (false == settings->has_fonts_and_colors()) {
    // if the current settings is missing the fonts and colors
    // apply the default theme
    settings->set_allocated_fonts_and_colors(
        new ride::FontsAndColors(default_theme->data()));
  }
}
