#include "ride/settings.h"

FoldFlags::FoldFlags()
  : LINEBEFORE_EXPANDED(false)
  , LINEBEFORE_CONTRACTED(false)
  , LINEAFTER_EXPANDED(false)
  , LINEAFTER_CONTRACTED(false)
  , LEVELNUMBERS(false)
{}

Style::Style(const wxFont& font, const wxColor& foreground, const wxColor& background)
  : font(font)
  , foreground(foreground)
  , background(background)
{}


Settings::Settings()
  : lineNumberEnable(true)
  , foldEnable(true)
  , displayEOLEnable(false)
  , indentGuideEnable(true)
  , whitespace(ViewWhitespace::HIDDEN)
  , wordWrap(WrapMode::NONE)
  , edgeStyle(EdgeStyle::NONE)
  , edgeColor(0,0,0)
  , edgeColumn(80)
  , tabWidth(4)
  , useTabs(false)
  , tabIndents(true)
  , backspaceUnindents(true)
  , foldComment(true)
  , foldCompact(true)
  , foldPreproc(true)
{}
