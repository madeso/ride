#include "ride/wx.h"
#include "ride/filepropertiesdlg.h"

FilePropertiesDlg::FilePropertiesDlg(wxWindow* parent, wxStyledTextCtrl* ctrl) : ::ui::FileProperties(parent, wxID_ANY), ctrl_(ctrl) {
}
