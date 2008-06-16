/* MC68000 Disassembler:
 *	ELS...
 *	This disassembler is a hack of the mc68dis disassembler.
 *	NOTE: this adds approximately 20K to the size of the monitor.
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
#if INCLUDE_DISASSEMBLER
#include "cpu.h"
#include "ctype.h"
#include "genlib.h"
#include "stddefs.h"
#include "warmstart.h"

#define NULL 0

#define	BIT3(x)	(int)((x >> 3) & 0x1L)	/* ----x--- bit 3 */
#define	BIT5(x)	 (int)((x >> 5) & 0x1L)	/* --x----- bit 5 */
#define	BIT6(x)	 (int)((x >> 6) & 0x1L)	/* -x------ bit 6 */
#define	BIT7(x)	 (int)((x >> 7) & 0x1L)	/* x------- bit 7 */
#define BIT8(x)	(int)((x >> 8) & 0x1L)
#define BIT10(x) (int)((x >> 10) & 0x1L)
#define BIT11(x) (int)((x >> 11) & 0x1L)
#define BIT12(x) (int)((x >> 12) & 0x1L)
#define BIT15(x) (int)((x >> 15) & 0x1L)
#define	BITS15_8(x) (int)((x >> 8) & 0xffL)	
#define BITS15_7(x) (int)((x >> 7) & 0x1ffL)
#define BITS14_12(x) (int)((x >> 12) & 0x7L)
#define	BITS11_9(x) (int)((x >> 9) & 0x7L)	/* bits 11 through 9 */
#define	BITS11_8(x) (int)((x >> 8) & 0xfL)	/* bits 11 through 8 */
#define	BITS10_9(x) (int)((x >> 9) & 0x3L)	/* bits 10 through 9 */
#define	BITS10_8(x) (int)((x >> 8) & 0x7L)	/* bits 10 through 8 */
#define	BITS8_6(x) (int)((x >> 6) & 0x7L)	/* bits 8 through 6 */
#define BITS8_3(x) (int)((x >> 3) & 0x3fL)	/* bits 8 through 3 */
#define	BITS7_6(x) (int)((x >> 6) & 0x3L)	/* bits 7 through 6 */
#define	BITS5_4(x) (int)((x >> 4) & 0x3L)	/* bits 5 through 4 */
#define	BITS5_3(x) (int)((x >> 3) & 0x7L)	/* bits 5 through 3 */
#define	BITS5_0(x) (int)(x & 0x3fL)		/* bits 5 through 0 */
#define	BITS4_3(x) (int)((x >> 3) & 0x3L)	/* bits 4 through 3 */
#define	BITS3_0(x) (int)(x & 0xfL)		/* bits 3 through 0 */
#define	BITS2_0(x) (int)(x & 0x7L)		/* bits 2 through 0 */
#define	LOW8(x) (int)(x & 0xffL)		/* low 8 bits of quantity	*/
#define HIOF32(x) (int)((x >> 31) & 0x1L)	/* sign bit of 32 bit quantity	*/
#define	HI4OF16(x) (int)((x >> 12) & 0xfL)

#define	NCPS	8	/* Number of chars per symbol.		*/
#define	NHEX	80	/* Maximum # chars in object per line.	*/
#define	NLINE	33	/* Maximum # chars in mnemonic per line.*/
#define	FAIL	0
#define	LEAD	1
#define	NOLEAD	0
#define	TERM	0

#define	BYTE	1	 /* values for size parm to eff_add */
#define	WORD	2
#define	LONG	3

#define	SIGNED		1 /* immediate value signed or unsigned */
#define	UNSIGNED	0
#define	NOTSIGNED	0

unsigned short curinstr;	/* for saving first part of instruction
				   when cur2bytes is used to read displ */
unsigned short oldinstr = 0;	/* to save previous instruction for
				   testing that swbeg follows jump	*/


unsigned short	cur2bytes;	/* for storing the results of 'get2bytes()' */

static int	shownext;
static long	loc;		/* byte location being disassembled */
/* IMPORTANT: remember that loc is incremented*/
/* only by the getbyte routine	*/
static char	object[NHEX];	/* array to store object code for output*/
static char	mneu[NLINE];	/* array to store mnemonic code for output*/

char	conv_temp[NHEX];	/* Temporary location for ascii	*/
/* representation of operands.	*/

char	comp_temp[NHEX];	/* temp for calling compoff	*/

static char	size[] = {
	'b','w','l'};

char	*cond_codes[] = {
			"t      ",	"f      ",	"hi     ",	"ls     ",
			"cc     ",	"cs     ",	"ne     ",	"eq     ",
			"vc     ",	"vs     ",	"pl     ",	"mi     ",
			"ge     ",	"lt     ",	"gt     ",	"le     "
		};

char 	*addregs[] = { 
	"%a0","%a1","%a2","%a3","%a4","%a5","%fp","%sp" };

void	confused();
char	*eff_add();
void	bit_movep_imm(), move_byte(), move_long(), move_word(), miscell();
void	addq_subq_scc_dbcc(), bcc_bsr_bra(), moveq(), or_div_sbcd(), sub_subx();
void	unassigned(), cmp_eor(), and_mul_abcd_exg(), add_addx(), shft_rot();

