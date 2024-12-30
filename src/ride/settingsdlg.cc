#include "ride/settingsdlg.h"

#include <ride/wx.h>

#include <wx/simplebook.h>
#include <wx/vlbox.h>
#include <wx/settings.h>

#include <vector>
#include <algorithm>

#include "ride/generated/ui.h"

#include "ride/form.h"
#include "ride/mainwindow.h"
#include "ride/wxutils.h"
#include "ride/guilist.h"
#include "ride/proto.h"
#include "ride/settingscommon.h"
#include "ride/settingsfontstab.h"
#include "ride/settingsindicatortab.h"
#include "ride/settingsmarkerstab.h"
#include "ride/settingseditortab.h"
#include "ride/settingswindowtab.h"
#include "ride/settingsthemestab.h"
#include "ride/settingsenviromenttab.h"
#include "ride/togui.h"

struct RideListBoxItem
{
public:

	wxString title;
	wxPanel* data;
};

class RideListBox : public wxVListBox
{
private:

	std::vector<RideListBoxItem> items;

public:

	explicit RideListBox(wxWindow* parent)
		: wxVListBox(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0)
		, background_(255, 255, 255)
		, text_(0, 0, 0)
		, selected_background_(137, 175, 223)
		, selected_text_(255, 255, 255)
		, item_font_(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT))
		, height_(32)
		, spacing_(3)
		, img_size_(32)
	{
		wxVListBox::SetMinClientSize(wxSize(120, 120));
		wxVListBox::SetBackgroundColour(background_);
		wxVListBox::SetOwnBackgroundColour(background_);
	}

	void OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const override
	{
		if (IsSelected(n))
		{
			dc.SetBrush(wxBrush(selected_background_));
			dc.SetPen(wxPen(selected_background_));
		}
		else
		{
			dc.SetBrush(wxBrush(background_));
			dc.SetPen(wxPen(background_));
		}
		dc.DrawRectangle(rect);

		if (IsSelected(n))
		{
			dc.SetPen(wxPen(selected_text_));
			dc.SetBrush(wxBrush(selected_text_));
		}
		else
		{
			dc.SetPen(wxPen(text_));
			dc.SetBrush(wxBrush(text_));
		}
		dc.SetFont(item_font_);
		int w, h;
		dc.GetTextExtent(items[n].title, &w, &h);
		wxPoint p = rect.GetTopLeft();
		p.y += std::max(0, (height_ - h) / 2);
		p.x += img_size_ + spacing_ * 2;
		dc.DrawText(items[n].title, p);
	}

	wxCoord OnMeasureItem(size_t n) const override
	{
		return height_;
	}

	void InsertItem(const RideListBoxItem& item)
	{
		items.push_back(item);
		SetItemCount(items.size());
	}

	wxPanel* GetClientData(int n)
	{
		return items[n].data;
	}

	wxString GetItemText(int n)
	{
		return items[n].title;
	}

private:

	wxColor background_;
	wxColor text_;
	wxColor selected_background_;
	wxColor selected_text_;
	wxFont item_font_;
	int height_;
	int spacing_;
	int img_size_;
};

std::vector<int> GetSelection(RideListBox* box)
{
	std::vector<int> ret;
	ret.push_back(box->GetSelection());
	return ret;
}

//////////////////////////////////////////////////////////////////////////

void AddItem(wxListCtrl* list, int id, const wxString& display, wxPanel* data)
{
	wxListItem item;
	item.SetData(data);
	item.SetImage(id);
	item.SetText(display);
	item.SetId(id);
	list->InsertItem(item);
}

void AddItem(RideListBox* list, int id, const wxString& display, wxPanel* data)
{
	RideListBoxItem item;
	item.data = data;
	item.title = display;
	list->InsertItem(item);
}

WXID g_last_selection_ = 0;
int g_last_width = 623;
int g_last_height = 665;

