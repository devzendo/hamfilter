hamfilter
=========

A DSP application for reducing noise and providing a narrow bandpass audio filter for CW/SSB amateur radio operators. Mac, Windows, Linux.

Contact: DevZendo.org @mattgumbley Matt Gumbley M0CUV matt.gumbley@devzendo.org

Prerequisites for building hamfilter
------------------------------------

GNU Toolchain: G++, Make
========================

OSX:
Install the Xcode command line tools

Linux:
to be determined...
apt-get build-essential g++ (haven't verified this)

Windows:
to be determined...
Probably cygwin is your best bet.


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


Qt 5
====
You need to have Qt 5 library, header and Creator installed.

OSX:
Get it from MacPorts with:
port install qt5-mac qt5-creator-mac

Linux:
to be determined

Windows:
to be determined



Building hamfilter
------------------
To build hamfilter:
cd src
make


Running hamfilter
-----------------
./hamfilter

