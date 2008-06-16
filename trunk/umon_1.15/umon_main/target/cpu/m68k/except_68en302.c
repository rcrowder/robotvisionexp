/* except_68en302.c:
	This code handles exceptions that are caught by the exception vectors
	that have been installed by the monitor through vinit().

	General notice:
	This code is part of a boot-monitor package developed as a generic base
	platform for embedded system designs.  As such, it is likely to be
	distributed to various projects beyond the control of the original
	author.  Please notify the author of any enhancements made or bugs found
	so that all may benefit from the changes.  In addition, notification back
	to the author will allow the new user to pick up changes that may have
	been made by other users after this version of the code was distributed.

	Author:	Ed Sutter
	email:	esutter@lucent.com
	phone:	908-582-2351
*/
#include "config.h"
#include "cpu.h"
#include "cpuio.h"
#include "genlib.h"
#include "stddefs.h"
#include "warmstart.h"

ulong	ExceptionAddr;
int	ExceptionType;

void busadderr();
extern	int getreg(), putreg();
extern	void putsr();

char *
ExceptionType2String(int type)
{
	char *vname;
	static char buf[32];

	switch(type) {
		case 2:
			vname = "Bus error";
			break;
		case 3:
			vname = "Address error";
			break;
		case 4:
			vname = "Illegal instruction";
			break;
		case 5:
			vname = "Zero divide";
			break;
		case 6:
			vname = "Check instr";
			break;
		case 7:
			vname = "Trapv instr";
			break;
		case 8:
			vname = "Privilege violation";
			break;
		case 10:
			vname = "Line 1010";
			break;
		case 11:
			vname = "Line 1111";
			break;
		default:
			vname = "";
			break;
	}
	sprintf(buf,"%s",vname);
	return(buf);
}

void
exception(void)
{
	char	*vname;

	putsr(0x2700);
	getreg("D0",(ulong *)&ExceptionType);
	ExceptionType &= 0xff;
	vname = ExceptionType2String(ExceptionType);
	getreg("PC",&ExceptionAddr);
	printf("\nEXCEPTION: #%ld %s @ 0x%lx\n \b \b",
		ExceptionType,vname,ExceptionAddr);
	if ((ExceptionType == 2) || (ExceptionType == 3))
		busadderr(ExceptionType);

	monrestart(EXCEPTION);
}


void
busadderr(int type)
{
	ulong	reg, reg1;

	getreg("A0",&reg);
	printf("  Stat word:    0x%04x\n",(ushort)reg);
	getreg("A1",&reg); 
	getreg("A2",&reg1);
	printf("  Address:      0x%04x%04x\n",(ushort)reg,(ushort)reg1);
	getreg("A3",&reg);
	printf("  Instr reg:    0x%04x\n",(ushort)reg);
	getreg("SR",&reg);
	printf("  Status reg:   0x%04x\n \b \b",(ushort)reg);
	if (type == 2)
		printf("  SysCtrl reg:  0x%04lx\n \b \b",*(ulong*)0xf4);
	monrestart(EXCEPTION);
}

void
vinit(void)
{
	extern void	vbase(), vnext(), buserr(), adderr();
	extern void	vbase128(), vnext128();
	ulong	*vtab, vfunc;
	int	i, delta;

	vtab = 0;
	vfunc = (ulong)vbase;
	delta = (ulong)vnext - (ulong)vbase;
	for(i=2;i<128;i++) {
		/* Don't overwrite the MBAR and SysCfg registers... */
		if ((&vtab[i] < (ulong *)0xec) || (&vtab[i] > (ulong *)0xff))
			vtab[i] = (ulong)vfunc;
		vfunc += delta;
	}

	vfunc = (ulong)vbase128;
	delta = (ulong)vnext128 - (ulong)vbase128;
	for(;i<256;i++) {
		/* Don't overwrite the MBAR and SysCfg registers... */
		if ((&vtab[i] < (ulong *)0xec) || (&vtab[i] > (ulong *)0xff))
			vtab[i] = (ulong)vfunc;
		vfunc += delta;
	}
	/* Special handlers for bus and address error: */
	vtab[2] = (ulong)buserr;
	vtab[3] = (ulong)adderr;
}

void	(*GlobalIntFtbl[24])();
int	GlobalIntBaseVector;

ulong
GlobalIntSet(ino,func)
int	ino;
void	(*func)();
{
	ulong	ofunc;

	if (ino == GLOBALINTBASE)
		return((ulong)GlobalIntBaseVector);

	if (ino > 23)
		return(0);
	ofunc = (ulong)GlobalIntFtbl[ino];
	GlobalIntFtbl[ino] = func;
	return(ofunc);
}

void
GlobalInt(ino)
int	ino;
{
	printf("Global Interrupt #%d (vector #%d) caught by monitor.\n",
	    ino,GlobalIntBaseVector+ino);
	*(ushort *)IPR = *(ushort *)IPR;
	*(ushort *)ISR = *(ushort *)ISR;
	monrestart(INITIALIZE);
}
void
Gint0()
{
	GlobalInt(0);
}
void
Gint1()
{
	GlobalInt(1);
}
void
Gint2()
{
	GlobalInt(2);
}
void
Gint3()
{
	GlobalInt(3);
}
void
Gint4()
{
	GlobalInt(4);
}
void
Gint5()
{
	GlobalInt(5);
}
void
Gint6()
{
	GlobalInt(6);
}
void
Gint7()
{
	GlobalInt(7);
}
void
Gint8()
{
	GlobalInt(8);
}
void
Gint9()
{
	GlobalInt(9);
}
void
Gint10()
{
	GlobalInt(10);
}
void
Gint11()
{
	GlobalInt(11);
}
void
Gint12()
{
	GlobalInt(12);
}
void
Gint13()
{
	GlobalInt(13);
}
void
Gint14()
{
	GlobalInt(14);
}
void
Gint15()
{
	GlobalInt(15);
}
void
Gint16()
{
	GlobalInt(16);
}
void
Gint17()
{
	GlobalInt(17);
}
void
Gint18()
{
	GlobalInt(18);
}
void
Gint19()
{
	GlobalInt(19);
}
void
Gint20()
{
	GlobalInt(20);
}
void
Gint21()
{
	GlobalInt(21);
}
void
Gint22()
{
	GlobalInt(22);
}
void
Gint23()
{
	GlobalInt(23);
}

