#include "ride/enabledisable.h"
#include "ride/wx.h"

EnableDisable::EnableDisable(bool enable) : enable_(enable) {}

const EnableDisable& EnableDisable::operator()(wxWindowBase* b) const {
  b->Enable(enable_);
  return *this;
}
