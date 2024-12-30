// Copyright (2015) Gustav

#include "ride/cargo.h"
#include <string>
#include <algorithm>
#include "cpptoml.h"  // NOLINT this is how it should be included

//////////////////////////////////////////////////////////////////////////

// cpptoml doesn't support wxString, only std::string
template <typename T>
std::shared_ptr<T> GetValue(const std::shared_ptr<cpptoml::base> base) {
  const auto t = base->as<T>();
  if (t.get() == nullptr) {
    // if null, return null
    return std::shared_ptr<T>();
  }
  return std::shared_ptr<T>(new T(t->get()));
}

// so the specialization solves it by using a temporary std::string
template <>
std::shared_ptr<wxString> GetValue<wxString>(
    const std::shared_ptr<cpptoml::base> base) {
  const auto str = GetValue<std::string>(base);
  if (str.get() == nullptr) {
    // if null, return null
    return std::shared_ptr<wxString>();
  }
  return std::shared_ptr<wxString>(new wxString(*str.get()));
}

template <typename T>
LoadResult SafeGet(const std::string& table_name,
                   std::shared_ptr<cpptoml::table> table,
                   const std::string& name, T* result) {
  if (false == table->contains(name)) {
    return LoadResult::Error(
        wxString::Format("table %s doesn't contain %s", table_name, name));
  }
  const auto base = table->get(name);
  if (false == base->is_value()) {
    return LoadResult::Error(
        wxString::Format("%s from table %s is not a value", name, table_name));
  }
  const auto value = GetValue<T>(base);
  if (value.get() == nullptr) {
    return LoadResult::Error(wxString::Format(
        "%s from table %s is not a valid value", name, table_name));
  }
  *result = *value;
  return LoadResult::Ok();
}

// array specialization
template <typename T>
LoadResult SafeGet(const std::string& table_name,
                   std::shared_ptr<cpptoml::table> table,
                   const std::string& name, std::vector<T>* result) {
  result->resize(0);
  if (false == table->contains(name)) {
    return LoadResult::Error(
        wxString::Format("table %s doesn't contain %s", table_name, name));
  }
  const auto array = table->get_array(name);
  if (array.get() == nullptr) {
    return LoadResult::Error(
        wxString::Format("%s from table %s is not a array", name, table_name));
  }

  int index = 0;
  for (const auto base : array->get()) {
    if (false == base->is_value()) {
      return LoadResult::Error(
          wxString::Format("value %d in array %s from table %s is not a value",
                           index, name, table_name));
    }
    const auto value = GetValue<T>(base);
    if (nullptr == value.get()) {
      return LoadResult::Error(wxString::Format(
          "value %d in array %s from table %s is not a valid value", index,
          name, table_name));
    }

    result->push_back(*value);
    ++index;
  }

  return LoadResult::Ok();
}

// like the rust try macro
#define TRY(x)                    \
  do {                            \
    const LoadResult result = x;  \
    if (false == result.IsOk()) { \
      return result;              \
    }                             \
  } while (false)

//////////////////////////////////////////////////////////////////////////

namespace cargo {
const std::string PACKAGE = "package";
const std::string NAME = "name";
const std::string VERSION = "version";
const std::string AUTHORS = "authors";
const std::string DEPENDENCIES = "dependencies";
const std::string FEATURES = "features";
}  // namespace cargo

//////////////////////////////////////////////////////////////////////////

void AddFeature(const std::vector<wxString>& deps, std::vector<wxString>* feats,
                const wxString& name) {
  // test for invalid characters
  if (name.Contains("/")) return;

  // the "feature" is a dependency, don't add
  if (std::find(deps.begin(), deps.end(), name) != deps.end()) return;

  // the features already contain this feature
  if (std::find(feats->begin(), feats->end(), name) != feats->end()) return;

  // lets add it
  feats->push_back(name);
}

LoadResult Cargo::Load(const wxString& file) {
  try {
    cpptoml::table root = cpptoml::parse_file(static_cast<std::string>(file));

    auto package = root.get_table(cargo::PACKAGE);
    if (package.get() == nullptr)
      return LoadResult::Error("No package in cargo file");

    TRY(SafeGet(cargo::PACKAGE, package, cargo::NAME, &name_));
    TRY(SafeGet(cargo::PACKAGE, package, cargo::VERSION, &version_));
    TRY(SafeGet(cargo::PACKAGE, package, cargo::AUTHORS, &authors_));

    auto dependcies = root.get_table(cargo::DEPENDENCIES);
    if (dependcies.get() != nullptr) {
      for (const auto dep : *dependcies) {
        const std::string& name = dep.first;
        dependencies_.push_back(name);
      }
    }

    auto features = root.get_table(cargo::FEATURES);
    if (features.get() != nullptr) {
      for (const auto feat : *features) {
        const std::string& name = feat.first;
        AddFeature(dependencies_, &features_, name);
        std::vector<wxString> deps;
        TRY(SafeGet(cargo::FEATURES, features, name, &deps));
        for (const wxString& dep : deps) {
          AddFeature(dependencies_, &features_, dep);
        }
      }
    }

    return LoadResult::Ok();
  } catch (const cpptoml::parse_exception& e) {
    return LoadResult::Error(e.what());
  }
}
