#ifndef RIDE_LANGUAGE_H
#define RIDE_LANGUAGE_H

#include <wx/wx.h>
#include "ride/style.h"
#include <vector>

enum FoldType {
  FOLD_NONE = 0,
  mySTC_FOLD_COMMENT = 1 << 1,
  mySTC_FOLD_COMPACT = 1 << 2,
  mySTC_FOLD_PREPROC = 1 << 3,
  mySTC_FOLD_HTML = 1 << 4,
  mySTC_FOLD_HTMLPREP = 1 << 5,
  mySTC_FOLD_COMMENTPY = 1 << 6,
  mySTC_FOLD_QUOTESPY = 1 << 7
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