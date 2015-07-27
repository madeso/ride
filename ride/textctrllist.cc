#include "ride/textctrllist.h"
#include <wx/listctrl.h>
#include "ride/wxutils.h"

TextCtrlList::TextCtrlList(wxWindow* parent, wxListCtrl* list)
  : wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER)
  , list_(list)
  , callback_(NULL)
  , last_selected_(0) {
  BindEvents();
}

TextCtrlList::~TextCtrlList() {
  // crash when we disconnect, the list might already be destroyed...
  // list_->Disconnect(wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler(TextCtrlList::OnFileDeselected), NULL, this);
  // list_->Disconnect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(TextCtrlList::OnFileSelected), NULL, this);
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
      long last = last_selected_; //  *selected.begin();
      if (event.ShiftDown() == false) ::ClearSelection(list_);
      long next = last - 1;
      ::SetSelection(list_, next, true);
      if (last_selected_ == last) {
        last_selected_ = next;
        ::SetSelection(list_, last, false);
      }
    }
  }
  else if (event.GetKeyCode() == WXK_DOWN) {
    auto selected = ::GetSelection(list_);
    if (selected.empty()){
      ::SetSelection(list_, 0, true);
    }
    else {
      long last = last_selected_; // *selected.rbegin();
      if (event.ShiftDown() == false) ::ClearSelection(list_);
      long next = last + 1;
      ::SetSelection(list_, next, true);
      if (last_selected_ == last) {
        last_selected_ = next;
        ::SetSelection(list_, last, false);
      }
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

void TextCtrlList::OnFileDeselected(wxListEvent& event) {
}

void TextCtrlList::OnFileSelected(wxListEvent& event) {
  last_selected_ = event.GetIndex();
}

void TextCtrlList::BindEvents()
{
  Bind(wxEVT_TEXT_ENTER, &TextCtrlList::OnEnter, this);
  Bind(wxEVT_TEXT, &TextCtrlList::OnUpdated, this);
  Bind(wxEVT_KEY_UP, &TextCtrlList::OnKeyUp, this);

  list_->Bind(wxEVT_COMMAND_LIST_ITEM_DESELECTED, &TextCtrlList::OnFileDeselected, this);
  list_->Bind(wxEVT_COMMAND_LIST_ITEM_SELECTED, &TextCtrlList::OnFileSelected, this);
}
