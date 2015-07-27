#ifndef RIDE_TAB_H
#define RIDE_TAB_H

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

#endif  // RIDE_TAB_H
