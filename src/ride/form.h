// Copyright (2015) Gustav

#ifndef RIDE_FORM_H_
#define RIDE_FORM_H_

#include <ride/wx.h>

#include <wx/artprov.h>
#include <string>
#include <vector>

#include "ride/settings.h"

class wxEditableListBox;
class wxColourPickerCtrl;

//////////////////////////////////////////////////////////////////////////
// entry data
void OnlyAllowNumberChars(wxKeyEvent& event);

//////////////////////////////////////////////////////////////////////////
// form images

void SetImageAndRemoveText(wxButton* button, const wxBitmap& img);

void SetImageAndRemoveText(wxButton* button, const wxArtID art);
void SetImageAndRemoveText(wxButton* button, const char* const* xpm);

//////////////////////////////////////////////////////////////////////////
// dialog data to gui and back

#define RETURN_COMBOBOX_VALUE(TYPE, VALUE) \
  return static_cast<ride::TYPE>(VALUE)
#define DIALOG_DATA(ROOT, FUN, UI, SETNAME) \
  do {                                      \
    if (togui) {                            \
      ToGui(ROOT.FUN, UI);                \
    } else {                                \
      ROOT.FUN = ToData##SETNAME(UI);  \
    }                                       \
  } while (false)
#define DIALOG_DATAX(ROOT, FUN, UI)                   \
  do {                                                \
    if (togui) {                                      \
      ToGui(ROOT.FUN, UI);                          \
    } else {                                          \
      ROOT.FUN = ToData(UI); \
    }                                                 \
  } while (false)

void ToGui(bool data, wxCheckBox* gui);
bool ToData(wxCheckBox* gui);

void ToGui(int data, wxTextCtrl* gui);
int ToData_I32(wxTextCtrl* gui);

void ToGui(std::string data, wxListBox* gui);
std::string ToData(wxListBox* gui);

void ToGui(const wxString& data, wxTextCtrl* gui);
wxString ToData_Str(wxTextCtrl* gui);

void ToGui(const std::vector<wxString>& data, wxEditableListBox* gui);
std::vector<wxString> ToData(wxEditableListBox* gui);

void ToGui(const std::vector<wxString>& data, wxListBox* gui);
std::vector<wxString> ToData_Content(wxListBox* gui);

void ToGui(ride::Color data, wxColourPickerCtrl* gui);
ride::Color ToData(wxColourPickerCtrl* gui);

#endif  // RIDE_FORM_H_
