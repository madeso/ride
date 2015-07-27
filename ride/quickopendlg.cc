// Copyright (2015) Gustav

#include "ride/quickopendlg.h"

#include "ride/wx.h"
#include "ride/generated/ui.h"
#include "ride/wxutils.h"
#include <vector>
#include <set>

// based on http://docs.wholetomato.com/default.asp?W193

class QuickOpenDlg : public ui::QuickOpen {
 public:
  QuickOpenDlg(wxWindow* parent, const wxString& root,
               const std::vector<wxString>& files);
  std::vector<wxString> GetSelectedFiles();

 private:
  void UpdateFilters();

 protected:
  void OnCancel(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
  void OnActivate(wxActivateEvent& event);
  void OnContextSensitive(wxCommandEvent& event);
  // virtual void OnFileDeselected(wxListEvent& event) { event.Skip(); }
  // virtual void OnFileSelected(wxListEvent& event) { event.Skip(); }

  void OnTextUpdated();
  void OnTextEnter();

 protected:
  void OnKeyUp(wxKeyEvent& event);
  void OnUpdated(wxCommandEvent& event);
  void OnEnter(wxCommandEvent& event);

  void OnFileDeselected(wxListEvent& event);
  void OnFileSelected(wxListEvent& event);
  int last_selected_;

 private:
  void BindEvents();
  wxString root_;
  std::vector<wxString> files_;
  std::vector<wxString> filtered_paths_;
};

//////////////////////////////////////////////////////////////////////////

void QuickOpenDlg::OnKeyUp(wxKeyEvent& event) {
  if (uiFileList == NULL) return;

  if (event.GetKeyCode() == WXK_UP) {
    auto selected = ::GetSelection(uiFileList);
    if (selected.empty()) {
      ::SetSelection(uiFileList, 0, true);
    } else {
      long last = last_selected_;  //  *selected.begin();
      if (event.ShiftDown() == false) ::ClearSelection(uiFileList);
      long next = last - 1;
      ::SetSelection(uiFileList, next, true);
      if (last_selected_ == last) {
        last_selected_ = next;
        ::SetSelection(uiFileList, last, false);
      }
    }
  } else if (event.GetKeyCode() == WXK_DOWN) {
    auto selected = ::GetSelection(uiFileList);
    if (selected.empty()) {
      ::SetSelection(uiFileList, 0, true);
    } else {
      long last = last_selected_;  // *selected.rbegin();
      if (event.ShiftDown() == false) ::ClearSelection(uiFileList);
      long next = last + 1;
      ::SetSelection(uiFileList, next, true);
      if (last_selected_ == last) {
        last_selected_ = next;
        ::SetSelection(uiFileList, last, false);
      }
    }
  } else {
    event.Skip();
  }
}

void QuickOpenDlg::OnUpdated(wxCommandEvent& event) { OnTextUpdated(); }

void QuickOpenDlg::OnEnter(wxCommandEvent& event) { OnTextEnter(); }

void QuickOpenDlg::OnFileDeselected(wxListEvent& event) {}

void QuickOpenDlg::OnFileSelected(wxListEvent& event) {
  last_selected_ = event.GetIndex();
}

void QuickOpenDlg::BindEvents() {
  uiFilterName->Bind(wxEVT_TEXT_ENTER, &QuickOpenDlg::OnEnter, this);
  uiFilterName->Bind(wxEVT_TEXT, &QuickOpenDlg::OnUpdated, this);
  uiFilterName->Bind(wxEVT_KEY_UP, &QuickOpenDlg::OnKeyUp, this);

  uiFileList->Bind(wxEVT_COMMAND_LIST_ITEM_DESELECTED,
                   &QuickOpenDlg::OnFileDeselected, this);
  uiFileList->Bind(wxEVT_COMMAND_LIST_ITEM_SELECTED,
                   &QuickOpenDlg::OnFileSelected, this);
}

//////////////////////////////////////////////////////////////////////////

void AddIfMoreThanOne(std::vector<wxString>& ret,
                      const std::vector<wxString>& space) {
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
    if (i >= base.length()) return i;

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
    if (base[base.length() - (i + 1)] != test[test.length() - (i + 1)])
      return i;
  }
  return test.length();
}

