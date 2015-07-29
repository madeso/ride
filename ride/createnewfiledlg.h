// Copyright (2015) Gustav

#ifndef RIDE_CREATENEWFILEDLG_H
#define RIDE_CREATENEWFILEDLG_H

#include <ride/wx.h>

class CreateNewFileDlgHandler {
 public:
  CreateNewFileDlgHandler(wxWindow* parent, const wxString& project_folder,
                          const wxString& fodler_hint);

  bool ShowModal();

  const wxString file_path() const;
  const wxString template_source() const;

 private:
  wxWindow* parent_;
  const wxString& project_folder_;
  const wxString& fodler_hint_;

  wxString file_path_;
  wxString template_source_;
};

#endif  // RIDE_CREATENEWFILEDLG_H
