include Makefile.inc

KERNEL=kernel.bin
KEYBOARD_DRIVER=$(wildcard KeyboardDriver/*.c)
MOUSE_DRIVER=$(wildcard MouseDriver/*.c)
VIDEO_DRIVER=$(wildcard VideoDriver/*.c)
PAGING_MANAGER=$(wildcard PagingManager/*.c)
MODULES_MANAGER=$(wildcard ModulesManager/*.c)
PAGE_ALLOCATOR=$(wildcard PageAllocator/*.c)
SCHEDULER=$(wildcard Scheduler/*.c)
SCHEDULER2=$(shell find scheduler2/ -type f -name '*.c' | sort)
INTERRUPTS=$(wildcard interrupts/*.c)
KLIB=$(wildcard klib/*.c)
IPC=$(wildcard ipc/*.c)
TTY=$(wildcard tty/*.c)
SYSCALLS=$(shell find syscalls/ -type f -name '*.c' | sort)
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
OBJ_SCHEDULER2=$(SCHEDULER2:.c=.o)
OBJ_TTY=$(TTY:.c=.o)
OBJ_SYSCALLS=$(SYSCALLS:.c=.o)
OBJ_INTERRUPTS=$(INTERRUPTS:.c=.o)
OBJ_KLIB=$(KLIB:.c=.o)
OBJ_IPC=$(IPC:.c=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJ) $(STATICLIBS) $(OBJ_ASM) $(OBJ_KEYBOARD_DRIVER) $(OBJ_MOUSE_DRIVER) $(OBJ_VIDEO_DRIVER) $(OBJ_PAGING_MANAGER) $(OBJ_MODULES_MANAGER) $(OBJ_PAGE_ALLOCATOR) $(OBJ_SCHEDULER) $(OBJ_TTY) $(OBJ_SYSCALLS) $(OBJ_INTERRUPTS) $(OBJ_KLIB) $(OBJ_IPC) $(OBJ_SCHEDULER2)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJ) $(OBJ_ASM) $(STATICLIBS) $(OBJ_KEYBOARD_DRIVER) $(OBJ_MOUSE_DRIVER) $(OBJ_VIDEO_DRIVER) $(OBJ_PAGING_MANAGER) $(OBJ_MODULES_MANAGER) $(OBJ_PAGE_ALLOCATOR) $(OBJ_SCHEDULER2) $(OBJ_TTY) $(OBJ_SYSCALLS) $(OBJ_INTERRUPTS) $(OBJ_KLIB) $(OBJ_IPC)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin
	rm -rf $(OBJ)
	rm -rf $(OBJ_ASM)
	rm -rf $(OBJ_KEYBOARD_DRIVER)
	rm -rf $(OBJ_MOUSE_DRIVER)
	rm -rf $(OBJ_VIDEO_DRIVER)
	rm -rf $(OBJ_PAGING_MANAGER)
	rm -rf $(OBJ_MODULES_MANAGER)
	rm -rf $(OBJ_PAGE_ALLOCATOR)
	rm -rf $(OBJ_SCHEDULER)
	rm -rf $(OBJ_SCHEDULER2)
	rm -rf $(OBJ_TTY)
	rm -rf $(OBJ_SYSCALLS)
	rm -rf  $(OBJ_IPC)
	rm -rf  $(OBJ_KLIB)

.PHONY: all clean
