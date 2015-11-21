#include "dialog.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

#include "style.h"

namespace switcher {

Dialog::Dialog(const ItemList& items, Index index, int group, bool forward,
               const Style& sstyle, wxWindow* parent, wxWindowID id,
               const wxString& title, const wxPoint& position,
               const wxSize& size,
               long style)  // NOLINT
    : list_ctrl_(NULL),
      path_ctrl_(NULL),
      is_closing_(false),
      switcher_border_style_(0),
      style_(sstyle) {
  Bind(wxEVT_CLOSE_WINDOW, &Dialog::OnCloseWindow, this);
  Bind(wxEVT_ACTIVATE, &Dialog::OnActivate, this);
  Bind(wxEVT_LISTBOX, &Dialog::OnSelectItem, this, wxID_ANY);

  switcher_border_style_ = (style & wxBORDER_MASK);
  if (switcher_border_style_ == wxBORDER_NONE)
    switcher_border_style_ = wxBORDER_SIMPLE;

  style &= wxBORDER_MASK;
  style |= wxBORDER_NONE;

  wxDialog::Create(parent, id, title, position, size, style);

  list_ctrl_ = new Ctrl(items, style_);
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

  list_ctrl_->SelectOrFirst(index, group, forward);

  wxDialog::SetBackgroundColour(style_.base_color());
  const wxColor foreground = style_.dialog_color();
  wxDialog::SetForegroundColour(foreground);

  title_ctrl_->SetForegroundColour(foreground);
  description_ctrl_->SetForegroundColour(foreground);
  path_ctrl_->SetForegroundColour(foreground);

  UpdateDescription();
}

void Dialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {  // NOLINT
  if (is_closing_) return;

  if (IsModal()) {
    is_closing_ = true;

    if (GetSelection() == SWITCHER_NOT_FOUND)
      EndModal(wxID_CANCEL);
    else
      EndModal(wxID_OK);
  }
}

void Dialog::OnActivate(wxActivateEvent& event) {
  if (!event.GetActive()) {
    if (!is_closing_) {
      is_closing_ = true;
      EndModal(wxID_CANCEL);
    }
  }
}

void Dialog::OnSelectItem(wxCommandEvent& event) { UpdateDescription(); }

// Get the selected item
Index Dialog::GetSelection() const { return list_ctrl_->selection(); }

void Dialog::ShowDescription(const Item& item) {
  title_ctrl_->SetLabel(item.title());
  description_ctrl_->SetLabel(item.description());
  path_ctrl_->SetLabel(item.path());
}

void Dialog::UpdateDescription() {
  Index selected = list_ctrl_->selection();
  if (selected != SWITCHER_NOT_FOUND) {
    ShowDescription(list_ctrl_->items().GetItem(selected));
  }
}

}  // namespace switcher
