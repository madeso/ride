# Ride pony
![screenshot](gfx/logo/256text.png)

Ride was a [Rust](http://www.rust-lang.org/) [IDE](https://en.wikipedia.org/wiki/Integrated_development_environment). It's named after concatenating R from rust and IDE. This can be found in the `classic` branch. Since the release of vs code, Ride took a turn towards a more general editor and less of a full blown IDE.

The ultimate goal (main breanch) of Ride is a simple editor with a fully custom rendering similar to vs code, but in c++ (and perhaps some scripting language). This is a long time goal. In the mean time, the **P**rototype **O**f the **N**ew **Y**ear will have to suffice.

Pony will be true to ride classic roots but move towards `dev` being based on scintilla and wxWidgets for the gui.


The current features are:

* A pretty ok editor (scintilla based)
* Shortcuts for running custom terminal commands
* Listing its output and
	- Double clicking on warnings/errors to go to them.
	- Context clicking on warnings/errors to google them.
* A basic project explorer
* Space/tab settings are saved per project so the same settings are used by everyone on the project


# Older screenshots
![screenshot](gfx/screenshots/3.png)
![screenshot](gfx/screenshots/2.png)

# Getting ride
This isn't finished. Eventually there should be a installer here, but that isn't finished. If you want to try it, you need to download the source and build it yourself.

# Building ride
You need CMake, a C++ compiler, and wxWidgets 3+. To actually build it, it is pretty standard. Use CMake to generate a configuration and then build that configuration.

# Contributing
I accept pull requests. See [todo.md](todo.md) for the active tasks.
