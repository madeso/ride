#ifndef CARGO_H
#define CARGO_H

#include "ride/wx.h"

class Cargo {
public:
  void Load(const wxString& file);

  const wxString& name() const;
  void set_name(const wxString& name);

private:
  wxString name_;
};

#endif  // CARGO_H
