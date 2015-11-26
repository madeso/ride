// Copyright (2015) Gustav

#ifndef RIDE_PROTO_H_
#define RIDE_PROTO_H_

#include <google/protobuf/message.h>

#include <ride/wx.h>
#include <wx/filename.h>

bool LoadProtoText(google::protobuf::Message* t, const wxFileName& path);
bool SaveProtoText(const google::protobuf::Message& t, const wxFileName& path);

bool LoadProtoBinary(google::protobuf::Message* message,
                     const wxFileName& file_name);
bool SaveProtoBinary(const google::protobuf::Message& message,
                     const wxFileName& file_name);

wxString LoadProtoJson(google::protobuf::Message* t, const wxFileName& path);
wxString SaveProtoJson(const google::protobuf::Message& t,
                       const wxFileName& path);

#endif  // RIDE_PROTO_H_
