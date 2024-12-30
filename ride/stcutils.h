// Copyright (2015) Gustav

#ifndef RIDE_STCUTILS_H_
#define RIDE_STCUTILS_H_

#include <optional>
#include <ride/wx.h>
#include <wx/stc/stc.h>

#include "settings.proto.h"  // NOLINT this is how we include it

class Language;
class Project;

namespace ride {
class Settings;
}

enum {
  STYLE_LASTPREDEF = wxSTC_STYLE_LASTPREDEFINED,
  STYLE_ANNOTATION_WARNING,
  STYLE_ANNOTATION_ERROR
};

enum { ID_MARGIN_LINENUMBER = 0, ID_MARGIN_DIVIDER = 1, ID_MARGIN_FOLDING = 2 };

enum {
  ID_INDICATOR_ERROR,
  ID_INDICATOR_WARNING,
  ID_INDICATOR_SEARCH_HIGHLIGHT,
  ID_INDICATOR_SELECT_HIGHLIGHT
};

void SetStyle(wxStyledTextCtrl* text, int id, const std::optional<ride::Style>& style,
              bool force = false);

void SetupScintilla(wxStyledTextCtrl* text_ctrl, const ride::Settings& set,
                    Language* language, Project* project);
void SetupLineMargin(wxStyledTextCtrl* text_ctrl, const ride::Settings& set);
void SetupScintillaCurrentLine(wxStyledTextCtrl* text_ctrl,
                               const ride::Settings& set);
void SetupScintillaDefaultStyles(wxStyledTextCtrl* text_ctrl,
                                 const ride::Settings& set);

#endif  // RIDE_STCUTILS_H_
