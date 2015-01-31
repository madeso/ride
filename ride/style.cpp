#include "ride/style.h"

StyleLink::StyleLink()
  :type(-1)
  ,hasWords(false)
  ,words("")
{
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
