UNAME=$(shell uname -s)
ifeq ($(UNAME),Linux)
    TARGET=gull.linux
endif
ifeq ($(UNAME),Darwin)
    TARGET=gull.macosx
endif
ifeq ($(UNAME),Linux)
    TARGET_SYZYGY=gull.syzygy.linux
endif
ifeq ($(UNAME),Darwin)
    TARGET_SYZYGY=gull.syzygy.macosx
endif
CC=g++
STRIP=strip
CFLAGS=-msse4.1 -mpopcnt -fno-exceptions -fno-rtti -Wno-parentheses -O3

main: $(TARGET)

syzygy: $(TARGET_SYZYGY)

gull.linux:
	$(CC) $(CFLAGS) -D LINUX Gull.cpp -o Gull
	$(STRIP) Gull
	cp Gull Gull.linux

gull.syzygy.linux: tbprobe.o
	$(CC) $(CFLAGS) -D LINUX -D TB Gull.cpp tbprobe.o -o Gull
	$(STRIP) Gull
	cp Gull Gull.syzygy.linux

gull.macosx:
	$(CC) $(CFLAGS) -D MACOSX Gull.cpp -o Gull
	$(STRIP) Gull
	cp Gull Gull.macosx

gull.syzygy.macosx:
	$(CC) $(CFLAGS) -D MACOSX -D TB Gull.cpp tbprobe.o -o Gull
	$(STRIP) Gull
	cp Gull Gull.syzygy.macosx

tbprobe.o: tbprobe.c tbcore.c
	$(CC) $(CFLAGS) -c tbprobe.c

tbprobe.c: Fathom-master.zip
	unzip -o -j Fathom-master.zip "Fathom-master/src/tbprobe.h" -d "./"
	unzip -o -j Fathom-master.zip "Fathom-master/src/tbprobe.c" -d "./"

tbcore.c: Fathom-master.zip
	unzip -o -j Fathom-master.zip "Fathom-master/src/tbcore.h" -d "./"
	unzip -o -j Fathom-master.zip "Fathom-master/src/tbcore.c" -d "./"

Fathom-master.zip:
	wget https://github.com/basil00/Fathom/archive/master.zip \
        -O Fathom-master.zip

clean:
	rm -f Gull

