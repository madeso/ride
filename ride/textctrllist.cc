#include "ride/textctrllist.h"
#include <wx/listctrl.h>
#include "ride/wxutils.h"

TextCtrlList::TextCtrlList(wxWindow* parent, wxListCtrl* list)
  : wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER)
  , list_(list)
  , callback_(NULL) {
}

void TextCtrlList::set_callback(TextCtrlCallback* callback) {
  callback_ = callback;
}

void TextCtrlList::OnKeyUp(wxKeyEvent& event)
{
  if (list_ == NULL) return;

  if (event.GetKeyCode() == WXK_UP) {
    auto selected = ::GetSelection(list_);
    if (selected.empty()){
      ::SetSelection(list_, 0, true);
    }
    else {
      long last = *selected.begin();
      if (event.ShiftDown() == false) ::ClearSelection(list_);
      ::SetSelection(list_, last - 1, true);
    }
  }
  else if (event.GetKeyCode() == WXK_DOWN) {
    auto selected = ::GetSelection(list_);
    if (selected.empty()){
      ::SetSelection(list_, 0, true);
    }
    else {
      long last = *selected.rbegin();
      if (event.ShiftDown() == false) ::ClearSelection(list_);
      ::SetSelection(list_, last + 1, true);
    }
  }
  else {
    event.Skip();
  }
}

void TextCtrlList::OnUpdated(wxCommandEvent& event)
{
  if (callback_ == NULL) return;
  callback_->OnTextUpdated();
}

void TextCtrlList::OnEnter(wxCommandEvent& event)
{
  if (callback_ == NULL) return;
  callback_->OnTextEnter();
}

wxBEGIN_EVENT_TABLE(TextCtrlList, wxTextCtrl)
EVT_TEXT_ENTER(wxID_ANY, TextCtrlList::OnEnter)
EVT_TEXT(wxID_ANY, TextCtrlList::OnUpdated)
EVT_KEY_UP(TextCtrlList::OnKeyUp)
wxEND_EVENT_TABLE()