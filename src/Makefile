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

gull.syzygy.macosx: tbprobe.o
	$(CC) $(CFLAGS) -D MACOSX -D TB Gull.cpp tbprobe.o -o Gull
	$(STRIP) Gull
	cp Gull Gull.syzygy.macosx

gull.windows: CC=x86_64-w64-mingw32-g++
gull.windows: STRIP=x86_64-w64-mingw32-strip
gull.windows:
	$(CC) $(CFLAGS) -D WINDOWS Gull.cpp -o Gull.exe
	$(STRIP) Gull.exe

gull.syzygy.windows: CC=x86_64-w64-mingw32-g++
gull.syzygy.windows: STRIP=x86_64-w64-mingw32-strip
gull.syzygy.windows: tbprobe_windows.o
	$(CC) $(CFLAGS) -D WINDOWS -D TB Gull.cpp tbprobe_windows.o -o Gull.exe
	$(STRIP) Gull.exe

tbprobe.o: tbprobe.c tbcore.c
	$(CC) $(CFLAGS) -c tbprobe.c

tbprobe_windows.o: CC=x86_64-w64-mingw32-g++
tbprobe_windows.o: tbprobe.c tbcore.c
	$(CC) $(CFLAGS) -c tbprobe.c -o tbprobe_windows.o

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

