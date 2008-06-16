/* strata.h:
 * Reusable macros that support the Intel STRATA flash device family.
 * for the most common configurations.
 *
 *  8x1: one  8-bit wide device
 *  8x2: two  8-bit wide devices in parallel
 *  8x4: four 8-bit wide devices in parallel
 * 16x1: one 16-bit wide device
 * 16x2: two 16-bit wide devices in parallel
 */

#ifndef FLASHCFG_8x1
#define FLASHCFG_8x1 0
#endif
#ifndef FLASHCFG_8x2
#define FLASHCFG_8x2 0
#endif
#ifndef FLASHCFG_8x4
#define FLASHCFG_8x4 0
#endif
#ifndef FLASHCFG_16x1
#define FLASHCFG_16x1 0
#endif
#ifndef FLASHCFG_16x2
#define FLASHCFG_16x2 0
#endif

#if FLASHCFG_16x1 // One 16-bit wide device

#define BUFFER_SIZE		32
#define BUFFER_ALIGN	(BUFFER_SIZE-1)

#ifndef ftype
#define ftype	volatile unsigned short
#endif

#define WSMS	0x0080
#define WSMS_L	0x0080
#define WSMS_H	0x0000
#define ESS		0x0040
#define ES		0x0020
#define PSS		0x0004
#define PS		0x0010
#define WBS		0x0080

#define STRATACMD_READARRAY()			(*(ftype *)(fdev->base) = 0x00ff)
#define STRATACMD_PROTPROGRAM()			(*(ftype *)(fdev->base) = 0x00c0)
#define STRATACMD_READQUERY()			(*(ftype *)(fdev->base) = 0x0098)
#define STRATACMD_READID()				(*(ftype *)(fdev->base) = 0x0090)
#define STRATACMD_READSTATUS()			(*(ftype *)(fdev->base) = 0x0070)
#define STRATACMD_LOCKBIT()				(*(ftype *)(fdev->base) = 0x0060)
#define STRATACMD_CLEARSTATUS()			(*(ftype *)(fdev->base) = 0x0050)
#define STRATACMD_PROGRAM()				(*(ftype *)(fdev->base) = 0x0040)
#define STRATACMD_WRITETOBUFFER(addr)	(*(ftype *)(addr) = 0x00e8)
#define STRATACMD_CONFIRM(addr)			(*(ftype *)(addr) = 0x00d0)
#define STRATACMD_BLOCKERASE(addr)		(*(ftype *)(addr) = 0x0020)
#define STRATACMD_SETLOCKCONFIRM(addr)	(*(ftype *)(addr) = 0x0001)

#elif FLASHCFG_16x2	// Two 16-bit wide devices in parallel

#define BUFFER_SIZE		(32*2)
#define BUFFER_ALIGN	(BUFFER_SIZE-1)

#ifndef ftype
#define ftype	volatile unsigned long
#endif

#define WSMS	0x00800080
#define WSMS_L	0x00800080
#define WSMS_H	0x00000000
#define ESS		0x00400040
#define ES		0x00200020
#define PSS		0x00040004
#define PS		0x00100010
#define WBS		0x00800080

#define STRATACMD_READARRAY()			(*(ftype *)(fdev->base) = 0x00ff00ff)
#define STRATACMD_PROTPROGRAM()			(*(ftype *)(fdev->base) = 0x00c000c0)
#define STRATACMD_READQUERY()			(*(ftype *)(fdev->base) = 0x00980098)
#define STRATACMD_READID()				(*(ftype *)(fdev->base) = 0x00900090)
#define STRATACMD_READSTATUS()			(*(ftype *)(fdev->base) = 0x00700070)
#define STRATACMD_LOCKBIT()				(*(ftype *)(fdev->base) = 0x00600060)
#define STRATACMD_CLEARSTATUS()			(*(ftype *)(fdev->base) = 0x00500050)
#define STRATACMD_PROGRAM()				(*(ftype *)(fdev->base) = 0x00400040)
#define STRATACMD_WRITETOBUFFER(addr)	(*(ftype *)(addr) = 0x00e800e8)
#define STRATACMD_CONFIRM(addr)			(*(ftype *)(addr) = 0x00d000d0)
#define STRATACMD_BLOCKERASE(addr)		(*(ftype *)(addr) = 0x00200020)
#define STRATACMD_SETLOCKCONFIRM(addr)	(*(ftype *)(addr) = 0x00010001)

