#include "ride/quickopendlg.h"

#include "ride/wx.h"
#include "ride/generated/ui.h"
#include "ride/wxutils.h"
#include <vector>
#include <set>

// based on http://docs.wholetomato.com/default.asp?W193

class QuickOpenDlg : public ui::QuickOpen {
public:
  QuickOpenDlg(wxWindow* parent);

private:
  void UpdateFilters();

protected:
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
  void OnActivate(wxActivateEvent& event);
  void OnFilterKeyUp(wxKeyEvent& event);
  void OnFilterUpdated(wxCommandEvent& event);
  void OnFilterNameEnter(wxCommandEvent& event);

  std::vector<wxString> files_;
};

void AddIfMoreThanOne(std::vector<wxString>& ret, const std::vector<wxString>& space) {
  if (space.size() != 1) {
    ret.insert(ret.begin(), space.begin(), space.end());
  }
}

std::vector<wxString> SmartSplit(const wxString& str, const wxString& full) {
  const auto space = Split(str, ' ');
  const auto dash = Split(str, '-');
  const auto under = Split(str, '_');
  std::vector<wxString> ret;
  ret.push_back(full);

  AddIfMoreThanOne(ret, space);
  AddIfMoreThanOne(ret, dash);
  AddIfMoreThanOne(ret, under);

  return ret;
}

int StartWithCount(const wxString& base, const wxString& test) {
  for (int i = 0; i < test.length(); ++i) {
    // base is shorter than test string, abort
    if (i >= base.length() ) return i;

    // base string doesn't match, abort
    if (base[i] != test[i]) return i;
  }
  return test.length();
}

int EndsWithCount(const wxString& base, const wxString& test) {
  for (int i = 0; i < test.length(); ++i) {
    // base is shorter than test string, abort
    if (i >= base.length()) return i;

    // base string doesn't match, abort
    if (base[base.length()-(i+1)] != test[test.length() - (i+1)]) return i;
  }
  return test.length();
}

bool MatchFilter(const wxString& filter, const wxString file, int* count) {
  const wxFileName name(file); 
  const auto filters = RemoveEmptyStrings(Split(filter, ' '));
  const auto parts = SmartSplit(name.GetName(), name.GetFullName());
  if (filters.empty()) return true;
  for (auto fi : filters) {
    const bool exclude = fi.StartsWith("-");
    const wxString fl = exclude ? fi.Mid(1) : fi;
    const bool check_end_of_part = fl.EndsWith(".");
    const wxString f = check_end_of_part ? fl.Left(fl.Length() - 1) : fl;
    const bool partial_matching = f.Contains(".");
    for (auto part : parts) {
      int change_match_count_by = 0;
      if (partial_matching) {
        if (part.find(f) != wxString::npos) {
          change_match_count_by = 1;
        }
      }
      else {
        if (check_end_of_part) {
          change_match_count_by = EndsWithCount(part, f);
        }
        else {
          change_match_count_by = StartWithCount(part, f);
        }
      }
      if (change_match_count_by > 0) {
        if (exclude) return false;
        *count += change_match_count_by;
      }
    }
  }
  return *count > 0;
}

struct FilterMatch {
  wxString path;
  int count;

  FilterMatch(const wxString& p, int c) : path(p), count(c) {}
  bool operator<(const FilterMatch& rhs) const {
    if (count == rhs.count) return path < rhs.path;
    else return count < rhs.count;
  }
};

void QuickOpenDlg::UpdateFilters() {
  const wxString filter = uiFilterName->GetValue();

  std::set<FilterMatch> matches;
  for (const wxString& file : files_) {
    int count = 0;
    if (MatchFilter(filter, file, &count)) {
      FilterMatch m(file, count);
      assert(matches.find(m) == matches.end());
      matches.insert(m);
    }
  }

  uiFileList->Freeze();
  uiFileList->DeleteAllItems();
  for (const FilterMatch& match: matches) {
    int i = uiFileList->InsertItem(0, "");
    uiFileList->SetItem(i, 0, wxFileName(match.path).GetFullName());
    uiFileList->SetItem(i, 1, match.path);
    uiFileList->SetItem(i, 2, wxString::Format("%d", match.count));
  }
  if (matches.empty() == false) {
    SetSelection(uiFileList, 0, true);
  }
  uiFileList->Thaw();
}

