#ifndef COMMONIMAGES_H
#define COMMONIMAGES_H

namespace image {

  // taken from https://github.com/wxWidgets/wxWidgets/blob/master/src/generic/editlbox.cpp

static const char* const edit_xpm[] = {
  "16 16 3 1",
  "   c None",
  ".  c #000000",
  "+  c #00007F",
  "                ",
  "                ",
  "      .. ..     ",
  "        .       ",
  "        .       ",
  "  ++++  .  ++++ ",
  "     ++ . ++  ++",
  "  +++++ . ++++++",
  " ++  ++ . ++    ",
  " ++  ++ . ++  ++",
  "  +++++ .  ++++ ",
  "        .       ",
  "        .       ",
  "      .. ..     ",
  "                ",
  "                " };

static const char* const new_xpm[] = {
  "16 16 5 1",
  "   c None",
  ".  c #7F7F7F",
  "+  c #FFFFFF",
  "@  c #FFFF00",
  "#  c #000000",
  "                ",
  "                ",
  " .  .+ .@       ",
  "  . .@.@# # #   ",
  "  @.@+....   #  ",
  " ... @@         ",
  "  @ . @.     #  ",
  " .# .@          ",
  "    .        #  ",
  "  #             ",
  "             #  ",
  "  #             ",
  "             #  ",
  "  # # # # # #   ",
  "                ",
  "                " };

static const char* const delete_xpm[] = {
  "16 16 3 1",
  "   c None",
  ".  c #7F0000",
  "+  c #FFFFFF",
  "                ",
  "                ",
  "                ",
  " ..+        ..+ ",
  " ....+     ..+  ",
  "  ....+   ..+   ",
  "    ...+ .+     ",
  "     .....+     ",
  "      ...+      ",
  "     .....+     ",
  "    ...+ ..+    ",
  "   ...+   ..+   ",
  "  ...+     .+   ",
  "  ...+      .+  ",
  "   .         .  ",
  "                " };

static const char* const down_xpm[] = {
  "16 16 2 1",
  "   c None",
  ".  c #000000",
  "                ",
  "                ",
  "         ...    ",
  "        ...     ",
  "       ...      ",
  "       ...      ",
  "       ...      ",
  "       ...      ",
  "   ...........  ",
  "    .........   ",
  "     .......    ",
  "      .....     ",
  "       ...      ",
  "        .       ",
  "                ",
  "                " };

static const char* const up_xpm[] = {
  "16 16 2 1",
  "   c None",
  ".  c #000000",
  "                ",
  "        .       ",
  "       ...      ",
  "      .....     ",
  "     .......    ",
  "    .........   ",
  "   ...........  ",
  "       ...      ",
  "       ...      ",
  "       ...      ",
  "       ...      ",
  "      ...       ",
  "     ...        ",
  "                ",
  "                ",
  "                " };

}

#endif // COMMONIMAGES_H
