#pragma once

#include "wx/filename.h"

struct Fil;
struct Dir;

struct Fil
{
	wxFileName path;

	explicit Fil(const wxFileName f);

	bool exist() const;

	bool ends_with(const wxString& suffix) const;

	wxString get_display() const;

	Dir dir() const;

	void write(const wxString& content) const;

	bool is_extension(const wxString& ext) const;

	/// return the system path, as a argument to fopen...
	wxString full_path() const;

	static Fil from_full_path(const wxString& path);
	Fil set_extension_if_missing(const wxString& ext) const;
};

struct Dir
{
	wxFileName path;

	explicit Dir(const wxFileName d);

	wxString get_display() const;

	static Dir from_full_path(const wxString& p);
	wxString full_path() const;

	bool exist() const;

	std::optional<Dir> parent() const;

	Dir subdir(const wxString& p) const;
	Fil file(const wxString& p) const;

	// p may be a relative path
	Fil join_file(const wxString& p) const;

	bool create() const;
};

bool operator==(const Fil& lhs, const Fil& rhs);
bool operator==(const std::optional<Fil>& lhs, const std::optional<Fil>& rhs);

bool operator!=(const Fil& lhs, const Fil& rhs);
bool operator!=(const std::optional<Fil>& lhs, const std::optional<Fil>& rhs);

bool operator==(const Dir& lhs, const Dir& rhs);

bool operator!=(const Dir& lhs, const Dir& rhs);

bool operator<(const Fil& lhs, const Fil& rhs);
bool operator<(const Dir& lhs, const Dir& rhs);
