## Ocelot Media Manager v0.3c
Ocelot is an audio player and library manager.
It is built using Qt Widgets, while also relying on TagLib for tag management, libProjectM for visualizations and QtMultimedia for audio playback.

>Ocelot is released under the MIT license, you can read more about it inside the supplied LICENSE file.
#

>### <b>BUILDING</b>
Ocelot requires Qt Frameworks version 5.10+

On Arch you might need to install the following packages:
`qt5-multimedia qt5-base taglib`

Then you can build and run with:
`qmake . && make && ./ocelot`

>### <b>PLANNED</b>

* Gapless playback
* Equalizer
* Lyrics and Panned Library widgets
* Milkdrop visualization support
* Mass tagger and batch renamer
* Batch transcode
* Custom album import
* MIDI playback support

>### <b>WHAT WORKS</b>

* Basic library management
* Music playback
* Playlist manager and export
* Layout editor

>### <b>SCREENSHOTS</b>
![alt text](https://raw.githubusercontent.com/mscatto/ocelot/master/screenshot.png)
