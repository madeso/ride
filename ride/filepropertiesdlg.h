#ifndef SETTINGS_DLG_H
#define SETTINGS_DLG_H

#include "ride/generated/ui.h"

class wxStyledTextCtrl;
class FileEdit;

class FilePropertiesDlg : public ui::FileProperties
{
public:
  FilePropertiesDlg(FileEdit* parent, wxStyledTextCtrl* ctrl);

protected:
  void OnChangeEncoding(wxCommandEvent& event);
  void OnChangeLineEnding(wxCommandEvent& event);

private:
  void UpdateGui();

private:
  wxStyledTextCtrl* ctrl_;
};

#endif // SETTINGS_DLG_H
