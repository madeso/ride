// Copyright (2015) Gustav

#ifndef RIDE_PROTO_H
#define RIDE_PROTO_H

#include "ride/wx.h"

#include <google/protobuf/message.h>

bool LoadProto(google::protobuf::Message* t, const wxString& path);
bool SaveProto(google::protobuf::Message* t, const wxString& path);

#endif  // RIDE_PROTO_H
