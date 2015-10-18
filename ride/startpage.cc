// Copyright (2015) Gustav

// TODO(gustav) move to a config or a better place
#define USE_WEBVIEW

#include "ride/startpage.h"

#include <ride/wx.h>

#include <wx/uri.h>
#ifdef USE_WEBVIEW
#include <wx/webview.h>
#ifndef wxUSE_WEBVIEW
#error need webview support
#endif
#endif

#include <wx/aui/aui.h>

#ifndef USE_WEBVIEW
#include <wx/html/htmlwin.h>
#endif

#include "ride/mainwindow.h"

StartPageTab::StartPageTab(wxAuiNotebook* anotebook, MainWindow* parent)
    : wxControl(parent, wxID_ANY), tab_(this), notebook_(anotebook) {
  this->SetClientData(&tab_);

#ifdef USE_WEBVIEW
  wxWebView* ctrl =
      wxWebView::New(this, wxID_ANY, "http://rust-ide.tumblr.com/",
                     wxDefaultPosition, wxSize(400, 300));
#else
  wxHtmlWindow* ctrl =
      new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
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

StartPageTab::~StartPageTab() {}
