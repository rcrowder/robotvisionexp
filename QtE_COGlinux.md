# Introduction #

**!!!!WORK - IN - PROGRESS!!!!**

_**!!Use at your own risk!!**_


# Details #

## Touch Screen Driver ##

http://www.jespersaur.com/drupal/node/30

http://linux-toolbox.blogspot.com/2008/04/cross-compiling-tslib.html

http://groups.google.co.uk/group/virtualcogs/browse_thread/thread/58528d8bf9f84e8d/51bffac0f45c68cb?lnk=gst&q=tslib#51bffac0f45c68cb

http://groups.google.co.uk/group/virtualcogs/browse_thread/thread/a45cbad09f73c852/4cc767cd11663d12?lnk=gst&q=DirectFB#4cc767cd11663d12
  * TODO: Sort out shared libraries setup in COGlinux

  1. Make the shared library and tools
```
  cd ~/dev/libs
  svn co svn://svn.berlios.de/tslib/trunk/tslib tslib
  cd tslib
  ./autogen.sh
  ./configure --host=arm-none-linux-gnueabi --prefix=/mnt/sd??? \
    --disable-linear-h2200 --disable-ucb1x00 --disable-corgi \
    --disable-collie --disable-h3600 --disable-mk712 \
    --disable-arctic2 --disable-tatung
  gedit config.h (to remove the '#define malloc rpl_malloc' line near end of file)
  make
  make install
  gedit /mnt/sd/etc/ts.conf (to uncomment the '# module_raw input' line)
  cp ~/dev/tsc2100.ko /mnt/sd (copy over to SD card the touchscreen kernel module)
```
  1. Transfer to COGlinux file system
  1. Test the tools, e.g. ts\_calibrate
  1. Copy over and soft-link missing shared libraries (libc, libdl, etc)
```
  cd ~/CodeSourcery/Sourcery_G++_Lite/arm-none-linux-gnueabi/libc/lib
  cp ld-2.5.so /mnt/sd/lib
  cp libc-2.5.so /mnt/sd/lib
  cp libdl-2.5.so /mnt/sd/lib
  cp libm-2.5.so /mnt/sd/lib

  ... Move SD card to VC21BR1

  mount /mnt/sd
  cd /mnt/sd
  insmod tsc2100.ko
  mknod /dev/input0 c 13 64
  cd lib
  cp *.so /lib
  ln -s ld-2.5.so ld-linux.so.3
  ln -s libc-2.5.so libc.so.6
  ln -s libdl-2.5.so libdl.so.2
  ln -s libm-2.5.so libm.so.6
```

## Qt Install ##

Qt Open Source Edition for C++ Developers: Embedded Linux Download (http://trolltech.com/downloads/opensource)

  * TODO: Create new mkspec for gnueabi-linux-arm-g++
  * TODO: Sort out ./configure options
```
cd ~/dev/QtE
gzip -cd qt-embedded-linux-opensource-src-4.4.0.tar.gz | tar xvf -
cd qt-embedded-linux-opensource-src-4.4.0
export QTEDIR=$PWD
./configure \
  -prefix /mnt/sd \
  -no-largefile -no-qvfb -no-svg -no-webkit \
  -qt-zlib -qt-gif -qt-libtiff -qt-libpng -qt-libmng -qt-libjpeg \
  -nomake demos \
  -pch \
  -xplatform qws/gnueabi-linux-arm-g++ \
  -embedded arm \
  -qconfig medium \
  -qt-mouse-tslib \
  -no-qt3support \
  -L/mnt/sd/lib/ -I/mnt/usb/include/
make
make install
```

## Extras ##

http://wiki.java.net/bin/view/Mobileandembedded/PhoneMEAdvancedBuildInstructions?TWIKISID=658314976c64cf27ad00d110126f55c5

http://jamvm.sourceforge.net/ (Not IBM or Sun Classpath??)