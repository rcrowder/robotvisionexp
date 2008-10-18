echo Setting up the environment for debugging gdb.\n

set complaints 1
set output-radix 16
set input-radix 16

target remote localhost:8888
#mon arm7_9 sw_bkpts enable

# Increase the packet size to improve download speed.
set remote memory-write-packet-size 1024
set remote memory-write-packet-size fixed


 
# Load the program executable called "test"
load main
# Load the symbols for the program.
symbol-file main




# Set a breakpoint at main().
break main
c
# b servo.c:ServoInt

# Run to the breakpoint.
# c


