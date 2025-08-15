#include "ride/path.h"

#include "wx/dir.h"

Fil::Fil(const wxFileName f)
	: path(f)
{
	// should we do this?: path.Normalize();
}

Fil Fil::from_full_path(const wxString& p)
{
	wxFileName cargo_file(p);
	cargo_file.Normalize();
	return Fil{cargo_file};
}

Dir Fil::dir() const
{
	const wxString folder = path.GetPathWithSep();
	return Dir::from_full_path(folder);
}

wxString Fil::get_display() const
{
	return path.GetFullPath();
}

bool Fil::is_extension(const wxString& ext) const
{
	return path.GetExt() == ext;
}

Fil Fil::set_extension_if_missing(const wxString& ext) const
{
	if (path.GetExt() != wxEmptyString) return *this;
	auto p = path;
	p.SetExt("rs");
	return Fil{p};
}

void Fil::write(const wxString& data) const
{
	wxFile file(path.GetFullPath(), wxFile::write);
	file.Write(data);
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

Dir::Dir(const wxFileName d)
	: path(d)
{
}

wxString Dir::get_display() const
{
	// or GetPathWithSep ?
	return path.GetPath();
}

bool Dir::exist() const
{
	const wxString full_path = path.GetFullPath();
	return wxDir::Exists(full_path);
}

Dir Dir::from_full_path(const wxString& p)
{
	wxFileName cargo_file(p);
	cargo_file.Normalize();
	return Dir{cargo_file};
}

Dir Dir::subdir(const wxString& p) const
{
	wxFileName folder = path;
	folder.AppendDir(p);
	return Dir{folder};
}

Fil Dir::file(const wxString& p) const
{
	wxFileName folder = path;
	folder.SetFullName(p);
	return Fil{folder};
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

bool operator==(const Fil& lhs, const Fil& rhs)
{
	return lhs.path == rhs.path;
}

bool operator==(const std::optional<Fil>& lhs, const std::optional<Fil>& rhs)
{
	if(lhs.has_value() && rhs.has_value())
	{
		return *lhs == *rhs;
	}

	return lhs.has_value() == rhs.has_value();
}

bool operator!=(const Fil& lhs, const Fil& rhs)
{
	return !(lhs == rhs);
}

bool operator!=(const std::optional<Fil>& lhs, const std::optional<Fil>& rhs)
{
	return !(lhs == rhs);
}

