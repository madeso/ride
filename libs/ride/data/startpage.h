// Copyright (2015) Gustav

#ifndef RIDE_STARTPAGE_H_
#define RIDE_STARTPAGE_H_

#include <ride/wx.h>
#include "ride/tab.h"

class wxAuiNotebook;
class MainWindow;

class StartPageTab : public wxControl {
 public:
  StartPageTab(wxAuiNotebook* anotebook, MainWindow* parent);
  ~StartPageTab();

 private:
  Tab tab_;
  wxAuiNotebook* notebook_;
};

#endif  // RIDE_STARTPAGE_H_
