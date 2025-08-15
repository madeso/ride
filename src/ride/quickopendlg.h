#ifndef RIDE_QUICKOPENDLG_H_
#define RIDE_QUICKOPENDLG_H_

#include <ride/wx.h>
#include <vector>

#include "ride/path.h"

bool ShowQuickOpenDlg(
	wxWindow* parent,
	const Dir& root,
	const std::vector<Fil>& files,
	std::vector<Fil>* selected
);

#endif	// RIDE_QUICKOPENDLG_H_
