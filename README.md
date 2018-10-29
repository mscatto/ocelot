# Ocelot Media Manager v0.3ac

Ocelot is an audio player and library organizer, built with Qt and TagLib.
It's aim is to create a player light on resources by utilizing near-stock Qt Widgets, while also retaining some of the native look-and-feel of each platform.

---
### BINARIES
A precompiled binary for linux x86_64 v0.3a is also available [here](https://raw.githubusercontent.com/mscatto/ocelot/master/ocelot-bin)
### BUILDING
Ocelot requires Qt5 at least 5.10, Qt Multimedia and TagLib development files.

On Arch, you'll need the following packages:
`qt5-multimedia qt5-base taglib make gcc`

You can build it and run with:
`qmake . && make && ./ocelot`

---

_This project is currently in an early stage of development! As such, many planned features are still not present._

### TODO

> * Milkdrop visualization support
> * Mass tagger and batch renamer
> * Batch transcode
> * Themes and layout editor

### WHAT WORKS

> * Basic library management
> * Music playback
> * Basic UI features

### TODO FOR 0.3b
> * UI features and initial transcoding support

## Screenshot
![alt text](https://raw.githubusercontent.com/mscatto/ocelot/master/ss.png)

