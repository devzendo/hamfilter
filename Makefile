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

main.cpp: 

main.o: main.cpp

hamfilter: main.o
	cc -o hamfilter main.o

clean:
	rm -f *.o hamfilter


tags: ctags
ctags:
	rm -f TAGS tags
	ctags *.[ch]

