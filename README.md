hamfilter
=========

A DSP application for reducing noise and providing a narrow bandpass audio filter for CW/SSB amateur radio operators. Mac, Windows, Linux.

Contact: DevZendo.org @mattgumbley Matt Gumbley M0CUV matt.gumbley@devzendo.org

Building hamfilter
------------------

Prerequisites

RtAudio
=======
You need to have the RtAudio library and header installed. See
http://www.music.mcgill.ca/~gary/rtaudio/index.html
Use the defaults for UNIX systems, install it under /usr/local:

OSX:
./configure --with-core
make
make install (I had to change the Makefile to say install -m 755 instead of
install --mode=755)


Linux:
to be determined
Windows:
to be determined (you'd have to be)

To build hamfilter:
cd src
make


Running hamfilter
-----------------
./hamfilter