#elif FLASHCFG_8x1	// One 8-bit wide device

#define BUFFER_SIZE		32
#define BUFFER_ALIGN	(BUFFER_SIZE-1)

#ifndef ftype
#define ftype	volatile unsigned char
#endif

#define WSMS	0x80
#define ESS		0x40
#define ES		0x20
#define PSS		0x04
#define PS		0x10
#define WBS		0x80

#define STRATACMD_READARRAY()			(*(ftype *)(fdev->base) = 0xff)
#define STRATACMD_PROTPROGRAM()			(*(ftype *)(fdev->base) = 0xc0)
#define STRATACMD_READID()				(*(ftype *)(fdev->base) = 0x90)
#define STRATACMD_READSTATUS()			(*(ftype *)(fdev->base) = 0x70)
#define STRATACMD_LOCKBIT()				(*(ftype *)(fdev->base) = 0x60)
#define STRATACMD_CLEARSTATUS()			(*(ftype *)(fdev->base) = 0x50)
#define STRATACMD_PROGRAM()				(*(ftype *)(fdev->base) = 0x40)
#define STRATACMD_WRITETOBUFFER(addr)	(*(ftype *)(addr) = 0xe8)
#define STRATACMD_CONFIRM(addr)			(*(ftype *)(addr) = 0xd0)
#define STRATACMD_BLOCKERASE(addr)		(*(ftype *)(addr) = 0x20)
#define STRATACMD_SETLOCKCONFIRM(addr)	(*(ftype *)(addr) = 0x01)

#elif FLASHCFG_8x2	// Two 8-bit wide devices in parallel

#define BUFFER_SIZE		(32*2)
#define BUFFER_ALIGN	(BUFFER_SIZE-1)

#ifndef ftype
#define ftype	volatile unsigned short
#endif

#define WSMS	0x8080
#define ESS		0x4040
#define ES		0x2020
#define PSS		0x0404
#define PS		0x1010
#define WBS		0x8080

#define STRATACMD_READARRAY()			(*(ftype *)(fdev->base) = 0xffff)
#define STRATACMD_PROTPROGRAM()			(*(ftype *)(fdev->base) = 0xc0c0)
#define STRATACMD_READID()				(*(ftype *)(fdev->base) = 0x9090)
#define STRATACMD_READSTATUS()			(*(ftype *)(fdev->base) = 0x7070)
#define STRATACMD_LOCKBIT()				(*(ftype *)(fdev->base) = 0x6060)
#define STRATACMD_CLEARSTATUS()			(*(ftype *)(fdev->base) = 0x5050)
#define STRATACMD_PROGRAM()				(*(ftype *)(fdev->base) = 0x4040)
#define STRATACMD_WRITETOBUFFER(addr)	(*(ftype *)(addr) = 0xe8e8)
#define STRATACMD_CONFIRM(addr)			(*(ftype *)(addr) = 0xd0d0)
#define STRATACMD_BLOCKERASE(addr)		(*(ftype *)(addr) = 0x2020)
#define STRATACMD_SETLOCKCONFIRM(addr)	(*(ftype *)(addr) = 0x0101)

#elif FLASHCFG_8x4	// Four 8-bit wide devices in parallel

#define BUFFER_SIZE		(32*4)
#define BUFFER_ALIGN	(BUFFER_SIZE-1)

#ifndef ftype
#define ftype	volatile unsigned long
#endif

#define WSMS	0x80808080
#define ESS		0x40404040
#define ES		0x20202020
#define PSS		0x04040404
#define PS		0x10101010
#define WBS		0x80808080

