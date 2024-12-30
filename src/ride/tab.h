#ifndef RIDE_TAB_H_
#define RIDE_TAB_H_

class StartPageTab;
class FileEdit;
class wxWindow;
class wxMenuBar;

class TabData
{
public:

	virtual wxWindow* ToControl() = 0;
	virtual void AppendToMenu(wxMenuBar* menu) = 0;
};

class Tab
{
public:

	explicit Tab(FileEdit* edit);
	explicit Tab(StartPageTab* start);
	explicit Tab(TabData* data);
	~Tab();

	StartPageTab* ToStartPage();
	FileEdit* ToFileEdit();
	wxWindow* ToControl();
	TabData* ToData();

private:

	FileEdit* edit_;
	StartPageTab* start_;
	TabData* data_;
};

#endif	// RIDE_TAB_H_
