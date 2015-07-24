#ifndef RIDE_CREATENEWFILEDLG_H
#define RIDE_CREATENEWFILEDLG_H

#include "ride/wx.h"

class CreateNewFileDlgHandler {
public:
  CreateNewFileDlgHandler(wxWindow* parent, const wxString& project_folder, const wxString& fodler_hint);

  bool ShowModal();

  const wxString GetFilePath() const;
  const wxString GetTemplateSource() const;
private:
  wxWindow* parent_;
  const wxString& project_folder_;
  const wxString& fodler_hint_;

  wxString FilePath;
  wxString TemplateSource;
};

#endif  // RIDE_CREATENEWFILEDLG_H
