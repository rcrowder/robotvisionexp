
set endian little

# Tell GDB to use 1024 bytes packes when downloading, this
# reduces load image download times
set remote memory-write-packet-size 1024
set remote memory-write-packet-size fixed

set output-radix 16


# Tell GDB where send it's "gdb monitor" commands:
# Tell GDB to send them to the OcdRemote monitoring port 8888
# that is running on your PC
#target remote 192.168.5.83:8888

target remote localhost:8888

cd build_VCMX212
load ramtst.elf
symbol-file ramtst.elf

