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

The project aim is to implement as much of it in C++ and then port it to rust when wxWidgets for rust is available.

Some of the development is streamed [on twitch](http://www.twitch.tv/imadesome) and later uploaded [to youtube](https://www.youtube.com/playlist?list=PLLZf3o2GDQ_hHiXd_xqL_ShzGcU44WMi3).

![screenshot](screenshots/3.png)

# Getting ride
To be written. Issue #73

# Building ride:
To be fleshed out. Issue #73

Here are the dependencies:
 - wxWidgets with modifications to expose a more complete find text inteface. Currently [forked here](https://github.com/madeso/wxWidgets) with a [pull request here](https://github.com/wxWidgets/wxWidgets/pull/23)
 - google protobuf

# Contributing
To be written. Issue #73
