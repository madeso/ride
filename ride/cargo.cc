#include "ride/cargo.h"

void Cargo::Load(const wxString& file)
{
  // todo
  name_ = "implement me";
}

const wxString& Cargo::name() const
{
  return name_;
}

void Cargo::set_name(const wxString& name)
{
  name_ = name;
}
