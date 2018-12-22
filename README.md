## Ocelot Media Manager v0.3a
Ocelot is an audio player and (hopefully soon) a library manager.
* _This project is currently in an early stage of development! As such, many planned features are still not present._

Built using Qt Widgets, some key features made possible by TagLib, KFileMetadata and ProjectM projects.
* TagLib is being phased out in favor of KFileMetadata. It is expected to be dropped somewhere in the future.

#

>### <b>BINARY</b>
A precompiled binary from v0.3a is available [here](https://raw.githubusercontent.com/mscatto/ocelot/master/ocelot-bin).
>### <b>BUILDING</b>
Ocelot requires Qt Frameworks version 5.10 or above, QtMultimedia and TagLib development files.
* QtMultimedia makes use of GStreamer for audio playback on unix-like platforms, so you need to install it and the required plugins for your media formats. Most likely you want the _good_, _bad_ and _base_ packages.

On Arch, you'll need the following packages:
`qt5-multimedia qt5-base taglib make gcc kfilemetadata`

You can build and run with:
`qmake . && make && ./ocelot`

>### <b>PLANNED</b>

* Milkdrop visualization support
* Mass tagger and batch renamer
* Batch transcode
* Themes and layout editor

>### <b>WHAT WORKS</b>

* Basic library management
* Music playback
* Basic UI features

>### <b>SCREENSHOTS</b>
![alt text](https://raw.githubusercontent.com/mscatto/ocelot/master/ss.png)
