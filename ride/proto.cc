// Copyright (2015) Gustav

#include "ride/proto.h"

#include <google/protobuf/text_format.h>

#include <wx/utils.h>

#include <cassert>
#include <fstream>  // NOLINT this is how we use fstrean
#include <sstream>  // NOLINT this is how we use sstream
#include <string>

#include "tinyxml2.h"  // NOLINT this is how we use tinyxml2

#include "ride/stringutils.h"

bool LoadProto(google::protobuf::Message* t, const wxFileName& file_name) {
  assert(t);

  if (file_name.IsFileReadable() == false) {
    return true;
  }
  const wxString path = file_name.GetFullPath();

  std::ifstream output(path.char_str());
  if (!output) return false;
  std::string data((std::istreambuf_iterator<char>(output)),
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

bool SaveProto(const google::protobuf::Message& t,
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
  if (false == VerifyFileForWriting(file_name)) return false;
  const wxString config_path = file_name.GetFullPath();

  std::fstream config_stream(
      config_path.c_str().AsChar(),
      std::ios::out | std::ios::trunc | std::ios::binary);
  return message.SerializeToOstream(&config_stream);
}

template <typename T>
wxString ToString(const T& t) {
  std::ostringstream ss;
  ss << t;
  return ss.str().c_str();
}

void FillElement(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* root,
                 const google::protobuf::Message& mess);

tinyxml2::XMLElement* CreateXmlNode(
    tinyxml2::XMLDocument* doc, const google::protobuf::FieldDescriptor* desc,
    const google::protobuf::Message& mess) {
  auto ref = mess.GetReflection();

  if (desc->is_repeated() == false) {
    if (ref->HasField(mess, desc) == false) {
      return nullptr;
    }
  }

  tinyxml2::XMLElement* el = doc->NewElement("field");
  el->SetAttribute("name", desc->name().c_str());
  el->SetAttribute("number", desc->number());

  // TODO(Gustav): handle repeated
  if (false == desc->is_repeated()) {
    switch (desc->cpp_type()) {
      case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
        el->SetText(ToString(ref->GetInt32(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
        el->SetText(ToString(ref->GetInt64(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
        el->SetText(ToString(ref->GetUInt32(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
        el->SetText(ToString(ref->GetUInt64(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
        el->SetText(ToString(ref->GetDouble(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
        el->SetText(ToString(ref->GetFloat(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
        el->SetText(ToString(ref->GetBool(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
        auto e = ref->GetEnum(mess, desc);
        el->SetAttribute("value", e->name().c_str());
        el->SetAttribute("enum", e->number());
      } break;
      case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
        const auto str = ref->GetString(mess, desc);
        auto t = doc->NewText(str.c_str());
        el->InsertEndChild(t);
        if (StartsWith(str, " ") || str.length() > 40) {
          // TODO(Gustav): change the logic here?
          t->SetCData(true);
        }
      } break;
      case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
        FillElement(doc, el, ref->GetMessage(mess, desc));
        break;
      default:
        assert(false && "Unhandled cpp type");
    }
  }
  return el;
}

void FillElement(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* root,
                 const google::protobuf::Message& mess) {
  auto desc = mess.GetDescriptor();
  for (int i = 0; i < desc->field_count(); ++i) {
    auto f = desc->field(i);
    tinyxml2::XMLElement* node = CreateXmlNode(doc, f, mess);
    if (node != nullptr) {
      root->InsertEndChild(node);
    }
  }
}

wxString SaveProtoXml(const google::protobuf::Message& mess,
                      const wxFileName& file_name) {
  if (false == VerifyFileForWriting(file_name)) {
    return "Unable to verify file";
  }

  tinyxml2::XMLDocument doc;

  auto desc = mess.GetDescriptor();
  tinyxml2::XMLElement* root = doc.NewElement("message");
  root->SetAttribute("name", desc->name().c_str());
  // node->SetAttribute("id", ToString(desc->index()));

  FillElement(&doc, root, mess);
  doc.InsertEndChild(root);
  wxString path = file_name.GetFullPath();
  if (doc.SaveFile(path) != tinyxml2::XML_NO_ERROR) {
    std::ostringstream ss;
    auto str1 = doc.GetErrorStr1();
    auto str2 = doc.GetErrorStr2();
    ss << "Unable to write xml doc(" << doc.ErrorName()
       << "): " << (str1 ? str1 : "UNKNOWN");
    if (str2) ss << "\n" << str2;
    return ss.str().c_str();
  }

  return "";
}

/*
wxXmlNode* CreateXmlNode(const google::protobuf::Message& mess, wxXmlNode*
parent) {
  auto ref = mess.GetReflection();
  auto desc = mess.GetDescriptor();
  wxXmlNode* node = new wxXmlNode(parent, wxXML_ELEMENT_NODE, "message");
  node->AddAttribute("name", desc->name());
  node->AddAttribute("id", ToString(desc->index()));
  return node;
}

wxString SaveProtoXml(const google::protobuf::Message& t, const wxFileName&
file_name) {
  if (false == VerifyFileForWriting(file_name)) {
    return "Unable to verify file";
  }

  wxXmlDocument doc;

  wxXmlNode* root = CreateXmlNode(t, nullptr);
  doc.SetRoot(root);

  if (false == doc.Save(file_name.GetFullPath()) {
    return "Unable to write xml doc";
  }

  return "";
}
*/
