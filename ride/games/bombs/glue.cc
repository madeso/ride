#include "glue.h"

#include <ride/wx.h>
#include <wx/aui/aui.h>
#include "ride/mainwindow.h"
#include "ride/tab.h"

#include "bombs.h"

class BombGameTab : public wxControl, TabData {
public:
  BombGameTab(wxAuiNotebook* anotebook, MainWindow* parent);
  ~BombGameTab() {}

  wxWindow* ToControl() {
    return m_frame;
  }

  virtual void AppendToMenu(wxMenuBar* menu) {
    SetupBombMenu(menu);
  }

private:
  Tab tab_;
  MainWindow* main_;
  wxAuiNotebook* notebook_;

  BombsFrame *m_frame;
  BombsGame m_game;
};

BombGameTab::BombGameTab(wxAuiNotebook* anotebook, MainWindow* parent)
  : wxControl(parent, wxID_ANY),
  tab_(this),
  main_(parent),
  notebook_(anotebook) {
  this->SetClientData(&tab_);

  m_frame = new BombsFrame(this, &m_game);

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(m_frame, 1, wxEXPAND);
  SetSizer(sizer);

  notebook_->AddPage(this, wxT("Bombs"), true);
}

//////////////////////////////////////////////////////////////////////////

void CreateBombGame(wxAuiNotebook* anotebook, MainWindow* parent)
{
  new BombGameTab(anotebook, parent);
}