void (*code_map[16])() =
{
		bit_movep_imm,		/* upper nibble 0x0 */
		move_byte,		/* upper nibble 0x1 */
		move_long,		/* upper nibble 0x2 */
		move_word,		/* upper nibble 0x3 */
		miscell,		/* upper nibble 0x4 */
		addq_subq_scc_dbcc, 	/* upper nibble 0x5 */
		bcc_bsr_bra,		/* upper nibble 0x6 */
		moveq,			/* upper nibble 0x7 */
		or_div_sbcd,		/* upper nibble 0x8 */
		sub_subx,		/* upper nibble 0x9 */
		unassigned,		/* upper nibble 0xa */
		cmp_eor,		/* upper nibble 0xb */
		and_mul_abcd_exg,	/* upper nibble 0xc */
		add_addx,		/* upper nibble 0xd */
		shft_rot,		/* upper nibble 0xe */
		unassigned		/* upper nibble 0xf */
};

char *DisHelp[] = {
	"Disassemble memory",
	"{address | .} [lines]",
	0,
};

extern int getreg();

/* ELS: With the original disassembler, it attempted to recover from 
	a confused state by re-syncing on the next line of source.
*/
void
confused()
{
	printf("\tDisassembler confused around 0x%lx\n",loc);
	monrestart(MISC);
}

/*
 *	compoff (lng, temp)
 *
 *	This routine will compute the location to which control is to be
 *	transferred.  'lng' is the number indicating the jump amount
 *	(already in proper form, meaning masked and negated if necessary)
 *	and 'temp' is a character array which already has the actual
 *	jump amount.  The result computed here will go at the end of 'temp'.
 *	(This is a great routine for people that don't like to compute in
 *	hex arithmetic.)
 */

void
compoff(long lng, char *temp)
{
	extern	long	loc;	/* from _extn.c */

	lng += loc;
	sprintf(temp,"%s <%lx>",temp,lng);
}


/*
 *	convert (num, temp, flag)
 *
 *	Convert the passed number to hex leaving the result in the
 *	supplied string array.
 *	If  LEAD  is specified, preceed the number with '0' or '0x' to
 *	indicate the base (used for information going to the mnemonic
 *	printout).  NOLEAD  will be used for all other printing (for
 *	printing the offset, object code, and the second byte in two
 *	byte immediates, displacements, etc.) and will assure that
 *	there are leading zeros.
 */

void
convert(num,temp,flag)
unsigned int	num;
char temp[];
int	flag;
{
	if (flag == NOLEAD)
		sprintf(temp,"%04x",num);
	if (flag == LEAD)
		sprintf(temp,"0x%x",num);
}

/*
 *	get2bytes()
 *
 *	This routine will get 2 bytes, print them in the object file
 *	and place the result in 'cur2bytes'.
 *
 */

void
get2bytes()
{
	uchar bytes[16], *bc;

	bc = (uchar *)loc;
	bytes[0] = bc[0];
	bytes[1] = bc[1];
	cur2bytes = *(unsigned short *)loc;
	loc += 2;
	convert( (cur2bytes & 0xffff), bytes, NOLEAD);
	sprintf(object,"%s%s ",object, bytes);
}

/*
 *	print_dis ()
 *
 *	Print the disassembled line, consisting of the object code
 *	and the mnemonics.  The breakpointable line number, if any,
 *	has already been printed, and 'object' contains the offset
 *	within the section for the instruction.
 */

void
print_dis()
{
	printf("%-35s%s\n",object,mneu);
}

/*
 *	prt_offset ()
 *
 *	Print the offset, right justified, followed by a ':'.
 */
void
prt_offset()
{
	if (shownext)
		strcpy(object,"NextInst: ");
	else
		sprintf(object,"0x%08lx: ",loc);
}

int
disass(ulong at,int lines,int next)
{
	int	i;
	ulong	start;

	shownext = next;

	if ((at == 0) && (lines == 0)) {
		lines = 1;
		getreg("PC",&loc);
		start = loc;
	}
	else {
		loc = at;
		start = at;
	}

	for(i=0;i<lines;i++) {
		prt_offset();
		mneu[0] = '\0';
		oldinstr = curinstr;
		get2bytes();
		/*save bytes in case eff_add changes it*/
		curinstr = cur2bytes;
		comp_temp[0] = '\0';
		(*code_map[HI4OF16(cur2bytes)])();
		/* if there was any pc rel computation 
				   put it at the end of assembly line */
		strcat(mneu,comp_temp);
		print_dis();
	}
	return((int)loc - start);
}

int
Dis(int argc,char *argv[])
{
	int lines;
	ulong	at;

	if (*argv[1] == '.')
		getreg("PC",&at);
	else
		at = strtoul(argv[1],0,0);

	if (argc == 3)
		lines = strtol(argv[2],0,0);
	else
		lines = 8;

	while(1) {
		disass(at,lines,0);
		if (More())
			at = loc;
		else
			break;
	}
	return(0);
}

void
move_address()
{
	strcat(mneu,"mova._  ");
	mneu[5] = BIT12(cur2bytes) ? 'w' : 'l';
	strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
	    BIT12(cur2bytes) ? WORD : LONG,NOTSIGNED));
	sprintf(mneu,"%s,%s",mneu,addregs[BITS11_9(curinstr)]);
}

