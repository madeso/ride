* don't expand folders when project is opened
* remember expanded folders
* grab the fil and dir api from blaggen and replace strings and wxWfileName with it
* expand json parser with better error detection/error log
* Replace ugly rgb json structure with hexstring
* rebind 'open file in project' to ctrl+p
* don't open project with all folders expanded
* look into cmdline dir handling when folder doesn't end with /
* look into possible threading when opening large folders
* read menu from json file?
* add attached (ie default to non attached) to the cmd line
* Have a list of projects when nothing is opened and a selector to reopen latest project with files
* store session in the project `.ride` folder
* task/action search and run `ctrl+shift+p`
* change project browser to a folder dialog (preference since windows is crappy?)
* json tab completion for settings file
* hotlreload and display json warnings/errors for settings file when saved
* `ctrl+.` shortcut for quick actions
* external file change detection
* Remove or replace rust specific things with generic
* Just have 2 build command, build selected config and enter build config each time, the new build command should be a list of commands that are chained
* Replace settings dialog with json hot reload
* Ctr-b shortcut to switch between perspectives
* view commands should be able to show the panes again (arch/i3 issue?)
* Replace shitty colors with
  > color palette -> grouping -> binding (-> stc)
  > color palette: `{ red: #4e2f00 }`
  >   essentially just a list of named color
  > group: `{ keyword: red }`
  > should perhaps be a layer here so we can switch between disco/
  > binding: `{ linecomment: [linecomment, comment] } `
  > binding keys are stc names, arrays is a basic priority/inheritence
* multi line editing show be able to be moved with arrow keys
* go to next error
* Implement basic lua extension
* move file mode/extension to config

