// Copyright (2015) Gustav

#include "ride/proto.h"

#include <wx/utils.h>
#include <google/protobuf/text_format.h>

#include <cassert>
#include <fstream>  // NOLINT this is how we use protobuf
#include <string>

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
  if (data.empty()) return false;
  output << data;
  return true;
}

bool LoadProtoBinary(google::protobuf::Message* message,
                     const wxFileName& file_name) {
  const wxString path = file_name.GetFullPath();
  if (file_name.IsFileReadable()) {
    std::fstream input(path.c_str().AsChar(), std::ios::in | std::ios::binary);
    const bool parse_result = message->ParseFromIstream(&input);
    if (false == parse_result) {
      return false;
    }
  }

  return true;
}

bool SaveProtoBinary(const google::protobuf::Message& message,
                     const wxFileName& file_name) {
  const bool create_result =
      file_name.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
  if (file_name.FileExists() && file_name.IsFileWritable() == false) {
    // abort if the file exist and isn't writable
    return false;
  }
  const wxString config_path = file_name.GetFullPath();
  std::fstream config_stream(
      config_path.c_str().AsChar(),
      std::ios::out | std::ios::trunc | std::ios::binary);
  return message.SerializeToOstream(&config_stream);
}