void
bit_movep_imm()
{
	static char *misc_ops[4] = {
					"btst    ",
					"bchg    ",
					"bclr    ",
					"bset    "
					};
	if (BIT8(cur2bytes)) {
		if (BITS5_3(cur2bytes) == 1) {	/* movep */
			strcat(mneu,"movp._  ");
			mneu[5] = BIT6(cur2bytes) ? 'l' : 'w';
			if (BIT7(cur2bytes))
				sprintf(mneu,"%s%%d%d,%s",mneu,
				    BITS11_9(curinstr),
				    eff_add(5,BITS2_0(cur2bytes),
				    NULL,NULL));
				else
				sprintf(mneu,"%s%s,%%d%d",mneu,
				    eff_add(5,BITS2_0(cur2bytes),NULL,
				    NULL), BITS11_9(curinstr));
		}
		else {	/* dynamic bit */
			strcat(mneu,misc_ops[BITS7_6(cur2bytes)]);
			sprintf(mneu,"%s%%d%d,%s",mneu,BITS11_9(curinstr),
			    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    NULL,NULL));
		}
		return;
	} /* end if (BIT8(cur2bytes)) */
	switch(BITS11_9(cur2bytes)) {
		char add_temp[16];
	case 0:
		if (BITS7_6(cur2bytes) == 3)
			confused();
		strcat(mneu,"ori._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED));
		strcat(mneu,",");
		if (BITS5_0(curinstr) == 074) {
			if (BITS7_6(curinstr) == 0)
				strcat(mneu,"%cc");
			else if (BITS7_6(curinstr) == 1)
				strcat(mneu,"%sr");
				else
				confused();
		}
		else
			strcat(mneu,eff_add(BITS5_3(curinstr),
			    BITS2_0(curinstr),NULL,NULL));
		return;
	case 1:
		if (BITS7_6(cur2bytes) == 3)
			confused();
		strcat(mneu,"andi._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED));
		strcat(mneu,",");
		if (BITS5_0(curinstr) == 074) {
			if (BITS7_6(curinstr) == 0)
				strcat(mneu,"%cc");
			else if (BITS7_6(curinstr) == 1)
				strcat(mneu,"%sr");
				else
				confused();
		}
		else
			strcat(mneu,eff_add(BITS5_3(curinstr),
			    BITS2_0(curinstr),NULL,NULL));
		return;
	case 2:
		if ((BITS7_6(cur2bytes) == 3) || (BITS5_0(cur2bytes) == 074))
			confused();
		strcat(mneu,"subi._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(curinstr),BITS2_0(curinstr),
		    NULL,NULL));
		return;
	case 3:
		if ((BITS7_6(cur2bytes) == 3) || (BITS5_0(cur2bytes) == 074))
			confused();
		strcat(mneu,"addi._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(curinstr),BITS2_0(curinstr),
		    NULL,NULL));
		return;
	case 4:
		strcat(mneu,misc_ops[BITS7_6(cur2bytes)]);
		strcat(mneu,eff_add(7,4,(BITS7_6(cur2bytes) ==1) ?
		    BYTE : WORD,UNSIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(curinstr),
		    BITS2_0(curinstr),NULL,NULL));
		return;
	case 5:
		if (BITS7_6(cur2bytes) == 3)
			confused();
		strcat(mneu,"eori._   ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED));
		strcat(mneu,",");
		if (BITS5_0(curinstr) == 074) {
			if (BITS7_6(curinstr) == 0)
				strcat(mneu,"%cc");
			else if (BITS7_6(curinstr == 1))
				strcat(mneu,"%sr");
				else
				confused();
		}
		else
			strcat(mneu,eff_add(BITS5_3(curinstr),
			    BITS2_0(curinstr),NULL,NULL));
		return;
	case 6:
		if ((BITS7_6(cur2bytes) == 3) || (BITS5_0(cur2bytes) == 074))
			confused();
		strcat(mneu,"cmpi._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcpy(add_temp,eff_add(7,4,BITS7_6(cur2bytes)+1,NOTSIGNED));
		strcat(mneu,eff_add(BITS5_3(curinstr),BITS2_0(curinstr),
		    NULL,NULL));
		strcat(mneu,",");
		strcat(mneu,add_temp);
		return;
#ifdef M68010
	case 7:
		{
			short osz, regtype, regnum, movsdir;
			char  curea[24], rreg[6];
			short ea1, eareg;
			osz = BITS7_6(cur2bytes);
			if (osz == 3)
				confused();
			ea1 = BITS5_3(cur2bytes);
			eareg = BITS2_0(cur2bytes);
			strcat(mneu,"movs.");
			strcat(mneu,(osz==2)?("l"):( (osz==1)?("w"):("b") ));
			strcat(mneu,"  ");
			get2bytes();
			movsdir = BIT11(cur2bytes);
			regtype = BIT15(cur2bytes);
			regnum  = BITS14_12(cur2bytes);
			strcpy(curea,
			    eff_add(ea1,eareg,
			    (osz==2)?(LONG):( (osz==1)?(WORD):(BYTE) ),
			    NULL));
			if (regtype)
				strcpy(rreg,addregs[regnum]);
				else
				sprintf(rreg,"%%d%1d",regnum);
			strcat(mneu,(movsdir)?(rreg):(curea));
			strcat(mneu,",");
			strcat(mneu,(movsdir)?(curea):(rreg));
		}
#else
	case 7:
		confused();
#endif
	}
}


void
move_byte()
{
	if (BITS8_6(cur2bytes) == 0x1L )
		confused();
	else {
		strcat(mneu,"mov.b   ");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),BYTE,
		    NOTSIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
		    NULL));
	}
}	/* move_byte()	*/



void
move_long()
{
	if (BITS8_6(cur2bytes) == 0x1L )
		move_address();
	else {
		strcat(mneu,"mov.l   ");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),LONG,
		    NOTSIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
		    NULL));
	}
}	/* move_long()	*/

void
move_word()
{
	if (BITS8_6(cur2bytes) == 0x1L )
		move_address();
	else {
		strcat(mneu,"mov.w   ");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),WORD,
		    NOTSIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
		    NULL));
	}
}	/* move_word()	*/



