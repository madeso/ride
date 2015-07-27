// Copyright (2015) Gustav

#include "ride/proto.h"
#include <wx/utils.h>

#include <cassert>
#include <fstream>
#include <google/protobuf/text_format.h>

bool LoadProto(google::protobuf::Message* t, const wxString& path) {
  assert(t);
  std::ifstream output(path.char_str());
  if (!output) return false;
  std::string data((std::istreambuf_iterator<char>(output)),
                   std::istreambuf_iterator<char>());
  if (false == google::protobuf::TextFormat::ParseFromString(data, t))
    return false;
  return true;
}

bool SaveProto(google::protobuf::Message* t, const wxString& path) {
  assert(t);
  std::ofstream output(path.char_str());
  std::string data;
  if (false == google::protobuf::TextFormat::PrintToString(*t, &data))
    return false;
  // const auto data = t->SerializeAsString();
  if (data.empty()) return false;
  output << data;
  return true;
}