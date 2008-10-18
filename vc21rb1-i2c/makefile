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
CFLAGS=-g 

main: main.c Makefile $(LDSCRIPT) crt0.o i2c.o motor.o servo.o adc.o
	$(CC) -g -c  main.c
	$(LD) -g -v -T $(LDSCRIPT) -o main crt0.o main.o i2c.o motor.o servo.o adc.o
	$(NM) main
	arm-elf-objcopy -O ihex  main main.hex 
	arm-elf-objcopy -O binary  main main.bin
	$(OBJDUMP) --disassemble -S main > main.src

	
crt0.o:  Makefile $(LDSCRIPT) crt0.S
	$(CC) -g -c -o crt0.o crt0.S
#	$(NM) crt0.o

dump:
	$(OBJDUMP) --all-headers main

dump_test:
	$(OBJDUMP) --disassemble main

clean:
	-/bin/rm *.o
	-/bin/rm main
