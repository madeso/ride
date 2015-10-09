/////////////////////////////////////////////////////////////////////////////
// Name:        switcherdlg.h
// Purpose:     Pane switcher dialog
// Author:      Julian Smart
// Modified by:
// Created:     2007-08-19
// RCS-ID:      $Id: switcherdlg.cpp,v 1.6 2007/08/20 17:38:24 anthemion Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "ride/switcher/dialog.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

#include "ride/switcher/style.h"

SwitcherDlg::SwitcherDlg(const SwitcherItemList& items, SwitcherIndex index,
                         const SwitcherStyle& sstyle, wxWindow* parent,
                         wxWindowID id, const wxString& title,
                         const wxPoint& position, const wxSize& size,
                         long style)  // NOLINT
    : list_ctrl_(NULL),
      path_ctrl_(NULL),
      is_closing_(false),
      switcher_border_style_(0),
      border_color_(sstyle.border_color()),
      style_(sstyle) {
  Bind(wxEVT_CLOSE_WINDOW, &SwitcherDlg::OnCloseWindow, this);
  Bind(wxEVT_ACTIVATE, &SwitcherDlg::OnActivate, this);
  Bind(wxEVT_LISTBOX, &SwitcherDlg::OnSelectItem, this, wxID_ANY);

  switcher_border_style_ = (style & wxBORDER_MASK);
  if (switcher_border_style_ == wxBORDER_NONE)
    switcher_border_style_ = wxBORDER_SIMPLE;

  style &= wxBORDER_MASK;
  style |= wxBORDER_NONE;

  wxDialog::Create(parent, id, title, position, size, style);

  list_ctrl_ = new SwitcherCtrl(items, style_);
  list_ctrl_->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                     wxWANTS_CHARS | wxNO_BORDER);
  list_ctrl_->CalculateLayout();

  title_ctrl_ = new wxStaticText(this, wxID_ANY, "");
  description_ctrl_ = new wxStaticText(this, wxID_ANY, "");
  wxFont font = title_ctrl_->GetFont();
  font.MakeBold();
  title_ctrl_->SetFont(font);
  path_ctrl_ = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition,
                                wxDefaultSize, wxST_ELLIPSIZE_START);

  wxBoxSizer* root = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

  const int mainborder = style_.dlg_main_border();
  const int border = style_.dlg_item_border();

  root->Add(sizer, 0, wxALL | wxEXPAND, mainborder);
  SetSizer(root);

  sizer->Add(title_ctrl_, 0, wxALL | wxEXPAND, border);
  sizer->Add(description_ctrl_, 0, wxALL | wxEXPAND, border);
  sizer->Add(list_ctrl_, 1, wxALL | wxEXPAND, border);
  sizer->Add(path_ctrl_, 0, wxALL | wxEXPAND, border);

  sizer->SetSizeHints(this);

  list_ctrl_->SetFocus();

  Centre(wxBOTH);

  list_ctrl_->SelectOrFirst(index);

  UpdateDescription();
}

void SwitcherDlg::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {  // NOLINT
  if (is_closing_) return;

  if (IsModal()) {
    is_closing_ = true;

    if (GetSelection() == SWITCHER_NOT_FOUND)
      EndModal(wxID_CANCEL);
    else
      EndModal(wxID_OK);
  }
}

void SwitcherDlg::OnActivate(wxActivateEvent& event) {
  if (!event.GetActive()) {
    if (!is_closing_) {
      is_closing_ = true;
      EndModal(wxID_CANCEL);
    }
  }
}

void SwitcherDlg::OnSelectItem(wxCommandEvent& event) { UpdateDescription(); }

// Get the selected item
SwitcherIndex SwitcherDlg::GetSelection() const {
  return list_ctrl_->selection();
}

void SwitcherDlg::ShowDescription(const SwitcherItem& item) {
  title_ctrl_->SetLabel(item.title());
  description_ctrl_->SetLabel(item.description());
  path_ctrl_->SetLabel(item.path());
}

void SwitcherDlg::set_border_color(const wxColour& colour) {
  border_color_ = colour;
}

void SwitcherDlg::AdvanceToNextSelection(bool forward) {
  list_ctrl_->AdvanceToNextSelection(forward);
  UpdateDescription();
}

void SwitcherDlg::UpdateDescription() {
  SwitcherIndex selected = list_ctrl_->selection();
  if (selected != SWITCHER_NOT_FOUND) {
    ShowDescription(list_ctrl_->items().GetItem(selected));
  }
}
