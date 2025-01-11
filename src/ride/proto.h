#ifndef RIDE_PROTO_H_
#define RIDE_PROTO_H_


#include <ride/wx.h>
#include <wx/filename.h>

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

wxString LoadProtoJson(ride::MachineSettings* mess, const wxFileName& file);
wxString SaveProtoJson(ride::MachineSettings* mess, const wxFileName& file);

wxString LoadProtoJson(ride::Settings* mess, const wxFileName& file);
wxString SaveProtoJson(ride::Settings* mess, const wxFileName& file);

wxString LoadProtoJson(ride::Session* mess, const wxFileName& file);
wxString SaveProtoJson(ride::Session* mess, const wxFileName& file);

wxString LoadProtoJson(ride::Project* mess, const wxFileName& file);
wxString SaveProtoJson(ride::Project* mess, const wxFileName& file);

wxString LoadProtoJson(ride::UserProject* mess, const wxFileName& file);
wxString SaveProtoJson(ride::UserProject* mess, const wxFileName& file);

wxString LoadProtoJson(ride::ProjectSession* mess, const wxFileName& file);
wxString SaveProtoJson(ride::ProjectSession* mess, const wxFileName& file);

wxString LoadProtoJson(ride::Theme* mess, const wxFileName& file);
wxString SaveProtoJson(ride::Theme* mess, const wxFileName& file);


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
