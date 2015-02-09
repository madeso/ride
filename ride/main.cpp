#include "ride/wx.h"

#include "ride/mainwindow.h"
#include "settings.pb.h"

class MyApp: public wxApp
{
public:
  virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  wxAppConsole* c = wxAppConsole::GetInstance();
  c->SetAppName("ride");
  c->SetAppDisplayName("RIDE");
  c->SetVendorName("madeso");
  c->SetVendorDisplayName("madeso");

	MainWindow *frame = new MainWindow("RIDE - Rust IDE", wxPoint(50, 50), wxSize(450, 340) );
	frame->Show( true );
	return true;
}
