include ../Makefile.inc

MODULE=../0008-philosophers.bin
SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_STDLIB=$(wildcard stdlib/*.c)
SOURCES_MXLIB=$(wildcard semlib/*.c)
SOURCES_SEMLIB=$(wildcard mxlib/*.c)
SOURCES_MQLIB=$(wildcard mqlib/*.c)
SOURCES_THREADLIB=$(wildcard threadlib/*.c)
OBJECTS_C=$(SOURCES_C:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_STDLIB=$(SOURCES_STDLIB:.c=.o)
OBJECTS_MXLIB=$(SOURCES_MXLIB:.c=.o)
OBJECTS_SEMLIB=$(SOURCES_SEMLIB:.c=.o)
OBJECTS_MQLIB=$(SOURCES_MQLIB:.c=.o)
OBJECTS_THREADLIB=$(SOURCES_THREADLIB:.c=.o)
all: $(MODULE)

$(MODULE): $(OBJECTS_C) $(OBJECTS_ASM) $(OBJECTS_STDLIB) $(OBJECTS_MXLIB) $(OBJECTS_SEMLIB) $(OBJECTS_MQLIB) $(OBJECTS_THREADLIB)
	$(LD) $(LDFLAGS) -T module.ld -o $(MODULE) $(OBJECTS_C) $(OBJECTS_ASM) $(OBJECTS_STDLIB) $(OBJECTS_MXLIB) $(OBJECTS_SEMLIB) $(OBJECTS_MQLIB) $(OBJECTS_THREADLIB)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS_ASM)
	rm -rf $(OBJECTS_C)
	rm -rf $(OBJECTS_STDLIB)

.PHONY: all clean print