class SettingsDlg
	: public wxDialog
	, ToGuiSender
{
public:

	SettingsCommon common_;
	wxSimplebook* notebook;
	RideListBox* nootebook_ctrl;
	wxPanel* m_null;
	SettingsFontsTab* m_fonts;
	SettingsIndicatorTab* m_indicators;
	SettingsMarkersTab* m_markers;
	SettingsEditorTab* m_editor;
	SettingsWindowTab* m_window;
	SettingsThemesTab* m_themes;
	SettingsEnviromentTab* m_enviroment;
	wxStaticText* selection_info;

	SettingsDlg(wxWindow* parent, MainWindow* mainwindow)
		: wxDialog(
			  parent,
			  wxID_ANY,
			  "Settings",
			  wxDefaultPosition,
			  wxDefaultSize,
			  wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
		  )
		, common_(mainwindow)
	{
		this->SetSizeHints(wxSize(623, 665), wxDefaultSize);

		auto main_sizer = new wxBoxSizer(wxVERTICAL);
		auto notebook_sizer = new wxFlexGridSizer(0, 2, 0, 0);
		notebook_sizer->AddGrowableCol(1);
		notebook_sizer->AddGrowableRow(1);
		notebook_sizer->SetFlexibleDirection(wxBOTH);
		notebook_sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

		selection_info = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
		selection_info->SetFont(
			wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD)
		);

		notebook = new wxSimplebook(this, wxID_ANY);
		m_null = new wxPanel(notebook);
		m_fonts = new SettingsFontsTab(notebook, &common_);
		m_indicators = new SettingsIndicatorTab(notebook, &common_);
		m_markers = new SettingsMarkersTab(notebook, &common_);
		m_editor = new SettingsEditorTab(notebook, &common_);
		m_window = new SettingsWindowTab(notebook, &common_);
		m_themes = new SettingsThemesTab(notebook, &common_, this);
		m_enviroment = new SettingsEnviromentTab(notebook, &common_);

		notebook->AddPage(m_fonts, "");
		notebook->AddPage(m_indicators, "");
		notebook->AddPage(m_markers, "");
		notebook->AddPage(m_editor, "");
		notebook->AddPage(m_window, "");
		notebook->AddPage(m_themes, "");
		notebook->AddPage(m_enviroment, "");

		nootebook_ctrl = new RideListBox(this);
		AddItem(nootebook_ctrl, 0, "Fonts", m_fonts);
		AddItem(nootebook_ctrl, 1, "Indicators", m_indicators);
		AddItem(nootebook_ctrl, 2, "Markers", m_markers);
		AddItem(nootebook_ctrl, 3, "Editor", m_editor);
		AddItem(nootebook_ctrl, 4, "Window", m_window);
		AddItem(nootebook_ctrl, 5, "Themes", m_themes);
		AddItem(nootebook_ctrl, 6, "Environment", m_enviroment);

		notebook_sizer->AddStretchSpacer();	 // find ctrl
		notebook_sizer->Add(selection_info);
		notebook_sizer->Add(nootebook_ctrl, 0, wxALL | wxEXPAND, 3);
		notebook_sizer->Add(notebook, 1, wxEXPAND | wxALL, 3);

		main_sizer->Add(notebook_sizer, 1, wxEXPAND | wxALL, 5);

		wxButton* ok_button = new wxButton(this, wxID_OK);
		wxButton* apply_button = new wxButton(this, wxID_APPLY);
		wxButton* cancel_buytton = new wxButton(this, wxID_CANCEL);

		auto button_sizer = new wxStdDialogButtonSizer();

		button_sizer->AddButton(ok_button);
		button_sizer->AddButton(apply_button);
		button_sizer->AddButton(cancel_buytton);
		button_sizer->Realize();
		button_sizer->SetMinSize(wxSize(-1, 50));

		main_sizer->Add(button_sizer, 0, wxEXPAND | wxFIXED_MINSIZE, 5);

		this->SetSizer(main_sizer);
		wxDialog::Layout();

		this->SetSize(g_last_width, g_last_height);

		this->Centre(wxBOTH);

		nootebook_ctrl->Bind(wxEVT_LIST_ITEM_SELECTED, &SettingsDlg::SelectionChanged, this);
		nootebook_ctrl->Bind(wxEVT_LISTBOX, &SettingsDlg::SelectionChangedCommand, this);

		apply_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingsDlg::OnApply, this);
		cancel_buytton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingsDlg::OnCancel, this);
		ok_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SettingsDlg::OnOk, this);

		this->Bind(wxEVT_SIZE, &SettingsDlg::OnSize, this);

		SetSelection(g_last_selection_);
	}

	void OnSize(wxSizeEvent& event)
	{
		wxDialog::OnSize(event);
		const wxSize s = event.GetSize();
		g_last_width = s.GetWidth();
		g_last_height = s.GetHeight();
	}

	void SendToGui(bool togui) override
	{
		m_editor->EditToGui(false);
		m_fonts->StyleToGui(togui);
		m_markers->MarkerToGui(false);
		m_window->WindowToGui(false);
		m_indicators->IndicatorToGui(togui);
		m_themes->ThemeToGui(togui);
		m_enviroment->SettingsToGui(togui);
	}

	void OnApply(wxCommandEvent& event)
	{
		// StyleSaveSelectedIndex();
		common_.UpdateMain();
	}

	void OnCancel(wxCommandEvent& event)
	{
		common_.Revert();
		common_.UpdateMain();
		// StyleSaveSelectedIndex();
		EndModal(wxCANCEL);
	}

	void OnOk(wxCommandEvent& event)
	{
		// StyleSaveSelectedIndex();
		SendToGui(false);
		common_.Apply(this);
		EndModal(wxOK);
	}

	void SelectionChangedCommand(wxCommandEvent& event)
	{
		DoSelectionChanged();
	}

	void SelectionChanged(wxListEvent& event)
	{
		DoSelectionChanged();
	}

	void DoSelectionChanged()
	{
		const auto selection = GetSelection(nootebook_ctrl);

		if (! selection.empty() && selection.size() == 1)
		{
			SetSelection(selection[0]);
		}
		else
		{
			SetSelection(-1);
		}
	}

	void SetSelection(WXID selection)
	{
		// remember selection
		g_last_selection_ = selection;

		wxPanel* panel = m_null;
		wxString text = "";

		if (selection != -1)
		{
			panel = reinterpret_cast<wxPanel*>(nootebook_ctrl->GetClientData(selection));
			assert(panel);
			text = nootebook_ctrl->GetItemText(selection);
		}

		int index = notebook->FindPage(panel);
		if (index != -1)
		{
			notebook->SetSelection(index);
			selection_info->SetLabelText(text);
		}
	}
};

void ShowSettingsDlg(wxWindow* parent, MainWindow* mainwindow)
{
	SettingsDlg dlg(parent, mainwindow);
	dlg.ShowModal();
}
