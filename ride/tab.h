#ifndef RIDE_TAB_H
#define RIDE_TAB_H

class FileEdit;

class Tab {
public:
  Tab(FileEdit* edit);
  ~Tab();

  FileEdit* ToFileEdit();

private:
  FileEdit* edit_;
};

#endif  // RIDE_TAB_H
