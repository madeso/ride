![screenshot](logo/256text.png)
Ride is a [Rust](http://www.rust-lang.org/) [IDE](https://en.wikipedia.org/wiki/Integrated_development_environment). It's named after concatenating R from rust and IDE. It's currently not in a usable state.

The current features are a pretty ok editor (scintilla based) and shortcuts for running various cargo commands, listing its output and double clicking on warnings/errors to go to them.

The project aim is to implement as much of it in C++ and then port it to rust when rust is released.

Most of the development is streamed [on twitch](http://www.twitch.tv/imadesome) and later uploaded [to youtube](https://www.youtube.com/playlist?list=PLLZf3o2GDQ_hHiXd_xqL_ShzGcU44WMi3).

![screenshot](screenshots/1.png)

# Somewhat sorted roadmap

* Detect changes outside ride and ask to reload file
* Open type in online documentation command/do a search...
* Search google for this compiler error
* Expose all aui settings in gui + settings
* Remember session data
* Comment / uncomment selection
* Go to next error shortcut
* Scintilla functions: encoding, line ending
* Find in files, find in solution, incremental search, other searches
* Fix syntax scintilla syntax highlighting
	- wxWidgets [doesn't support](http://trac.wxwidgets.org/ticket/16776)  rust yet, perhaps temporarily solve it with conditional compilation with a cpp lexer and rust keywords
* Quick open file from solution/project
* Project/solution explorer
* Create project with cargo and add a README.md file
* Cpack support (=installer for ride)
* Intellisense/autocomplete support via [racer](https://github.com/phildawes/racer)
* Code snippets
* Implement ctrl+tab like visual studio to switch between open documents
* Error list
* Start page
* Option to force charset, lineending and ending with newline when saving
* Sort selected lines
* Align/remove space left/right
* Trim left/right/both selected lines
* Mark important "word" for highlighting
* Change the color dialog button/control to a more photshop looking color dialog that sends the color directly to all the edits for instant feedback
* Highlight misspelled words/comments
* Function for selecting current word
* Goto definition & goto definition of type
* Switch tabs by scrolling with cursor over tab-bar
* Format file/selection
* Color member variables, local variables and member variables different.
* Startup options (open homepage, load last loaded project, show open project dialog box, show new project dialog box, show empty enviroment, show start page)
* Project file so we can open project with File->Open
* Visual studio like debug settings
* Context menu support
* Simple/basic git commit shortcut with specifying the commit message
* Investigate debugging support (gdb? https://bjbell.wordpress.com/2015/02/17/emacs-rust-debugging/ )
* Custom styles (like zenburn, obsidian, ...)
* Import styles from other editors (like visualstudio / https://studiostyl.es/ )
* View documents inline instead of in a new tab
* File templates
* Numbered bookmarks
* Built in syntax highlight for shading languges like glsl like http://nshader.codeplex.com/
* Algorithm [debug/developing design](https://vimeo.com/36579366) by bret victor

# Debug ideas

- Color tint IDE depending on current thread executing?
- Add note to code (when debugging) to comeback to later
