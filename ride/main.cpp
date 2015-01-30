#include <wx/wx.h>

#include "ride/mainwindow.h"

class MyApp: public wxApp
{
public:
  virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MainWindow *frame = new MainWindow("RIDE - Rust IDE", wxPoint(50, 50), wxSize(450, 340) );
	frame->Show( true );
	return true;
}
