## Ocelot Media Manager v0.3b
Ocelot is an audio player and (hopefully soon) a library manager.

It is built using Qt Widgets, with some key features made possible by the TagLib and ProjectM projects.

>Ocelot is released under the MIT license, you can read more about it inside the supplied LICENSE file.

* _This project is currently in an early stage of development! As such, many planned features are still not present._

#

>### <b>BINARY</b>
A precompiled binary from FEB/2019 is available [here](https://raw.githubusercontent.com/mscatto/ocelot/master/ocelot-bin).
>### <b>BUILDING</b>
Ocelot requires Qt Frameworks version 5.10 or above and the latest TagLib development files. It also depends on Qt Multimedia, which in turn requires GStreamer on UNIX-like systems.

* Qt Multimedia requires GStreamer for audio playback on UNIX, so you will need to install it and the required plugins for your media formats. Most likely you want the _good_, _bad_ and _base_ packages.

On Arch, you'll need the following packages:
`qt5-multimedia qt5-base taglib`

You can build and run with:
`qmake . && make && ./ocelot`

>### <b>PLANNED</b>

* Milkdrop visualization support
* Mass tagger and batch renamer
* Batch transcode
* Custom album import
* Colored UI

>### <b>WHAT WORKS</b>

* Basic library management
* Music playback
* Playlist manager and export
* Layout editor

>### <b>SCREENSHOTS</b>
![alt text](https://raw.githubusercontent.com/mscatto/ocelot/master/ss.png)
