Arrow image: https://openclipart.org/detail/26734/aiga-right-arrow
Sky blue stolen from a image in this page http://installbuilder.bitrock.com/docs/installbuilder-userguide/ar01s27.html

A small note on creating the ds_store:

1. First build a dmg so that you have something to work with, the package target should do this.

2. Make a writeable copy of that dmg:
Note that there is a i in hd I util, and it's not called hdutil
$ hdiutil convert -format UDRW -o ride-test.dmg ride-0.1.1-Darwin.dmg

3. Mount the new writeable dmg by double clicking on it

4. Now do your changes. Change the background, move around the icons etc...

5. In a terminal, go to that mounted folder
$ cd /Volumes/ride-0.1.1-Darwin/

6. And copy the .DS_Store to a place in your git and give it a better name so it won't be overwritten by accident
$ cp -f .DS_Store ~/ride/gfx/install/ds_store

Done.
