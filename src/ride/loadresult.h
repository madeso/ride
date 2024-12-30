#ifndef RIDE_LOADRESULT_H_
#define RIDE_LOADRESULT_H_

#include <ride/wx.h>

class LoadResult
{
public:

	static LoadResult Ok();
	static LoadResult Error(const wxString& message);
	const wxString& message() const;
	bool IsOk() const;

private:

	LoadResult();
	wxString message_;
	bool ok_;
};

#endif	// RIDE_LOADRESULT_H_
