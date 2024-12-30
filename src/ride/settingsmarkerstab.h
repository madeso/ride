#ifndef RIDE_SETTINGSMARKERSTAB_H_
#define RIDE_SETTINGSMARKERSTAB_H_

#include <ride/wx.h>
#include "ride/generated/ui.h"
#include "ride/settingscommon.h"

class MainWindow;

class SettingsMarkersTab : public ui::SettingsMarkersPanel
{
public:

	SettingsMarkersTab(wxWindow* parent, SettingsCommon* common);
	void MarkerToGui(bool togui);

protected:

	void OnMarkerListChanged(wxCommandEvent& event);
	void OnMarkerComboChanged(wxCommandEvent& event);
	void OnMarkerColorChanged(wxColourPickerEvent& event);
	void OnMarkerComboChanged(wxColourPickerEvent& event);

private:

	SettingsCommon* common_;

	bool allow_send_marker_to_main_;
	void SendMarkerToMain();
};

#endif	// RIDE_SETTINGSMARKERSTAB_H_
