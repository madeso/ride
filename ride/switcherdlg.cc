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

#include "ride/switcherdlg.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

SwitcherDlg::SwitcherDlg(const SwitcherItemList& items, wxWindow* parent,
                         wxWindowID id, const wxString& title,
                         const wxPoint& position, const wxSize& size,
                         long style)  // NOLINT
    : list_ctrl_(NULL),
      path_ctrl_(NULL),
      is_closing_(false),
      switcher_border_style_(0),
      modifier_key_(-1),
      extra_navigation_key_(-1),
      border_color_(*wxBLACK) {
  Bind(wxEVT_CLOSE_WINDOW, &SwitcherDlg::OnCloseWindow, this);
  Bind(wxEVT_ACTIVATE, &SwitcherDlg::OnActivate, this);
  Bind(wxEVT_LISTBOX, &SwitcherDlg::OnSelectItem, this, wxID_ANY);
  Bind(wxEVT_PAINT, &SwitcherDlg::OnPaint, this);

  switcher_border_style_ = (style & wxBORDER_MASK);
  if (switcher_border_style_ == wxBORDER_NONE)
    switcher_border_style_ = wxBORDER_SIMPLE;

  style &= wxBORDER_MASK;
  style |= wxBORDER_NONE;

  wxDialog::Create(parent, id, title, position, size, style);

  list_ctrl_ = new SwitcherCtrl();
  list_ctrl_->set_items(items);
  list_ctrl_->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                     wxWANTS_CHARS | wxNO_BORDER);
  list_ctrl_->CalculateLayout();

  if (extra_navigation_key_ != -1)
    list_ctrl_->set_extra_navigation_key(extra_navigation_key_);

  if (modifier_key_ != -1) list_ctrl_->set_modifier_key(modifier_key_);

  int borderStyle = wxBORDER_NONE;
#if defined(__WXMSW__) && wxCHECK_VERSION(2, 8, 5)
// borderStyle = wxBORDER_THEME;
#endif

  title_ctrl_ = new wxStaticText(this, wxID_ANY, "");
  description_ctrl_ = new wxStaticText(this, wxID_ANY, "");
  // title_ctrl_->SetFont(title_ctrl_->GetFont().Set)
  path_ctrl_ = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition,
                                wxDefaultSize, wxST_ELLIPSIZE_START);

#ifdef __WXGTK20__
  int fontSize = 11;
  path_ctrl_->SetStandardFonts(fontSize);
#endif

  wxBoxSizer* root = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  root->Add(sizer, 1, wxALL | wxEXPAND, 8);
  SetSizer(root);

  const int border = 3;
  sizer->Add(title_ctrl_, 0, wxALL | wxEXPAND, border);
  sizer->Add(description_ctrl_, 0, wxALL | wxEXPAND, border);
  sizer->Add(list_ctrl_, 1, wxALL | wxEXPAND, border);
  sizer->Add(path_ctrl_, 0, wxALL | wxEXPAND, border);

  sizer->SetSizeHints(this);

  list_ctrl_->SetFocus();

  Centre(wxBOTH);

  if (list_ctrl_->items().selection() == -1)
    list_ctrl_->items().set_selection(0);

  list_ctrl_->MakeSureGroupIsNotSelected(1);

  UpdateDescription();
}

void SwitcherDlg::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {  // NOLINT
  if (is_closing_) return;

  if (IsModal()) {
    is_closing_ = true;

    if (GetSelection() == -1)
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

void SwitcherDlg::OnSelectItem(wxCommandEvent& event) {
  ShowDescription(event.GetSelection());
}

// Get the selected item
int SwitcherDlg::GetSelection() const {
  return list_ctrl_->items().selection();
}

void SwitcherDlg::ShowDescription(int i) {
  SwitcherItem& item = list_ctrl_->items().GetItem(i);
  title_ctrl_->SetLabel(item.title());
  description_ctrl_->SetLabel(item.description());
  path_ctrl_->SetLabel(item.path());
}

void SwitcherDlg::set_border_color(const wxColour& colour) {
  border_color_ = colour;
}

void SwitcherDlg::set_extra_navigation_key(int keyCode) {
  extra_navigation_key_ = keyCode;
  if (list_ctrl_) list_ctrl_->set_extra_navigation_key(keyCode);
}

int SwitcherDlg::extra_navigation_key() const { return extra_navigation_key_; }

void SwitcherDlg::set_modifier_key(int modifierKey) {
  modifier_key_ = modifierKey;
  if (list_ctrl_) list_ctrl_->set_modifier_key(modifierKey);
}

int SwitcherDlg::modifier_key() const { return modifier_key_; }

void SwitcherDlg::AdvanceToNextSelection(bool forward) {
  list_ctrl_->AdvanceToNextSelection(forward);
  UpdateDescription();
}

void SwitcherDlg::UpdateDescription() {
  ShowDescription(list_ctrl_->items().selection());
}
