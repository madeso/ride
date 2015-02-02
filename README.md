# Ride
Ride is a Rust IDE. It's named after concatenating R from rust and IDE. It's currently not in a usable state.

The project aim is to implement as much of it in C++ and then port it to rust when it stabilizes.

# Unsorted Roadmap

* Fix syntax scintilla syntax highlighting
	- wxWidgets [doesn't support](http://trac.wxwidgets.org/ticket/16776)  rust yet, perhaps temporarily solve it with conditional compilation with a cpp lexer and rust keywords
* project/solution explorer
* project/solution(=cargo) commands
* start page
* build output
* error list
* visual studio like debug settings
* context menu support
* go to next error shortcut
* quick open file from solution/project
* remember opened file since last time
* scintilla text commands
* settings/options window
* cpack support (=installer for ride)
* phaser support
* investigate debugging support (gdb?)
* custom styles (like zenburn, obsidian, ...)