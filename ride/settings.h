#ifndef RIDE_SETTINGS_H
#define RIDE_SETTINGS_H

#include <wx/wx.h>

namespace EdgeStyle {
  enum Type {
    NONE, LINE, BACKGROUND
  };
}

namespace ViewWhitespace {
  enum Type {
    HIDDEN, ALWAYS, AFTER_IDENT
  };
}

namespace WrapMode {
  enum Type {
    NONE, CHAR, WORD
  };
}

struct FoldFlags {
  FoldFlags();

  bool LINEBEFORE_EXPANDED;
  bool LINEBEFORE_CONTRACTED;
  bool LINEAFTER_EXPANDED;
  bool LINEAFTER_CONTRACTED;
  bool LEVELNUMBERS;
};

class Style {
public:
  Style(const wxFont& font, const wxColor& foreground=wxColor(0,0,0), const wxColor& background = wxNullColour);
  wxFont font;
  wxColor foreground;
  wxColor background;
};

class Settings {
public:
  Settings();

  bool lineNumberEnable;
  bool foldEnable;
  bool displayEOLEnable;
  bool indentGuideEnable;
  ViewWhitespace::Type whitespace;
  WrapMode::Type wordWrap;

  EdgeStyle::Type edgeStyle;
  wxColor edgeColor;
  int edgeColumn;

  int tabWidth;
  bool useTabs;
  bool tabIndents;
  bool backspaceUnindents;

  FoldFlags foldflags;

  bool foldComment;
  bool foldCompact;
  bool foldPreproc;

  bool     styling_within_preprocessor;     //  For C++ code, determines whether all preprocessor code is styled in the preprocessor style (0, the default) or only from the initial # to the end of the command word(1).
  bool     lexer_cpp_allow_dollars;         // Set to 0 to disallow the '$' character in identifiers with the cpp lexer.
  bool     lexer_cpp_track_preprocessor;    //  Set to 1 to interpret #if/#else/#endif to grey out code that is not active.
  bool     lexer_cpp_update_preprocessor;   //  Set to 1 to update preprocessor definitions when #define found.
  bool     lexer_cpp_triplequoted_strings;  //  Set to 1 to enable highlighting of triple-quoted strings.
  bool     lexer_cpp_hashquoted_strings;    //  Set to 1 to enable highlighting of hash-quoted strings.
  bool     fold_cpp_syntax_based;           //  Set this property to 0 to disable syntax based folding.
  bool     fold_cpp_comment_multiline;      //  Set this property to 0 to disable folding multi-line comments when fold.comment=1.
  bool     fold_cpp_comment_explicit;       //  Set this property to 0 to disable folding explicit fold points when fold.comment=1.
  bool     fold_cpp_explicit_anywhere;      //  Set this property to 1 to enable explicit fold points anywhere, not just in line comments.
  bool     fold_at_else;                    //  This option enables C++ folding on a "} else {" line of an if statement.
};

#endif  // RIDE_SETTINGS_H