#include <ride/wx.h>

#include "settings.proto.h"	 // NOLINT this is how we include it

#include "ride/mainwindow.h"

class MyApp : public wxApp
{
public:

	bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	const wxString DISPLAY_NAME = "Ride";

	SetAppName("ride");
	SetAppDisplayName(DISPLAY_NAME);
	SetVendorName("madeso");
	SetVendorDisplayName("madeso");

	auto main_window = new MainWindow(DISPLAY_NAME, wxPoint(50, 50), wxSize(450, 340));
	main_window->Show(true);
	return true;
}
