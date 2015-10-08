// Copyright (2015) Gustav

#ifndef RIDE_TAB_H_
#define RIDE_TAB_H_

class StartPageTab;
class FileEdit;
class wxControl;

class Tab {
 public:
  explicit Tab(FileEdit* edit);
  explicit Tab(StartPageTab* start);
  ~Tab();

  StartPageTab* ToStartPage();
  FileEdit* ToFileEdit();
  wxControl* ToControl();

 private:
  FileEdit* edit_;
  StartPageTab* start_;
};

#endif  // RIDE_TAB_H_
