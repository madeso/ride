#include "ride/path.h"


Fil::Fil(const wxFileName f)
	: path(f)
{
	// should we do this?: path.Normalize();
}

wxString Fil::get_display() const
{
	return path.GetFullPath();
}

bool Fil::is_extension(const wxString& ext) const
{
	return path.GetFullPath().EndsWith(ext);
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

