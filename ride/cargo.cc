#include "ride/cargo.h"
#include "cpptoml.h"

LoadResult Cargo::Load(const wxString& file)
{
  try
  {
    cpptoml::table g = cpptoml::parse_file(static_cast<std::string>(file));

    name_ = "implement me";
    return LoadResult::Ok();
  }
  catch (const cpptoml::parse_exception& e)
  {
    return LoadResult::Error(e.what());
  }
}

const wxString& Cargo::name() const
{
  return name_;
}

void Cargo::set_name(const wxString& name)
{
  name_ = name;
}
