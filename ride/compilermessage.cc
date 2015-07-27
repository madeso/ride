#include "ride/compilermessage.h"
#include <wx/regex.h>
#include <wx/filename.h>

CompilerMessage::CompilerMessage()
    : file_(""),
      start_line_(-1),
      start_index_(-1),
      end_line_(-1),
      end_index_(-1),
      type_(CompilerMessage::TYPE_UNKNOWN),
      message_("") {}

CompilerMessage::CompilerMessage(const wxString& file, int start_line,
                                 int start_index, int end_line, int end_index,
                                 CompilerMessage::Type type, wxString message)
    : file_(file),
      start_line_(start_line),
      start_index_(start_index),
      end_line_(end_line),
      end_index_(end_index),
      type_(type),
      message_(message) {}

namespace regex {
const wxString WS = "[ \\t]*";
const wxString SEP = "[/\\\\]";
const wxString DRIVE = "(?:[a-zA-Z]\\:)?";
const wxString NAME = "[a-zA-Z\\.0-9\\-_]+";
const wxString FILE =
    "(" + DRIVE + SEP + "?(?:" + NAME + SEP + ")*" + NAME + "?)";
// const wxString FILE =
// "(?:[\\w]\\:|\\\\)(\\\\[a-z_\\-\\s0-9\\.]+)+\\.[a-zA-Z]+";
const wxString INT = "([0-9]+)";
const wxString ID = "([a-zA-Z]+)";
const wxString TEXT = "(.+)";

// C:\Users\gustav\WorkingFolder\librust\src\rng.rs:16 : 1 : 21 : 2 warning :
// type could implement `Copy`; consider adding `impl Copy`,
// #[warn(missing_copy_implementations)] on by default
const wxString COMPLEX_REGEX_OUTPUT =
    "^" + FILE + "\\:" + INT + WS + "\\:" + WS + INT + WS + "\\:" + WS + INT +
    WS + "\\:" + WS + INT + WS + ID + WS + "\\:" + WS + TEXT + "$";

// C:\Users\gustav\WorkingFolder\librust\src\crc32.rs:4 pub struct Crc32 {
const wxString SIMPLE_REGEX_OUTPUT = "^" + FILE + "\\:" + INT + WS + TEXT + "$";

// settings.proto:5:9: Expected "]".
const wxString PROTOC_REGEX_OUTPUT =
    "^" + FILE + "\\:" + INT + ":" + INT + ":" + WS + TEXT + "$";
}

namespace {
struct FileTest {
  FileTest() {
    wxRegEx file("^" + regex::FILE + "$", wxRE_ADVANCED);
    assert(file.IsValid() && "File regex failed to compile");

    assert(file.Matches("C:\\test.txt"));
    assert(file.Matches("C:/test.txt"));
    assert(file.Matches("C:/test/lala.txt"));
    assert(file.Matches("/test/lala.txt"));
    assert(file.Matches("test/lala.txt"));
    assert(file.Matches("lala.txt"));
  }
} test;
}

const wxRegEx& ComplexRegexOutput() {
  static wxRegEx ret(regex::COMPLEX_REGEX_OUTPUT, wxRE_ADVANCED);
  assert(ret.IsValid() && "Complex output regex failed to compile");
  return ret;
}

const wxRegEx& RegexOutputRelated() {
  static wxRegEx ret(regex::SIMPLE_REGEX_OUTPUT, wxRE_ADVANCED);
  assert(ret.IsValid() && "Simple output regex failed to compile");
  return ret;
}

const wxRegEx& ProtocRegexOutput() {
  static wxRegEx ret(regex::PROTOC_REGEX_OUTPUT, wxRE_ADVANCED);
  assert(ret.IsValid() && "Protoc regex failed to compile");
  return ret;
}

CompilerMessage::Type ParseCMT(const wxString& str) {
  if (str == "warning")
    return CompilerMessage::TYPE_WARNING;
  else if (str == "error")
    return CompilerMessage::TYPE_ERROR;
  else if (str == "note")
    return CompilerMessage::TYPE_NOTE;
  else
    return CompilerMessage::TYPE_UNKNOWN;
}

wxString CleanupFilePath(const wxString& root, const wxString& path) {
  wxFileName file_name(path);
  if (false == file_name.IsRelative()) return path;
  // if a relative path, it might be relative to the project root folder, try
  // that...
  const wxString new_path = root + path;
  wxFileName new_file(new_path);
  if (new_file.Exists()) {
    return new_path;
  } else {
    return path;
  }
}

bool CompilerMessage::Parse(Source source, const wxString& root,
                            const wxString& text, CompilerMessage* output) {
  if (source == SOURCE_RUSTC) {
    const wxRegEx& complex = ComplexRegexOutput();
    if (complex.Matches(text)) {
      const wxString file = complex.GetMatch(text, 1);
      const int start_line = wxAtoi(complex.GetMatch(text, 2));
      const int start_index = wxAtoi(complex.GetMatch(text, 3));
      const int end_line = wxAtoi(complex.GetMatch(text, 4));
      const int end_index = wxAtoi(complex.GetMatch(text, 5));
      const CompilerMessage::Type type = ParseCMT(complex.GetMatch(text, 6));
      const wxString message = complex.GetMatch(text, 7);

      *output =
          CompilerMessage(CleanupFilePath(root, file), start_line, start_index,
                          end_line, end_index, type, message);
      return true;
    }

    const wxRegEx& related = RegexOutputRelated();
    if (related.Matches(text)) {
      const wxString file = related.GetMatch(text, 1);
      const int start_line = wxAtoi(related.GetMatch(text, 2));
      const wxString message = related.GetMatch(text, 3);

      *output = CompilerMessage(CleanupFilePath(root, file), start_line, -1, -1,
                                -1, CompilerMessage::TYPE_RELATED, message);
      return true;
    }
  } else if (source == SOURCE_PROTOC) {
    const wxRegEx& complex = ProtocRegexOutput();
    if (complex.Matches(text)) {
      const wxString file = complex.GetMatch(text, 1);
      const int start_line = wxAtoi(complex.GetMatch(text, 2));
      const int start_index = wxAtoi(complex.GetMatch(text, 3));
      const wxString message = complex.GetMatch(text, 4);

      *output = CompilerMessage(CleanupFilePath(root, file), start_line,
                                start_index, start_line, start_index,
                                CompilerMessage::TYPE_ERROR, message);
      return true;
    }
  } else {
    assert(false && "Invalid source");
  }

  return false;
}

wxString CompilerMessage::ToStringRepresentation(const Source source) {
  if (source == SOURCE_RUSTC) {
    const wxString type = "error";

    // like C:\Users\gustav\WorkingFolder\librust\src\rng.rs:16 : 1 : 21 : 2
    // warning : type could implement `Copy`; consider adding `impl Copy`,
    // #[warn(missing_copy_implementations)] on by default
    return wxString::Format("%s:%d : %d : %d : %d %s : %s", file(),
                            start_line(), start_index(), end_line(),
                            end_index(), type, message());
  } else {
    assert(false && "Invalid source");
    return "";
  }
}

//////////////////////////////////////////////////////////////////////////

const wxString& CompilerMessage::file() const { return file_; }

int CompilerMessage::start_line() const { return start_line_; }

int CompilerMessage::start_index() const { return start_index_; }

int CompilerMessage::end_line() const { return end_line_; }

int CompilerMessage::end_index() const { return end_index_; }

CompilerMessage::Type CompilerMessage::type() const { return type_; }

const wxString& CompilerMessage::message() const { return message_; }
