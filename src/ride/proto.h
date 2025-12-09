#ifndef RIDE_PROTO_H_
#define RIDE_PROTO_H_


#include <ride/wx.h>
#include <wx/filename.h>

#include "path.h"

namespace ride
{
struct MachineSettings;
struct Settings;
struct Session;
struct Project;
struct UserProject;
struct ProjectSession;
struct Theme;
}  //  namespace ride

struct SerError
{
	wxString msg;
	std::size_t line;
	std::size_t column;
};

struct SerLog
{
	std::vector<SerError> errors;
};

wxString LoadProtoJson(SerLog* log, ride::MachineSettings* mess, const Fil& file);
wxString SaveProtoJson(ride::MachineSettings* mess, const Fil& file);

wxString LoadProtoJson(SerLog* log, ride::Settings* mess, const Fil& file);
wxString SaveProtoJson(ride::Settings* mess, const Fil& file);

wxString LoadProtoJson(SerLog* log, ride::Session* mess, const Fil& file);
wxString SaveProtoJson(ride::Session* mess, const Fil& file);

wxString LoadProtoJson(SerLog* log, ride::Project* mess, const Fil& file);
wxString SaveProtoJson(ride::Project* mess, const Fil& file);

wxString LoadProtoJson(SerLog* log, ride::UserProject* mess, const Fil& file);
wxString SaveProtoJson(ride::UserProject* mess, const Fil& file);

wxString LoadProtoJson(SerLog* log, ride::ProjectSession* mess, const Fil& file);
wxString SaveProtoJson(ride::ProjectSession* mess, const Fil& file);

wxString LoadProtoJson(SerLog* log, ride::Theme* mess, const Fil& file);
wxString SaveProtoJson(ride::Theme* mess, const Fil& file);


/*
bool LoadProtoText(google::protobuf::Message* t, const wxFileName& path);
bool SaveProtoText(const google::protobuf::Message& t, const wxFileName& path);

bool LoadProtoBinary(google::protobuf::Message* message,
                     const wxFileName& file_name);
bool SaveProtoBinary(const google::protobuf::Message& message,
                     const wxFileName& file_name);

wxString LoadProtoJson(google::protobuf::Message* t, const wxFileName& path);
wxString SaveProtoJson(const google::protobuf::Message& t,
                       const wxFileName& path);
*/

#endif	// RIDE_PROTO_H_
