// TODO(gustav) move to a config or a better place
// #define RIDE_USE_WEBVIEW

#include "ride/startpage.h"

#include <ride/wx.h>

#include "ride_compiler_settings.h"	 // NOLINT this is how we include it

#include <wx/uri.h>
#ifdef RIDE_USE_WEBVIEW
	#include <wx/webview.h>
	#ifndef wxUSE_WEBVIEW
		#error "need webview support"
	#endif
#endif

#include <wx/aui/aui.h>

#ifndef RIDE_USE_WEBVIEW
	#include <wx/html/htmlwin.h>
#endif

#include "ride/mainwindow.h"

StartPageTab::StartPageTab(wxAuiNotebook* anotebook, MainWindow* parent)
	: wxControl(parent, wxID_ANY)
	, tab_(this)
	, notebook_(anotebook)
{
	this->SetClientData(&tab_);

#ifdef RIDE_USE_WEBVIEW
	wxWebView* ctrl = wxWebView::New(
		this, wxID_ANY, "http://rust-ide.tumblr.com/", wxDefaultPosition, wxSize(400, 300)
	);
#else
	wxHtmlWindow* ctrl = new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
	wxString html
		= "<html><body>"
		  "<h3>Welcome to RIDE</h3>"
		  "<br/><b>Overview</b><br/>"
		  "<p>RIDE is a Rust IDE.</p>"
		  "</body></html>";
	ctrl->SetPage(html);
#endif

	auto sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(ctrl, 1, wxEXPAND);
	SetSizer(sizer);

	notebook_->AddPage(this, wxT("Start"), true);
}

StartPageTab::~StartPageTab()
{
}
