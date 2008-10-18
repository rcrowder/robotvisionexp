PROC=arm
TYPE=elf
LDSCRIPT=ldscript

LIBPATH=/usr/local/$(PROC)-$(TYPE)/lib
INCPATH=/usr/local/$(PROC)-$(TYPE)/include
CC=$(PROC)-$(TYPE)-gcc
AS=$(PROC)-$(TYPE)-as
AR=$(PROC)-$(TYPE)-ar
LD=$(PROC)-$(TYPE)-ld
NM=$(PROC)-$(TYPE)-nm
OBJDUMP=$(PROC)-$(TYPE)-objdump
CPPFLAGS=
CFLAGS=-g -c

main: main.o Makefile $(LDSCRIPT) crt0.o
	$(LD) -g -v -T $(LDSCRIPT) -o $@ crt0.o $@.o
	$(NM) $@
	arm-elf-objcopy -O binary $@ $@.bin 
	arm-elf-objdump -S -l $@ >$@.src

crt0.o:  Makefile $(LDSCRIPT) crt0.S
	$(CC) -g -c -o crt0.o crt0.S

clean:
	-/bin/rm *.o
	-/bin/rm main
