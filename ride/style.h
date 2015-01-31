#ifndef RIDE_STYLE_H
#define RIDE_STYLE_H

#include <wx/wx.h>

class StyleLink {
public:
  StyleLink();
  StyleLink(int type);
  StyleLink(int type, const wxString& words);

  int type; // the index to the style
  bool hasWords;
  wxString words;
};

class StyleInfo {
public:
  StyleInfo();
  StyleInfo(const wxString& fontname, int fontsize, wxColour foreground, bool bold);

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
