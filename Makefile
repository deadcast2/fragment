CC = x86_64-w64-mingw32-gcc
CFLAGS = -std=c99 -Os -ffreestanding -Wall -Wextra -Werror \
	-Wno-cast-function-type -fomit-frame-pointer \
	-ffunction-sections -fdata-sections -Wl,--gc-sections \
	-fno-asynchronous-unwind-tables \
	-fno-stack-check -fno-stack-protector -mno-stack-arg-probe -Wno-multichar
LDFLAGS = -s -nostdlib -mwindows
LDLIBS = -lkernel32 -luser32 -lgdi32 -ld3d9 -lole32 -lntdll
RES = windres

main.exe: main.c resources.o
	$(CC) $(LDFLAGS) $(CFLAGS) log.c graphics.c xaudio2.c audio.c scene.c main.c \
	-o $@ resources.o $(LDLIBS)

resources.o:
	$(RES) resources.rc resources.o

clean:
	rm -f *.exe *.o
