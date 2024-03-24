#
# lugarudc Makefile
# Charlotte Koch <dressupgeekout@gmail.com>
#
# Requires a functioning SH4 toolchain from KallistiOS
#

.PHONY: all
all: lugaru.elf

lugaru.elf: main.o
	kos-c++ -o $@ $^ -lkosutils
	kos-strip $@

main.o: main.cpp
	kos-c++ -c -o $@ $<

.PHONY: clean
clean:
	rm -f lugaru.elf *.o
