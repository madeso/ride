#include "ride/cargo.h"
#include "cpptoml.h"

//////////////////////////////////////////////////////////////////////////
template <typename T>
LoadResult SafeGet(const std::string& table_name, std::shared_ptr<cpptoml::table> table, const std::string& name, T* result)
{
  if (false == table->contains(name)) {
    return LoadResult::Error(wxString::Format(
      "table %s doesn't contain %s",
      table_name, name));
  }
  const auto base = table->get(name);
  if (false == base->is_value()) {
    return LoadResult::Error(wxString::Format(
      "%s from table %s is not a value",
      name, table_name));
  }
  const auto value = base->as<T>();
  *result = value->get();
  return LoadResult::Ok();
}

// cpptoml doesn't support wxString, only std::string so lets use a specialization with a temporary std::string
template<>
LoadResult SafeGet<wxString>(const std::string& table_name, std::shared_ptr<cpptoml::table> table, const std::string& name, wxString* result)
{
  std::string temp;
  const auto ret = SafeGet(table_name, table, name, &temp);
  *result = temp;
  return ret;
}

// like the rust try macro
#define TRY(x) do { const LoadResult result = x; if( false == result.IsOk() ) {return result;} } while(false)

//////////////////////////////////////////////////////////////////////////

namespace cargo
{
  const std::string PACKAGE = "package";
  const std::string NAME = "name";
}

//////////////////////////////////////////////////////////////////////////

LoadResult Cargo::Load(const wxString& file)
{
  try
  {
    cpptoml::table root = cpptoml::parse_file(static_cast<std::string>(file));

    

    auto package = root.get_table(cargo::PACKAGE);
    if ( package.get() == NULL ) return LoadResult::Error("No package in cargo file");

    TRY(SafeGet(cargo::PACKAGE, package, cargo::NAME, &name_));
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
