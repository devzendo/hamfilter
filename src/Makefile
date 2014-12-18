# Choose one platform

# For Mac OS X 10.7
PLATFORM=MACOSX

# For Ubuntu, Debian, Mint
#PLATFORM=DEBIAN

# For Red Hat, Fedora, CentOS
#PLATFORM=REDHAT

# For the Raspberry Pi, hurrah!
#PLATFORM=RASPBIAN

CFLAGS=-Wall -D${PLATFORM} -I/usr/local/include -O0 -g3
LDFLAGS=-L/usr/local/lib


all: hamfilter


buffers.cpp: buffers.h

buffers.o: buffers.cpp
	g++ ${CFLAGS} -c -o buffers.o buffers.cpp


hamfilter.cpp: buffers.h

hamfilter.o: hamfilter.cpp
	g++ ${CFLAGS} -c -o hamfilter.o hamfilter.cpp


hamfilter: hamfilter.o buffers.o
	g++ ${LDFLAGS} -o hamfilter hamfilter.o buffers.o -lrtaudio


clean:
	rm -f *.o hamfilter


tags: ctags
ctags:
	rm -f TAGS tags
	ctags *.[ch]