void
handler23()
{
	asm("movm.l &0xffff, -(%sp)");
	GlobalIntFtbl[23]();
	asm("movm.l (%sp)+, &0xffff");
	asm("rte");
}

void
handler22()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[22]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler17()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[17]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler15()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[15]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler14()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[14]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler13()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[13]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler12()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[12]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler11()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[11]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler10()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[10]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler09()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[9]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler08()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[8]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler07()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[7]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler06()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[6]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler05()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[5]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler04()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[4]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler03()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[3]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler02()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[2]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler01()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[1]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}
void
handler00()
{
		asm("movm.l &0xffff, -(%sp)");
		GlobalIntFtbl[0]();
		asm("movm.l (%sp)+, &0xffff");
		asm("rte");
}

/* GlobalIntInit():
   Initialized the interrupt handlers for the GLOBAL Interrupt within the
   68302.
*/
void
GlobalIntInit(ushort gimr)
{
	extern	void async_brk_hdlr();

	GlobalIntFtbl[0] = Gint0;
	GlobalIntFtbl[1] = Gint1;
	GlobalIntFtbl[2] = Gint2;
	GlobalIntFtbl[3] = Gint3;
	GlobalIntFtbl[4] = Gint4;
	GlobalIntFtbl[5] = Gint5;
	GlobalIntFtbl[6] = Gint6;
	GlobalIntFtbl[7] = Gint7;
	GlobalIntFtbl[8] = Gint8;
	GlobalIntFtbl[9] = Gint9;
	GlobalIntFtbl[10] = Gint10;
	GlobalIntFtbl[11] = Gint11;
	GlobalIntFtbl[12] = Gint12;
	GlobalIntFtbl[13] = Gint13;
	GlobalIntFtbl[14] = Gint14;
	GlobalIntFtbl[15] = Gint15;
	GlobalIntFtbl[16] = Gint16;
	GlobalIntFtbl[17] = Gint17;
	GlobalIntFtbl[18] = Gint18;
	GlobalIntFtbl[19] = Gint19;
	GlobalIntFtbl[20] = Gint20;
	GlobalIntFtbl[21] = Gint21;
	GlobalIntFtbl[22] = Gint22;
	GlobalIntFtbl[23] = Gint23;

	/* Clear any possibly queued up ints... */
	*(uchar *)SCCM1 = 0x00;
	*(uchar *)SCCE1 = 0xff;
	*(ushort *)IMR = 0x0000;
	*(ushort *)IPR = 0xffff;
	*(ushort *)ISR = 0xffff;

	/* Clear any currently pending state: */
	*(ushort *)IPR = *(ushort *)IPR;
	*(ushort *)ISR = *(ushort *)ISR;
	*(ushort *)INTR_EVENT = *(ushort *)INTR_EVENT;

#if CPU_68EN302
	/* Assign GIMR and extract the base vector: */
	/* For EN302, the IER sets up MOD and ET[7,6,1], so */
	/* they must be zero in GIMR of core 302. */
	/* Set Module Interrupt Level (for EN302 extensions) to 5. */
	/* See note above. */
	*(ushort *)GIMR = gimr & ~0x8700;
	*(ushort *)IER = (gimr & 0x8700);	/* MIL=0 */
#elif CPU_68302
	/* Assign GIMR and extract the base vector: */
	*(ushort *)GIMR = gimr;
#endif
	GlobalIntBaseVector = gimr & 0x00e0;

	VECTOR(GlobalIntBaseVector+0) = (ulong)handler00 + 6;
	VECTOR(GlobalIntBaseVector+1) = (ulong)handler01 + 6;
	VECTOR(GlobalIntBaseVector+2) = (ulong)handler02 + 6;
	VECTOR(GlobalIntBaseVector+3) = (ulong)handler03 + 6;
	VECTOR(GlobalIntBaseVector+4) = (ulong)handler04 + 6;
	VECTOR(GlobalIntBaseVector+5) = (ulong)handler05 + 6;
	VECTOR(GlobalIntBaseVector+6) = (ulong)handler06 + 6;
	VECTOR(GlobalIntBaseVector+7) = (ulong)handler07 + 6;
	VECTOR(GlobalIntBaseVector+8) = (ulong)handler08 + 6;
	VECTOR(GlobalIntBaseVector+9) = (ulong)handler09 + 6;
	VECTOR(GlobalIntBaseVector+10) = (ulong)handler10 + 6;
	VECTOR(GlobalIntBaseVector+11) = (ulong)handler11 + 6;
	VECTOR(GlobalIntBaseVector+12) = (ulong)handler12 + 6;
	VECTOR(GlobalIntBaseVector+13) = (ulong)handler13 + 6;
	VECTOR(GlobalIntBaseVector+14) = (ulong)handler14 + 6;
	VECTOR(GlobalIntBaseVector+15) = (ulong)handler15 + 6;
	VECTOR(GlobalIntBaseVector+17) = (ulong)handler17 + 6;
	VECTOR(GlobalIntBaseVector+22) = (ulong)handler22 + 6;
	VECTOR(GlobalIntBaseVector+23) = (ulong)handler23 + 6;
}

