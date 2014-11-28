hamfilter
=========

A DSP application for reducing noise and providing a narrow bandpass audio filter for CW/SSB amateur radio operators. Mac, Windows, Linux.


Building hamfilter
------------------

Prerequisites

You need to have the RtAudio library and header installed. See
http://www.music.mcgill.ca/~gary/rtaudio/index.html
Use the defaults for UNIX systems, install it under /usr/local:

OSX:
./configure --with-core
make
make install (I had to change the Makefile to say install -m 755 instead of
install --mode=755)


To build hamfilter:
make


Running hamfilter
-----------------
./hamfilter

