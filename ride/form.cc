#include "ride/wx.h"
#include "ride/form.h"
#include "wx/editlbox.h"

//////////////////////////////////////////////////////////////////////////

void ToGui(bool data, wxCheckBox* gui)  {
  gui->SetValue(data);
}
bool ToData(wxCheckBox* gui)  {
  return gui->GetValue();
}

//////////////////////////////////////////////////////////////////////////

void ToGui(google::protobuf::int32 data, wxTextCtrl* gui)  {
  wxString value = wxString::FromDouble(data, 0);
  gui->SetValue(value);
}
google::protobuf::int32 ToData_I32(wxTextCtrl* gui)  {
  const wxString value = gui->GetValue();
  long ret = 0;
  if (true == value.ToLong(&ret)) {
    return ret;
  }
  if (value.length() == 0) return -1;
  assert(false && "Unable to get integer value");
  return -1;
}

void ToGui(std::string data, wxListBox* gui)  {
  if (data == "") {
    gui->SetSelection(wxNOT_FOUND);
    return;
  }

  wxString str = wxString::FromUTF8(data.c_str());
  int index = gui->FindString(str);
  if (index == wxNOT_FOUND) {
    index = gui->GetCount();
    gui->AppendString(str);
  }
  gui->EnsureVisible(index);
  gui->SetSelection(index);
}

std::string ToData(wxListBox* gui)  {
  int selected = gui->GetSelection();
  if (selected == wxNOT_FOUND) return "";
  else return std::string(gui->GetString(selected).ToUTF8());
}

void ToGui(const wxString& data, wxTextCtrl* gui) {
  gui->SetValue(data);
}
wxString ToData_Str(wxTextCtrl* gui) {
  return gui->GetValue();
}

void ToGui(const std::vector<wxString>& data, wxEditableListBox * gui)
{
  wxArrayString wxarray(data.size(), &(data[0]));
  gui->SetStrings(wxarray);
}
std::vector<wxString> ToData(wxEditableListBox * gui)
{
  wxArrayString wxarray;
  gui->GetStrings(wxarray);
  std::vector<wxString> ret(wxarray.size());
  for (const wxString& s : wxarray) {
    ret.push_back(s);
  }
  return ret;
}