void
miscell()
{
	if (BIT8(cur2bytes))
	{
		if (!BIT7(cur2bytes))
			confused();
		if (BIT6(cur2bytes))
			sprintf(mneu,"lea.l   %s,%s",eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),LONG,UNSIGNED),
			    addregs[BITS11_9(curinstr)]);
			else
			sprintf(mneu,"chk.w   %s,%%d%d",
			    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    WORD,UNSIGNED),BITS11_9(curinstr));
		return;
	}

	switch (BITS11_9(cur2bytes))
	{
	case 0:
		if (BITS7_6(cur2bytes) == 3) {
			strcat(mneu,"mov.w   %sr,");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),NULL,NULL));
		}
		else {
			strcat(mneu,"negx._  ");
			mneu[5] = size[BITS7_6(cur2bytes)];
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),NULL,NULL));
		}
		return;
	case 1:
		if (BITS7_6(cur2bytes) == 3) {
#ifdef M68010
			strcat(mneu,"mov.w   %cc,");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    WORD,NULL));
			return;
#else
			confused();
#endif
		}
		strcat(mneu,"clr._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    NULL,NULL));
		return;
	case 2:
		if (BITS7_6(cur2bytes) == 3)
		{
			strcat(mneu,"mov.w   ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),WORD,UNSIGNED));
			strcat(mneu,",%cc");
			return;
		}
		strcat(mneu,"neg._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    NULL,NULL));
		return;
	case 3:
		if (BITS7_6(cur2bytes) == 3)
		{
			strcat(mneu,"mov.w   ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),WORD,UNSIGNED));
			strcat(mneu,",%sr");
			return;
		}
		strcat(mneu,"not._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    NULL,NULL));
		return;
	case 5:
		if (BITS7_6(cur2bytes) == 3)
		{
			if (BITS5_0(cur2bytes) == 074) {
				unsigned short i;
				if (BITS15_7(oldinstr) == 0235){
					get2bytes();
					curinstr = cur2bytes;
					convert(cur2bytes,conv_temp,LEAD);
					sprintf(mneu,"swbeg   &%s",conv_temp);
					for(i=1;i<=curinstr;i++) {
						print_dis();
						printf("\t");
						/* no need to call line_							nums - just need tab */
						mneu[0] = '\0';
						prt_offset();
						strcat(mneu,eff_add(7,4,WORD,
						    NOTSIGNED));
					}
					return;
				}
				else {
					sprintf(mneu,"illegal ");
					return;
				}
			}
			strcat(mneu,"tas.b   ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),NULL,NULL));
			return;
		}
		strcat(mneu,"tst._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    NULL,NULL));
		return;
	case 4:
		switch(BITS7_6(cur2bytes)) {
		case 0:
			strcat(mneu,"nbcd.b  ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),NULL,NULL));
			return;
		case 1:
#ifdef M68010
			if (BITS5_3(cur2bytes) == 1) {
				char atemp[4];
				strcat(mneu,"bkpt    &");
				sprintf(atemp,"%d", BITS2_0(cur2bytes));
				strcat(mneu,atemp);
				return;
			}
			else
#endif
				if (BITS5_3(cur2bytes)) {
					strcat(mneu,"pea.l   ");

					strcat(mneu,eff_add(BITS5_3(cur2bytes),
					    BITS2_0(cur2bytes),NULL,NULL));
					return;
				}
				else {
					sprintf(mneu,"swap.w  %%d%d",
					    BITS2_0(cur2bytes));
					return;
				}
		case 2:
			if (!BITS5_3(cur2bytes)) {
				sprintf(mneu,"ext.w   %%d%d",
				    BITS2_0(cur2bytes));
				return;
			}
			/* movem falls thru here */
		case 3:
			if (!BITS5_3(cur2bytes)) {
				sprintf(mneu,"ext.l   %%d%d",
				    BITS2_0(cur2bytes));
				return;
			}
			/* movem falls thru here */
		} /* end switch BITS7_6 */
	case 6:
		{
			unsigned short mask;		/* gets movm.[wl] mask */
			char add_temp[25];
			add_temp[0] = '\0';
			if (!BIT7(cur2bytes))
				confused();
			get2bytes();		/* read mask and save */
			mask = cur2bytes;
			strcpy(mneu,"movm._  ");
			mneu[5] = BIT6(curinstr) ? 'l' : 'w';
			strcat(add_temp,eff_add(BITS5_3(curinstr),BITS2_0(curinstr),
			    NULL,NULL));
			if(BIT10(curinstr))
				sprintf(mneu,"%s%s,",mneu,add_temp);
			strcat(mneu,"&");
			convert(mask, conv_temp,LEAD);
			strcat(mneu,conv_temp);
			if (!BIT10(curinstr))
				sprintf(mneu,"%s,%s",mneu,add_temp);
			return;
		}
	case 7:
		switch (BITS7_6(cur2bytes))
		{
		case 0:
			confused();
		case 1:
			switch (BITS5_4(cur2bytes))
			{
			case 0:
				strcat(mneu,"trap    &");
				convert(BITS3_0(cur2bytes),conv_temp,LEAD);
				strcat(mneu,conv_temp);
				return;
			case 1:
				if (BIT3(cur2bytes))
					sprintf(mneu,"unlk    %s",
					    addregs[BITS2_0(cur2bytes)]);
					else
				{
					sprintf(mneu,"link    %s,&",
					    addregs[BITS2_0(cur2bytes)]);
					get2bytes();
					if (BIT15(cur2bytes))
						strcat(mneu,"-");
					convert(BIT15(cur2bytes) ?
					    -(short)cur2bytes : cur2bytes,
					    conv_temp,LEAD);
					strcat(mneu,conv_temp);
				}
				return;
			case 2:
				if (BIT3(cur2bytes))
					sprintf(mneu,"mov.l   %%usp,%s",
					    addregs[BITS2_0(cur2bytes)]);
					else
					sprintf(mneu,"mov.l   %s,%%usp",
					    addregs[BITS2_0(cur2bytes)]);
				return;
			case 3:
				{
					static char	*misc_ops[8] =
					{
												"reset",
												"nop",
												"stop    &",
												"rte",
												"",
												"rts",
												"trapv",
												"rtr"
																};
#ifdef M68010
					if (BIT3(cur2bytes) &&
					    (BITS2_0(cur2bytes) == 2  ||
					    BITS2_0(cur2bytes) == 3))
					{
						short movcdir, regtype, regnum;
						char  rreg[6], creg[6];
						movcdir = cur2bytes & 0x01;
						get2bytes();
						regtype = BIT15(cur2bytes);
						regnum  = BITS14_12(cur2bytes);
						if (regtype)
							strcpy(rreg,addregs[regnum]);
							else
							sprintf(rreg,"%%d%1d",regnum);
						regnum = cur2bytes & 0x0fff;
						if (regnum == 0) strcpy(creg,"%sfc");
						else
							if (regnum == 1) strcpy(creg,"%dfc");
							else
								if (regnum == 0x0800) strcpy(creg,"%usp");
								else
/* code folded from here */
	if (regnum == 0x0801) strcpy(creg,"%vbr");
	else
		confused();
/* unfolding */
						strcat(mneu,"movc.l  ");
						if (movcdir) {
							strcat(mneu,rreg);
							strcat(mneu,",");
							strcat(mneu,creg);
						}
						else {
							strcat(mneu,creg);
							strcat(mneu,",");
							strcat(mneu,rreg);
						}
						return;
					}
					else
						if (BIT3(cur2bytes) == 0  &&
						    BITS2_0(cur2bytes) == 4) {
							char disptemp[16];
							strcat(mneu,"rtd     &");
							get2bytes();
							sprintf(disptemp,"%d",cur2bytes);
							strcat(mneu,disptemp);
							return;
						}
					else
#endif
							if (BIT3(cur2bytes)||(BITS2_0(cur2bytes) ==4))
								confused();
					strcat(mneu,misc_ops[BITS2_0(cur2bytes)]);
					if (BITS2_0(cur2bytes) == 2)	/* stop	*/
					{
						get2bytes();
						convert(cur2bytes,conv_temp,LEAD);
						strcat(mneu,conv_temp);
					}
					return;
				}
			}	/* switch (BITS5_4(cur2bytes))	*/
		case 2:
			strcat(mneu,"jsr     ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),NULL,NULL));
			return;
		case 3:
			strcat(mneu,"jmp     ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),NULL,NULL));
			return;
		}	/* switch (BITS7_6(cur2bytes))	*/
	}	/* switch (BITS11_9(cur2bytes))	*/
}	/* miscell() */



void
addq_subq_scc_dbcc()
{
	if (BITS7_6(cur2bytes) == 3) {
		if (BITS5_3(cur2bytes) == 1) {
			strcat(mneu,"db");
			strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
			/* dbCC is one character longer than other CCs,	*/
			/* so null out the ninth byte to keep aligned.	*/
			mneu[8] = '\0';
			sprintf(conv_temp,"%%d%d,",BITS2_0(cur2bytes));
			strcat(mneu,conv_temp);
			get2bytes();
			if (BIT15(cur2bytes))
				strcat(mneu,"-");
			convert(BIT15(cur2bytes) ? -(short) cur2bytes : 
			    cur2bytes,conv_temp,LEAD);
			strcat(mneu,conv_temp);
			compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
			    (long) cur2bytes) -2, comp_temp);
			/* the -2 above is needed because loc has been 
							   updated when getting the displacement, but 
							   for Motorola the pc is the address of the 
							   extension word */
		}
		else {
			strcat(mneu,"s");
			strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
			if (BITS11_8(cur2bytes) < 2)
			{
				/* cc is only one character */
				mneu[2] = '.';
				mneu[3] = 'b';
			}
			else
			{
				/* cc is two characters */
				mneu[3] = '.';
				mneu[4] = 'b';
			}
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
			    BITS2_0(cur2bytes),NULL,NULL));
		}
	}
	else
	{
		strcpy(mneu,BIT8(cur2bytes) ? "subq._  &" : "addq._  &");
		mneu[5] = size[BITS7_6(cur2bytes)];
		convert(BITS11_9(cur2bytes) ? BITS11_9(cur2bytes) : 8,
		    conv_temp,LEAD);
		strcat(mneu,conv_temp);
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    NULL,NULL));
	}
}	/* addq_subq_scc_dbcc()	*/



void
bcc_bsr_bra()
{
	strcpy(mneu,"b");
	if (BITS11_8(cur2bytes) == 1)
		strcat(mneu,"sr     ");
	else if (BITS11_8(cur2bytes) == 0)
		strcat(mneu,"ra     ");
		else
		strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
	if (LOW8(cur2bytes))
	{
		mneu[3] = '.';
		mneu[4] = 'b';
		convert(BIT7(cur2bytes) ?
		    -((short) ( LOW8(cur2bytes) | 0xff00)) :
		    LOW8(cur2bytes),conv_temp,LEAD);
		compoff(BIT7(cur2bytes) ? 
		    ((long)(short) (LOW8(cur2bytes) | 0xff00)) : 
		    (long) LOW8(cur2bytes), comp_temp);
		if (BIT7(cur2bytes))
			strcat(mneu,"-");
	}
	else
	{
		get2bytes();
		convert(BIT15(cur2bytes) ? -((short) cur2bytes) :
		    cur2bytes,conv_temp,LEAD);
		compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
		    (long) cur2bytes) -2, comp_temp);
		/* the -2 above is needed because loc has been 
						   updated when getting the displacement, but 
						   for Motorola the pc is the address of the 
						   extension word */
		if (BIT15(cur2bytes))
			strcat(mneu,"-");
	}
	strcat(mneu,conv_temp);
}	/* bcc_bsr_bra() */



