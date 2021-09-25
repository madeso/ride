# ride todo/roadmap

## current

* view: drag scrollbar
* commandline support for not opening project in cwd
* icon demo
* title demo
* cmd palette backend with keybind support
* editor component
* save, open, close buffers
* cnd palette
* run command/compile project
* tmlanguage/treesitter color support

## basic
* file explorer/tree view and tab control open documents
* port i3 or something else so we can have multiple widgets in same window

* make editor behave as expected, including cut/copy/paste support
* rework document to use proper data structure
* integrate "classic ride" sources
* some form of basic color/theme editor
  - text editor for theme file with hotreload
  - edit inline: see color, run command and get a color widget inline for the specific entry under the cursor
* "open with ride" for windows installation

## advanced
* preview markdown
* preview graphviz
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
