echo Setting up the environment for debugging gdb.\n

set complaints 1
set output-radix 16
set input-radix 16

# This connects to OpenOcd at localhost:8888
# You will need to change this to reflect the address of your JTAG server.
target remote localhost:8888

# Increase the packet size to improve download speed.
set remote memory-write-packet-size 1024
set remote memory-write-packet-size fixed

mon arm7_9 sw_bkpts enable


#init PLL
mon mww 0x10027000 0x1f18060f
mon mww 0x10027004 0x007b1c73
mon mww	0x10027000 0x1f38060f

#init SDRAM
mon mww 0x10000000  0x00040304 
mon mww 0x10020000  0x00000000 
mon mww 0x10000004  0xFFFBFCFB 
mon mww 0x10020004  0xFFFFFFFF 
mon mww 0xDF000000  0x8212c339 
mon mww 0xC0200000  0x00000000 
mon mww 0xDF000000  0xA212c339 
mon mww 0xC0000000  0x00000000 
mon mww 0xC0000000  0x00000000 
mon mww 0xC0000000  0x00000000 
mon mww 0xC0000000  0x00000000 
mon mww 0xC0000000  0x00000000 
mon mww 0xC0000000  0x00000000 
mon mww 0xC0000000  0x00000000 
mon mww 0xC0000000  0x00000000 
mon mww 0xDF000000  0xB212c339 
mon mww 0xC0119800  0x00000000 
mon mww 0xDF000000  0x8212C339 

#init I cache
mon arm926ejs cp15 0 0 1 0 0x51078
mon arm926ejs cp15 0 0 7 5 0

mon arm7_9 fast_memory_access enable 

# Load the program executable called "test"
load main
# Load the symbols for the program.
symbol-file main

# Set a breakpoint at main().
b main

# Run to the breakpoint.
c


