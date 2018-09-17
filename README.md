# Ocelot Media Manager v0.2c

Ocelot is an audio player and library organizer, built using Qt and TagLib. The goal is to build a player light on resources by utilizing near-stock Qt Widgets, while also retaining some of the native look-and-feel of each platform.

_This project is currently in an early stage of development! As such, many planned features are still not present._

Aside from the Qt Framework itself, you'll need qtmultimedia and taglib development packages to build Ocelot. Both packages are available on the standard repos of pretty much every distro around, although the exact name may vary.

On Arch you'll need:
~~~~
pacman -Sy qt5-multimedia qt5-base taglib
~~~~

You can build with:
~~~~
qmake-qt5 && make && ./ocelot
~~~~


### PLANNED
> * Milkdrop visualization support
> * Mass tagger and batch renamer
> * Batch transcode
> * Themes and layout editor

### WHAT WORKS
> * Basic database management
> * Single-layout UI with basic features
> * Music playback and metadata extraction

### TODO FOR 0.3
> * Allow for on-the-fly library management
> * Refinements for the standard UI widgets (tagview, playlist editor, etc)
----
For now, I'm building Ocelot for my own use, as I am very much a Foobar2k widower, but contributions are much appreciated as development progresses.

![Imgur](https://i.imgur.com/lKeJrHR.png)
