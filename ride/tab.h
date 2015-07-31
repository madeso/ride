// Copyright (2015) Gustav

#ifndef RIDE_TAB_H_
#define RIDE_TAB_H_

class StartPageTab;
class FileEdit;

class Tab {
 public:
  Tab(FileEdit* edit);
  Tab(StartPageTab* start);
  ~Tab();

  StartPageTab* ToStartPage();
  FileEdit* ToFileEdit();

 private:
  FileEdit* edit_;
  StartPageTab* start_;
};

#endif  // RIDE_TAB_H_
