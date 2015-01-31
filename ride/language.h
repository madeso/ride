#ifndef RIDE_LANGUAGE_H
#define RIDE_LANGUAGE_H

#include <wx/wx.h>
#include "ride/style.h"
#include <vector>

enum FoldType {
  FOLD_NONE = 0,
  mySTC_FOLD_COMMENT,
  mySTC_FOLD_COMPACT,
  mySTC_FOLD_PREPROC,
  mySTC_FOLD_HTML,
  mySTC_FOLD_HTMLPREP,
  mySTC_FOLD_COMMENTPY,
  mySTC_FOLD_QUOTESPY
};

class LanguageInfo {
public:
  LanguageInfo();

  wxString filepattern;
  wxString name;
  int lexer;

  FoldType folds;

  std::vector<StyleLink> styles;
};

#endif  // RIDE_LANGUAGE_H