/*
 * cmds_cf.c:
 * This file contains monitor commands that are specific to the ColdFire.
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
 *	Note1: the majority of this code was edited with 4-space tabs.
 *	Note2: as more and more contributions are accepted, the term "author"
 *		   is becoming a mis-representation of credit.
 *
 *	Original author:	Ed Sutter
 *	Email:				esutter@lucent.com
 *	Phone:				908-582-2351
 */
#include "config.h"
#include "cli.h"
#include "genlib.h"
#include "stddefs.h"

#define TRAP0		0x4e40

#if INCLUDE_TRAPCMD

char *TrapHelp[] = {
	"Use trap instruction for breakpoints",
	"-[irv] {address}",
	"Options:",
	" -i    install exception vector",
	" -r    restore exception vector",
	" -t#   trap number (default = 0)",
	0,
};

/* TrapCmd():
 * With no options, this command will insert a TRAP #0 instruction at
 * the address specified.
 *
 * The -t option allows the user to specify the trap number (ranging
 * from 0 thru 15).
 *
 * The -i option will install the monitor's trap handler into the vector
 * table and keep a copy of what was there.
 * The -r option will install what was copied (by -i) back into the
 * vector table.
 *
 * The idea behind -i & -r is this...
 * An RTOS may start up and install a full vector table.  After
 * the RTOS is up you want to use the monitor's ability to catch a
 * trap, so you need to put the monitor's handler back into the
 * table.  Then, sometime later you have the option to restore the
 * original vector.
 * Summary: if your RTOS doesn't touch the trap handling vector that
 * you plan to used for debugging, then you don't need to use -i or -r
 * because the monitor's trap handler will be left in place.
 */
int
TrapCmd(int argc,char **argv)
{
	extern	ulong	*getvbr();
	extern	void	vinstall(int);
	static	ulong	saved_trap_handler, installed;
	ushort	*addr = 0;
	ulong	*vtable;
	int		trapnum, opt, install, restore;
	
	if (argc < 2)
		return(CMD_PARAM_ERROR);

	vtable = getvbr();
	install = restore = trapnum = 0;
	while((opt=getopt(argc,argv,"irt:")) != -1) {
		switch(opt) {
		case 'i':
			install = 1;
			break;
		case 'r':
			restore = 1;
			break;
		case 't':
			trapnum = strtol(optarg,0,0);
			break;
		}
	}

	if ((trapnum < 0) || (trapnum > 15)) {
		printf("trapnum '%d' out of range\n",trapnum);
		return(CMD_FAILURE);
	}

	if ((!restore) && (!install) && (argc != (optind+1)))
		return(CMD_PARAM_ERROR);

	if ((restore) || (install))  {
		if (install) {
			installed = 1;
			saved_trap_handler = vtable[trapnum+32];
			vinstall(trapnum+32);
		}
		if (restore) {
			if (installed) {
				vtable[trapnum+32] = saved_trap_handler;
				installed = 0;
			}
			else {
				printf("Nothing to restore\n");
				return(CMD_FAILURE);
			}
		}
	}

	if (argc == (optind + 1)) {
		addr = (ushort *)strtoul(argv[optind],0,0);
		*addr = (TRAP0 + trapnum);
	}

	if (dcacheFlush)
		dcacheFlush((char *)addr,4);
	if (icacheInvalidate)
		icacheInvalidate((char *)addr,4);

	return(CMD_SUCCESS);
}
#endif	/* INCLUDE_TRAPCMD */
