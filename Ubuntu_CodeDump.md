# Kernel rebuilding #

```
cd /usr/src
sudo apt-get build-dep linux-image-$(uname -r)
apt-get source linux-image-$(uname -r)
sudo tar xjvf linux-source-2.6.24.tar.bz2
cd linux-source-2.6.24
sudo cp -vi /boot/config-`uname -r` .config
sudo make oldconfig
sudo make prepare
sudo make
```

# Extracting a JFFS2 file #
```
mknod /tmp/mtdblock0 b 31 0
modprobe mtdblock
modprobe mtdram total_size=65536 erase_size=256
modprobe jffs2
dd if=/pathtoimage/rootfs.jffs2 of=/tmp/mtdblock0
mkdir /media/jffs2
mount -t jffs2 /tmp/mtdblock0 /media/jffs2
```