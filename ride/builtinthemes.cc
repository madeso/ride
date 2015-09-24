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
  return temp;
}

ride::Color MakeColor(google::protobuf::int32 r, google::protobuf::int32 g,
                      google::protobuf::int32 b) {
  ride::Color c;

  c.set_r(r);
  c.set_g(g);
  c.set_b(b);

  return c;
}

ride::Color* NewColor(google::protobuf::int32 r, google::protobuf::int32 g,
                      google::protobuf::int32 b) {
  return new ride::Color(MakeColor(r, g, b));
}

ride::Color* NewColor(google::protobuf::int32 c) { return NewColor(c, c, c); }

ride::Style* NewStyle(ride::Color* front, ride::Color* back = NULL,
                      bool bold = false) {
  ride::Style* style = new ride::Style();

  if (front) {
    style->set_use_foreground(true);
    style->set_allocated_foreground(front);
  }

  if (back) {
    style->set_use_background(true);
    style->set_allocated_background(back);
  }

  if (bold) {
    style->set_bold(true);
  }

  return style;
}

//////////////////////////////////////////////////////////////////////////

void SetupDefaultTemplate(ride::FontsAndColors* colors) {
  colors->set_use_selection_background(true);
  colors->set_use_selection_foreground(true);
  colors->set_allocated_selection_foreground(NewColor(255, 255, 255));
  colors->set_allocated_selection_background(NewColor(0, 0, 0));

  colors->set_allocated_default_style(NewStyle(NewColor(0), NewColor(211)));

  colors->set_allocated_selected_line(NewColor(255, 255, 0));  // yellow
  colors->set_allocated_style_comment(NewStyle(NewColor(0, 255, 0)));
  colors->set_allocated_style_commentline(NewStyle(NewColor(0, 255, 0)));
  colors->set_allocated_style_commentdoc(NewStyle(NewColor(0, 255, 0)));
  colors->set_allocated_style_commentlinedoc(NewStyle(NewColor(0, 255, 0)));
  colors->set_allocated_style_keyword(
      NewStyle(NewColor(0, 0, 255), NULL, true));
}

//////////////////////////////////////////////////////////////////////////

// solarized colors from http://ethanschoonover.com/solarized
const ride::Color solarized_base03 = MakeColor(0, 43, 54);
const ride::Color solarized_base02 = MakeColor(7, 54, 66);
const ride::Color solarized_base01 = MakeColor(88, 110, 117);
const ride::Color solarized_base00 = MakeColor(101, 123, 131);
const ride::Color solarized_base0 = MakeColor(131, 148, 150);
const ride::Color solarized_base1 = MakeColor(147, 161, 161);
const ride::Color solarized_base2 = MakeColor(238, 232, 213);
const ride::Color solarized_base3 = MakeColor(253, 246, 227);
const ride::Color solarized_yellow = MakeColor(181, 137, 0);
const ride::Color solarized_orange = MakeColor(203, 75, 22);
const ride::Color solarized_red = MakeColor(220, 50, 47);
const ride::Color solarized_magenta = MakeColor(211, 54, 130);
const ride::Color solarized_violet = MakeColor(108, 113, 196);
const ride::Color solarized_blue = MakeColor(38, 139, 210);
const ride::Color solarized_cyan = MakeColor(42, 161, 152);
const ride::Color solarized_green = MakeColor(133, 153, 0);

//////////////////////////////////////////////////////////////////////////

void AddBuiltInThemes(::ride::Settings* settings) {
  ThemeList* themes = settings->mutable_themes();

  const std::string RIDE_AUTHOR = "ride-default";

  ride::Theme* default_theme = GetOrCreateTheme(themes, "Ride (default)");
  default_theme->set_author(RIDE_AUTHOR);
  SetupDefaultTemplate(default_theme->mutable_data());

  if (false == settings->has_fonts_and_colors()) {
    // if the current settings is missing the fonts and colors
    // apply the default theme
    settings->set_allocated_fonts_and_colors(
      new ride::FontsAndColors(default_theme->data()));
  }
}
