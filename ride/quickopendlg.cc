#include "ride/quickopendlg.h"

#include "ride/wx.h"
#include "ride/generated/ui.h"
#include <vector>

// based on http://docs.wholetomato.com/default.asp?W193

class QuickOpenDlg : public ui::QuickOpen {
public:
  QuickOpenDlg(wxWindow* parent);

private:
  void UpdateFilters();

protected:
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
  void OnFilterUpdated(wxCommandEvent& event);

  std::vector<wxString> files_;
};

bool MatchFilter(const wxString& filter, const wxString file) {
  return file.EndsWith(filter);
}

void QuickOpenDlg::UpdateFilters() {
  const wxString filter = uiFilterName->GetValue();

  uiFileList->Freeze();
  uiFileList->DeleteAllItems();
  for (const wxString& file : files_) {
    if (MatchFilter(filter, file)) {
      int i = uiFileList->InsertItem(0, "");
      uiFileList->SetItem(i, 0, wxFileName(file).GetFullName());
      uiFileList->SetItem(i, 1, file);
    }
  }
  uiFileList->Thaw();
}

QuickOpenDlg::QuickOpenDlg(wxWindow* parent)
  : ui::QuickOpen(parent, wxID_ANY)
{
  const long file_index = uiFileList->InsertColumn(0, "File");
  const long path_index = uiFileList->InsertColumn(1, "Path");
  uiFileList->SetColumnWidth(file_index, 100);
  uiFileList->SetColumnWidth(path_index, 200);

  files_.push_back("/project/dog/src/BigCalendarCtrl.rs");
  files_.push_back("/project/dog/src/BigCalendarTask.rs");
  files_.push_back("/project/dog/src/CalendarButtonsDlg.rs");
  files_.push_back("/project/dog/src/CalendarData.rs");
  files_.push_back("/project/dog/src/CalendarDefines.rs");
  files_.push_back("/project/dog/src/CalendarExt.rs");
  files_.push_back("/project/dog/src/CalendarExt.rs");

  /*files_.push_back("/project/dog/src/big-calendar-ctrl.rs");
  files_.push_back("/project/dog/src/big-calendar-task.rs");
  files_.push_back("/project/dog/src/calendar-buttons-dlg.rs");
  files_.push_back("/project/dog/src/calendar-data.rs");
  files_.push_back("/project/dog/src/calendar-defines.rs");
  files_.push_back("/project/dog/src/calendar-ext.rs");
  files_.push_back("/project/dog/src/calendar-ext.rs");

  files_.push_back("/project/dog/src/big_calendar_ctrl.rs");
  files_.push_back("/project/dog/src/big_calendar_task.rs");
  files_.push_back("/project/dog/src/calendar_buttons_dlg.rs");
  files_.push_back("/project/dog/src/calendar_data.rs");
  files_.push_back("/project/dog/src/calendar_defines.rs");
  files_.push_back("/project/dog/src/calendar_ext.rs");
  files_.push_back("/project/dog/src/calendar_ext.rs");*/

  UpdateFilters();
}

void QuickOpenDlg::OnFilterUpdated(wxCommandEvent& event) {
  UpdateFilters();
}

void QuickOpenDlg::OnCancel(wxCommandEvent& event) {
  EndModal(wxID_OK);
}

void QuickOpenDlg::OnOk(wxCommandEvent& event) {
  EndModal(wxID_CANCEL);
}

bool ShowQuickOpenDlg(wxWindow* parent) {
  QuickOpenDlg dlg(parent);
  if (wxID_OK != dlg.ShowModal()) return false;;
  // do something!
  return true;
}
