ARCH		= ARM
MONCOMPTR	= 0xC8000038
APPRAMBASE	= 0xC0200000

APPNAME		= fattest
NM			= $(TOOL_PREFIX)-nm
AR			= $(TOOL_PREFIX)-ar
LD			= $(TOOL_PREFIX)-ld
ASM			= $(TOOL_PREFIX)-as
CC			= $(TOOL_PREFIX)-gcc
STRIP		= $(TOOL_PREFIX)-strip
OBJCOPY		= $(TOOL_PREFIX)-objcopy
OBJDUMP		= $(TOOL_PREFIX)-objdump
LIBGCC		= `$(CC) --print-libgcc-file-name`
LIBDIR		= $(LIBGCC:/libgcc.a=)

EFSL_DIR	= ../efsl-0.2
EFSL_INC	= -I$(EFSL_DIR)/inc -I$(EFSL_DIR)/conf
EFSL_LIBDIR	= $(EFSL_DIR)
EFSL_LIBS	= -lefsl

LIBCDIR		= /cygdrive/d/gnuarm/arm-elf/lib
LDSCRIPT	= ldscript

TOOL_PREFIX	:= arm-elf
CFLAGS		:= -fno-builtin -mcpu=arm9tdmi -c -Wall -g -I. $(EFSL_INC)
CRT0		:= crt0_arm.o
CPU			:= -D CPU_IS_ARM=1

OBJS=$(CRT0) main.o vcmx212.o monlib.o

$(APPNAME): $(OBJS) $(LDSCRIPT) makefile
	$(LD) -e start -o $(APPNAME) -T $(LDSCRIPT) $(OBJS) -L $(LIBCDIR) -L $(EFSL_LIBDIR) $(EFSL_LIBS) -lc $(LIBGCC)
	$(NM) --numeric-sort $(APPNAME) >$(APPNAME).sym
	$(OBJDUMP) --source --disassemble $(APPNAME) > $(APPNAME).dis
	$(STRIP) $(APPNAME) 

crt0_arm.o: crt0_arm.S
	$(CC) $(CFLAGS) -o $@ crt0_arm.S

main.o: main.c 
	$(CC) $(CFLAGS) -D MONCOMPTR=$(MONCOMPTR) -o $@ main.c

vcmx212.o: vcmx212.c
	$(CC) $(CFLAGS) -o $@ vcmx212.c

monlib.o: monlib.c
	$(CC) $(CFLAGS) -o $@ monlib.c


clean:
	rm -f *.o $(APPNAME) $(APPNAME).ezip $(APPNAME).sym $(APPNAME).dis symtbl

