![screenshot](logo/256text.png)
Ride is a [Rust](http://www.rust-lang.org/) [IDE](https://en.wikipedia.org/wiki/Integrated_development_environment). It's named after concatenating R from rust and IDE. It's currently not in a usable state.
If you're looking for something more complete, perhaps [RustDT](http://rustdt.github.io/) for Eclipse or [SolidOak](https://github.com/oakes/SolidOak) is your thing.

[![Build Status](https://travis-ci.org/madeso/ride.svg)](https://travis-ci.org/madeso/ride)

The current features are:

* A pretty ok editor (scintilla based)
* Shortcuts for running various cargo commands
* Listing its output and
	- Double clicking on warnings/errors to go to them.
	- Context clicking on warnings/errors to google them.
* A basic project/cargo (file) explorer
* Space/tab settings are saved per project so the same settings are used by everyone on the project
* Autocompile protobuf files

The project aim is to implement as much of it in C++ and then port it to rust when rust is released.

Most of the development is streamed [on twitch](http://www.twitch.tv/imadesome) and later uploaded [to youtube](https://www.youtube.com/playlist?list=PLLZf3o2GDQ_hHiXd_xqL_ShzGcU44WMi3).

![screenshot](screenshots/3.png)

# Features needed for a useful state

* Project file so we can open project with File->Open
* Implement all missing event handlers.
* Close project
* Stop build
* Fix startpage
* Build spinner indicator via wxInfoBar?
* View build list
* Fix pdf paste issue, "optionally only allow safe characters"
* Fix create project issue
* Specify paths to external tools under settings
* A msi installer for windows (or CPack)
* Detect invalid path/configurations by running cargo -v on startup
* Helpful setup-rust setting page
* Cleanup the massive editor settings dialog
* Create a better start page

# Somewhat sorted roadmap

* Add support for https://github.com/maxsnew/cargo-dot
* Paste selection to http://lightpaste.com/ or http://hastebin.com/
* Support aui docking, http://trac.wxwidgets.org/ticket/9419 https://github.com/redtide/wxWidgets/tree/aui-dynamic-notebook, wxAui rewrite planned in 3.2 http://trac.wxwidgets.org/wiki/Roadmap
* integrate taskbar status for build status https://github.com/zhchbin/wxWidgets/wiki/SOC2014_TASKBAR
* add test for different indentation styles and make sure indentation works: http://en.wikipedia.org/wiki/Indent_style#Styles
* Reformat code
* check syntax when typing?
* Add support for rust templates https://github.com/rust-lang/cargo/issues/396
* Find & replace in selection
* Incremental search
* Sync file structure instead of reloading in project explorer
* Comment / uncomment selection
* Quick fix for compiler error: "error: unresolved name `hellos`. Did you mean `hello`?"
* Error list
* Go to next error shortcut
* Expose all aui settings in gui + settings
* Add println! format! argument highlighting
* Add configurable shortcuts
* Goto definition & goto definition of type via [racer](https://github.com/phildawes/racer)
* Code snippets
* Implement ctrl+tab like visual studio to switch between open documents
* Start page
* Option to force charset, lineending and ending with newline when saving
* Sort selected lines
* Align/remove space left/right
* Trim left/right/both selected lines
* Mark important "word" for highlighting
* Switch between documentation and code for current file (auto generated, visible on command or if marked in the editor by a comment?)
* Editors for different files such as gui, bitmaps, FSMs.
* Change the color dialog button/control to a more photshop looking color dialog that sends the color directly to all the edits for instant feedback
* Highlight misspelled words/comments
* Function for selecting current word
* Implement smarter file template with [handlebars-rust](https://github.com/sunng87/handlebars-rust/tree/master)
* "aPRESS_ENTER_HEREb" should be replaced by concat!("a", NEW_LINE_HERE"CURSOR_HEREb")
* Switch tabs by scrolling with cursor over tab-bar
* Format file/selection
* Color member variables, local variables and member variables different.
* Startup options (open homepage, load last loaded project, show open project dialog box, show new project dialog box, show empty enviroment, show start page)
* Visual studio like debug settings
* Context menu support
* Simple/basic git commit shortcut with specifying the commit message
* Investigate debugging support (gdb? https://bjbell.wordpress.com/2015/02/17/emacs-rust-debugging/ )
* Custom styles (like zenburn, obsidian, ...)
* Import styles from other editors (like visualstudio / https://studiostyl.es/ ) and netbeans http://netbeansthemes.com/
* View documents inline instead of in a new tab
* File templates
* Numbered bookmarks
* Built in syntax highlight for shading languges like glsl like http://nshader.codeplex.com/
* Algorithm [debug/developing design](https://vimeo.com/36579366) by bret victor

# Debug ideas

- Color tint IDE depending on current thread executing?
- Add note to code (when debugging) to comeback to later
- Add a data dsiplay output breakpoint, that instead of breaking printf the value, perhaps also to a csv...

# Dependencies:

 - wxWidgets with modifications to expose a more complete find text inteface. Currently [forked here](https://github.com/madeso/wxWidgets) with a [pull request here](https://github.com/wxWidgets/wxWidgets/pull/23)
 - google protobuf