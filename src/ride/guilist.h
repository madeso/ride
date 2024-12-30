// Copyright (2015) Gustav

#ifndef RIDE_GUILIST_H_
#define RIDE_GUILIST_H_

#include <ride/wx.h>
#include "ride/resources/commonimages.h"

#include "ride/form.h"

template <typename TContainer, typename TContainerFunctions>
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

  void Select(int selected, TContainer* build) {
    if (build == NULL) return;
    if (selected >= TContainerFunctions::Size(build)) return;
    if (selected < 0) return;
    uiBuildFeatures->SetSelection(selected);
  }

  bool ToGui(TContainer* build, bool togui) {
    if (build == NULL) return false;
    if (togui) {
      int selection = uiBuildFeatures->GetSelection();
      uiBuildFeatures->Clear();
      for (int i = 0; i < TContainerFunctions::Size(build); ++i) {
        uiBuildFeatures->AppendString(
            TContainerFunctions::GetDisplayString(build, i));
      }
      if (TContainerFunctions::Size(build) > 0) {
        uiBuildFeatures->SetSelection(selection);
      }
    }
    return true;
  }

  bool Add(TContainer* build) {
    if (build == NULL) return false;
    wxTextEntryDialog entry(window_, TContainerFunctions::ADD_TEXT);
    if (entry.ShowModal() != wxID_OK) return false;
    TContainerFunctions::Add(build, entry.GetValue());
    return true;
  }

  bool Edit(TContainer* build) {
    if (build == NULL) return false;
    const int selection = uiBuildFeatures->GetSelection();
    if (selection == -1) return false;

    wxTextEntryDialog entry(window_, TContainerFunctions::EDIT_TEXT);
    entry.SetValue(TContainerFunctions::GetDisplayString(build, selection));
    if (entry.ShowModal() != wxID_OK) return false;
    TContainerFunctions::SetDisplayString(build, selection, entry.GetValue());
    return true;
  }

  bool Remove(TContainer* build) {
    if (build == NULL) return false;
    const int selection = uiBuildFeatures->GetSelection();
    if (selection == -1) return false;

    TContainerFunctions::Remove(build, selection);

    // move back one
    int new_selection = selection - 1;
    // if there aren't a selection and there are more items, select the first
    // one
    if (new_selection == -1 && TContainerFunctions::Size(build) > 0)
      new_selection = 0;
    uiBuildFeatures->SetSelection(new_selection);
    return true;
  }

  bool Up(TContainer* build) {
    if (build == NULL) return false;
    const int selection = uiBuildFeatures->GetSelection();
    if (selection == -1) return false;

    const int next_index = selection - 1;
    if (next_index == -1) return false;

    TContainerFunctions::Swap(build, selection, next_index);
    uiBuildFeatures->SetSelection(next_index);
    return true;
  }

  bool Down(TContainer* build) {
    if (build == NULL) return false;
    const int selection = uiBuildFeatures->GetSelection();
    if (selection == -1) return false;

    const int next_index = selection + 1;
    if (next_index >= TContainerFunctions::Size(build)) return false;

    TContainerFunctions::Swap(build, selection, next_index);
    uiBuildFeatures->SetSelection(next_index);
    return true;
  }

 private:
  wxListBox* uiBuildFeatures;
  wxWindow* window_;
};

#endif  // RIDE_GUILIST_H_
