#ifndef RIDE_DELETEFOLDERDLG_H
#define RIDE_DELETEFOLDERDLG_H

#include "ride/wx.h"

bool ShowDeleteFolderDlg(wxWindow* parent, const wxString& folder, bool* full,
                         bool* recursive);

#endif  // RIDE_DELETEFOLDERDLG_H
