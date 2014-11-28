# Choose one platform

# For Mac OS X 10.7
#PLATFORM=MACOSX

# For Ubuntu, Debian, Mint
PLATFORM=DEBIAN

# For Red Hat, Fedora, CentOS
#PLATFORM=REDHAT

# For the Raspberry Pi, hurrah!
#PLATFORM=RASPBIAN

CFLAGS=-Wall -D${PLATFORM} -I/usr/local/include
LDFLAGS=-L/usr/local/lib


all: hamfilter

hamfilter.cpp: 

hamfilter.o: hamfilter.cpp

hamfilter: hamfilter.o
	cc -o hamfilter hamfilter.o

clean:
	rm -f *.o hamfilter


tags: ctags
ctags:
	rm -f TAGS tags
	ctags *.[ch]

