#include "bombs_glue.h"

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
  wxAuiNotebook* notebook_;

  BombsFrame *m_frame;
  BombsGame m_game;
};

BombGameTab::BombGameTab(wxAuiNotebook* anotebook, MainWindow* parent)
  : wxControl(parent, wxID_ANY),
  tab_(this),
  notebook_(anotebook) {
  this->SetClientData(&tab_);

  m_frame = new BombsFrame(this, &m_game);

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);
  sizer2->Add(m_frame, 1, wxALIGN_CENTER, 0, NULL);
  sizer->Add(sizer2, 1, wxALIGN_CENTER, 0, NULL);
  SetSizer(sizer);

  SetBackgroundColour(*wxLIGHT_GREY);
  SetForegroundColour(*wxLIGHT_GREY);

  notebook_->AddPage(this, wxT("Bombs"), true);
}

//////////////////////////////////////////////////////////////////////////

void CreateBombGame(wxAuiNotebook* anotebook, MainWindow* parent)
{
  new BombGameTab(anotebook, parent);
}
