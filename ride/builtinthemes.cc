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

ride::Style Style(ride::Color* front, ride::Color* back = NULL,
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
  BasicThemeBuilder& set_warning(const ride::Color& c) {
    warning_ = c;
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
  const ride::Color& warning() { return warning_; }
  const ride::Color& search_hi() { return search_hi_; }
  const ride::Color& select_hi() { return select_hi_; }

  void Setup(ride::FontsAndColors* colors) {
    colors->set_use_selection_background(true);
    colors->set_use_selection_foreground(true);
    colors->set_allocated_selection_foreground(New(selection_foreground_));
    colors->set_allocated_selection_background(New(selection_background_));

    colors->set_allocated_default_style(New(Style(New(front_), New(bkg_))));
    colors->set_allocated_line_number_style(New(Style(NULL, New(bkg_))));
    colors->set_allocated_fold_margin_hi(New(fold_hi_));
    colors->set_allocated_fold_margin_low(New(fold_lo_));

    colors->set_allocated_selected_line(New(selected_line_));  // yellow

    colors->set_allocated_style_comment(New(Style(New(comment_))));
    colors->set_allocated_style_commentline(New(Style(New(comment_))));
    colors->set_allocated_style_commentdoc(New(Style(New(comment_))));
    colors->set_allocated_style_commentlinedoc(New(Style(New(comment_))));
    colors->set_allocated_style_keyword(New(Style(New(keyword_), NULL, true)));

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
    colors->set_allocated_props_section(New(Style(NULL, NULL, true)));

    colors->set_allocated_indicator_error(New(Indicator(error_)));
    colors->set_allocated_indicator_warning(New(Indicator(warning_)));
    colors->set_allocated_indicator_search_highlight(
        New(Indicator(search_hi_)));
    colors->set_allocated_indicator_select_highlight(
        New(Indicator(select_hi_)));

    colors->set_allocated_annotation_error_style(New(Style(NULL, New(error_))));
    colors->set_allocated_annotation_warning_style(
        New(Style(NULL, New(warning_))));
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
  ride::Color warning_;
  ride::Color search_hi_;
  ride::Color select_hi_;
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
      .set_warning(Color(0, 255, 0))
      .set_search_hi(Color(200))
      .set_select_hi(Color(180))
      .Setup(colors);
}

//////////////////////////////////////////////////////////////////////////

// solarized colors from http://ethanschoonover.com/solarized
const ride::Color solarized_base03 = Color(0, 43, 54);
const ride::Color solarized_base02 = Color(7, 54, 66);
const ride::Color solarized_base01 = Color(88, 110, 117);
const ride::Color solarized_base00 = Color(101, 123, 131);
const ride::Color solarized_base0 = Color(131, 148, 150);
const ride::Color solarized_base1 = Color(147, 161, 161);
const ride::Color solarized_base2 = Color(238, 232, 213);
const ride::Color solarized_base3 = Color(253, 246, 227);
const ride::Color solarized_yellow = Color(181, 137, 0);
const ride::Color solarized_orange = Color(203, 75, 22);
const ride::Color solarized_red = Color(220, 50, 47);
const ride::Color solarized_magenta = Color(211, 54, 130);
const ride::Color solarized_violet = Color(108, 113, 196);
const ride::Color solarized_blue = Color(38, 139, 210);
const ride::Color solarized_cyan = Color(42, 161, 152);
const ride::Color solarized_green = Color(133, 153, 0);

void SetupSolarizedDarkTheme(ride::FontsAndColors* colors) {
  BasicThemeBuilder()
      .set_selection_foreground(solarized_base1)
      .set_selection_background(solarized_base00)
      .set_front(solarized_base0)
      .set_bkg(solarized_base03)
      .set_fold_hi(solarized_base02)
      .set_fold_lo(solarized_base02)
      .set_selected_line(solarized_base02)
      .set_comment(solarized_base01)
      .set_keyword(solarized_base1)
      .set_error(solarized_red)
      .set_warning(solarized_base00)
      .set_search_hi(solarized_base01)
      .set_select_hi(solarized_base02)
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
