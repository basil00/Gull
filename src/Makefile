CC=clang++
STRIP=strip
CFLAGS=-msse4.1 -mpopcnt -fpermissive -fno-exceptions -fno-rtti -w -O2

gull:
	$(CC) $(CFLAGS) Gull.cpp -o Gull
	$(STRIP) Gull

clean:
	rm -f Gull

