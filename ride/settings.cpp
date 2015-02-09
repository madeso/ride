#include "localwx.h"
#include "ride/settings.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <fstream>

wxFileName GetConfigFile() {
  // wxStandardPaths::Get().UseAppInfo(wxStandardPaths::AppInfo_AppName | wxStandardPaths::AppInfo_VendorName);
  wxFileName folder(wxStandardPaths::Get().GetUserDataDir(), "settings", "data");
  return folder;
}

void LoadSettings(::ride::Settings& settings) {
  const wxFileName confPath = GetConfigFile();
  const wxString path = confPath.GetFullPath();
  if (confPath.IsFileReadable()) {
    std::fstream input(path.c_str().AsChar(), std::ios::in | std::ios::binary);
    const bool parse_result = settings.ParseFromIstream(&input);
  }
}

bool SaveSettings(::ride::Settings& settings) {
  const wxFileName confPath = GetConfigFile();
  const bool create_result = confPath.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
  if (confPath.FileExists() && confPath.IsFileWritable() == false) {
    // abort if the file exist and isn't writable
    return false;
  }
  const wxString path = confPath.GetFullPath();
  std::fstream input(path.c_str().AsChar(), std::ios::out | std::ios::trunc | std::ios::binary);
  return settings.SerializeToOstream(&input);
}

wxColor C(const ride::Color& c) {
  return wxColor(c.r(), c.g(), c.b());
}

ride::Color C(const wxColor& c) {
  ride::Color r;
  r.set_r(c.Red());
  r.set_g(c.Green());
  r.set_b(c.Blue());
  return r;
}

/*
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
  , styling_within_preprocessor(false)
  , lexer_cpp_allow_dollars(false)
  , lexer_cpp_track_preprocessor(false)
  , lexer_cpp_update_preprocessor(false)
  , lexer_cpp_triplequoted_strings(false)
  , lexer_cpp_hashquoted_strings(false)
  , fold_cpp_syntax_based(true)
  , fold_cpp_comment_multiline(true)
  , fold_cpp_comment_explicit(true)
  , fold_cpp_explicit_anywhere(false)
  , fold_at_else(true)

{}


void Settings::load() {
}

void Settings::save() {
}

*/