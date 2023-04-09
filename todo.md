# ride todo/roadmap

## initial
* verify correct coordinate spaces with tests and documentation
* add unit tests for core
* bring back blit rendering/support for multiple backends

## basic
* vertical scrollbar needs to account for horizontal scrollbar when calculating view_size
* drag scrollbar with mouse
* introduce buffer concept to the editor widget
* animate/blink cursor
* text input
* cmd/chord functions
* cmd palette functionality
* cmd palette to run commands
* save, open, close buffers
* open files from file/tree view
* tab bar with current open files
* cmd palete to quickly open files (port lite and classic ride functionality)
* search/replace command in current buffer
* ctrl-tab feature from classic ride
* multi cursor functionality
* port classic ride commands editor functionality (smart tab)
* make editor behave as expected, including undo/cut/copy/paste support
* tmlanguage color support
* port i3 or something else so we can have multiple resizable widgets in same window
* tile view inside tab and tile windows outside tab
* run command/compile project
* treesitter color support with commands
* highlight with color in current buffer
* import build result from a log
* command to switch beteen header and source file
* command to switch between test and implementation file

## first version
* extenstion/mutator support (c++ plugins)
* addon support (lua plugins)
* rework buffer to use proper data structure (piece table)
* "open with ride" for windows installation
* app icon demo
* app title demo
* commandline support for not opening project in cwd
* integrate "classic ride" sources that aren't integrated yet
* option to draw current line gutter text with a different color
* option to extend current line rendering to include gutter?
* treeview: remember open/close state after restart/reload
* view: make it optional to scroll outside the view
* some form of basic color/theme editor
  - add loading/saving via gaf or protobuf
  - text editor for theme file with hotreload
  - edit inline: see color, run command and get a color widget inline for the specific entry under the cursor


## advanced
* custom tilebar to follow color scheme
* preview markdown
* preview graphviz
* preview plantuml https://www.planttext.com/
* preview https://www.nomnoml.com/ https://github.com/skanaar/nomnoml
* git blame in editor
* view diffs
* debugger/gdb integration
* LSP support https://microsoft.github.io/language-server-protocol/ https://langserver.org/
* treeview: change project folder to a new (sub/parent/folder-browser) folder
* treeview: bookmarks
  - new setting: running `ride` with no argument means open without project (not cwd)
* numbered command "dynamic/temporary keybind"... select command and type keybind to bind it. displayed in statusbar and saved per project(?). (one way to mimic vim macros)
* open script editor to the side and write "vim macros" and hot-reload instant run on the current buffer without actually running it on the "real" buffer (run/undo)
* (per project) scripting support
* quick change between different highlighting modes
  - one that highlights comments and dims code
  - one that highlights code and dims comments
* highlight todo comments (list of regexes + color/theme)
* list todo comments in a tasklisk
* view documentation for funtion call under the cursor in the sidebar
