// Copyright (2015) Gustav

#ifndef LOADRESULT_H
#define LOADRESULT_H

#include <ride/wx.h>

class LoadResult {
 public:
  static LoadResult Ok();
  static LoadResult Error(const wxString& message);
  const wxString& message() const;
  bool IsOk() const;

 private:
  LoadResult();
  wxString message_;
  bool ok_;
};

#endif  // LOADRESULT_H
