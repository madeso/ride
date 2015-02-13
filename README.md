# Ride
Ride is a Rust IDE. It's named after concatenating R from rust and IDE. It's currently not in a usable state.

The project aim is to implement as much of it in C++ and then port it to rust when it stabilizes.

# Unsorted Roadmap

=======
* Create filter list from known languages
* Remember session data
* Expose all scintilla settings in gui + settings
* Expose all aui settings in gui + settings
* scintilla text commands (edit->select all, etc)
* Indenting feature: None, block, smart
* Find in files, find in solution, incremental search, other searches
* Fix syntax scintilla syntax highlighting
	- wxWidgets [doesn't support](http://trac.wxwidgets.org/ticket/16776)  rust yet, perhaps temporarily solve it with conditional compilation with a cpp lexer and rust keywords
* Project/solution explorer
* Project/solution(=cargo) commands
* Start page
* Option to force charset, lineending and ending with newline when saving
* Build output
* Error list
* Visual studio like debug settings
* Context menu support
* Go to next error shortcut
* Quick open file from solution/project
* Cpack support (=installer for ride)
* Intellisense/autocomplete support via [racer](https://github.com/phildawes/racer)
* Investigate debugging support (gdb?)
* Custom styles (like zenburn, obsidian, ...)
* Import styles from other editors (like visualstudio / https://studiostyl.es/ )
* Code snippets
* File templates
* Numbered bookmarks
* Implement ctrl+tab like visual studio to switch between open documents
* Open type in online documentation command/do a search...
* Search google for this compiler error
* Built in syntax highlight for shading languges like glsl like http://nshader.codeplex.com/

# Debug ideas

- Color tint IDE depending on current thread executing?
- Add note to code (when debugging) to comeback to later