void
moveq()
{
	/*
		if (BIT8(cur2bytes))
			confused();
	*/
	strcpy(mneu,"movq.l  &");
	if (BIT7(cur2bytes))
		strcat(mneu,"-");
	convert(BIT7(cur2bytes) ? 
	    -(short) (LOW8(cur2bytes) | 0xff00) : 
	    LOW8(cur2bytes), conv_temp,LEAD);
	strcat(mneu,conv_temp);
	sprintf(conv_temp,",%%d%d",BITS11_9(cur2bytes));
	strcat(mneu,conv_temp);
}	/* moveq() */

void
or_div_sbcd()
{
	if (BITS7_6(cur2bytes) == 3) {	/* divide */
		strcat(mneu,"div_.w  ");
		if (BIT8(cur2bytes))
			mneu[3] = 's';
			else
			mneu[3] = 'u';
		sprintf(mneu,"%s%s,%%d%d",mneu,
		    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),WORD,
		    BIT8(cur2bytes) ? SIGNED : UNSIGNED),
		    BITS11_9(curinstr));
	}
	else if (BITS8_6(cur2bytes) == 4 && BITS5_4(cur2bytes) == 0) {
		/* sbcd */
		if (BIT3(cur2bytes))
			sprintf(mneu,"sbcd.b  -(%s),-(%s)",
			    addregs[BITS2_0(cur2bytes)],
			    addregs[BITS11_9(cur2bytes)]);
			else
			sprintf(mneu,"sbcd.b  %%d%d,%%d%d",
			    BITS2_0(cur2bytes),BITS11_9(cur2bytes));
	}
	else {				/* or */
		unsigned short inst = cur2bytes;	/* remember inst. */
		char operand[32];

		strcat(mneu,"or._    ");
		mneu[3] = size[BITS7_6(inst)];
		operand[0] = '\0';
		strcat(operand, eff_add(BITS5_3(inst),BITS2_0(inst),
		    BITS7_6(inst)+1,UNSIGNED));
		if (!BIT8(inst))
			sprintf(mneu, "%s%s,",mneu,operand);
		sprintf(mneu,"%s%%d%d",mneu,BITS11_9(inst));
		if (BIT8(inst))
			sprintf(mneu, "%s,%s",mneu,operand);
	}
}

