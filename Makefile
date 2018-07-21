CC = x86_64-w64-mingw32-gcc
CFLAGS = -std=c99 -Os -ffreestanding -Wall -Wextra -Werror \
	-Wno-cast-function-type -fomit-frame-pointer \
	-ffunction-sections -fdata-sections -Wl,--gc-sections \
	-fno-asynchronous-unwind-tables \
	-fno-stack-check -fno-stack-protector -mno-stack-arg-probe -Wno-multichar
LDFLAGS = -s -nostdlib -mwindows
LDLIBS = -lkernel32 -luser32 -lgdi32 -ld3d9 -lole32 -lntdll -lxaudio2_8 \
	-ld3dx9 -lwinmm
RES = windres

fragment.exe: main.c resources.o
	$(CC) $(LDFLAGS) $(CFLAGS) \
	vendor/memmem.c vendor/strtok_r.c vendor/fastlz.c \
	*.c -o $@ resources.o $(LDLIBS)

resources.o:
	$(RES) resources.rc resources.o

clean:
	rm -f *.exe *.o