#define STRATACMD_READARRAY()			(*(ftype *)(fdev->base) = 0xffffffff)
#define STRATACMD_PROTPROGRAM()			(*(ftype *)(fdev->base) = 0xc0c0c0c0)
#define STRATACMD_READID()				(*(ftype *)(fdev->base) = 0x90909090)
#define STRATACMD_READSTATUS()			(*(ftype *)(fdev->base) = 0x70707070)
#define STRATACMD_LOCKBIT()				(*(ftype *)(fdev->base) = 0x60606060)
#define STRATACMD_CLEARSTATUS()			(*(ftype *)(fdev->base) = 0x50505050)
#define STRATACMD_PROGRAM()				(*(ftype *)(fdev->base) = 0x40404040)
#define STRATACMD_WRITETOBUFFER(addr)	(*(ftype *)(addr) = 0xe8e8e8e8)
#define STRATACMD_CONFIRM(addr)			(*(ftype *)(addr) = 0xd0d0d0d0)
#define STRATACMD_BLOCKERASE(addr)		(*(ftype *)(addr) = 0x20202020)
#define STRATACMD_SETLOCKCONFIRM(addr)	(*(ftype *)(addr) = 0x01010101)

#endif

#define FLASH_WRITEVAL(to,val)			(*(ftype *)(to) = val)
#define FLASH_WRITEPTR(to,frm)			(*(ftype *)(to) = *(ftype *)frm)
#define FLASH_READ(addr)				(*(ftype *)(addr))
#define FLASH_READBASE()				(*(ftype *)(fdev->base))
#define FLASH_READ_DEVICEID()			(*(ftype *)(fdev->base+sizeof(ftype)))
#define FLASH_READ_BLOCKSTATUS(sbase)	(*(ftype *)(sbase+(sizeof(ftype)*2)))

#define WAIT_FOR_DATA(add,data) \
	{ \
		volatile int timeout = FLASH_LOOP_TIMEOUT; \
		while(*(ftype *)add != *(ftype *)data) { \
			if (--timeout <= 0) { \
				STRATACMD_READARRAY(); \
				return(-2); \
			} \
			WATCHDOG_MACRO; \
		} \
	}

#define WAIT_FOR_WSMS_READY() \
	{ \
		volatile int timeout = FLASH_LOOP_TIMEOUT; \
		while((*(ftype *)(fdev->base) & WSMS) != WSMS) { \
			if (--timeout <= 0) { \
				STRATACMD_READARRAY(); \
				return(-4); \
			} \
			WATCHDOG_MACRO; \
		} \
	}

#define WAIT_FOR_WSMS_STATUS_READY() \
	{ \
		volatile int timeout = FLASH_LOOP_TIMEOUT; \
		while(1) { \
			STRATACMD_READSTATUS(); \
			if ((*(ftype *)(fdev->base) & WSMS) == WSMS) \
				break; \
			if (--timeout <= 0) { \
				STRATACMD_READARRAY(); \
				return(-4); \
			} \
			WATCHDOG_MACRO; \
		} \
	}

#define WAIT_FOR_FF(add) \
	{ \
		volatile int timeout = FLASH_LOOP_TIMEOUT; \
		while(*(ftype *)add != (ftype)0xffffffff) { \
			if (--timeout <= 0) { \
				STRATACMD_READARRAY(); \
				return(-3); \
			} \
			WATCHDOG_MACRO; \
		} \
	}

#define DELAY_LOOP(a) \
	{ \
		volatile int i, j; \
		for(i=0;i<a;i++) { \
			for(j=0;j<FLASH_LOOP_TIMEOUT;j++); \
		} \
	}

/* FLASHOP_PRINT() & FLASHOP_FLUSH():
 * Macros that can be placed within the flash operation function to provide
 * printf-trace but will only be enabled if FLASH_COPY_TO_RAM is not
 * defined.
 */
#ifndef FLASH_COPY_TO_RAM
#define FLASHOP_PRINT(a) \
	{ \
		if (FlashTrace) \
			printf a ;\
	}
#else
#define FLASHOP_PRINT(a)
#endif

#ifndef FLASH_COPY_TO_RAM
#define FLASHOP_FLUSH() \
	{ \
		if (FlashTrace) \
			flush_console_out(); \
	}
#else
#define FLASHOP_FLUSH()
#endif
