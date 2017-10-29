include Makefile.inc

KERNEL=kernel.bin
KEYBOARD_DRIVER=$(wildcard KeyboardDriver/*.c)
MOUSE_DRIVER=$(wildcard MouseDriver/*.c)
VIDEO_DRIVER=$(wildcard VideoDriver/*.c)
PAGING_MANAGER=$(wildcard PagingManager/*.c)
MODULES_MANAGER=$(wildcard ModulesManager/*.c)
PAGE_ALLOCATOR=$(wildcard PageAllocator/*.c)
SCHEDULER=$(wildcard Scheduler/*.c)
TTY=$(wildcard tty/*.c)
SYSCALLS=$(shell find Syscalls/ -type f -name '*.c')
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJ=$(SOURCES:.c=.o)
OBJ_ASM=$(SOURCES_ASM:.asm=.o)
OBJ_KEYBOARD_DRIVER=$(KEYBOARD_DRIVER:.c=.o)
OBJ_MOUSE_DRIVER=$(MOUSE_DRIVER:.c=.o)
OBJ_VIDEO_DRIVER=$(VIDEO_DRIVER:.c=.o)
OBJ_PAGING_MANAGER=$(PAGING_MANAGER:.c=.o)
OBJ_MODULES_MANAGER=$(MODULES_MANAGER:.c=.o)
OBJ_PAGE_ALLOCATOR=$(PAGE_ALLOCATOR:.c=.o)
OBJ_SCHEDULER=$(SCHEDULER:.c=.o)
OBJ_TTY=$(TTY:.c=.o)
OBJ_SYSCALLS=$(SYSCALLS:.c=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJ) $(STATICLIBS) $(OBJ_ASM) $(OBJ_KEYBOARD_DRIVER) $(OBJ_MOUSE_DRIVER) $(OBJ_VIDEO_DRIVER) $(OBJ_PAGING_MANAGER) $(OBJ_MODULES_MANAGER) $(OBJ_PAGE_ALLOCATOR) $(OBJ_SCHEDULER) $(OBJ_TTY) $(OBJ_SYSCALLS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJ) $(OBJ_ASM) $(STATICLIBS) $(OBJ_KEYBOARD_DRIVER) $(OBJ_MOUSE_DRIVER) $(OBJ_VIDEO_DRIVER) $(OBJ_PAGING_MANAGER) $(OBJ_MODULES_MANAGER) $(OBJ_PAGE_ALLOCATOR) $(OBJ_SCHEDULER) $(OBJ_TTY) $(OBJ_SYSCALLS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin

.PHONY: all clean
