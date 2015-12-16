# Introduction #

Quoting from the Buildroot webpage:

> Buildroot is a set of Makefiles and patches that makes it easy generate a
> cross-compilation toolchain and root filesystem for your target Linux system
> using the uClibc C library.

These steps help us produce three valuable things;
  * uClibc based cross-compiler
  * BusyBox build, with ability to tweak and reconfigure
  * Jffs2 filesystem, complete with BusyBox and shared uClibc libraries

# Details #

First we need to grab the latest SVN snapshot, and setup buildroot.

```
cd ~/dev
svn co svn://uclibc.org/trunk/buildroot
cd ~/dev/buildroot
make menuconfig
cp ~/dev/busybox_vcmx212_defconfig .
```

Most of the settings are obvious. The main one to modify is the location of the busy box configuration file. Included in the Downloads section here is a slimmed down BB configuration that matches the original coglinux.jffs2 contents.

We are building coglinux separately, so no need to build a Linux kernel.

For the Target filesystem options, make sure that 'jffs2 root filesystem' is selected. And the 'Memory Type' is changed to '4 kB pagesize and 64 kB erase size'.

Now we need to prepare the target directory structure, and download the latest sources.

```
make source
```

Finally we can copy over the coglinux root file system device table into the generic target. And run the final make.

Note: Make sure that you install the Zlib development package via Synaptic Package Manager.

```
cp ~/dev/rootfs/devtable.txt ~/dev/buildroot/target/generic/device_table.txt
sudo apt-get install liblzo2-dev acl-dev 
make WITHOUT_XATTR=1
```

Changes can be made using;
```
make busybox-menuconfig
make uclibc-menuconfig
```