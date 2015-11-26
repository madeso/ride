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

void AddItem(tinyxml2::XMLElement* el, tinyxml2::XMLDocument* doc,
             const wxString& s) {
  auto x = doc->NewElement("x");
  x->SetText(s.c_str().AsChar());
  el->InsertEndChild(x);
}

void FillElement(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* root,
                 const google::protobuf::Message& mess);

bool UseCdata(const std::string& str) {
  return str.find_first_of(" \n\t") != std::string::npos;
}

void SetText(tinyxml2::XMLElement* el, const wxString str) {
  el->SetText(str.c_str().AsChar());
}

tinyxml2::XMLElement* CreateXmlNode(
    tinyxml2::XMLDocument* doc, const google::protobuf::FieldDescriptor* desc,
    const google::protobuf::Message& mess) {
  auto ref = mess.GetReflection();

  if (desc->is_repeated() == false) {
    if (ref->HasField(mess, desc) == false) {
      return nullptr;
    }
  }

  tinyxml2::XMLElement* el = doc->NewElement("f");
  el->SetAttribute("n", desc->name().c_str());

  // TODO(Gustav): cleanup code
  if (desc->is_repeated()) {
    const int count = ref->FieldSize(mess, desc);
    switch (desc->cpp_type()) {
      case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
        for (int i = 0; i < count; ++i) {
          AddItem(el, doc, ToString(ref->GetRepeatedInt32(mess, desc, i)));
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
        for (int i = 0; i < count; ++i) {
          AddItem(el, doc, ToString(ref->GetRepeatedInt64(mess, desc, i)));
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
        for (int i = 0; i < count; ++i) {
          AddItem(el, doc, ToString(ref->GetRepeatedUInt32(mess, desc, i)));
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
        for (int i = 0; i < count; ++i) {
          AddItem(el, doc, ToString(ref->GetRepeatedUInt64(mess, desc, i)));
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
        for (int i = 0; i < count; ++i) {
          AddItem(el, doc, ToString(ref->GetRepeatedDouble(mess, desc, i)));
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
        for (int i = 0; i < count; ++i) {
          AddItem(el, doc, ToString(ref->GetRepeatedFloat(mess, desc, i)));
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
        for (int i = 0; i < count; ++i) {
          AddItem(el, doc, ToString(ref->GetRepeatedBool(mess, desc, i)));
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
        for (int i = 0; i < count; ++i) {
          auto e = ref->GetRepeatedEnum(mess, desc, i);
          auto x = doc->NewElement("x");
          x->SetAttribute("v", e->name().c_str());
          el->InsertEndChild(x);
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
        for (int i = 0; i < count; ++i) {
          const auto str = ref->GetRepeatedString(mess, desc, i);
          auto t = doc->NewText(str.c_str());
          auto x = doc->NewElement("x");
          x->InsertEndChild(t);
          if (UseCdata(str)) {
            // TODO(Gustav): change the logic here?
            t->SetCData(true);
          }
          el->InsertEndChild(x);
        }
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
        for (int i = 0; i < count; ++i) {
          auto x = doc->NewElement("x");
          FillElement(doc, x, ref->GetRepeatedMessage(mess, desc, i));
          el->InsertEndChild(x);
        }
        break;
      default:
        assert(false && "Unhandled cpp type");
    }
  } else {
    switch (desc->cpp_type()) {
      case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
        SetText(el, ToString(ref->GetInt32(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
        SetText(el, ToString(ref->GetInt64(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
        SetText(el, ToString(ref->GetUInt32(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
        SetText(el, ToString(ref->GetUInt64(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
        SetText(el, ToString(ref->GetDouble(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
        SetText(el, ToString(ref->GetFloat(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
        SetText(el, ToString(ref->GetBool(mess, desc)));
        break;
      case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
        auto e = ref->GetEnum(mess, desc);
        el->SetAttribute("v", e->name().c_str());
      } break;
      case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
        const auto str = ref->GetString(mess, desc);
        auto t = doc->NewText(str.c_str());
        el->InsertEndChild(t);
        if (UseCdata(str)) {
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

class CustomPrinter : public tinyxml2::XMLPrinter {
 public:
  void PrintSpace(int depth) override {
    for (int i = 0; i < depth; ++i) {
      Print("\t");
    }
  }
};

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

  CustomPrinter p;
  doc.Print(&p);

  wxFile f;
  if (false == f.Open(path, wxFile::write)) {
    return "Unable to open file";
  }
  f.Write(p.CStr());
  f.Close();

  return "";
}
