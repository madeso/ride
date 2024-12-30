// Copyright (2015) Gustav

#ifndef RIDE_QUICKOPENDLG_H_
#define RIDE_QUICKOPENDLG_H_

#include <ride/wx.h>
#include <vector>

bool ShowQuickOpenDlg(
	wxWindow* parent,
	const wxString& root,
	const std::vector<wxString>& files,
	std::vector<wxString>* selected
);

#endif	// RIDE_QUICKOPENDLG_H_
