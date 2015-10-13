#include "forty_glue.h"

#include <ride/wx.h>
#include <wx/aui/aui.h>
#include "ride/mainwindow.h"
#include "ride/tab.h"

#include "forty.h"

class FortyGameTab : public wxControl, TabData {
public:
  FortyGameTab(wxAuiNotebook* anotebook, MainWindow* parent);
  ~FortyGameTab() {}

  wxWindow* ToControl() {
    return m_frame;
  }

  virtual void AppendToMenu(wxMenuBar* menu) {
    m_frame->SetupFortyMenu(menu);
  }

private:
  Tab tab_;
  MainWindow* main_;
  wxAuiNotebook* notebook_;

  FortyFrame *m_frame;
};

FortyGameTab::FortyGameTab(wxAuiNotebook* anotebook, MainWindow* parent)
  : wxControl(parent, wxID_ANY),
  tab_(this),
  main_(parent),
  notebook_(anotebook) {
  this->SetClientData(&tab_);

  m_frame = new FortyFrame(this);

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);
  sizer2->Add(m_frame, 1, wxALIGN_CENTER, 0, NULL);
  sizer->Add(sizer2, 1, wxALIGN_CENTER, 0, NULL);
  SetSizer(sizer);

  notebook_->AddPage(this, wxT("Forty thieves"), true);
}

//////////////////////////////////////////////////////////////////////////

void CreateFortyGame(wxAuiNotebook* anotebook, MainWindow* parent)
{
  new FortyGameTab(anotebook, parent);
}
