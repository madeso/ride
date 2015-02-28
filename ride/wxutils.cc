#include "ride/wxutils.h"
#include "ride/wx.h"

wxPoint GetContextEventPosition(const wxContextMenuEvent& event) {
  wxPoint ret = event.GetPosition();
  // according to the documentation: http://docs.wxwidgets.org/trunk/classwx_context_menu_event.html#a291e3437b4bf913128ea14e511d161cb
  if (ret == wxDefaultPosition) {
    return wxGetMousePosition();
  }
  else {
    return ret;
  }
}

void AppendEnabled(wxMenu& menu, int id, const wxString& text, bool enabled) {
  menu.Append(id, text);
  menu.Enable(id, enabled);
}
