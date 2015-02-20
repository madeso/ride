#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include "ride/generated/ui.h"

class wxStyledTextCtrl;

class FilePropertiesDlg : public ui::FileProperties
{
public:
  FilePropertiesDlg(wxWindow* parent, wxStyledTextCtrl* ctrl);

protected:
  

private:
  wxStyledTextCtrl* ctrl_;
};

#endif // SETTINGS_DLG_H
