APPRAMBASE	= 0x20100000

APPNAME		= uumon
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
TARGET_IP	= 135.222.140.72

TOOL_PREFIX	:= arm-elf
CFLAGS		:= -fno-builtin -mcpu=arm9tdmi -c -Wall -O -g -I.

OBJS=crt0_arm.o chario.o cmdtbl.o cmds.o cpuio.o docmd.o genlib.o \
	mprintf.o main.o xmodem.o

#####
#
# $(APPNAME):
# Top level target builds the application.
#
$(APPNAME): $(OBJS) makefile
	$(LD) -e start -o $(APPNAME) -Ttext $(APPRAMBASE) $(OBJS) $(LIBGCC) 
	$(NM) --numeric-sort $(APPNAME) >$(APPNAME).sym
	$(OBJDUMP) --source --disassemble $(APPNAME) > $(APPNAME).dis
	elf -B uumon.bin uumon
	elf -m uumon

##	$(STRIP) $(APPNAME) 

#####
#
# Objects:
#
crt0_arm.o: crt0_arm.S
	$(CC) $(CFLAGS) -o $@ crt0_arm.S

chario.o: chario.c 
	$(CC) $(CFLAGS) -o $@ chario.c

cmdtbl.o: cmdtbl.c 
	$(CC) $(CFLAGS) -o $@ cmdtbl.c

docmd.o: docmd.c 
	$(CC) $(CFLAGS) -o $@ docmd.c

genlib.o: genlib.c 
	$(CC) $(CFLAGS) -o $@ genlib.c

mprintf.o: mprintf.c 
	$(CC) $(CFLAGS) -o $@ mprintf.c

cmds.o: cmds.c 
	$(CC) $(CFLAGS) -o $@ cmds.c

main.o: main.c 
	$(CC) $(CFLAGS) -o $@ main.c

xmodem.o: xmodem.c 
	$(CC) $(CFLAGS) -o $@ xmodem.c

#####
#
# clean:
# Remove all files created by this make.
#
clean:
	rm -f *.o $(APPNAME) $(APPNAME).ezip $(APPNAME).sym $(APPNAME).dis $(APPNAME).bin symtbl

#####
#
# sym:
# Create and download the symbol table file that can be used by uMon
# with this application...
#
sym: 
	@if ! test -f $(APPNAME).sym; then echo Must build $(APPNAME) first; exit 1; fi
	monsym -p0x $(APPNAME).sym >symtbl
	ttftp $(TARGET_IP) put symtbl


	
#####
#
# dld:
# Use the ttftp tool (supplied with MicroMonitor) to download the
# application to the target.
#
dld: 
	ttftp $(TARGET_IP) put $(APPNAME).bin $(APPRAMBASE)
