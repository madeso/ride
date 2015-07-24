#include "ride/wx.h"
#include <wx/aui/aui.h>
#include <wx/uri.h>
#include "ride/mainwindow.h"
#include "ride/startpage.h"

#define USE_WEBVIEW

#ifdef USE_WEBVIEW
#include <wx/webview.h>

#ifndef wxUSE_WEBVIEW
#error need webview support
#endif

#else
#include <wx/html/htmlwin.h>
#endif

StartPageTab::StartPageTab(wxAuiNotebook* anotebook, MainWindow* parent)
  : wxControl(parent, wxID_ANY)
  , tab_(this)
  , main_(parent)
  , notebook_(anotebook)
{
  this->SetClientData(&tab_);

#ifdef USE_WEBVIEW
  wxWebView* ctrl = wxWebView::New(this, wxID_ANY, "http://rust-ide.tumblr.com/", wxDefaultPosition, wxSize(400, 300));
#else
  wxHtmlWindow* ctrl = new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
  wxString html = 
    "<html><body>"
    "<h3>Welcome to RIDE</h3>"
    "<br/><b>Overview</b><br/>"
    "<p>RIDE is a Rust IDE.</p>"
    "</body></html>";
  ctrl->SetPage(html);
#endif

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(ctrl, 1, wxEXPAND);
  SetSizer(sizer);

  notebook_->AddPage(this, wxT("Start"), true);
}

StartPageTab::~StartPageTab() {
}

wxBEGIN_EVENT_TABLE(StartPageTab, wxControl)
wxEND_EVENT_TABLE()
