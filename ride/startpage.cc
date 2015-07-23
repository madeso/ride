#include "ride/wx.h"
#include <wx/aui/aui.h>
#include <wx/uri.h>
#include "ride/mainwindow.h"
#include "ride/startpage.h"
#include <wx/webview.h>

StartPageTab::StartPageTab(wxAuiNotebook* anotebook, MainWindow* parent)
  : wxControl(parent, wxID_ANY)
  , tab_(this)
  , main_(parent)
  , notebook_(anotebook)
{
  this->SetClientData(&tab_);

  wxWebView* ctrl = wxWebView::New(parent, wxID_ANY, "http://rust-ide.tumblr.com/", wxDefaultPosition, wxSize(400, 300));

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(ctrl, 1, wxEXPAND);
  SetSizer(sizer);

  notebook_->AddPage(this, wxT("Start"), true);
}

StartPageTab::~StartPageTab() {
}

wxBEGIN_EVENT_TABLE(StartPageTab, wxControl)
wxEND_EVENT_TABLE()
