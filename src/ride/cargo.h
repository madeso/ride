#ifndef RIDE_CARGO_H_
#define RIDE_CARGO_H_

#include <ride/wx.h>
#include <vector>
#include "ride/loadresult.h"

class Cargo
{
public:

	LoadResult Load(const wxString& file);

	wxString name_;
	wxString version_;
	std::vector<wxString> authors_;
	std::vector<wxString> dependencies_;
	std::vector<wxString> features_;
};

#endif	// RIDE_CARGO_H_
