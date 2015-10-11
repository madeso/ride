// Copyright (2015) Gustav

#include "ride/form.h"
#include <ride/wx.h>
#include <wx/editlbox.h>

#include "ride/wxid.h"

//////////////////////////////////////////////////////////////////////////

void OnlyAllowNumberChars(wxKeyEvent& event) {
  const wxString NUMBERS = "0123456789\b";
  if (event.m_uniChar == 0) {
    event.Skip();
  }
  int index = NUMBERS.Index(event.m_uniChar);
  if (index < 0) {
    // event.Skip();
  } else {
    event.Skip();
  }
}

//////////////////////////////////////////////////////////////////////////

void SetImageAndRemoveText(wxButton* button, const wxBitmap& img) {
  button->SetLabel("");
  button->SetBitmapLabel(img);
  const auto size = 28;
  button->SetSize(size, size);
}

void SetImageAndRemoveText(wxButton* button, const wxArtID art) {
  SetImageAndRemoveText(
      button, wxArtProvider::GetBitmap(art, wxART_BUTTON, wxSize(16, 16)));
}
void SetImageAndRemoveText(wxButton* button, const char* const* xpm) {
  SetImageAndRemoveText(button, wxBitmap(xpm));
}

//////////////////////////////////////////////////////////////////////////

void ToGui(bool data, wxCheckBox* gui) { gui->SetValue(data); }
bool ToData(wxCheckBox* gui) { return gui->GetValue(); }

void ToGui(google::protobuf::int32 data, wxTextCtrl* gui) {
  wxString value = wxString::FromDouble(data, 0);
  gui->SetValue(value);
}
google::protobuf::int32 ToData_I32(wxTextCtrl* gui) {
  const wxString value = gui->GetValue();
  WXID ret = 0;
  if (true == value.ToLong(&ret)) {
    return ret;
  }
  if (value.length() == 0) {
    return -1;
  }

  assert(false && "Unable to get integer value");
  return -1;
}

void ToGui(std::string data, wxListBox* gui) {
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

std::string ToData(wxListBox* gui) {
  int selected = gui->GetSelection();
  if (selected == wxNOT_FOUND)
    return "";
  else
    return std::string(gui->GetString(selected).ToUTF8());
}

void ToGui(const wxString& data, wxTextCtrl* gui) { gui->SetValue(data); }
wxString ToData_Str(wxTextCtrl* gui) { return gui->GetValue(); }

void ToGui(const std::vector<wxString>& data, wxEditableListBox* gui) {
  wxArrayString wxarray(data.size(), &(data[0]));
  gui->SetStrings(wxarray);
}
std::vector<wxString> ToData(wxEditableListBox* gui) {
  wxArrayString wxarray;
  gui->GetStrings(wxarray);
  std::vector<wxString> ret(wxarray.size());
  for (const wxString& s : wxarray) {
    ret.push_back(s);
  }
  return ret;
}

void ToGui(const std::vector<wxString>& data, wxListBox* gui) {
  gui->Clear();
  if (data.empty()) return;
  gui->InsertItems(data.size(), &(data[0]), 0);
}

std::vector<wxString> ToData_Content(wxListBox* gui) {
  const wxArrayString content = gui->GetStrings();
  std::vector<wxString> ret(content.size());
  for (const wxString& c : content) {
    ret.push_back(c);
  }
  return ret;
}
