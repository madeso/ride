#ifndef RIDE_DELETEFOLDERDLG_H
#define RIDE_DELETEFOLDERDLG_H

#include "ride/wx.h"
#include <vector>

bool ShowQuickOpenDlg(wxWindow* parent, const wxString& root, const std::vector<wxString>& files, std::vector<wxString>* selected);

#endif  // RIDE_DELETEFOLDERDLG_H
