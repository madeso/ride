// Copyright (2015) Gustav

#ifndef RIDE_SETTINGSTHEMESTAB_H_
#define RIDE_SETTINGSTHEMESTAB_H_

#include <ride/wx.h>

#include <algorithm>

#include "ride/generated/ui.h"
#include "ride/settingscommon.h"
#include "ride/guilist.h"
#include "ride/togui.h"

class MainWindow;

struct ThemeFunctions {
  static const wxString ADD_TEXT;
  static const wxString EDIT_TEXT;

  static int Size(ride::Settings* bs) { return bs->themes_size(); }

  static wxString GetDisplayString(ride::Settings* bs, int i) {
    return bs->themes(i).name();
  }

  static void SetDisplayString(ride::Settings* bs, int i,
                               const wxString& new_string) {
    bs->mutable_themes(i)->set_name(new_string);
  }

  static void Add(ride::Settings* bs, const wxString& name) {
    ride::Theme* theme = bs->add_themes();
    theme->set_name(name);
    theme->set_allocated_data(new ride::FontsAndColors(bs->fonts_and_colors()));
  }

  static void Remove(ride::Settings* bs, int i) {
    bs->mutable_themes()->DeleteSubrange(i, 1);
  }

  static void Swap(ride::Settings* bs, int selection, int next_index) {
    std::swap(*bs->mutable_themes(selection), *bs->mutable_themes(next_index));
  }
};

class SettingsThemesTab : public ui::SettingsThemesPanel {
 public:
  SettingsThemesTab(wxWindow* parent, SettingsCommon* common,
                    ToGuiSender* togui);
  void ThemeToGui(bool togui);

 protected:
  SettingsCommon* common_;
  void OnThemeApplySelected(wxCommandEvent& event);
  void OnThemeExportSelected(wxCommandEvent& event);
  void OnThemeImport(wxCommandEvent& event);

  void OnAdd(wxCommandEvent& event);
  void OnEdit(wxCommandEvent& event);
  void OnRemove(wxCommandEvent& event);
  void OnUp(wxCommandEvent& event);
  void OnDown(wxCommandEvent& event);

 private:
  ToGuiSender* main_;
  GuiList<ride::Settings, ThemeFunctions> theme_list_;
  bool allow_to_gui_;
};

#endif  // RIDE_SETTINGSTHEMESTAB_H_
