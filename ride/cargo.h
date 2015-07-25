#ifndef CARGO_H
#define CARGO_H

#include "ride/wx.h"
#include "ride/loadresult.h"

#include <vector>

class Cargo {
public:
  LoadResult Load(const wxString& file);

  const wxString& name() const;
  void set_name(const wxString& name);

  const wxString& version() const;
  void set_version(const wxString& version);

  const std::vector<wxString>& authors() const;
  void set_authors(const std::vector<wxString>& authors);
private:
  wxString name_;
  wxString version_;
  std::vector<wxString> authors_;
};

#endif  // CARGO_H
