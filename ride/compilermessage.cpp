#include "ride/compilermessage.h"
#include <wx/regex.h>

CompilerMessage::CompilerMessage()
:file_       ("")
,start_line_ (-1)
,start_index_(-1)
,end_line_   (-1)
,end_index_  (-1)
,type_       (CompilerMessage::TYPE_UNKNOWN)
,message_    ("")
{
}

CompilerMessage::CompilerMessage(wxString file, int start_line, int start_index, int end_line, int end_index, CompilerMessage::Type type, wxString message)
:file_       (file       )
,start_line_ (start_line )
,start_index_(start_index)
,end_line_   (end_line   )
,end_index_  (end_index  )
,type_       (type       )
,message_    (message    )
{
}

bool CompileComplexRegexOutput(wxRegEx& ret) {
  // C:\Users\gustav\WorkingFolder\librust\src\rng.rs:16 : 1 : 21 : 2 warning : type could implement `Copy`; consider adding `impl Copy`, #[warn(missing_copy_implementations)] on by default
  const wxString WS = "[ \\t]*";
  const wxString SEP = "[/\\\\]";
  const wxString DRIVE = "(?:[a-zA-Z]\\:)?";
  const wxString NAME = "[a-zA-Z\\.0-9\\-_]+";
  const wxString FILE = "(" + DRIVE + "(?:" + SEP + NAME + ")+)";
  // const wxString FILE = "(?:[\\w]\\:|\\\\)(\\\\[a-z_\\-\\s0-9\\.]+)+\\.[a-zA-Z]+";
  const wxString INT = "([0-9]+)";
  const wxString ID = "([a-zA-Z]+)";
  const wxString TEXT = "(.+)";
  const wxString COMPLEX_REGEX_OUTPUT = FILE + "\\:" + INT + WS+ "\\:" + WS + INT + WS + "\\:" + WS + INT + WS + "\\:" + WS + INT + WS + ID + WS + "\\:" + WS + TEXT;
  
  bool result = ret.Compile(COMPLEX_REGEX_OUTPUT, wxRE_ADVANCED);
  return result;
}

const wxRegEx& ComplexRegexOutput() {
  static wxRegEx ret;
  static bool compiled = CompileComplexRegexOutput(ret);
  assert(compiled);
  return ret;
}

CompilerMessage::Type ParseCMT(const wxString& str) {
  if (str == "warning") return CompilerMessage::TYPE_WARNING;
  else if (str == "error") return CompilerMessage::TYPE_ERROR;
  else return CompilerMessage::TYPE_UNKNOWN;
}

bool CompilerMessage::Parse(const wxString& text, CompilerMessage* output) {
  if (ComplexRegexOutput().Matches(text)) {
    const wxString              file         =          ComplexRegexOutput().GetMatch(text, 1);
    const int                   start_line   =   wxAtoi(ComplexRegexOutput().GetMatch(text, 2));
    const int                   start_index  =   wxAtoi(ComplexRegexOutput().GetMatch(text, 3));
    const int                   end_line     =   wxAtoi(ComplexRegexOutput().GetMatch(text, 4));
    const int                   end_index    =   wxAtoi(ComplexRegexOutput().GetMatch(text, 5));
    const CompilerMessage::Type type         = ParseCMT(ComplexRegexOutput().GetMatch(text, 6));
    const wxString              message      =          ComplexRegexOutput().GetMatch(text, 7);

    *output = CompilerMessage(file, start_line, start_index, end_line, end_index, type, message);
    return true;
  }

  return false;
}

//////////////////////////////////////////////////////////////////////////

const wxString& CompilerMessage::file() const {
  return file_;
}

int CompilerMessage::start_line() const{
  return start_line_;
}

int CompilerMessage::start_index() const{
  return start_index_;
}

int CompilerMessage::end_line() const{
  return end_line_;
}

int CompilerMessage::end_index() const{
  return end_index_;
}

CompilerMessage::Type CompilerMessage::type() const{
  return type_;
}

const wxString& CompilerMessage::message() const{
  return message_;
}
