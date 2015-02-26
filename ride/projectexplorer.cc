#include "ride/projectexplorer.h"

ProjectExplorer::ProjectExplorer(wxWindow* parent)
  : wxTreeCtrl(parent, wxID_ANY) {
  this->AppendItem(this->GetRootItem(), "Hello");
  this->AppendItem(this->GetRootItem(), "World");
  this->AppendItem(this->GetRootItem(), "This");
  this->AppendItem(this->GetRootItem(), "is");
  this->AppendItem(this->GetRootItem(), "fun");
}
