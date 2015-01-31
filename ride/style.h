#ifndef RIDE_STYLE_H
#define RIDE_STYLE_H

#include <wx/wx.h>

class StyleLink {
public:
  StyleLink();

  int type; // the index to the style
  bool hasWords;
  wxString words;
};

class StyleInfo {
public:
  StyleInfo();

  int fontsize;
  wxString fontname;

  bool hasforeground;
  wxColour foreground;

  bool hasbackground;;
  wxColour background;

  bool bold;
  bool italic;
  bool underline;
  bool visible;
};


#endif  // RIDE_STYLE_H
