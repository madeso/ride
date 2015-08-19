// Copyright (2015) Gustav

#ifndef RIDE_GUILIST_H_
#define RIDE_GUILIST_H_

#include <ride/wx.h>

#include <string>
#include <algorithm>

template <typename TContainer>
class GuiList {
 public:
  GuiList(wxListBox* list, wxWindow* window)
      : uiBuildFeatures(list), window_(window) {}

  void Setup(wxButton* add_button, wxButton* remove_button,
             wxButton* edit_button, wxButton* up_button,
             wxButton* down_button) {
    SetImageAndRemoveText(add_button, wxART_NEW);
    SetImageAndRemoveText(remove_button, wxART_DELETE);
    SetImageAndRemoveText(edit_button, image::edit_xpm);
    SetImageAndRemoveText(up_button, wxART_GO_UP);
    SetImageAndRemoveText(down_button, wxART_GO_DOWN);
  }

  bool ToGui(TContainer* build, bool togui) {
    if (build == NULL) return false;
    if (togui) {
      int selection = uiBuildFeatures->GetSelection();
      uiBuildFeatures->Clear();
      for (int i = 0; i < build->features_size(); ++i) {
        uiBuildFeatures->AppendString(build->features(i));
      }
      if (build->features_size() > 0) {
        uiBuildFeatures->SetSelection(selection);
      }
    }
    return true;
  }

  bool Add(TContainer* build) {
    if (build == NULL) return false;
    wxTextEntryDialog entry(window_, "Feature name");
    if (entry.ShowModal() != wxID_OK) return false;
    std::string* new_feature = build->add_features();
    *new_feature = entry.GetValue().c_str();
    return true;
  }

  bool Edit(TContainer* build) {
    if (build == NULL) return false;
    const int selection = uiBuildFeatures->GetSelection();
    if (selection == -1) return false;

    wxTextEntryDialog entry(window_, "New feature name");
    entry.SetValue(build->features(selection));
    if (entry.ShowModal() != wxID_OK) return false;
    build->set_features(selection, entry.GetValue().c_str());
    return true;
  }

  bool Remove(TContainer* build) {
    if (build == NULL) return false;
    const int selection = uiBuildFeatures->GetSelection();
    if (selection == -1) return false;

    build->mutable_features()->DeleteSubrange(selection, 1);

    // move back one
    int new_selection = selection - 1;
    // if there aren't a selection and there are more items, select the first
    // one
    if (new_selection == -1 && build->features_size() > 0) new_selection = 0;
    uiBuildFeatures->SetSelection(new_selection);
    return true;
  }

  bool Up(TContainer* build) {
    if (build == NULL) return false;
    const int selection = uiBuildFeatures->GetSelection();
    if (selection == -1) return false;

    const int next_index = selection - 1;
    if (next_index == -1) return false;

    std::swap(*build->mutable_features(selection),
              *build->mutable_features(next_index));
    uiBuildFeatures->SetSelection(next_index);
    return true;
  }

  bool Down(TContainer* build) {
    if (build == NULL) return false;
    const int selection = uiBuildFeatures->GetSelection();
    if (selection == -1) return false;

    const int next_index = selection + 1;
    if (next_index >= build->features_size()) return false;

    std::swap(*build->mutable_features(selection),
              *build->mutable_features(next_index));
    uiBuildFeatures->SetSelection(next_index);
    return true;
  }

 private:
  wxListBox* uiBuildFeatures;
  wxWindow* window_;
};

#endif  // RIDE_GUILIST_H_
