// Copyright (2015) Gustav

#include "ride/tab.h"

#include "ride/wx.h"
#include <cassert>

Tab::Tab(FileEdit* edit) : edit_(edit), start_(NULL) { assert(edit); }

Tab::Tab(StartPageTab* start) : edit_(NULL), start_(start) { assert(start); }

Tab::~Tab() {}

FileEdit* Tab::ToFileEdit() {
  assert(this);
  return edit_;
}

StartPageTab* Tab::ToStartPage() {
  assert(this);
  return start_;
}
