// Copyright (2015) Gustav

#ifndef CARGO_H
#define CARGO_H

#include <ride/wx.h>
#include <vector>
#include "ride/loadresult.h"

class Cargo {
 public:
  LoadResult Load(const wxString& file);

  const wxString& name() const;
  void set_name(const wxString& name);

  const wxString& version() const;
  void set_version(const wxString& version);

  const std::vector<wxString>& authors() const;
  void set_authors(const std::vector<wxString>& authors);

  const std::vector<wxString>& dependencies() const;
  void set_dependencies(const std::vector<wxString>& dependencies);

  const std::vector<wxString>& features() const;
  void set_features(const std::vector<wxString>& features);

 private:
  wxString name_;
  wxString version_;
  std::vector<wxString> authors_;
  std::vector<wxString> dependencies_;
  std::vector<wxString> features_;
};

#endif  // CARGO_H