void
sub_subx()
{
	if (BITS7_6(cur2bytes) == 3) {	/* suba */
		strcat(mneu,"suba._  ");
		mneu[5] = BIT8(cur2bytes) ? 'l' : 'w';
		sprintf(mneu,"%s%s,%s",mneu,
		    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    BIT8(cur2bytes) ? LONG : WORD,NOTSIGNED),
		    addregs[BITS11_9(curinstr)]);
	}
	else if (BIT8(cur2bytes) && !BITS5_4(cur2bytes)) { /* subx */
		strcat(mneu,"subx._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		if (BIT3(cur2bytes))
			sprintf (mneu,"%s-(%s),-(%s)",mneu,
			    addregs[BITS2_0(cur2bytes)],
			    addregs[BITS11_9(cur2bytes)]);
		else 
			sprintf(mneu,"%s%%d%d,%%d%d",mneu,BITS2_0(cur2bytes),
			    BITS11_9(cur2bytes));
	}
	else {				/* sub */
		strcat(mneu,"sub._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		if (BIT8(cur2bytes))
			sprintf(mneu,"%s%%d%d,%s",mneu,BITS11_9(curinstr),
			    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    NULL,NULL));
			else
			sprintf(mneu,"%s%s,%%d%d",mneu,
			    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    BITS7_6(cur2bytes) + 1,UNSIGNED),
			    BITS11_9(curinstr));
	}
}

void
cmp_eor()
{
	if (BITS7_6(cur2bytes) == 3) {	/* cmpa */
		strcat(mneu,"cmpa._  ");
		mneu[5] = BIT8(cur2bytes) ? 'l' : 'w';
		sprintf(mneu,"%s%s,%s",mneu,addregs[BITS11_9(curinstr)],
		    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    BIT8(cur2bytes) ? LONG : WORD,NOTSIGNED));
	}
	else if (BIT8(cur2bytes) && (BITS5_3(cur2bytes) == 1)) { /* cmpm */
		strcat(mneu,"cmpm._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		sprintf(mneu,"%s(%s)+,(%s)+",mneu,
		    addregs[BITS11_9(cur2bytes)],
		    addregs[BITS2_0(cur2bytes)]);
	}
	else {				/* cmp or eor */
		if (BIT8(cur2bytes))
			strcat(mneu,"eor._   ");
			else
			strcat(mneu,"cmp._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		sprintf(mneu,"%s%%d%d,%s",mneu,BITS11_9(curinstr),
		    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    BITS7_6(cur2bytes) +1,BIT8(cur2bytes) ?
		    UNSIGNED : SIGNED));
	}
}

