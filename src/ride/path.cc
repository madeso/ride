#include "ride/path.h"

#include "wx/dir.h"

Fil::Fil(const wxFileName f)
	: path(f)
{
	// should we do this?: path.Normalize();
}

bool Fil::exist() const
{
	const wxString full_path = path.GetFullPath();
	return wxFile::Exists(full_path);
}

bool Fil::ends_with(const wxString& suffix) const
{
	return path.GetFullPath().EndsWith(suffix);
}

Fil Fil::from_full_path(const wxString& p)
{
	wxFileName cargo_file(p);
	cargo_file.Normalize();
	return Fil{cargo_file};
}

Dir Fil::dir() const
{
	// or GetPath()
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

wxString Fil::full_path() const
{
	return path.GetFullPath();
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

wxString Dir::full_path() const
{
	return path.GetPathWithSep();
}

std::optional<Dir> Dir::parent() const
{
	wxFileName parent_path = path;
	parent_path.RemoveLastDir();
	if (parent_path == path)
	{
		return std::nullopt;
	}
	return Dir{parent_path};
}

Dir Dir::subdir(const wxString& p) const
{
	// todo(Gustav): this only handles a single folder name...
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

Fil Dir::file(const wxString& name, const wxString& ext) const
{
	wxFileName ret = path;
	ret.SetName(name);
	ret.SetExt(ext);
	return Fil{ret};
}

Fil Dir::join_file(const wxString& p) const
{
	wxString folders;
	wxString name;
	wxString ext;
	wxFileName::SplitPath(p, nullptr, &folders, &name, &ext);

	// todo(Gustav): this doesn't handle nested folders
	return subdir(folders).file(name, ext);
}

bool Dir::create() const
{
	const wxString dir = path.GetFullPath();
	return wxDir::Make(dir);
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------

bool operator==(const Fil& lhs, const Fil& rhs)
{
	return lhs.path == rhs.path;
}

bool operator==(const Dir& lhs, const Dir& rhs)
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

bool operator<(const Fil& lhs, const Fil& rhs)
{
	return lhs.path.GetFullPath() < rhs.path.GetFullPath();
}

bool operator<(const Dir& lhs, const Dir& rhs)
{
	return lhs.path.GetFullPath() < rhs.path.GetFullPath();
}

