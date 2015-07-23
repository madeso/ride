#include "ride/tab.h"

#include "ride/fileedit.h"

Tab::Tab(FileEdit* edit) : edit_(edit) {
  assert(edit_);
}

Tab::~Tab() {
}

FileEdit* Tab::ToFileEdit() {
  assert(this);
  assert(edit_);
  return edit_;
}
