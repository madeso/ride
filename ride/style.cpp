#include "ride/style.h"

StyleLink::StyleLink()
  :type(-1)
  ,hasWords(false)
  ,words("")
{
}

StyleLink::StyleLink(int type) : type(type), hasWords(false), words("") {
}

StyleLink::StyleLink(int type, const wxString& words) : type(type), hasWords(true), words(words) {
}

StyleInfo::StyleInfo()
:fontsize(10)
, fontname("Arial")
, hasforeground(false)
, foreground(0,0,0)
, hasbackground(false)
, background(0,0,0)
, bold(false)
, italic(false)
, underline(false)
, visible(true)
{
}

StyleInfo::StyleInfo(const wxString& fontname, int fontsize, wxColour foreground, bool bold)
  :fontsize(fontsize)
  , fontname(fontname)
  , hasforeground(true)
  , foreground(foreground)
  , hasbackground(false)
  , background(0, 0, 0)
  , bold(bold)
  , italic(false)
  , underline(false)
  , visible(true)
{
}
