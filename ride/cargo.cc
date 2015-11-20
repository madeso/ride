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
  if (t.get() == NULL) {
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
  if (str.get() == NULL) {
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
  if (value.get() == NULL) {
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
  if (array.get() == NULL) {
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
    if (NULL == value.get()) {
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
    if (package.get() == NULL)
      return LoadResult::Error("No package in cargo file");

    TRY(SafeGet(cargo::PACKAGE, package, cargo::NAME, &name_));
    TRY(SafeGet(cargo::PACKAGE, package, cargo::VERSION, &version_));
    TRY(SafeGet(cargo::PACKAGE, package, cargo::AUTHORS, &authors_));

    auto dependcies = root.get_table(cargo::DEPENDENCIES);
    if (dependcies.get() != NULL) {
      for (const auto dep : *dependcies) {
        const std::string& name = dep.first;
        dependencies_.push_back(name);
      }
    }

    auto features = root.get_table(cargo::FEATURES);
    if (features.get() != NULL) {
      for (const auto dep : *features) {
        const std::string& name = dep.first;
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

const wxString& Cargo::name() const { return name_; }

void Cargo::set_name(const wxString& name) { name_ = name; }

const wxString& Cargo::version() const { return version_; }

void Cargo::set_version(const wxString& version) { version_ = version; }

const std::vector<wxString>& Cargo::authors() const { return authors_; }

void Cargo::set_authors(const std::vector<wxString>& authors) {
  authors_ = authors;
}

const std::vector<wxString>& Cargo::dependencies() const {
  return dependencies_;
}

void Cargo::set_dependencies(const std::vector<wxString>& dependencies) {
  dependencies_ = dependencies;
}

const std::vector<wxString>& Cargo::features() const { return features_; }

void Cargo::set_features(const std::vector<wxString>& features) {
  features_ = features;
}
