UNAME=$(shell uname -s)
ifeq ($(UNAME),Linux)
    TARGET=gull.linux
endif
ifeq ($(UNAME),Darwin)
    TARGET=gull.macosx
endif
CC=clang++
STRIP=strip
CFLAGS=-msse4.1 -mpopcnt -fno-exceptions -fno-rtti -Wno-parentheses -O2

main: $(TARGET)

gull.linux:
	$(CC) $(CFLAGS) -D LINUX Gull.cpp -o Gull
	$(STRIP) Gull

gull.macosx:
	$(CC) $(CFLAGS) -D MACOSX Gull.cpp -o Gull
	$(STRIP) Gull

clean:
	rm -f Gull

