#include "ride/settings.h"

#include <ride/wx.h>

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "ride/mainwindow.h"
#include "ride/proto.h"
#include "ride/wxutils.h"
#include "ride/builtinthemes.h"

wxString GetConfigFolder()
{
	return wxStandardPaths::Get().GetUserDataDir();
}

Fil GetSettingsFile()
{
	auto f = wxFileName(GetConfigFolder(), "settings", "data");
	f.SetExt("json");
	return Fil{f};
}

Fil GetSessionFile()
{
	auto f = wxFileName(GetConfigFolder(), "session", "data");
	f.SetExt("json");
	return Fil{f};
}

Fil GetMachineFile()
{
	auto f = wxFileName(GetConfigFolder(), "machine", "data");
	f.SetExt("json");
	return Fil{f};
}

template<typename T>
bool LoadProto(SerLog* log, T* message, const Fil& file, wxWindow* main, const wxString& name)
{
	if (file.exist())
	{
		const wxString jsonerr = LoadProtoJson(log, message, file);
		if (jsonerr != "")
		{
			ShowError(main, "Unable to load " + name + "(" + file.full_path() + ") as json: " + jsonerr, "Error while loading");
			return false;
		}
	}
	else
	{
		if(file.exist() == false) return false;
		const wxString jsonerr = LoadProtoJson(log, message, file);
		if (jsonerr != "")
		{
			ShowError(main, "Unable to load " + name + "(" + file.full_path() + "): " + jsonerr, "Error while loading");
			return false;
		}
	}
	return true;
}

template<typename T>
bool SaveProto(T* message, const Fil& file, wxWindow* main, const wxString& name)
{
	const wxString jsonerr = SaveProtoJson(message, file);
	if (jsonerr != "")
	{
		ShowError(main, "Error while saving " + name + " as json: " + jsonerr, "Unable to save");
		return false;
	}

	return true;
}

bool LoadSettings(SerLog* log, wxWindow* main, ::ride::MachineSettings* settings)
{
	return LoadProto(log, settings, GetMachineFile(), main, "machine settings");
}

bool SaveSettings(wxWindow* main, ::ride::MachineSettings* settings)
{
	return SaveProto(settings, GetMachineFile(), main, "machine settings");
}

bool LoadSettings(SerLog* log, wxWindow* main, ::ride::Settings* settings)
{
	const auto file = GetSettingsFile();
	const auto r = LoadProto(log, settings, file, main, "settings");
	AddBuiltInThemes(settings);
	if (file.exist() == false)
	{
		SaveSettings(main, settings);
	}
	return r;
}

bool SaveSettings(wxWindow* main, ::ride::Settings* settings)
{
	return SaveProto(settings, GetSettingsFile(), main, "settings");
}

bool LoadSession(SerLog* log, wxWindow* main, ::ride::Session* settings)
{
	return LoadProto(log, settings, GetSessionFile(), main, "last session");
}

bool SaveSession(wxWindow* main, ::ride::Session* settings)
{
	return SaveProto(settings, GetSessionFile(), main, "current session");
}

wxColor C(const ride::Color& c)
{
	return wxColor(c.r, c.g, c.b);
}

ride::Color C(const wxColor& c)
{
	ride::Color r;
	r.r = c.Red();
	r.g = c.Green();
	r.b = c.Blue();
	return r;
}
