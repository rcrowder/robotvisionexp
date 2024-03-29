/* regs.c:
 *
 *	This file is used in conjunction with common/monitor/reg_cache.c and
 *	target/monitor/regnames.c. It is included in a file in the
 *	target-specific directory called "regnames.c".  That file is then
 *	included in the common file called reg_cache.c to build the
 *	target-specific register cache code.
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
static char  *regnames[] = {
		"PC", "SR",
		"A0", "A1", "A2", "A3", "A4", "A5", "A6", "SP",
		"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
};