void
and_mul_abcd_exg()
{
	if(BITS7_6(cur2bytes) == 3) {	/* mul */
		if (BIT8(cur2bytes))
			strcat(mneu,"muls.w  ");
			else
			strcat(mneu,"mulu.w  ");
		sprintf(mneu,"%s%s,%%d%d",mneu,eff_add(BITS5_3(cur2bytes),
		    BITS2_0(cur2bytes),WORD,
		    BIT8(cur2bytes) ? SIGNED : UNSIGNED),
		    BITS11_9(curinstr));
	}
	else if(BIT8(cur2bytes) && !BITS5_4(cur2bytes)) { /* abcd or exg */
		if (BITS8_6(cur2bytes) == 4) {	/* abcd */
			strcat(mneu,"abcd.b  ");
			if (BIT3(cur2bytes))
				sprintf(mneu,"%s-(%s),-(%s)",mneu,
				    addregs[BITS2_0(cur2bytes)],
				    addregs[BITS11_9(cur2bytes)]);
				else
				sprintf(mneu,"%s%%d%d,%%d%d",mneu,
				    BITS2_0(cur2bytes),BITS11_9(cur2bytes));
		}
		else if (BITS8_3(cur2bytes) == 050)
			sprintf(mneu,"exgd    %%d%d,%%d%d",BITS11_9(cur2bytes),
			    BITS2_0(cur2bytes));
		else if (BITS8_3(cur2bytes) == 051)
			sprintf(mneu,"exga    %s,%s",
			    addregs[BITS11_9(cur2bytes)],
			    addregs[BITS2_0(cur2bytes)]);
		else if (BITS8_3(cur2bytes) == 061)
			sprintf(mneu,"exgm    %%d%d,%s",BITS11_9(cur2bytes),
			    addregs[BITS2_0(cur2bytes)]);
		else 
			confused();
	}
	else {	/* and */
		strcat(mneu,"and._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		if (BIT8(cur2bytes))
			sprintf(mneu,"%s%%d%d,%s",mneu,BITS11_9(curinstr),
			    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    NULL,NULL));
			else
			sprintf(mneu,"%s%s,%%d%d",mneu,
			    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    BITS7_6(cur2bytes) + 1,UNSIGNED),
			    BITS11_9(curinstr));
	}
}

void
add_addx()
{
	if (BITS7_6(cur2bytes) == 3) {	/* adda */
		strcat(mneu,"adda._  ");
		mneu[5] = BIT8(cur2bytes) ? 'l' : 'w';
		sprintf(mneu,"%s%s,%s",mneu,
		    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    BIT8(cur2bytes) ? LONG : WORD,UNSIGNED),
		    addregs[BITS11_9(curinstr)]);
	}
	else if (BIT8(cur2bytes) && !BITS5_4(cur2bytes)) { /* addx */
		strcat(mneu,"addx._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		if (BIT3(cur2bytes))
			sprintf (mneu,"%s-(%s),-(%s)",mneu,
			    addregs[BITS2_0(cur2bytes)],
			    addregs[BITS11_9(cur2bytes)]);
		else 
			sprintf(mneu,"%s%%d%d,%%d%d",mneu,BITS2_0(cur2bytes),
			    BITS11_9(cur2bytes));
	}
	else {				/* add */
		strcat(mneu,"add._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		if (BIT8(cur2bytes))
			sprintf(mneu,"%s%%d%d,%s",mneu,BITS11_9(curinstr),
			    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    NULL,NULL));
			else
			sprintf(mneu,"%s%s,%%d%d",mneu,
			    eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			    BITS7_6(cur2bytes) + 1,UNSIGNED),
			    BITS11_9(curinstr));
	}
}

void
shft_rot()
{
	char	direction;		/* l -- left, r -- right	*/

	direction = BIT8(cur2bytes) ? 'l' : 'r';

	if (BITS7_6(cur2bytes) == 3)	/* Memory rotate.		*/
	{
		if (BIT11(cur2bytes) != 0)
			confused();

		switch (BITS10_9(cur2bytes))
		{
		case 0:	
			strcat(mneu,"as_.w   &1,");
			mneu[2] = direction;
			break;
		case 1: 
			strcat(mneu,"ls_.w   &1,");
			mneu[2] = direction;
			break;
		case 2: 
			strcat(mneu,"rox_.w  &1,");
			mneu[3] = direction;
			break;
		case 3: 
			strcat(mneu,"ro_.w   &1,");
			mneu[2] = direction;
			break;
		}
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		    NULL,NULL));
	}
	else	/* Register rotate.		*/
	{

		switch (BITS4_3(cur2bytes))
		{
		case 0:	
			strcat(mneu,"as_._   ");
			mneu[2] = direction;
			mneu[4] = size[BITS7_6(cur2bytes)];
			break;
		case 1: 
			strcat(mneu,"ls_._   ");
			mneu[2] = direction;
			mneu[4] = size[BITS7_6(cur2bytes)];
			break;
		case 2: 
			strcat(mneu,"rox_._  ");
			mneu[3] = direction;
			mneu[5] = size[BITS7_6(cur2bytes)];
			break;
		case 3: 
			strcat(mneu,"ro_._   ");
			mneu[2] = direction;
			mneu[4] = size[BITS7_6(cur2bytes)];
			break;
		}

		if (BIT5(cur2bytes))
		{
			sprintf(conv_temp,"%%d%d,%%d%d",
			    BITS11_9(cur2bytes),BITS2_0(cur2bytes));
			strcat(mneu,conv_temp);
		}
		else
		{
			strcat(mneu,"&");
			convert(BITS11_9(cur2bytes) ? BITS11_9(cur2bytes) : 8, 
			    conv_temp, LEAD);
			sprintf(conv_temp,"%s,%%d%d", conv_temp,
			    BITS2_0(cur2bytes));
			strcat(mneu,conv_temp);
		}
	}
}	/* shft_rot()	*/



void
unassigned()
{
	printf("\tUunassigned op code around 0x%lx\n",loc);
	monrestart(MISC);
}

