# Introduction #

Previously I had setup a Virtual Cogs development environment that matched the environment that the Virtual Cogs Wiki outlines. It is based around Windows OS with cygwin providing the Linux distro to allow target cross-compiling.

With an interest in gaining Linux development experience I have now moved across to using Ubuntu Linux distro (via the Wubi installer).

The following outlines the procedures I have taken to setup Ubuntu for Virtual Cogs development and cross-compiling.

## Ubuntu Installation ##

My development laptop (IBM Thinkpad T42) has it's single hard drive partitioned into two drives of equal size. My exisiting Windows based development environment spanned across both drives, with the majority of the environment residing on the second hard drive.

Disk space was created (~18GB) on the second hard drive to allow for a Wubi install of Ubuntu. Annoyingly the disk partitions are not NTFS, so the FAT maximum file limit limits the maximum size of disk image files that Wubi creates. Telling Wubi to use 15GB for installation, for example, still only allows the virtual disk files to be a maximum of 4GB. Hopefully that will be enough space, particularly in the 'home.disk' to contain all the development libraries, toolchains, etc. But then there is access via root folders to the Windows drives and folders.

## Ubuntu Setup ##

An initial install of Ubuntu is quite bare for development purposes. So the first thing required is to grab various packages required to configure and build various libraries and drivers.

When the initial required packages are installed, the additional Virtual Cogs specific packages and libraries will be collected and built into one location, namely ~/dev
```
cd ~
mkdir dev
cd dev
mkdir libs
mkdir drivers
```

### Packages ###
  * build-essential
  * autoconf
  * automake
  * texinfo
  * mtd-tools
  * flex & bison
  * libtool
  * libusb-dev
  * libncurses5-dev
  * SVN(Subversion) & CVS
  * RPM & alien

To grab and install the latest versions of these packages, the following can be performed;
```
cd ~
sudo apt-get install build-essential autoconf automake texinfo mtd-tools flex bison libtool libusb-dev libncurses5-dev subversion cvs rpm alien
```

Make sure we're using bash instead of dash
```
sudo ln -sf /bin/bash /bin/sh
```

## GNU ARM Toolchain (Cross compiler) ##

Now we need an ARM compiler to cross compile for the VCMX212 target. Virtual Cogs recommends and supplies (via a link on their main web support page) the GNUArm.org toolchain. That is quite a simply way of installing the ARM toolchain under Windows/Cygwin, but we need to make some changes to one of the build patches to make it compiler under Ubuntu (Hardy 8.04).

The version used for this installation is; http://www.virtualcogs.com/downloads/crosstool-0.43.tar.gz

Installation steps are simply;
```
# Prepare target directory
sudo mkdir /opt/crosstool
sudo chmod 775 /opt
sudo chmod 775 /opt/crosstool
sudo chown ????:???? /opt/crosstool (change to your user:group)
# Refer to this VC Wiki page for building the crosstool chain http://wiki.virtualcogs.com/index.php?title=Toolchain_Installation
# Extract crosstool-0.43.tar.gz into ~/dev/crosstool-0.43
cd ~/dev/crosstool-0.43
# Dealing with demo-arm.sh and arm.dat as outlined in the VC Wiki
# We also need to modify a patch to allow 4.2+ gcc compilers.
gedit patches/glibc-2.3.2/glibc-2.3.3-allow-gcc-4.0-configure.patch
# Change the line
'+    3.[2-9]*|4.[01]*)' to
'+    3.[2-9]*|4.[012]*)'
# Remove the backup file gedit creates
rm patches/glibc-2.3.2/glibc-2.3.3-allow-gcc-4.0-configure.patch~
# Lets try a build
./demo-arm.sh
```

If you get issues with version-info.h missing terminating " character, you need to upgrate automake to v1.9+ This forum [thread](http://www.triple-dragon-fan.de/board/thread.php?threadid=2846) describes the problem and fixes.

### Insight / GDB ###

```
cd ~/dev/insight-6.6
sudo apt-get install libx11-dev
./configure --prefix=/opt/crosstool/gcc-4.0.2-glibc-2.3.2/arm-linux --target=arm-linux
make
sudo make install
```
Test using arm-linux-insight

## Libraries & Drivers ##

### Silabs USB Bridge ###
http://www.etheus.net/CP210x_Linux_Driver

The cp2101 kernel module exists in my version of Ubutu (Hardy 8.04). And should work OK. IF it doesn't then check out this [Wiki](Ubuntu_CP2101_Install.md) page to install the latest version from Silabs.

