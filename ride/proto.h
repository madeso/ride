// Copyright (2015) Gustav

#ifndef RIDE_PROTO_H_
#define RIDE_PROTO_H_

#include <google/protobuf/message.h>

#include <ride/wx.h>
#include <wx/filename.h>

bool LoadProto(google::protobuf::Message* t, const wxFileName& path);
bool SaveProto(const google::protobuf::Message& t, const wxFileName& path);

bool LoadProtoBinary(google::protobuf::Message* message,
                     const wxFileName& file_name);
bool SaveProtoBinary(const google::protobuf::Message& message,
                     const wxFileName& file_name);

#endif  // RIDE_PROTO_H_
