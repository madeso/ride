#pragma once

#include "wx/filename.h"


struct Fil
{
	wxFileName path;

	explicit Fil(const wxFileName f);

	bool exist() const;

	wxString get_display() const;

	bool is_extension(const wxString& ext) const;

	/// return the system path, as a argument to fopen...
	wxString full_path() const;
};

struct Dir
{
	wxFileName path;

	explicit Dir(const wxFileName d);

	wxString get_display() const;

	bool exist() const;

	// p may be a relative path
	Fil join_file(const wxString& p) const;
};

