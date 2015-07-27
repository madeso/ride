// Copyright (2015) Gustav

#include "ride/wx.h"

#include "ride/mainwindow.h"
#include "settings.pb.h"

class MyApp : public wxApp {
 public:
  virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  const wxString DISPLAY_NAME = "Ride";

  wxAppConsole* app_console = wxAppConsole::GetInstance();
  app_console->SetAppName("ride");
  app_console->SetAppDisplayName(DISPLAY_NAME);
  app_console->SetVendorName("madeso");
  app_console->SetVendorDisplayName("madeso");

  MainWindow* main_window =
      new MainWindow(DISPLAY_NAME, wxPoint(50, 50), wxSize(450, 340));
  main_window->Show(true);
  return true;
}
