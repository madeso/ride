#ifndef RIDE_COMPILER_MESSAGE_H
#define RIDE_COMPILER_MESSAGE_H

#include "ride/wx.h"

class CompilerMessage {
public:
  enum Type {
    TYPE_UNKNOWN, TYPE_WARNING, TYPE_ERROR
  };

  CompilerMessage();
  CompilerMessage(wxString file, int start_line, int start_index, int end_line, int end_index, Type type, wxString message);
  static bool Parse(const wxString& text, CompilerMessage* output);

  const wxString& file() const;
  int start_line() const;
  int start_index() const;
  int end_line() const;
  int end_index() const;
  Type type() const;
  const wxString& message() const;

private:
  wxString file_;
  int start_line_;
  int start_index_;
  int end_line_;
  int end_index_;
  Type type_;
  wxString message_;
};

#endif  // RIDE_COMPILER_MESSAGE_H