QuickOpenDlg::QuickOpenDlg(wxWindow* parent)
  : ui::QuickOpen(parent, wxID_ANY)
{
  const long file_index = uiFileList->InsertColumn(0, "File");
  const long path_index = uiFileList->InsertColumn(1, "Path");
  const long count_index = uiFileList->InsertColumn(2, "Hits");
  uiFileList->SetColumnWidth(file_index, 150);
  uiFileList->SetColumnWidth(path_index, 300);
  uiFileList->SetColumnWidth(count_index, 50);

  /*files_.push_back("/project/dog/src/BigCalendarCtrl.rs");
  files_.push_back("/project/dog/src/BigCalendarTask.rs");
  files_.push_back("/project/dog/src/CalendarButtonsDlg.rs");
  files_.push_back("/project/dog/src/CalendarData.rs");
  files_.push_back("/project/dog/src/CalendarDefines.rs");
  files_.push_back("/project/dog/src/CalendarExt.rs");
  files_.push_back("/project/dog/src/CalendarExt.rs");
  */

  files_.push_back("/project/dog/src/big-calendar-ctrl.rs");
  files_.push_back("/project/dog/src/big-calendar-task.rs");
  files_.push_back("/project/dog/src/calendar-buttons-dlg.rs");
  files_.push_back("/project/dog/src/calendar-data.rs");
  files_.push_back("/project/dog/src/calendar-defines.rs");
  files_.push_back("/project/dog/src/calendar-ext.rs");
  files_.push_back("/project/dog/assets/pixel-shader.glsl");
  files_.push_back("/project/dog/assets/fragment-shader.glsl");
  files_.push_back("/project/dog/assets/texture-enemy.png");

  files_.push_back("/project/dog/assets/texture-enemy.template.png");
  /*
  files_.push_back("/project/dog/src/big_calendar_ctrl.rs");
  files_.push_back("/project/dog/src/big_calendar_task.rs");
  files_.push_back("/project/dog/src/calendar_buttons_dlg.rs");
  files_.push_back("/project/dog/src/calendar_data.rs");
  files_.push_back("/project/dog/src/calendar_defines.rs");
  files_.push_back("/project/dog/src/calendar_ext.rs");
  files_.push_back("/project/dog/src/calendar_ext.rs");*/

  uiFileList->Bind(wxEVT_KEY_DOWN, &QuickOpenDlg::OnFilterKeyUp, this);
  UpdateFilters();
}

void QuickOpenDlg::OnActivate(wxActivateEvent& event) {
  uiFilterName->SelectAll();
  uiFilterName->SetFocus();
  uiFilterName->SetFocusFromKbd();
}

void QuickOpenDlg::OnFilterKeyUp(wxKeyEvent& event) {
  if (event.GetKeyCode() == wxUP) {
    auto selected = GetSelection(uiFileList);
    if (selected.empty()){
      SetSelection(uiFileList, 0, true);
    }
    else {
      long last = *selected.begin();
      if (event.ShiftDown() == false) ClearSelection(uiFileList);
      SetSelection(uiFileList, last - 1, true);
    }
  }
  else if (event.GetKeyCode() == wxDOWN) {
    auto selected = GetSelection(uiFileList);
    if (selected.empty()){
      SetSelection(uiFileList, 0, true);
    }
    else {
      long last = *selected.rbegin();
      if (event.ShiftDown() == false) ClearSelection(uiFileList);
      SetSelection(uiFileList, last + 1, true);
    }
  }
  else {
    event.Skip();
  }
}

void QuickOpenDlg::OnFilterNameEnter(wxCommandEvent& event) {
  EndModal(wxID_OK);
}

void QuickOpenDlg::OnFilterUpdated(wxCommandEvent& event) {
  UpdateFilters();
}

void QuickOpenDlg::OnCancel(wxCommandEvent& event) {
  EndModal(wxID_CANCEL);
}

void QuickOpenDlg::OnOk(wxCommandEvent& event) {
  EndModal(wxID_OK);
}

bool ShowQuickOpenDlg(wxWindow* parent) {
  QuickOpenDlg dlg(parent);
  if (wxID_OK != dlg.ShowModal()) return false;;
  // do something!
  return true;
}
