# Ocelot Media Manager v0.3a

Ocelot is an audio player and library organizer, built using Qt and TagLib.
It's aim is to create a player light on resources by utilizing near-stock Qt Widgets, while also retaining some of the native look-and-feel of each platform.

_This project is currently in an early stage of development! As such, many planned features are still not present._

Aside from the Qt Framework, you'll need qtmultimedia and taglib development packages to build Ocelot. Both are available on the standard repos of pretty much every distro around, although the exact name may vary.

On Arch you'll need:
~~~~
sudo pacman -Sy qt5-multimedia qt5-base taglib
~~~~

And to build:
~~~~
 qmake-qt5 && make && ./ocelot
~~~~


### TODO

> * Milkdrop visualization support
> * Mass tagger and batch renamer
> * Batch transcode
> * Themes and layout editor
