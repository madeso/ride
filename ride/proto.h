// Copyright (2015) Gustav

#ifndef RIDE_PROTO_H
#define RIDE_PROTO_H

#include <google/protobuf/message.h>

#include <ride/wx.h>
#include <wx/filename.h>

bool LoadProto(google::protobuf::Message* t, const wxString& path);
bool SaveProto(google::protobuf::Message* t, const wxString& path);

bool LoadProtoBinary(google::protobuf::Message* message,
                     const wxFileName& file_name);
bool SaveProtoBinary(const google::protobuf::Message& message,
                     const wxFileName& file_name);

#endif  // RIDE_PROTO_H
