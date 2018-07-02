CC = x86_64-w64-mingw32-gcc
CFLAGS = -std=c99 -Os -ffreestanding -Wall -Wextra -Werror \
	-Wno-cast-function-type -fomit-frame-pointer \
	-ffunction-sections -fdata-sections -Wl,--gc-sections \
	-fno-asynchronous-unwind-tables
LDFLAGS = -s -nostdlib -mwindows
LDLIBS = -lkernel32 -luser32 -lgdi32 -ld3d9

main.exe: main.c
	$(CC) $(LDFLAGS) $(CFLAGS) log.c graphics.c scene.c main.c \
	-o $@ $(LDLIBS)

clean:
	rm -f *.exe *.o