Now to test if it is working.. Firstly plug the mini-USB edge of the cable into the VCMX212, then plug the other end into the PC. We can then use dmesg to see if the Silabs driver gets loaded, and minicom as the UART terminal program to talk to the VCMX212.

The first time minicom is called, we need to use the -s option to enter the setup menus, so that we can change the port and baud rate. Typically the Silabs driver hangs off of /dev/ttyUSB0. Plus the required baud rate is different from the rate described in the VCMX212 Manual PDF, and needs to be set to 230400 (8N1).
```
dmesg | grep USB
lsmod | grep cp21
sudo apt-get install minicom
sudo apt-get install lrzsz
sudo minicom -s
minicom
```

### Olimex ARM-OCD-Tiny JTAG dongle ###
I'm using this version of the Olimex JTAG dongle to connect to the VCMX212 and VC21RB1 JTAG ports. Thankfully the dongle uses a FTDI FT2232 based chip, so we can build the opensource libftdi library. This library will then be used with OpenOCD to allow the GDB debugger to talk via JTAG with the ARM processors.

#### libftdi ####
The current version of libftdi is 0.13, taken from; http://www.intra2net.com/de/produkte/opensource/ftdi/

Steps required to build libftdi are;
```
cd ~/dev/libs
gzip -cd libftdi-0.13.tar.gz | tar xvf -
cd libftdi-0.13
./configure
make
sudo make install
```

#### OpenOCD ####
http://openfacts.berlios.de/index-en.phtml?title=Open_On-Chip_Debugger

http://buffalo.nas-central.org/index.php/JTAG_&_OpenOCD_for_LS-Pro

http://forum.sparkfun.com/viewtopic.php?p=47245&highlight=#47245

  * TODO: Does the USB codes match the existing OpenOCD VC config file?
> > Yes, but ft2232\_device\_desc needed to change to "Olimex OpenOCD JTAG TINY"
  * TODO: GDB Server??
> > Comes with Codesourcery??

```
cd ~/dev/drivers
svn co svn://svn.berlios.de/openocd/trunk openocd
cd openocd
./bootstrap
./configure --enable-ft2232_libftdi
make
sudo make install
Add "none /proc/bus/usb usbfs auto,users,devmode=0666 0 0" to /etc/fstab
cd ~/dev
sudo openocd -f vcmx212.cfg &
```
Use lsusb to check that the dongle is connected, eg. for my Olimex ARM-USB-OCD there is the line "Bus 002 Device 003: ID 15ba:0004" (For others it could be "15ba:0003")..
```
cat /sys/bus/usb/devices/2-2/idProduct should equal "0004"
cat /sys/bus/usb/devices/2-2/idVendor should equal "15ba"
cat /sys/bus/usb/devices/2-2/manufacturer should equal "Olimex"
cat /sys/bus/usb/devices/2-2/product should equal "Olimex OpenOCD JTAG TINY"
```

We now need to set up the permissions correctly for the Olimex JTAG programmer. Create a file called /etc/udev/rules.d/45-ft2232.rules with the following text:
```
BUS!="usb", ACTION!="add", SUBSYSTEM!=="usb_device", GOTO="kcontrol_rules_end"

SYSFS{idProduct}=="0004", SYSFS{idVendor}=="15ba", MODE="664", GROUP="adm"

LABEL="kcontrol_rules_end"
```
You will have to reboot your machine for the changes to take effect.

## Eclipse ##

Install from Eclipse website. Update Java;
```
sudo apt-get install sun-java6-jdk
sudo update-alternatives --config java
```
Install Zylin CDT (http://www.zylin.com/zylincdt)

Qt integration (http://trolltech.com/developer/downloads/qt/eclipse-integration-download)

  * TODO: OpenOCD/GDB integration

  1. Follow the instructions for installing Eclipse and JRE from https://help.ubuntu.com/community/EclipseIDE
  1. Continue with Virtual Cogs Wiki installation steps from http://wiki.virtualcogs.com/index.php?title=Eclipse_IDE_setup_with_Virtual_Cogs

## COGLinux ##

  1. Use the instructions from the Wiki (http://wiki.virtualcogs.com/index.php?title=Obtaining_Sources_via_Sourceforge_CVS)
  1. And the SourceForge VC FAQ (http://sourceforge.net/cvs/?group_id=196361)
  1. Rebuild kernel (http://wiki.virtualcogs.com/index.php?title=Building_Linux)
    * Don't for get to grab libncurses5-dev
    * And edit the Makefile (after 'make menuconfig' step) to change the CROSS\_COMPILE define
  1. Reflash kernel (http://wiki.virtualcogs.com/index.php?title=Reflashing_Linux_Kernel)