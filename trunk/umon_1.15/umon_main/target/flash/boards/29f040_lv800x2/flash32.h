/*  Header for 32-bit flash position independent code (PIC)
 *
 *  This file defines the command sequence operations to program
 *	two AMD 29LV800B flash memory devices (in 16-bit mode) which
 *	are configured together to behave as one 32-bit memory device.
 *	
 *	General notice:
 *	This code is part of a boot-monitor package developed as a generic base
 *	platform for embedded system designs.  As such, it is likely to be
 *	distributed to various projects beyond the control of the original
 *	author.  Please notify the author of any enhancements made or bugs found
 *	so that all may benefit from the changes.  In addition, notification back
 *	to the author will allow the new user to pick up changes that may have
 *	been made by other users after this version of the code was distributed.
 *	
 *	Author:	Ed Sutter
 *	email:	esutter@lucent.com	(home: lesutter@worldnet.att.net)
 *	phone:	908-582-2351		(home: 908-889-5161)
 *	
 *	Mar 1999	Modified for 2 chips in parallel (32-bit) by Robert Snyder
 *	
 */

#define	ftype				volatile unsigned long
#define	NotAligned(ptr)		((unsigned long)ptr & 3)

/*
 *  The following commands are used to program two AMD 29LV800B devices,
 *  each in 16-bit mode, combined to effectively create a 32-bit part.
 *  Chip address bits A18-A0 map to PPC address bits 11 - 29.  Bits A18-A11
 *  (11 - 18) are "don't care" for unlock and command cycles except when
 *  Program Address or Sector Address are needed.  The upper data byte
 *  DQ15-DQ8 is "don't care" in each device except when supplying program
 *  data.
 *
 *  Because of this, the write to 0x555 is the same as to 0x5555 and the
 *  write to 0x2aa is the same as to 0xaaaa seen in other similar routines.
 */
#define Write_aa_to_555()  (*(ftype *)(fdev->base+(0x555<<2)) = 0x00aa00aa)
#define Write_80_to_555()  (*(ftype *)(fdev->base+(0x555<<2)) = 0x00800080)
#define Write_a0_to_555()  (*(ftype *)(fdev->base+(0x555<<2)) = 0x00a000a0)
#define Write_f0_to_555()  (*(ftype *)(fdev->base+(0x555<<2)) = 0x00f000f0)
#define Write_20_to_555()  (*(ftype *)(fdev->base+(0x555<<2)) = 0x00200020)
#define Write_90_to_555()  (*(ftype *)(fdev->base+(0x555<<2)) = 0x00900090)
#define Write_00_to_555()  (*(ftype *)(fdev->base+(0x555<<2)) = 0x00000000)
#define Write_55_to_2aa()  (*(ftype *)(fdev->base+(0x2aa<<2)) = 0x00550055)
#define Write_30_to_(add)  (*(ftype *)add = 0x00300030)
#define Read_000()		   (*(ftype *)(fdev->base+(0x000<<2)))
#define Read_001()		   (*(ftype *)(fdev->base+(0x001<<2)))
#define Read_555()		   (*(ftype *)(fdev->base+(0x555<<2)))
