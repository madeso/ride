#ifndef RIDE_TEXTCTRLLIST_H
#define RIDE_TEXTCTRLLIST_H

#include "ride/wx.h"

class wxListCtrl;
class wxListEvent;

class TextCtrlCallback {
public:
  virtual void OnTextUpdated() = 0;
  virtual void OnTextEnter() = 0;
};

class TextCtrlList : public wxTextCtrl {
public:
  TextCtrlList(wxWindow* parent, wxListCtrl* list);
  ~TextCtrlList();

  void set_callback(TextCtrlCallback* callback);

protected:
  void OnKeyUp(wxKeyEvent& event);
  void OnUpdated(wxCommandEvent& event);
  void OnEnter(wxCommandEvent& event);

  void OnFileDeselected(wxListEvent& event);
  void OnFileSelected(wxListEvent& event);

  void BindEvents();
private:
  wxListCtrl* list_;
  TextCtrlCallback* callback_;
  int last_selected_;
};

#endif  // RIDE_TEXTCTRLLIST_H
