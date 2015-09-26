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

void SetupDefaultTemplate(ride::FontsAndColors* colors) {
  colors->set_use_selection_background(true);
  colors->set_use_selection_foreground(true);
  colors->set_allocated_selection_foreground(New(Color(255, 255, 255)));
  colors->set_allocated_selection_background(New(Color(0, 0, 0)));

  ride::Color bkg = Color(224);

  colors->set_allocated_default_style(New(Style(New(Color(0)), New(bkg))));
  colors->set_allocated_line_number_style(New(Style(NULL, New(bkg))));
  colors->set_allocated_fold_margin_hi(New(Color(192)));
  colors->set_allocated_fold_margin_low(New(Color(224)));

  colors->set_allocated_selected_line(New(Color(255, 255, 0)));  // yellow

  ride::Color comment = Color(128, 64, 0);
  colors->set_allocated_style_comment(New(Style(New(comment))));
  colors->set_allocated_style_commentline(New(Style(New(comment))));
  colors->set_allocated_style_commentdoc(New(Style(New(comment))));
  colors->set_allocated_style_commentlinedoc(New(Style(New(comment))));
  colors->set_allocated_style_keyword(
      New(Style(New(Color(0, 0, 255)), NULL, true)));

  colors->set_allocated_folderend_background(New(bkg));
  colors->set_allocated_folderopenmid_background(New(bkg));
  colors->set_allocated_foldermidtail_background(New(bkg));
  colors->set_allocated_foldertail_background(New(bkg));
  colors->set_allocated_foldersub_background(New(bkg));
  colors->set_allocated_folder_background(New(bkg));
  colors->set_allocated_folderopen_background(New(bkg));

  colors->set_allocated_props_key(New(Style(New(Color(128, 128, 255)))));
  colors->set_allocated_props_section(New(Style(NULL, NULL, true)));

  const ride::Color error = Color(255, 60, 60);
  const ride::Color warning = Color(0, 255, 0);

  colors->set_allocated_indicator_error(New(Indicator(error)));
  colors->set_allocated_indicator_warning(New(Indicator(warning)));
  colors->set_allocated_indicator_search_highlight(New(Indicator(Color(200))));
  colors->set_allocated_indicator_select_highlight(New(Indicator(Color(180))));

  colors->set_allocated_annotation_error_style(New(Style(NULL, New(error))));
  colors->set_allocated_annotation_warning_style(
      New(Style(NULL, New(warning))));
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

//////////////////////////////////////////////////////////////////////////

void AddBuiltInThemes(::ride::Settings* settings) {
  ThemeList* themes = settings->mutable_themes();

  ride::Theme* default_theme = GetOrCreateTheme(themes, "Ride (default)");
  SetupDefaultTemplate(default_theme->mutable_data());

  if (false == settings->has_fonts_and_colors()) {
    // if the current settings is missing the fonts and colors
    // apply the default theme
    settings->set_allocated_fonts_and_colors(
        new ride::FontsAndColors(default_theme->data()));
  }
}
