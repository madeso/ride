// Copyright (2015) Gustav

#include "ride/proto.h"

#include <google/protobuf/text_format.h>
#include <google/protobuf/json/json.h>

#include <wx/utils.h>

#include <streambuf>
#include <cassert>
#include <fstream>  // NOLINT this is how we use fstrean
#include <sstream>  // NOLINT this is how we use sstream

#include <string>

#include "tinyxml2.h"  // NOLINT this is how we use tinyxml2

#include "ride/stringutils.h"

bool LoadProtoText(google::protobuf::Message* t, const wxFileName& file_name) {
  assert(t);
  if (file_name.FileExists() == false) return true;

  const wxString path = file_name.GetFullPath();

  std::ifstream file(path.char_str());
  if (!file) return false;
  std::string data((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  if (false == google::protobuf::TextFormat::ParseFromString(data, t))
    return false;
  return true;
}

bool VerifyFileForWriting(const wxFileName& file_name) {
  file_name.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
  if (file_name.FileExists() && file_name.IsFileWritable() == false) {
    // abort if the file exist and isn't writable
    return false;
  }
  return true;
}

bool SaveProtoText(const google::protobuf::Message& t,
                   const wxFileName& file_name) {
  if (false == VerifyFileForWriting(file_name)) return false;
  const wxString path = file_name.GetFullPath();

  std::ofstream output(path.char_str());
  std::string data;
  if (false == google::protobuf::TextFormat::PrintToString(t, &data))
    return false;
  output << data;
  return true;
}

bool LoadProtoBinary(google::protobuf::Message* message,
                     const wxFileName& file_name) {
  if (file_name.FileExists() == false) return true;

  const wxString path = file_name.GetFullPath();
  if (file_name.IsFileReadable()) {
    std::fstream input(path.c_str().AsChar(), std::ios::in | std::ios::binary);
    const bool parse_result = message->ParseFromIstream(&input);
    if (false == parse_result) {
      return false;
    }

    return true;
  }

  return false;
}

bool SaveProtoBinary(const google::protobuf::Message& message,
                     const wxFileName& file_name) {
  if (false == VerifyFileForWriting(file_name)) return false;
  const wxString config_path = file_name.GetFullPath();

  std::fstream config_stream(
      config_path.c_str().AsChar(),
      std::ios::out | std::ios::trunc | std::ios::binary);
  return message.SerializeToOstream(&config_stream);
}

wxString LoadProtoJson(google::protobuf::Message* message,
                       const wxFileName& file_name) {
  if (file_name.FileExists() == false) return "";

  const wxString path = file_name.GetFullPath();
  if (file_name.IsFileReadable() == false) {
    return "file is not readable";
  }

  std::ifstream file(path.char_str());
  if (!file) return "Failed to open file for reading";
  std::string data((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  const auto load_result = google::protobuf::json::JsonStringToMessage(data, message);
  if (load_result.ok() == false) {
    return "Failed to parse json";
  }

  return "";
}

wxString SaveProtoJson(const google::protobuf::Message& t,
                       const wxFileName& file_name) {
  if (false == VerifyFileForWriting(file_name)) {
    return "Unable to verify file";
  }

  wxString path = file_name.GetFullPath();

  std::string data;
  const auto write_result = google::protobuf::json::MessageToJsonString(t, &data);
  if (write_result.ok() == false) {
    return "Failed to write proto";
  }

  {
    std::ofstream output(path.char_str());
    output << data;
  }

  return "";
}
