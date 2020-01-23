## Ocelot Media Manager v0.3c
Ocelot is an audio player and library manager.
It is built using Qt Widgets, while also relying on TagLib for tag management, libProjectM for visualizations and QtMultimedia for audio playback.

>Ocelot is released under the MIT license, you can read more about it inside the supplied LICENSE file.
#

>### <b>BUILDING</b>
Ocelot requires Qt Frameworks version 5.10+<br>
_clang_ is the default compiler for Ocelot, but _gcc_ works as well. to change it, edit or comment out the _QMAKE_LINK_ and _QMAKE_CXX_ variables inside _ocelot.pro_

On Arch you might need the following packages:
`clang qt5-multimedia qt5-base taglib`

Then compile with: `qmake .` and `make`<br>
A binary will be placed inside `bin`

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
