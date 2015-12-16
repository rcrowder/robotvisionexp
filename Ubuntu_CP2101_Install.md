# Introduction #

The current version at time of writing is 3.0.0, from; linux\_VCP\_v26\_driver\_src.zip (https://www.silabs.com/support/pages/support.aspx?ProductFamily=USB+MCUs)

```
sudo touch /usr/src/`uname -r`/include/linux/config.h
cd ~/dev/drivers
#Extract cp2103-0.11.0-1.src.rpm from linux_VCP_v26_driver_src.zip
#Extract cp2103-0.11.0.tar.gz from the rpm
gunzip cp2103-0.11.0.tar.gz
tar xvf cp2103-0.11.0.tar
cd cp2103-0.11.0
./configure -kver $(uname -r)
cp Makefile26 Makefile
gedit cp2101.c (to change '#include "usb-serial.h"' to '#include <linux/usb/serial.h>')
sudo make -f Makefile.go modules
chmod 755 installmod
sudo make -f Makefile.go install
```

Now to test if it is working.. Firstly plug the mini-USB edge of the cable into the VCMX212, then plug the other end into the PC. We can then use dmesg to see if the Silabs driver gets loaded, and minicom as the UART terminal program to talk to the VCMX212.

The first time minicom is called, we need to use the -s option to enter the setup menus. So that we can change the port and baud rate. Typically the Silabs driver hangs off of /dev/ttyUSB0, although this can vary. Plus the required baud rate is different from the rate described in the VCMX212 Manual PDF, and needs to be set to 230400 (8N1).
```
dmesg | grep USB
lsmod | grep cp21
sudo apt-get install minicom
sudo apt-get install lrzsz
sudo minicom -s
minicom
```