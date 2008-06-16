/* except.c:
 *	This code handles exceptions that are caught by the exception vectors
 *	that have been installed by the monitor through vinit().
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
 *	email:	esutter@lucent.com
 *	phone:	908-582-2351		
 */
#include "config.h"
#include "cpu.h"
#include "cpuio.h"
#include "genlib.h"
#include "stddefs.h"
#include "warmstart.h"

ulong	ExceptionAddr;
int		ExceptionType;

extern	int getreg(), putreg();
extern	void putsr(ushort);
void	setvbr(ulong);

int
ExceptionIsATrap(ulong type)
{
	type &= ~EXCEPTION_FAULT;

	if ((type >= 32) && (type <= 47))
		return(1);
	return(0);
}

/* exception():
 * This is the first 'C' function called out of a monitor-installed
 * exception handler.  It assumes that the cached register value of
 * D0 contains the vector number that is the caller and that the
 * cached register value of PC contains the location of the exception.
 */
void
exception(void)
{
	ulong	vnum, pc;

	putsr(0x2700);
	getreg("D0",&vnum);
	vnum &= 0xff;
	getreg("PC",&pc);
	ExceptionAddr = pc;
	ExceptionType = vnum;

	/* Certain exception types indicate that the PC defines the location 
	 * of the faulting instruction (fault = 1) and others indicate that
	 * the PC is the address of the next instruction (fault = 0).
	 * See section 3.3 of MCF5206 User's Manual.
	 */
	switch(ExceptionType) {
		case 2:				/* Access error */
		case 3:				/* Address error */
		case 4:				/* Illegal instruction */
		case 8:				/* Privilege violation */
		case 10:			/* Unimplemented line-a opcode */
		case 11:			/* Unimplemented line-f opcode */
		case 14:			/* Format error */
			ExceptionType |= EXCEPTION_FAULT;
			break;
		default:
			break;
	}

	/* Allow the console uart fifo to empty...
	 */
	flush_console_out();
	monrestart(EXCEPTION);
}

/* vinit():
 * This function is called by init1() at startup of the monitor to
 * install the monitor-based vector table.  The actual functions are
 * in vectors.s.
 */
void
vinit()
{
	extern void	vbase(), vnext();
	extern ulong resetPC, resetSP;
	ulong	*vtab;
	ulong	vfunc;
	int	i, delta;

	/* Vector table is a block of 0x400 bytes of RAM starting at 
	 * the location specified by RAM_VECTOR_TABLE (in config.h)...
	 */
	vtab = (ulong *)RAM_VECTOR_TABLE;

	/* Special case for vtab[0] and vtab[1]: */
	vtab[0] = resetSP;
	vtab[1] = resetPC;

	vfunc = (ulong)vbase;
	delta = (ulong)vnext - (ulong)vbase;
	for(i=2;i<256;i++) {
		vtab[i] = (ulong)vfunc;
		vfunc += delta;
	}

	setvbr((ulong)vtab);
}

/* vinstall():
 * This function is used to re-install a specified monitor-based vector
 * table entry with the pointer that was originally installed by vinit().
 * This is used by the "trap" command to allow the user to put a monitor-
 * based pointer in the vector table after an RTOS may have written over
 * the vector table.
 */
void
vinstall(int vnum)
{
	extern void	vbase(), vnext();
	ulong getvbr();
	ulong	*vtab;
	int		delta;

	if ((vnum < 2) || (vnum > 255))
		return;

	vtab = (ulong *)getvbr();
	delta = (ulong)vnext - (ulong)vbase;
	vtab[vnum] = (ulong)vbase + (delta * (vnum-2));
}

static ulong VbrReg;

/* setvbr()/getvbr():
 * According to the 5272 manual, the actual VBR register can only be
 * read through the DEBUG module.  This means we need to keep track
 * of what we set the VBR to...
 */
void
setvbr(ulong vbrval)
{
	/* Set the actual VBR register, and keep a copy.
	 */
	mcf5272_wr_vbr(vbrval);
	VbrReg = vbrval;
}

ulong
getvbr(void)
{
	return(VbrReg);
}

/* ExceptionType2String():
 * This function simply returns a string that verbosely describes
 * the incoming exception type (vector number).
 */
char *
ExceptionType2String(int type)
{
	char	*string;
	static char	buf[32];

	type &= ~EXCEPTION_FAULT;

	if ((type >= 25) && (type <= 31)) {
		sprintf(buf,"Autovector #%d",type-24);
		string = buf;
	}
	else if ((type >= 32) && (type <= 47)) {
		sprintf(buf,"Trap #%d",type-32);
		string = buf;
	}
	else if (((type >= 16) && (type <= 23)) ||
			 ((type >= 48) && (type <= 60))) {
		string = "Reserved";
	}
	else if ((type >= 64) && (type <= 255)) {
		sprintf(buf,"User defined vector %d",type);
		string = buf;
	}
	else {
		switch(type) {
			case 2:
				string = "Access error";
				break;
			case 3:
				string = "Address error";
				break;
			case 4:
				string = "Illegal instruction";
				break;
			case 5:
				string = "Divide by zero";
				break;
			case 6:
			case 7:
			case 13:
			case 62:
			case 63:
				string = "Reserved";
				break;
			case 8:
				string = "Privilege violation";
				break;
			case 9:
				string = "Trace";
				break;
			case 10:
				string = "Unimplemented line-a opcode";
				break;
			case 11:
				string = "Unimplemented line-f opcode";
				break;
			case 12:
				string = "Debug interrupt";
				break;
			case 14:
				string = "Format error";
				break;
			case 15:
				string = "Uninitialized interrupt";
				break;
			case 24:
				string = "Spurious interrupt";
				break;
			case 61:
				string = "Unsupported instruction";
				break;
			default:
				sprintf(buf,"vtbl offset 0x%x",type);
				string = buf;
				break;
		}
	}
	return(string);
}