bool MatchFilter(const wxString& filter, const wxString file, int* count,
                 bool lower) {
  const wxFileName name(lower ? file.Lower() : file);
  const auto filters =
      RemoveEmptyStrings(Split(lower ? filter.Lower() : filter, ' '));
  const auto parts = SmartSplit(name.GetName(), name.GetFullName());
  if (filters.empty()) return true;
  for (auto fi : filters) {
    const bool exclude = fi.StartsWith("-");
    const wxString fl = exclude ? fi.Mid(1) : fi;
    const bool check_end_of_part = fl.EndsWith(".");
    const wxString f = check_end_of_part ? fl.Left(fl.Length() - 1) : fl;
    const bool partial_matching = f.Contains(".");
    const bool folder_matching = f.StartsWith("/");
    if (folder_matching) {
      if (file.find(f) != wxString::npos) {
        *count += f.length();
      }
    } else {
      for (auto part : parts) {
        int change_match_count_by = 0;
        if (partial_matching) {
          if (part.find(f) != wxString::npos) {
            change_match_count_by = f.length();
          }
        } else {
          if (check_end_of_part) {
            change_match_count_by = EndsWithCount(part, f);
          } else {
            change_match_count_by = StartWithCount(part, f);
          }
        }
        if (change_match_count_by > 0) {
          if (exclude) return false;
          *count += change_match_count_by;
        }
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
    if (count == rhs.count)
      return path < rhs.path;
    else
      return count < rhs.count;
  }
};

void QuickOpenDlg::UpdateFilters() {
  const wxString filter = uiFilterName->GetValue();
  const bool case_insensitivity = false == uiCaseSensitive->GetValue();
  std::set<FilterMatch> matches;
  for (const wxString& file : files_) {
    int count = 0;
    if (MatchFilter(filter, file, &count, case_insensitivity)) {
      FilterMatch m(file, count);
      assert(matches.find(m) == matches.end());
      matches.insert(m);
    }
  }

  uiFileList->Freeze();
  uiFileList->DeleteAllItems();
  std::vector<wxString> paths;
  for (const FilterMatch& match : matches) {
    int i = uiFileList->InsertItem(0, "");
    uiFileList->SetItem(i, 0, wxFileName(match.path).GetFullName());
    uiFileList->SetItem(i, 1, match.path);
    uiFileList->SetItem(i, 2, wxString::Format("%d", match.count));

    paths.push_back(wxFileName(root_ + match.path).GetFullPath());
  }
  // we added the elements in reverse, so let's reverse the vector
  filtered_paths_ = std::vector<wxString>(paths.rbegin(), paths.rend());
  if (matches.empty() == false) {
    SetSelection(uiFileList, 0, true);
  }
  uiFileList->Thaw();
}

QuickOpenDlg::QuickOpenDlg(wxWindow* parent, const wxString& root,
                           const std::vector<wxString>& files)
    : ui::QuickOpen(parent, wxID_ANY), files_(files), root_(root) {
  const long file_index = uiFileList->InsertColumn(0, "File");
  const long path_index = uiFileList->InsertColumn(1, "Path");
  const long count_index = uiFileList->InsertColumn(2, "Hits");
  uiFileList->SetColumnWidth(file_index, 150);
  uiFileList->SetColumnWidth(path_index, 300);
  uiFileList->SetColumnWidth(count_index, 50);

  BindEvents();
  // uiFilterName->set_callback(this);
  // uiFileList->Bind(wxEVT_KEY_DOWN, &QuickOpenDlg::OnFilterKeyUp, this);
  UpdateFilters();
}

void QuickOpenDlg::OnActivate(wxActivateEvent& event) {
  uiFilterName->SelectAll();
  uiFilterName->SetFocus();
  uiFilterName->SetFocusFromKbd();
}

void QuickOpenDlg::OnTextEnter() { EndModal(wxID_OK); }

void QuickOpenDlg::OnTextUpdated() { UpdateFilters(); }

void QuickOpenDlg::OnContextSensitive(wxCommandEvent& event) {
  UpdateFilters();
}

void QuickOpenDlg::OnCancel(wxCommandEvent& event) { EndModal(wxID_CANCEL); }

void QuickOpenDlg::OnOk(wxCommandEvent& event) { EndModal(wxID_OK); }

std::vector<wxString> QuickOpenDlg::GetSelectedFiles() {
  const auto selection = GetSelection(uiFileList);
  std::vector<wxString> ret;
  for (auto sel : selection) {
    ret.push_back(filtered_paths_[sel]);
  }
  return ret;
}

bool ShowQuickOpenDlg(wxWindow* parent, const wxString& root,
                      const std::vector<wxString>& files,
                      std::vector<wxString>* selected) {
  assert(selected);
  QuickOpenDlg dlg(parent, root, files);
  if (wxID_OK != dlg.ShowModal()) return false;
  ;
  // do something!
  *selected = dlg.GetSelectedFiles();
  return true;
}
