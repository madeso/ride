#ifndef RIDE_SETTINGS_H
#define RIDE_SETTINGS_H

class Settings {
public:
  Settings();

  bool syntaxEnable;
  bool lineNumberEnable;
  bool foldEnable;
  bool indentEnable;
  bool displayEOLEnable;
  bool indentGuideEnable;
  bool longLineOnEnable;
  bool whiteSpaceEnable;
  bool overTypeInitial;
  bool wrapModeInitial;
};

#endif  // RIDE_SETTINGS_H