char *
eff_add(mode,reg,Size,sign)
int mode,reg,Size;	/* size will be NULL, BYTE, WORD, or LONG */
short sign;
{
	unsigned long fourbytes;
	static char address_fld[32];
	address_fld[0] = '\0';
	switch(mode)
	{
	case 0:			/* data register direct */
		sprintf(address_fld, "%%d%d", reg);
		break;
	case 1:			/* address register direct */
		sprintf(address_fld, "%s", addregs[reg]);
		break;
	case 2:			/* address register indirect */
		sprintf(address_fld, "(%s)", addregs[reg]);
		break;
	case 3:			/* address register indirect with post incr */
		sprintf(address_fld, "(%s)+", addregs[reg]);
		break;
	case 4:			/* address register indirect with pre decr */
		sprintf(address_fld, "-(%s)", addregs[reg]);
		break;
	case 5:			/* address register indirect with displ */
		get2bytes();
		if (BIT15(cur2bytes))
			strcat(address_fld, "-");
		convert(BIT15(cur2bytes) ? -(short) cur2bytes : cur2bytes,
		    conv_temp,LEAD);
		sprintf(address_fld,"%s%s(%s)", address_fld, conv_temp, 
		    addregs[reg]);
		break;
	case 6:			/* address register indirect with index */
		get2bytes();
		if (BITS10_8(cur2bytes))
			confused();
		if (BIT7(cur2bytes))
			strcat(address_fld, "-");
		convert(BIT7(cur2bytes) ?
		    -(short) (LOW8(cur2bytes) | 0xff00) : LOW8(cur2bytes),
		    conv_temp,LEAD);
		sprintf(address_fld,"%s%s(%s,%%%c%d.%c)", 
		    address_fld, conv_temp, addregs[reg],
		    BIT15(cur2bytes) ? 'a' : 'd' , BITS14_12(cur2bytes),
		    BIT11(cur2bytes) ? 'l' : 'w' );
		break;
	case 7:			/* absolute, pc, and immediate */
		switch(reg)		/* reg is not really a register here */
		{
		case 0:		/* absolute short */
			get2bytes();
			convert((short) cur2bytes,address_fld,LEAD);
			strcat(address_fld,".W");
			break;
		case 1:		/* absolute long */
			get2bytes();
			fourbytes = cur2bytes << 16;
			get2bytes();
			fourbytes |= cur2bytes;
			convert(fourbytes,address_fld,LEAD);
			strcat(address_fld,".L");
			break;
		case 2:		/* pc with displ */
			get2bytes();
			if (BIT15(cur2bytes))
				strcat(address_fld, "-");
			convert(BIT15(cur2bytes) ?
			    -(short) cur2bytes : cur2bytes,
			    conv_temp, LEAD);
			sprintf(address_fld, "%s%s(%%pc)",
			    address_fld, conv_temp);
			compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
			    (long) cur2bytes) -2, comp_temp);
			/* the -2 above is needed because loc has been 
							   updated when getting the displacement, but 
							   for Motorola the pc is the address of the 
							   extension word */
			break;
		case 3:		/* pc with index */
			get2bytes();
			if (BITS10_8(cur2bytes))
				confused();
			if (BIT7(cur2bytes))
				strcat(address_fld, "-");
			convert(BIT7(cur2bytes) ?
			    -(short) (LOW8(cur2bytes) | 0xff00) :
			    LOW8(cur2bytes),
			    conv_temp,LEAD);
			sprintf(address_fld,"%s%s(%%pc,%%%c%d.%c)", 
			    address_fld, conv_temp, 
			    BIT15(cur2bytes) ? 'a' : 'd' ,
			    BITS14_12(cur2bytes),
			    BIT11(cur2bytes) ? 'l' : 'w' );
			compoff((BIT7(cur2bytes) ? 
			    ((long)(short) (LOW8(cur2bytes) | 0xff00)) : 
			    (long) LOW8(cur2bytes)) - 2, comp_temp);
			/* the -2 above is needed because loc has been 
							   updated when getting the index word, but 
							   for Motorola the pc is the address of
							   this word */
			sprintf(comp_temp,"%s+%%%c%d",comp_temp,
			    BIT15(cur2bytes) ? 'a' : 'd' ,
			    BITS14_12(cur2bytes));
			break;
		case 4:		/* immediate */
			switch(Size)
			{
			case NULL:
				confused();
				break;
			case BYTE:
				get2bytes();
				if (BITS15_8(cur2bytes))
					confused();
				strcat(address_fld, "&");
				if ((sign == SIGNED) && BIT7(cur2bytes))
					strcat(address_fld, "-");
				convert(((sign == SIGNED) && BIT7(cur2bytes)) ?
				    -(short) (LOW8(cur2bytes) | 0xff00) :
				    LOW8(cur2bytes),
				    conv_temp,LEAD);
				strcat(address_fld, conv_temp);
				break;
			case WORD:
				get2bytes();
				strcat(address_fld, "&");
				if ((sign == SIGNED) && BIT15(cur2bytes))
					strcat(address_fld, "-");
				convert(((sign == SIGNED) && BIT15(cur2bytes)) ?
				    -(short) cur2bytes : cur2bytes ,
				    conv_temp,LEAD);
				strcat(address_fld, conv_temp);
				break;
			case LONG:
				get2bytes();
				strcat(address_fld, "&");
				fourbytes = cur2bytes << 16;
				get2bytes();
				fourbytes |= cur2bytes;
				if ((sign == SIGNED) && HIOF32(fourbytes))
					strcat(address_fld, "-");
				convert(((sign == SIGNED) && HIOF32(fourbytes))?
				    -(long) fourbytes : fourbytes,
				    conv_temp, LEAD);
				strcat(address_fld, conv_temp);
				break;
			} /* end of size switch */
			break;
		default:
			confused();
		} /* end reg switch */
		break;
	} /* end mode switch */
	return(address_fld);
}

#endif
