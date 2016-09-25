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
CFLAGS=-msse4.1 -mpopcnt -fno-exceptions -fno-rtti -Wno-parentheses -O3 \
    -lpthread -lrt

main: $(TARGET)

gull.linux: tbprobe.o
	$(CC) $(CFLAGS) -D LINUX Gull.cpp tbprobe.o -o LazyGull
	$(STRIP) LazyGull

tbprobe.o: tbprobe.c tbcore.c
	$(CC) $(CFLAGS) -D LINUX -c tbprobe.c

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
	rm -f LazyGull tbprobe.o

