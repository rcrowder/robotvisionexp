/*********************************************************************
 *
 * Copyright:
 *	1998-1999 MOTOROLA, INC. All Rights Reserved.  
 *  You are hereby granted a copyright license to use, modify, and
 *  distribute the SOFTWARE so long as this entire notice is
 *  retained without alteration in any modified and/or redistributed
 *  versions, and that such modified versions are clearly identified
 *  as such. No licenses are granted by implication, estoppel or
 *  otherwise under any patents or trademarks of Motorola, Inc. This 
 *  software is provided on an "AS IS" basis and without warranty.
 *
 *  To the maximum extent permitted by applicable law, MOTOROLA 
 *  DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED, INCLUDING 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR
 *  PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH REGARD TO THE 
 *  SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF) AND ANY 
 *  ACCOMPANYING WRITTEN MATERIALS.
 * 
 *  To the maximum extent permitted by applicable law, IN NO EVENT
 *  SHALL MOTOROLA BE LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING 
 *  WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS 
 *  INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY
 *  LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.   
 * 
 *  Motorola assumes no responsibility for the maintenance and support
 *  of this software
 ********************************************************************/
/*
 * dis_cf.c:
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
 *
 * NOTE:
 *	This file originated from the Motorola DBUG file:
 *	dss/src/dbug/v2/cpu/m68k/mc68k.c
 *	(see above copyright notice).
 *	It was downloaded from the Motorola website:
 *	http://www.mot/com/SPS/HPESD/prod/coldfire/dbugfirm.html
 *
 */


#include "config.h"
#include "genlib.h"
#if INCLUDE_DISASSEMBLER

#define SYMBOL_TABLE
#undef SYMBOL_TABLE
#define MCF5206

typedef unsigned char		BYTE;	/*  8 bits */
typedef unsigned short int	WORD;	/* 16 bits */
typedef unsigned long int	LONG;	/* 32 bits */

typedef signed char			SBYTE;	/*  8 bits */
typedef signed short int	SWORD;	/* 16 bits */
typedef signed long int		SLONG;	/* 32 bits */

typedef void *ADDRESS;

ADDRESS cpu_disasm (ADDRESS, int);

int
cpu_read_data(ADDRESS add,int wide)
{
	if (wide == 8) {
		return((int)*(BYTE *)add);
	}
	else if (wide == 16) {
		return((int)*(WORD *)add);
	}
	else if (wide == 32) {
		return((int)*(LONG *)add);
	}
	else {
		printf("cpu_read_data() confused: wide = %d\n",wide);
		return(0);
	}
}

/* Don't change any of the following defines !!! */
#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0
#ifdef TRUE
#undef TRUE
#endif
#define TRUE  1

#define SIZE_BYTE	(8)
#define SIZE_WORD	(16)
#define SIZE_LONG	(32)

/*
 * M68K addressing modes
 */
#define DRD		(0x00000001)		/* data register direct */
#define ARD		(0x00000002)		/* address register direct */
#define ARI		(0x00000004)		/* address register indirect */
#define ARIPO	(0x00000008)		/* ari with postincrement */
#define ARIPR	(0x00000010)		/* ari with predecrement */
#define ARID	(0x00000020)		/* ari with displacement */
#define ARII8	(0x00000040)		/* ari with index 8-bit */
#define ARIIB	(0x00000080)		/* ari with index base */
#define MIPO	(0x00000100)		/* memory indirect postindexed */
#define MIPR	(0x00000200)		/* memory indirect preindexed */
#define PCID	(0x00000400)		/* program counter indirect disp */
#define PCII8	(0x00000800)		/* pci with index 8-bit */
#define PCIIB	(0x00001000)		/* pci with index base */
#define PCMIPO	(0x00002000)		/* pc memory indirect postindexed */
#define PCMIPR	(0x00004000)		/* pc memory indirect preindexed */
#define AS		(0x00008000)		/* absolute short */
#define AL		(0x00010000)		/* absolute long */
#define IM		(0x00020000)		/* immediate */

/*
 * Addressing mode categories
 */
#if (defined(MCF5200))

#define EA_DATA		(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 \
					| PCID | PCII8 | AS | AL | IM )

#define EA_MEMORY	(ARI | ARIPO | ARIPR | ARID | ARII8 \
					| PCID | PCII8 | AS | AL | IM )

#define EA_CONTROL	(ARI | ARID | ARII8 | PCID | PCII8 | AS | AL )

#define EA_ALTER	(DRD | ARD | ARI | ARIPO | ARIPR | ARID | ARII8 )

#define EA_DATA_ALTER	(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 )

#define EA_MEM_ALTER	(ARI | ARIPO | ARIPR | ARID | ARII8 )

#define EA_CTRL_ALTER	(ARI | ARID | ARII8 )

#define EA_ALL		(DRD | ARD | ARI | ARIPO | ARIPR | ARID | ARII8 \
					| PCID | PCII8 | AS | AL | IM )

#define EA_NONE		(0)

#define EA_DATA1	(DRD | ARI | ARIPO | ARIPR | ARID)
#define EA_DATA2	(DRD | IM)
#define EA_DATA3	(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 | AS | AL \
					| IM | PCID | PCII8)
#define EA_DATALT1	(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 | AS | AL )
#define EA_DATALT2	(DRD)
#define EA_DATALT3	(DRD | ARI | ARIPO | ARIPR | ARID)
#define EA_ALTER1	(DRD | ARD | ARI | ARIPO | ARIPR | ARID | ARII8 | AS | AL)
#define EA_MEMALT1	(ARI | ARIPO | ARIPR | ARID | ARII8 | AS | AL)
#define EA_CTRL1	(ARI | ARID)
#define EA_CTRALT3	(ARI | ARID)

#ifdef MCF5200M
#define EA_MAC1		(ARI | ARIPO | ARIPR | ARID)
#define EA_MAC2		(DRD | ARD | IM)
#endif

#else


#define EA_DATA		(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 \
					| ARIIB | MIPO | MIPR | PCID | PCII8 | PCIIB \
					| PCMIPO | PCMIPR | AS | AL | IM )

#define EA_MEMORY	(ARI | ARIPO | ARIPR | ARID | ARII8 \
					| ARIIB | MIPO | MIPR | PCID | PCII8 | PCIIB \
					| PCMIPO | PCMIPR | AS | AL | IM )

#define EA_CONTROL	(ARI | ARID | ARII8 | ARIIB \
					| MIPO | MIPR | PCID | PCII8 | PCIIB \
					| PCMIPO | PCMIPR | AS | AL )

#define EA_ALTER	(DRD | ARD | ARI | ARIPO | ARIPR | ARID | ARII8 \
					| ARIIB | MIPO | MIPR \
					| PCMIPO | PCMIPR )


#define EA_DATA_ALTER	(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 | ARIIB \
						| MIPO | MIPR | PCMIPO | PCMIPR )

#define EA_MEM_ALTER	(ARI | ARIPO | ARIPR | ARID | ARII8 | ARIIB \
						| PCMIPO | PCMIPR )

#define EA_CTRL_ALTER	(ARI | ARID | ARII8 | ARIIB | MIPO | MIPR \
						| PCMIPO | PCMIPR )

#define EA_ALL		(DRD | ARD | ARI | ARIPO | ARIPR | ARID | ARII8 \
					| ARIIB | MIPO | MIPR | PCID | PCII8 | PCIIB \
					| PCMIPO | PCMIPR | AS | AL | IM )


#define EA_NONE		(0)

#define EA_DATA1	(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 | ARIIB \
					| MIPO | MIPR | PCID | PCII8 | PCIIB | PCMIPO \
					| PCMIPR | AS | AL )

#define EA_DATALT1	(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 | ARIIB \
					| MIPO | MIPR | AS | AL )

#define EA_DATALT2	(DRD | ARI | ARIPO | ARIPR | ARID | ARII8 | AS | AL )

#define EA_ALTER1	(DRD | ARD | ARI | ARIPO | ARIPR | ARID | ARII8 \
					| ARIIB | MIPO | MIPR | AS | AL)

#define EA_MEMALT1	(ARI | ARIPO | ARIPR | ARID | ARII8 | ARIIB \
					| MIPO | MIPR | AS | AL)

#define EA_CTRALT1	(DRD | ARI | ARID | ARII8 | ARIIB | MIPO | MIPR \
					| AS | AL )

#define EA_CTRALT2	(DRD | ARI | ARID | ARII8 | ARIIB | MIPO | MIPR \
					| PCID | PCII8 | PCIIB | PCMIPO | PCMIPR | AS | AL )

#define EA_CTRALT3	(ARI | ARIPR | ARID | ARII8 | ARIIB | MIPO | MIPR \
					| AS | AL )

#define EA_CTRL1	(ARI | ARIPO | ARID | ARII8 | ARIIB | MIPO | MIPR \
					| PCID | PCII8 | PCIIB | PCMIPO | PCMIPR | AS | AL )

#endif

typedef struct
{
	WORD	keepers;
	WORD	match;
	char	*instruction;
	int		ea_mask;
	void	(*handler)(int, WORD);
} INSTRENTRY;

static const INSTRENTRY isa[];

/********************************************************/

#define ADDRESS_REGISTER -2
#define DATA_REGISTER    -3

/********************************************************/

static char dstr[80];
static ADDRESS disasm_pc;
static int disasm_op_size;
static int valid_instruction;

/********************************************************/
static void
inc_disasm_pc (unsigned int num)
{
	disasm_pc = (ADDRESS)((unsigned int)disasm_pc +
		(unsigned int)num);
}

/********************************************************/

static void
append_instruction (char *buf, char *instruction)
{
	sprintf(buf,"%-10s",instruction);
}

#if 0
static void
append_string (char *buf, char *string)
{
	/*
	 * This routine appends a string.
	 */
	strcat(buf,string);
}
#endif
#define append_string(a,b)	strcat(a,b)

static void
append_value (char *buf, int value, int size)
{
	/*
	 * This routine appends a value in hex notation.
	 */
	char buffer[9];

	buffer[0] = '\0';
	switch (size)
	{
		case 8:
			sprintf(buffer,"0x%02X",value & 0x000000FF);
			break;
		case 16:
			sprintf(buffer,"0x%04X",value & 0x0000FFFF);
			break;
		case 32:
			sprintf(buffer,"0x%08X",value);
			break;
	}
	strcat(buf,buffer);
}

static int
append_size2 (char *buf, int opword, int size_offset, int instr_size)
{
	/*
	 * This field accepts `opword' and then determines according
	 * to the offset which size is specified.
	 *
	 * The `offset' are given by the bit number as listed in the
	 * M68000 Family Programmer's Reference, Chapter 8. [15 .. 0]
	 */
	int i, j, mask;

	mask = 0x3;	/* 2 bits */
	for (i = 1; i < size_offset; i++)
		mask = mask << 1;
	i = (opword & mask) >> (size_offset - 1);

	if (instr_size)
	{
		for (j = 0; *(char *)((int)buf +j) != ' '; ++j)
			;
		buf[j] = '.';
		switch (i)
		{
			case 1:
				buf[++j] = 'B';
				disasm_op_size = SIZE_BYTE;
				break;
			case 2:
				buf[++j] = 'L';
				disasm_op_size = SIZE_LONG;
				break;
			case 3:
				buf[++j] = 'W';
				disasm_op_size = SIZE_WORD;
				break;
			default:
				valid_instruction = FALSE;
				break;
		}
	}
	else
	{
		switch (i)
		{
			case 1:
				strcat(buf,".B");
				break;
			case 2:
				strcat(buf,".L");
				break;
			case 3:
				strcat(buf,".W");
				break;
			default:
				valid_instruction = FALSE;
				break;
		}
	}
	return i;
}

static int
append_size (char *buf, int opword, int size_offset, int instr_size)
{
	/*
	 * This field accepts `opword' and then determines according
	 * to the offset which size is specified.
	 *
	 * The `offset' are given by the bit number as listed in the
	 * M68000 Family Programmer's Reference, Chapter 8. [15 .. 0]
	 */
	int i, j, mask;

	mask = 0x3;	/* 2 bits */
	for (i = 1; i < size_offset; i++)
		mask = mask << 1;
	i = (opword & mask) >> (size_offset - 1);

	disasm_op_size = -1;
	if (instr_size)
	{
		for (j = 0; *(char *)((int)buf +j) != ' '; ++j)
			;
		buf[j] = '.';
		switch (i)
		{
			case 0:
				buf[++j] = 'B';
				disasm_op_size = SIZE_BYTE;
				break;
			case 1:
				buf[++j] = 'W';
				disasm_op_size = SIZE_WORD;
				break;
			case 2:
				buf[++j] = 'L';
				disasm_op_size = SIZE_LONG;
				break;
			default:
				valid_instruction = FALSE;
				break;
		}
	}
	else
	{
		switch (i)
		{
			case 0:
				strcat(buf,".B");
				break;
			case 1:
				strcat(buf,".W");
				break;
			case 2:
				strcat(buf,".L");
				break;
			default:
				valid_instruction = FALSE;
				break;
		}
	}
	return i;
}

static void
append_register (char *buf, int opword, int reg_offset, int reg_num_offset)
{
	/*
	 * This field accepts `opword' and then determines according
	 * to the offsets which register (A0..A7,D0..D7) is specified.
	 * The register name is then concatenated to the end of the
	 * disasm_stmt.
	 *
	 * The `offsets' are given by the bit number as listed in the
	 * M68000 Family Programmer's Reference, Chapter 8. [15 .. 0]
	 */
	int i, mask;
	char regnum[3];

	/* Determine what kind of register */
	if (reg_offset == ADDRESS_REGISTER)
	{
		strcat(buf,"A");
	}
	else
	{
		if (reg_offset == DATA_REGISTER)
		{
		strcat(buf,"D");
		}
		else
		{
			mask = 1;
			for (i = 0; i < reg_offset; i++)
				mask = mask << 1;
			if (opword & mask)
				strcat(buf,"A");
			else
				strcat(buf,"D");
		}
	}

	/* determine register number */
	/* The offset given is the msb of the 3 bit field containing */
	/* the register number. */
	mask = 0x7;	/* 3 bits */
	for (i = 2; i < reg_num_offset; i++)
		mask = mask << 1;
	i = (opword & mask) >> (reg_num_offset - 2);
	sprintf(regnum,"%d",i);
	strcat(buf,regnum);
}

static void
append_displacement (char *buf, int extension, int disp_offset)
{
	/*
	 * This function determines and appends a 16 or 32 bit disp.
	 * The `offsets' are given by the bit number as listed in the
	 * M68000 Family Programmer's Reference, Chapter 2. [15 .. 0]
	 */
	int i, mask, disp;

	mask = 0x3;	/* 2 bits */
	for (i = 1; i < disp_offset; i++)
		mask = mask << 1;
	i = (extension & mask) >> (disp_offset - 1);

	switch (i)
	{
		case 0:
		case 1:
			break;
		case 2:
			disp = (int)((SWORD)cpu_read_data((ADDRESS)disasm_pc,16));
			inc_disasm_pc(2);
			append_value(buf,disp,16);
			break;
		case 3:
			disp = (int)((SLONG)cpu_read_data((ADDRESS)disasm_pc,32));
			inc_disasm_pc(4);
			append_value(buf,disp,32);
			break;
	}
}

static void
append_size_scale (char *buf, int extension, int size_offset, int scale_offset)
{
	/*
	 * This function determines the size and scale information
	 * for addressing modes that require it.
	 * 
	 * The `offsets' are given by the bit number as listed in the
	 * M68000 Family Programmer's Reference, Chapter 2. [15 .. 0]
	 */
	int i, mask, size, scale;

	mask = 0x1;	/* 1 bits */
	for (i = 0; i < size_offset; i++)
		mask = mask << 1;
	size = (extension & mask) >> size_offset;

	mask = 0x3;	/* 2 bits */
	for (i = 1; i < scale_offset; i++)
		mask = mask << 1;
	scale = (extension & mask) >> (scale_offset - 1);

	if (size)
		append_string(buf,".L");
	else
		append_string(buf,".W");

	switch (scale)
	{
		case 0:
			append_string(buf,"*1");
			break;
		case 1:
			append_string(buf,"*2");
			break;
		case 2:
			append_string(buf,"*4");
			break;
		case 3:
			/* valid_instruction = FALSE; */
			append_string(buf,"*8");
			break;
	}
}

static int
append_ea (char *buf, int opword, int offset, int ea_mask)
{
	/*
	 * This routine creates the addressing mode.  The
	 * extensions for the addressing mode, if necessary,
	 * start at disasm_pc
	 */
	int i, mask, mode, reg, ea;
	char buffer[9];
#ifdef SYMBOL_TABLE
	char tstr[100];
#endif

	ea = EA_NONE;

	/* get addressing mode */
	mask = 0x7;	/* 3 bits */
	for (i = 2; i < offset; i++)
		mask = mask << 1;
	mode = (opword & mask) >> (offset - 2);

	/* get register */
	mask = 0x7;	/* 3 bits */
	for (i = 2; i < (offset - 3); i++)
		mask = mask << 1;
	reg = (opword & mask) >> (offset -3 - 2);

	switch (mode)
	{
		case 0:	/* data register direct mode */
			append_register(buf,reg,DATA_REGISTER,2);
			ea = DRD;
			break;
		case 1:	/* address register direct mode */
			append_register(buf,reg,ADDRESS_REGISTER,2);
			ea = ARD;
			break;
		case 2:	/* address register indirect mode (ARI) */
			append_string(buf,"(");
			append_register(buf,reg,ADDRESS_REGISTER,2);
			append_string(buf,")");
			ea = ARI;
			break;
		case 3:	/* ARI with postincrement mode */
			append_string(buf,"(");
			append_register(buf,reg,ADDRESS_REGISTER,2);
			append_string(buf,")+");
			ea = ARIPO;
			break;
		case 4:	/* ARI with predecrement mode */
			append_string(buf,"-(");
			append_register(buf,reg,ADDRESS_REGISTER,2);
			append_string(buf,")");
			ea = ARIPR;
			break;
		case 5:	/* ARI with displacement mode */
			{
				int disp;
				disp = (int)((SWORD)cpu_read_data((ADDRESS)disasm_pc,16));
				inc_disasm_pc(2);
				sprintf(buffer,"%d",disp);
				append_string(buf,buffer);
				append_string(buf,"(");
				append_register(buf,reg,ADDRESS_REGISTER,2);
				append_string(buf,")");
				ea = ARID;
			}
			break;
		case 6:
			{
				/* this mode is overloaded.  the encoding in the */
				/* extension byte indicate which of the 4 modes  */
				/*                                               */
				/* [xxxxxxx0xxxx0000] ARI 8bit displacement      */
				/* [xxxxxxx1xxxx0000] ARI base displacement      */
				/* [xxxxxxx1xxxx00xx] memory indirect pre index  */
				/* [xxxxxxx1xxxx01xx] memory indirect post index */
				/*                                               */

				int extension;

				extension = (int)*(WORD *)disasm_pc;
				inc_disasm_pc(2);

				if (extension & 0x0100)
				{
					/* ARI base or memory indirects */
					if (extension & 0x0007)
					{
						/* memory indirects */
						if (extension & 0x0004)
						{
							/* memory indirect post index */
							append_string(buf,"(");
							append_string(buf,"[");
							append_displacement(buf,extension,5);
							append_string(buf,",");
							append_register(buf,reg,ADDRESS_REGISTER,2);
							append_string(buf,"]");
							append_string(buf,",");
							append_register(buf,extension,15,14);
							append_size_scale(buf,extension,11,10);
							append_string(buf,",");
							append_displacement(buf,extension,1);
							ea = MIPO;
						}
						else
						{
							/* memory indirect pre index */
							append_string(buf,"(");
							append_string(buf,"[");
							append_displacement(buf,extension,5);
							append_string(buf,",");
							append_register(buf,reg,ADDRESS_REGISTER,2);
							append_string(buf,",");
							append_register(buf,extension,15,14);
							append_size_scale(buf,extension,11,10);
							append_string(buf,"]");
							append_string(buf,",");
							append_displacement(buf,extension,1);
							ea = MIPR;
						}
					}
					else
					{
						/* ARI with BASE displacement */
						append_string(buf,"(");
						append_displacement(buf,extension,5);
						append_string(buf,",");
						append_register(buf,reg,ADDRESS_REGISTER,2);
						append_string(buf,",");
						append_register(buf,extension,15,14);
						append_size_scale(buf,extension,11,10);
						ea = ARIIB;
					}
				}
				else
				{
					SBYTE disp8;

					disp8 = (SBYTE)(extension & 0x00FF);
					sprintf(buffer,"%d",disp8);
					append_string(buf,buffer);
					append_string(buf,"(");
					append_register(buf,reg,ADDRESS_REGISTER,2);
					append_string(buf,",");
					append_register(buf,extension,15,14);
					append_size_scale(buf,extension,11,10);
					ea = ARII8;
				}
			append_string(buf,")");
			}
			break;
		case 7:
			switch (reg)
			{
				case 0x0:
					{
						int data;
						data = (int)((SWORD)cpu_read_data((ADDRESS)disasm_pc,16));
						inc_disasm_pc(2);
						append_string(buf,"(");
						append_value(buf,data,16);
						append_string(buf,".W");
						append_string(buf,")");
					}
					ea = AS;
					break;
				case 0x1:
					{
						int data;
						data = (int)((SLONG)cpu_read_data((ADDRESS)disasm_pc,32));
						inc_disasm_pc(4);

#ifdef SYMBOL_TABLE
						if (symtab_convert_address((ADDRESS)data,tstr))
						{
							append_string(buf,tstr);
						}
						else
						{
							append_string(buf,"(");
							append_value(buf,data,32);
							append_string(buf,".L");
							append_string(buf,")");
						}
#else
						append_string(buf,"(");
						append_value(buf,data,32);
						append_string(buf,".L");
						append_string(buf,")");
#endif
					}
					ea = AL;
					break;
				case 0x2:
					{
						int disp;
						disp = (int)((SWORD)cpu_read_data((ADDRESS)disasm_pc,16));
						inc_disasm_pc(2);
						sprintf(buffer,"%d",disp);
						append_string(buf,buffer);
						append_string(buf,"(PC)");
					}
					ea = PCID;
					break;
				case 0x3:
					{
						int extension, disp;

						extension = (int)*(WORD *)disasm_pc;
						inc_disasm_pc(2);

						/* this mode is overloaded.  the encoding in the */
						/* extension byte indicate which of the 4 modes  */
						/*                                               */
						/* [xxxxxxx0xxxx0000] PC  8bit displacement      */
						/* [xxxxxxx1xxxx0000] PC  base displacement      */
						/* [xxxxxxx1xxxx00xx] PC mem indirect pre index  */
						/* [xxxxxxx1xxxx01xx] PC mem indirect post index */
						/*                                               */

						if (extension & 0x0100)
						{
							/* PC base or PC memory indirects */
							if (extension & 0x0007)
							{
								/* PC memory indirects */
								if (extension & 0x0004)
								{
									/* memory indirect post index */
									append_string(buf,"(");
									append_string(buf,"[");
									append_displacement(buf,extension,5);
									append_string(buf,",PC],");
									append_register(buf,extension,15,14);
									append_size_scale(buf,extension,11,10);
									append_string(buf,",");
									append_displacement(buf,extension,1);
									ea = PCMIPO;
								}
								else
								{
									/* memory indirect pre index */
									append_string(buf,"(");
									append_string(buf,"[");
									append_displacement(buf,extension,5);
									append_string(buf,",PC,");
									append_register(buf,extension,15,14);
									append_size_scale(buf,extension,11,10);
									append_string(buf,"]");
									append_string(buf,",");
									append_displacement(buf,extension,1);
									ea = PCMIPR;
								}
							}
							else
							{
								/* base disp */
								append_string(buf,"(");
								append_displacement(buf,extension,5);
								append_string(buf,",PC,");
								append_register(buf,extension,15,14);
								append_size_scale(buf,extension,11,10);
								ea = PCIIB;
							}
						}
						else
						{
							disp = (int)((SBYTE)(extension & 0x00FF));
							sprintf(buffer,"%d",disp);
							append_string(buf,buffer);
							append_string(buf,"(PC,");
							append_register(buf,extension,15,14);
							append_size_scale(buf,extension,11,10);
							append_string(buf,")");
							ea = PCII8;
						}
					}
					break;
				case 0x4:
					{
						int data;
						switch (disasm_op_size)
						{
							case SIZE_BYTE:
								data = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
								inc_disasm_pc(2);
								data = (int)(data & 0x00FF);
								append_string(buf,"#");
								append_value(buf,data,8);
								break;
							case SIZE_WORD:
								data = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
								inc_disasm_pc(2);
								append_string(buf,"#");
								append_value(buf,data,16);
								break;
							case SIZE_LONG:
								data = (int)((LONG)cpu_read_data((ADDRESS)disasm_pc,32));
								inc_disasm_pc(4);
								append_string(buf,"#");
#ifdef SYMBOL_TABLE
								if (symtab_convert_address((ADDRESS)data,tstr))
								{
									append_string(buf,tstr);
								}
								else
									append_value(buf,data,32);
#else
								append_value(buf,data,32);
#endif
								break;
							default:
								break;
						}
					}
					ea = IM;
					break;
			}
		default:
			break;
	}
	if (!(ea_mask & ea))
	{
		/* EA was not one of ones in mask */
		valid_instruction = FALSE;
	}
	return ea;	/* needed by MOVE */
}

/********************************************************/
#if 0
static void
func (int index, WORD opword)
{
	sprintf(dstr,"%s -- ??????",isa[index].instruction);
}
#endif

static void
func1 (int index, WORD opword)
{
	/* BTST, BCLR, BCHG, BSET statics */
	int data;
	data = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	if (0xFF00 & data)
	{
		valid_instruction = FALSE;
		return;
	}
	append_string(dstr,"#");
	append_value(dstr,data,8);
	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

#ifndef MCF5200
static void
func2 (int index, WORD opword)
{
	/* ORI ANDI EORI to CCR */
	int data;

	(void)index;
	(void)opword;

	data = (int)((LONG)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);
	append_string(dstr,"#");
	append_value(dstr,data,8);
	append_string(dstr,",CCR");
}

static void
func3 (int index, WORD opword)
{
	/* ORI ANDI EORI to SR */
	int data;

	(void)index;
	(void)opword;

	data = (int)((LONG)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);
	append_string(dstr,"#");
	append_value(dstr,data,16);
	append_string(dstr,",SR");
}

static void
func4 (int index, WORD opword)
{
	/* RTM */

	(void)index;
	(void)opword;

	append_register(dstr,opword,3,2);
}

static void
func5 (int index, WORD opword)
{
	/* CALLM */
	int data;

	data = (int)((LONG)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);
	append_string(dstr,"#");
	append_value(dstr,data,8);
	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}
#endif /* MCF5200 */

static void
func6 (int index, WORD opword)
{
	/* ORI EORI ANDI SUBI ADDI  */
	int data;

	append_size(dstr,opword,7,TRUE);
	append_string(dstr,"#");

	switch (disasm_op_size)
	{
		case SIZE_BYTE:
			data = (int)((LONG)cpu_read_data((ADDRESS)disasm_pc,16));
			inc_disasm_pc(2);
			append_value(dstr,data,8);
			break;
		case SIZE_WORD:
			data = (int)((LONG)cpu_read_data((ADDRESS)disasm_pc,16));
			inc_disasm_pc(2);
			append_value(dstr,data,16);
			break;
		case SIZE_LONG:
			data = (int)((LONG)cpu_read_data((ADDRESS)disasm_pc,32));
			inc_disasm_pc(4);
			append_value(dstr,data,32);
			break;
	}

	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

static void
func7 (int index, WORD opword)
{
	/* BTST, BCHG, BSET, BCLR reg */
	append_register(dstr,opword,DATA_REGISTER,11);
	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

static void
func8 (int index, WORD opword)
{
	/* MOVE .b,.w,.l */
	int opw1, opw2;
#ifdef MCF5200
	int sea;
#endif

	/* the destination <ea> is swapped from the form expected by */
	/* append_ea().  so make it look right */
	opw1 = opword & 0x0FC0;
	opword = opword & 0xF03F;		/* punch out dest */
	opw2 = (opw1 & 0x01C0) << 3;	/* mode */
	opword = opword | opw2;			/* swapped mode */
	opw2 = (opw1 & 0x0E00) >> 3;	/* reg */
	opword = opword | opw2;			/* swapped reg */

	append_size2(dstr,opword,13,TRUE);
#ifndef MCF5200
	append_ea(dstr,opword,5,EA_ALL);
	append_string(dstr,",");
	append_ea(dstr,opword,11,isa[index].ea_mask);
#endif

#ifdef MCF5200
	sea = append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");

	/* NOTE: On Coldfire, not all dest <ea> are possible! */
	/* See MCF5200 Family Programmer's Reference Manual,  */
	/* pages 4-53 thru 4-55 for further information.      */

	switch (sea)
	{
		case DRD:
		case ARD:
		case ARI:
		case ARIPO:
		case ARIPR:
			append_ea(dstr,opword,11,
				(DRD | ARD | ARI | ARIPO | ARIPR | ARID | ARII8 \
				| AS | AL));
			break;
		case ARID:
		case PCID:
			append_ea(dstr,opword,11,
				(DRD | ARD | ARI | ARIPO | ARIPR | ARID));
			break;
		case ARII8:
		case PCII8:
		case AS:
		case AL:
		case IM:
			append_ea(dstr,opword,11,
				(DRD | ARD | ARI | ARIPO | ARIPR));
			break;
		default:
			valid_instruction = FALSE;
			break;
	}
#endif

}

static void
func9 (int index, WORD opword)
{
	/* MOVE from SR */
	append_string(dstr,"SR,");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

static void
func10 (int index, WORD opword)
{
	/* MOVE to SR */
	disasm_op_size = SIZE_WORD;
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",SR");
}

static void
func11 (int index, WORD opword)
{
	/* MOVE from CCR */
	append_string(dstr,"CCR,");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

static void
func12 (int index, WORD opword)
{
	/* MOVE to CCR */
	disasm_op_size = SIZE_WORD;
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",CCR");
}

static void
func13 (int index, WORD opword)
{
	/* SWAP EXT EXTB */
	(void)index;
	append_register(dstr,opword,DATA_REGISTER,2);
}

static void
func14 (int index, WORD opword)
{
	/* ADDX SUBX */
	(void)index;
	append_size(dstr,opword,7,TRUE);

	if (opword & 0x0008)
	{
		/* ADDX/SUBX -(Ax),-(Ay) */
		append_string(dstr,"-(");
		append_register(dstr,opword,ADDRESS_REGISTER,2);
		append_string(dstr,"),-(");
		append_register(dstr,opword,ADDRESS_REGISTER,11);
		append_string(dstr,")");
	}
	else
	{
		/* ADDX/SUBX Dx,Dy */
		append_register(dstr,opword,DATA_REGISTER,2);
		append_string(dstr,",");
		append_register(dstr,opword,DATA_REGISTER,11);
	}
}

static void
func15 (int index, WORD opword)
{
	/* NEGX NEG NOT CLR WDDATA */
	append_size(dstr,opword,7,TRUE);
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

static void
func16 (int index, WORD opword)
{
	/* LINK word */
	int disp;
	char buffer[20];

	(void)index;
	disp = (int)((SWORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);
	append_register(dstr,opword,ADDRESS_REGISTER,2);
	append_string(dstr,",#");
	sprintf(buffer,"%d",disp);
	append_string(dstr,buffer);
}

static void
func17 (int index, WORD opword)
{
	/* RTE HALT RTS NOP */
	/* instruction printed by calling routine */
	(void)index;
	(void)opword;
}

static void
func18 (int index, WORD opword)
{
	/* ASL ASR LSL LSR ROR ROL ROXL ROXR NBCD PEA JSR JMP TAS */
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

#ifndef MCF5200
static void
func19 (int index, WORD opword)
{
	/* CMP2 and CHK2 */
	int opword2;

	opword2 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	/* both instruction have same EA mask */
	if (opword2 & 0x000800)
	{
		append_instruction(dstr,"CHK2");
	}
	else
	{
		append_instruction(dstr,"CMP2");
	}
	append_size(dstr,opword,10,TRUE);
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	append_register(dstr,opword2,15,14);
}
#if 0
static void
func20 (int index, WORD opword)
{
	int data;

	(void)index;

	append_string(dstr,"#");
	data = opword & 0x0007;
	append_value(dstr,data,8);
}
#endif
#endif /* MCF5200 */

static void
func21 (int index, WORD opword)
{
	(void)index;
	append_register(dstr,opword,ADDRESS_REGISTER,2);
}

static void
func22 (int index, WORD opword)
{
	/* TRAP */
	int data;
	(void)index;
	append_string(dstr,"#");
	data = opword & 0x000F;
	append_value(dstr,data,8);
}

static void
func23 (int index, WORD opword)
{
	/* STOP RTD */
	int data;

	(void)index;
	(void)opword;
	append_string(dstr,"#");
	data = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);
	append_value(dstr,data,16);
}

static void
func24 (int index, WORD opword)
{
	/* MOVEQ */
	int data;

	(void)index;
	append_string(dstr,"#");
	data = opword & 0x00FF;
	append_value(dstr,data,8);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,11);
}

static void
func25 (int index, WORD opword)
{
	/* Bcc */
	int disp, target;
#ifdef SYMBOL_TABLE
	char tstr[100];
#endif

	(void)index;
	disp = opword & 0x00FF;
	if (disp == 0)
	{
		/* 16 bit disp */
		disp = (int)((SWORD)cpu_read_data((ADDRESS)disasm_pc,16));
		target = (int)disasm_pc + disp;
		inc_disasm_pc(2);
	}
	else
	{
		if (disp == 0x00FF)
		{
			/* 32 bit disp */
			valid_instruction = FALSE;
			return;
		}
		else
		{
			/* 8 bit disp */
			disp = (int)((SBYTE)disp);
			target = (int)disasm_pc + disp;
		}
	}
#ifdef SYMBOL_TABLE
	if (symtab_convert_address((ADDRESS)target,tstr))
	{
		append_string(dstr,tstr);
	}
	else
		append_value(dstr,target,32);
#else
	append_value(dstr,target,32);
#endif
}


static void
func26 (int index, WORD opword)
{
	/* MOVEA */

	append_size2(dstr,opword,13,TRUE);
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	append_register(dstr,opword,ADDRESS_REGISTER,11);
}

int
more_bits_set (int cm, int cc, int dir, int reg_mask)
{
	/* cm = current mask, cc = current count */
	switch (dir)
	{
		case 1:
			cm = cm << 1;
			break;
		case -1:
			cm = cm >> 1;
			break;
	}
	++cc;
	while (cc < 8)
	{
		if (cm & reg_mask)
		{
			return TRUE;
		}
		switch (dir)
		{
			case 1:
				cm = cm << 1;
				break;
			case -1:
				cm = cm >> 1;
				break;
		}
		++cc;
	}
	return FALSE;
}

static void
append_reg_list (int reg_mask, int opword)
{
	int mask, i, j, first, need_slash, dir, next_set, some_set;
	char buffer[9];
	char *reg;
	char regA[] = "A";
	char regD[] = "D";

	next_set = mask = 0;

	/* Check for predecrement mode */
	if ((opword & 0x0038) == 0x0020)
	{
		/* predecrement */
		dir = -1;
	}
	else
	{
		dir = 1;
	}

	need_slash = FALSE;
	for (j = 0; j < 2; j++)
	{
		if (j == 0)
		{
			switch (dir)
			{
				case 1:
					mask = 0x0001;
					break;
				case -1:
					mask = 0x8000;
					break;
			}
			reg = regD;
		}
		else
		{
			switch (dir)
			{
				case 1:
					mask = 0x0100;
					break;
				case -1:
					mask = 0x0080;
					break;
			}
			reg = regA;
			if (need_slash)
				append_string(dstr,"/");
		}

		some_set = FALSE;
		first = TRUE;
		need_slash = FALSE;

		for (i = 0; i < 8; i++)
		{
			if (reg_mask & mask)
			{
				some_set = TRUE;
				if (first)
				{
					/* first one in subrange */
					if (need_slash)
					{
						append_string(dstr,"/");
						need_slash = FALSE;
					}
					append_string(dstr,reg);
					sprintf(buffer,"%d",i);
					append_string(dstr,buffer);
					if (!more_bits_set(mask,i,dir,reg_mask))
						need_slash = TRUE;
					first = FALSE;
				}
				else
				{
					/* check to see if next set */
					switch (dir)
					{
						case 1:
							next_set = (((reg_mask & (mask << 1))) &&
								(i != 7));
							break;
						case -1:
							next_set = (((reg_mask & (mask >> 1))) &&
								(i != 7));
							break;
					}
					if (!next_set)
					{
						/* the next isn't set, so display */
						append_string(dstr,"-");
						append_string(dstr,reg);
						sprintf(buffer,"%d",i);
						append_string(dstr,buffer);
							need_slash = TRUE;
						first = TRUE;
					}
				}
			}
			else first = TRUE;
			switch (dir)
			{
				case 1:
					mask = mask << 1;
					break;
				case -1:
					mask = mask >> 1;
					break;
			}
		}
		if ((i == 8) && (j == 0) && some_set)
		{
			switch (dir)
			{
				case 1:
					need_slash = more_bits_set(0x0080,-1,dir,reg_mask);
					break;
				case -1:
					need_slash = more_bits_set(0x0100,-1,dir,reg_mask);
					break;
				default:
					break;
			}
		}
	}
}

static void
func27 (int index, WORD opword)
{
	/* MOVEM */
	int mask;

	mask = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	if (opword & 0x0040)
	{
		/* long */
		append_size(dstr,0x0002,1,TRUE);
	}
	else
	{
		/* word */
		append_size(dstr,0x0002,2,TRUE);
	}

	if (opword & 0x0400)
	{
		/* memory to register */
		append_ea(dstr,opword,5,isa[index].ea_mask);
		append_string(dstr,",");
		append_reg_list(mask,opword);
	}
	else
	{
		/* register to memory */
		append_reg_list(mask,opword);
		append_string(dstr,",");
		append_ea(dstr,opword,5,isa[index].ea_mask);
	}
}

static void
func28 (int index, WORD opword)
{
	/* CMPA ADDA SUBA */

	if (opword & 0x0100)
	{
		/* long */
		append_size(dstr,0x0002,1,TRUE);
	}
	else
	{
		/* word */
		append_size(dstr,0x0002,2,TRUE);
	}
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	append_register(dstr,opword,ADDRESS_REGISTER,11);
}


static void
func29 (int index, WORD opword)
{
	/* SUBQ ADDQ */
	int data;

	data = ((opword & 0x0E00) >> 9);
	if (data == 0)
		data = 8;

	append_size(dstr,opword,7,TRUE);
	append_string(dstr,"#");
	append_value(dstr,data,8);
	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

#ifndef MCF5200
static void
func30 (int index, WORD opword)
{
	/* MOVES */
	int opword2;

	append_size(dstr,opword,7,TRUE);
	opword2 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	if (opword2 & 0x000800)
	{
		/* MOVES Rn,<ea> */
		append_register(dstr,opword2,15,14);
		append_string(dstr,",");
		append_ea(dstr,opword,5,isa[index].ea_mask);
	}
	else
	{
		/* MOVES <ea>,Rn */
		append_ea(dstr,opword,5,isa[index].ea_mask);
		append_string(dstr,",");
		append_register(dstr,opword2,15,14);
	}
}
#endif /* MCF5200 */


static void
func31 (int index, WORD opword)
{
	/* AND OR EOR */
	append_size(dstr,opword,7,TRUE);
	append_register(dstr,opword,DATA_REGISTER,11);
	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}


static void
func32 (int index, WORD opword)
{
	/* LEA */
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	append_register(dstr,opword,ADDRESS_REGISTER,11);
}


static void
func33 (int index, WORD opword)
{
	/* SUB AND ADD OR CMP */
	append_size(dstr,opword,7,TRUE);
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,11);
}

static void
func34 (int index, WORD opword)
{
	/* LSL,LSR,ROL,ROR,ROXL,ROXR,ASL,ASR #<data> */
	int data;

	(void)index;
	data = ((opword & 0x0E00) >> 9);
	if (data == 0)
		data = 8;
	append_size(dstr,opword,7,TRUE);
	append_string(dstr,"#");
	append_value(dstr,data,8);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,2);
}

static void
func35 (int index, WORD opword)
{
	/* LSL,LSR,ROL,ROR,ROXL,ROXR,ASL,ASR Dx */

	(void)index;
	append_size(dstr,opword,7,TRUE);
	append_register(dstr,opword,DATA_REGISTER,11);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,2);
}

static void
func36 (int index, WORD opword)
{
	/* MOVEC */
	int opword2;
	char buffer[9];

	(void)index;
	opword2 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	if (opword & 0x0001)
	{
		append_register(dstr,opword2,15,14);
		append_string(dstr,",");
	}

	switch (opword2 & 0x0FFF)
	{
#if (defined(MCF5202) || defined(MCF5204) || defined(MCF5206))
		case 0x002:
			append_string(dstr,"CACR");
			break;
		case 0x004:
			append_string(dstr,"ACR0");
			break;
		case 0x005:
			append_string(dstr,"ACR1");
			break;
#endif

#if (defined(MCF5200D) || defined(MCF5202) || \
	 defined(MCF5204)  || defined(MCF5206))
		case 0x801:
			append_string(dstr,"VBR");
			break;
#endif

#if (defined(MCF5204) || defined(MCF5206))
		case 0xC04:
			append_string(dstr,"RAMBAR");
			break;
		case 0xC0F:
			append_string(dstr,"MBAR");
			break;
#endif

#if (defined(MC68010) || defined(MC68020) || defined(MC68030) || \
	defined(MC68040) || defined(MC68EC040) || defined(CPU32))
		case 0x000:
			append_string(dstr,"SFC");
			break;
		case 0x001:
			append_string(dstr,"DFC");
			break;
		case 0x800:
			append_string(dstr,"USP");
			break;
		case 0x801:
			append_string(dstr,"VBR");
			break;
#endif

#if (defined(MC68020) || defined(MC68030) || defined(MC68040) || \
	defined(MC68EC040))

		case 0x002:
			append_string(dstr,"CACR");
			break;
		case 0x802:
			append_string(dstr,"CAAR");
			break;
		case 0x803:
			append_string(dstr,"MSP");
			break;
		case 0x804:
			append_string(dstr,"ISP");
			break;
#endif

#if (defined(MC68040) || defined(MC68LC040))
		case 0x003:
			append_string(dstr,"TCR");
			break;
		case 0x004:
			append_string(dstr,"ITT0");
			break;
		case 0x005:
			append_string(dstr,"ITT1");
			break;
		case 0x006:
			append_string(dstr,"DTT0");
			break;
		case 0x007:
			append_string(dstr,"DTT1");
			break;
		case 0x805:
			append_string(dstr,"MMUSR");
			break;
		case 0x806:
			append_string(dstr,"URP");
			break;
		case 0x807:
			append_string(dstr,"SRP");
			break;
#endif

#ifdef MC68EC040
		case 0x004:
			append_string(dstr,"IACR0");
			break;
		case 0x005:
			append_string(dstr,"IACR1");
			break;
		case 0x006:
			append_string(dstr,"DACR0");
			break;
		case 0x007:
			append_string(dstr,"DACR1");
			break;
#endif

		default:
			sprintf(buffer,"Rc%03X",opword2);
			append_string(dstr,buffer);
			break;
	}
	if (!(opword & 0x0001))
	{
		append_string(dstr,",");
		append_register(dstr,opword2,15,14);
	}

}

#ifdef MCF5200
static void
func37 (int index, WORD opword)
{
	/* WDEBUG */
	int opword2;

	opword2 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	if (opword2 & 0x0003)
	{
		append_ea(dstr,opword,5,isa[index].ea_mask);
	}
	else
	{
		valid_instruction = FALSE;
	}
}
#endif

static void
func38 (int index, WORD opword)
{
	/* MULS MULU .W */
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,11);
}

static void
func39 (int index, WORD opword)
{
	/* MULS MULU .L */
	int opword2,size;

	opword2 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	switch (opword2 & 0x8FF8)
	{
		case 0x0000:
			append_instruction(dstr,"MULU.L");
			size=32;
			break;
		case 0x0400:
			append_instruction(dstr,"MULU.L");
			size=64;
			break;
		case 0x0800:
			append_instruction(dstr,"MULS.L");
			size=32;
			break;
		case 0x0C00:
			append_instruction(dstr,"MULS.L");
			size=64;
			break;
		default:
			valid_instruction = FALSE;
			return;
			break;
	}

	if (size == 32)
	{
		/* 32 bit */
		append_ea(dstr,opword,5,isa[index].ea_mask);
		append_string(dstr,",");
		append_register(dstr,opword2,DATA_REGISTER,14);
	}
	else
	{
		/* 64 bit */
		append_ea(dstr,opword,5,isa[index].ea_mask);
		append_string(dstr,",");
		append_register(dstr,opword2,DATA_REGISTER,2);
		append_string(dstr,"-");
		append_register(dstr,opword2,DATA_REGISTER,14);
	}
}

#ifndef MCF5200
static void
func40 (int index, WORD opword)
{
	/* CAS2 */
	int opword2, opword3;

	(void)index;

	opword2 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);
	opword3 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	if (opword & 0x0200)
	{
		/* long */
		append_size(dstr,0x0002,1,TRUE);
	}
	else
	{
		/* word */
		append_size(dstr,0x0002,2,TRUE);
	}
	append_register(dstr,opword2,DATA_REGISTER,2);
	append_string(dstr,":");
	append_register(dstr,opword3,DATA_REGISTER,2);
	append_string(dstr,",");
	append_register(dstr,opword2,DATA_REGISTER,8);
	append_string(dstr,":");
	append_register(dstr,opword3,DATA_REGISTER,8);
	append_string(dstr,",(");
	append_register(dstr,opword2,15,14);
	append_string(dstr,"),(");
	append_register(dstr,opword3,15,14);
	append_string(dstr,")");
}

static void
func41 (int index, WORD opword)
{
	/* CAS */
	int opword2;

	opword2 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	append_size2(dstr,opword,10,TRUE);
	append_register(dstr,opword2,DATA_REGISTER,2);
	append_string(dstr,",");
	append_register(dstr,opword2,DATA_REGISTER,8);
	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

static void
func42 (int index, WORD opword)
{
	/* MOVEP */
	int opword2;

	(void)index;

	opword2 = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	inc_disasm_pc(2);

	if (opword & 0x0040)
	{
		/* long */
		append_size(dstr,opword,6,TRUE);
	}
	else
	{
		/* word */
		append_size(dstr,opword,4,TRUE);
	}

	if (opword & 0x0080)
	{
		/* MOVEP Dx,(d16,Ay) */
		append_register(dstr,opword,DATA_REGISTER,11);
		append_string(dstr,",(");
		append_value(dstr,opword2,16);
		append_string(dstr,",");
		append_register(dstr,opword,ADDRESS_REGISTER,2);
		append_string(dstr,")");
	}
	else
	{
		/* MOVEP (d16,Ay),Dx */
		append_string(dstr,"(");
		append_value(dstr,opword2,16);
		append_string(dstr,",");
		append_register(dstr,opword,ADDRESS_REGISTER,2);
		append_string(dstr,"),");
		append_register(dstr,opword,DATA_REGISTER,11);
	}
}

static void
func43 (int index, WORD opword)
{
	/* LINK long */
	int disp;
	char buffer[20];

	(void)index;

	disp = (int)cpu_read_data((ADDRESS)disasm_pc,32);
	inc_disasm_pc(4);
	append_register(dstr,opword,ADDRESS_REGISTER,2);
	append_string(dstr,",#");
	sprintf(buffer,"%d",disp);
	append_string(dstr,buffer);
}

static void
func44 (int index, WORD opword)
{
	/* BKPT */
	int data;

	(void)index;

	append_string(dstr,"#");
	data = opword & 0x0007;
	append_value(dstr,data,8);
}

static void
func45 (int index, WORD opword)
{
	/* SBCD ABCD Dy,Dx */

	(void)index;

	append_register(dstr,opword,DATA_REGISTER,2);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,11);
}

static void
func46 (int index, WORD opword)
{
	/* SBCD ABCD -(Ay),-(Ax) */

	(void)index;

	append_string(dstr,"-(");
	append_register(dstr,opword,ADDRESS_REGISTER,2);
	append_string(dstr,"),-(");
	append_register(dstr,opword,ADDRESS_REGISTER,11);
	append_string(dstr,")");
}
#endif /* MCF5200 */

static void
func47 (int index, WORD opword)
{
	/* EOR SUB ADD OR */
	append_size(dstr,opword,7,TRUE);
	append_register(dstr,opword,DATA_REGISTER,11);
	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
}

#ifndef MCF5200
static void
func48 (int index, WORD opword)
{
	/* BFCHG BFCLR BFSET BFTST */
	int opword2;

	opword2 = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,"{");
	if (opword2 & 0x0800)
	{
		/* Do = 1 -- Dn */
		append_register(dstr,opword2,DATA_REGISTER,8);
	}
	else
	{
		append_value(dstr,((opword2 & 0x07C0) >> 6),SIZE_BYTE);
	}
	append_string(dstr,":");
	if (opword2 & 0x0020)
	{
		/* Dw = 1 -- Dn */
		append_register(dstr,opword2,DATA_REGISTER,2);
	}
	else
	{
		append_value(dstr,(opword2 & 0x001F),SIZE_BYTE);
	}
	append_string(dstr,"}");
}


static void
func49 (int index, WORD opword)
{
	/* BFEXTS BFEXTU */
	int opword2;

	opword2 = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,"{");
	if (opword2 & 0x0800)
	{
		/* Do = 1 -- Dn */
		append_register(dstr,opword2,DATA_REGISTER,8);
	}
	else
	{
		append_value(dstr,((opword2 & 0x07C0) >> 6),SIZE_BYTE);
	}
	append_string(dstr,":");
	if (opword2 & 0x0020)
	{
		/* Dw = 1 -- Dn */
		append_register(dstr,opword2,DATA_REGISTER,2);
	}
	else
	{
		append_value(dstr,(opword2 & 0x001F),SIZE_BYTE);
	}
	append_string(dstr,"},");
	append_register(dstr,opword2,DATA_REGISTER,14);
}

static void
func50 (int index, WORD opword)
{
	/* BFINS */
	int opword2;

	opword2 = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);

	append_register(dstr,opword2,DATA_REGISTER,14);
	append_string(dstr,",");
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,"{");
	if (opword2 & 0x0800)
	{
		/* Do = 1 -- Dn */
		append_register(dstr,opword2,DATA_REGISTER,8);
	}
	else
	{
		append_value(dstr,((opword2 & 0x07C0) >> 6),SIZE_BYTE);
	}
	append_string(dstr,":");
	if (opword2 & 0x0020)
	{
		/* Dw = 1 -- Dn */
		append_register(dstr,opword2,DATA_REGISTER,2);
	}
	else
	{
		append_value(dstr,(opword2 & 0x001F),SIZE_BYTE);
	}
	append_string(dstr,"}");
}

static void
func51 (int index, WORD opword)
{
	/* CHK */
	append_size2(dstr,opword,8,TRUE);
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,11);
}

static void
func52 (int index, WORD opword)
{
	/* CMPM */

	(void)index;

	append_size(dstr,opword,7,TRUE);
	append_string(dstr,"(");
	append_register(dstr,opword,ADDRESS_REGISTER,2);
	append_string(dstr,")+,(");
	append_register(dstr,opword,ADDRESS_REGISTER,11);
	append_string(dstr,")+");
}

static void
func53 (int index, WORD opword)
{
	/* DBcc */
	int disp, target;
#ifdef SYMBOL_TABLE
	char tstr[100];
#endif

	(void)index;

	disp = (int)((SWORD)cpu_read_data((ADDRESS)disasm_pc,16));
	target = (unsigned int)disasm_pc + disp;
	inc_disasm_pc(2);

	append_register(dstr,opword,DATA_REGISTER,2);
	append_string(dstr,",");
#ifdef SYMBOL_TABLE
	if (symtab_convert_address((ADDRESS)target,tstr))
	{
		append_string(dstr,tstr);
	}
	else
		append_value(dstr,target,SIZE_LONG);
#else
	append_value(dstr,target,SIZE_LONG);
#endif
}

static void
func54 (int index, WORD opword)
{
	/* DIVS.W DIVU.W */
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,11);
}

static void
func55 (int index, WORD opword)
{
	/* DIVS.L DIVU.L DIVSL.L DIVUL.L*/
	int opword2, same_reg;

	opword2 = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);

	same_reg = (((opword2 & 0x7000) >> 12) ==
				 (opword2 & 0x0007));

	if (opword2 & 0x0800)
	{
		/* DIVS */
		if ((opword2 & 0x0400) || same_reg)
		{
			append_instruction(dstr,"DIVS.L");
		}
		else
		{
			append_instruction(dstr,"DIVSL.L");
		}
	}
	else
	{
		/* DIVU */
		if ((opword2 & 0x0400) || same_reg)
		{
			append_instruction(dstr,"DIVU.L");
		}
		else
		{
			append_instruction(dstr,"DIVUL.L");
		}
	}
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");
	if (same_reg)
	{
		append_register(dstr,opword2,DATA_REGISTER,14);
	}
	else
	{
		append_register(dstr,opword2,DATA_REGISTER,2);
		append_string(dstr,":");
		append_register(dstr,opword2,DATA_REGISTER,14);
	}
}

static void
func56 (int index, WORD opword)
{
	/* EXG.L */

	(void)index;

	switch (opword & 0x00F8)
	{
		case 0x0040:
			append_register(dstr,opword,DATA_REGISTER,11);
			append_string(dstr,",");
			append_register(dstr,opword,DATA_REGISTER,2);
			break;
		case 0x0048:
			append_register(dstr,opword,ADDRESS_REGISTER,11);
			append_string(dstr,",");
			append_register(dstr,opword,ADDRESS_REGISTER,2);
			break;
		case 0x0088:
			append_register(dstr,opword,DATA_REGISTER,11);
			append_string(dstr,",");
			append_register(dstr,opword,ADDRESS_REGISTER,2);
			break;
		default:
			valid_instruction = FALSE;
	}
}

static void
func57 (int index, WORD opword)
{
	/* PACK UNPK -(Ax),-(Ay),#<adj> */
	int adj;

	(void)index;

	adj = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);

	append_string(dstr,"-(");
	append_register(dstr,opword,ADDRESS_REGISTER,2);
	append_string(dstr,"),-(");
	append_register(dstr,opword,ADDRESS_REGISTER,11);
	append_string(dstr,"),#");
	append_value(dstr,adj,SIZE_WORD);
}

static void
func58 (int index, WORD opword)
{
	/* PACK UNPK Dx,Dy,#<adj> */
	int adj;

	(void)index;

	adj = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);

	append_register(dstr,opword,DATA_REGISTER,2);
	append_string(dstr,",");
	append_register(dstr,opword,DATA_REGISTER,11);
	append_string(dstr,",#");
	append_value(dstr,adj,SIZE_WORD);
}

static void
func59 (int index, WORD opword)
{
	/* TRAPcc .W .L #<data> */
	int data;

	(void)index;

	if (opword & 0x0001)
	{
		/* long */
		data = cpu_read_data((ADDRESS)disasm_pc,SIZE_LONG);
		inc_disasm_pc(4);
		append_size(dstr,0x0002,1,TRUE);
		append_string(dstr,"#");
		append_value(dstr,data,SIZE_LONG);
	}
	else
	{
		/* word */
		data = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
		inc_disasm_pc(2);
		append_size(dstr,0x0002,2,TRUE);
		append_string(dstr,"#");
		append_value(dstr,data,SIZE_WORD);
	}
}

static void
func60 (int index, WORD opword)
{
	/* MOVE to USP */

	(void)index;

	append_register(dstr,opword,ADDRESS_REGISTER,2);
	append_string(dstr,",USP");
}

static void
func61 (int index, WORD opword)
{
	/* MOVE from USP */

	(void)index;

	append_string(dstr,"USP,");
	append_register(dstr,opword,ADDRESS_REGISTER,2);
}
#endif /* MCF5200 */

#ifdef CPU32
static void
func62 (int index, WORD opword)
{
	/* TBL */
	int opword2;

	opword2 = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);

	switch (opword2 & 0x0C00)
	{
		case 0x0000:
			append_instruction(dstr,"TBLU");
			break;
		case 0x0400:
			append_instruction(dstr,"TBLUN");
			break;
		case 0x0800:
			append_instruction(dstr,"TBLS");
			break;
		case 0x0C00:
			append_instruction(dstr,"TBLSN");
			break;
	}
	append_size(dstr,opword2,7,TRUE);

	if ((opword & 0x0038) == 0)
	{
		/* TBL Dym:Dyn,Dx */
		append_register(dstr,opword,DATA_REGISTER,2);
		append_string(dstr,":");
		append_register(dstr,opword2,DATA_REGISTER,2);
		append_string(dstr,",");
		append_register(dstr,opword2,DATA_REGISTER,14);
	}
	else
	{
		/* TBL <ea>,Dx */
		append_ea(dstr,opword,5,isa[index].ea_mask);
		append_string(dstr,",");
		append_register(dstr,opword2,DATA_REGISTER,14);
	}
}

static void
func63 (int index, WORD opword)
{
	/* LPSTOP */
	int opword2, data;

	opword2 = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);

	if (opword == 0x01C0)
	{
		data = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
		inc_disasm_pc(2);

		append_string(dstr,"#");
		append_value(dstr,data,SIZE_WORD);
	}
	else
	{
		/* this opcode matches TBL as well, check it */
		disasm_pc = disasm_pc - 2;
		func62(index,opword);
	}
}
#endif /* CPU32 */

/*************************************************************/
#ifdef MCF5200M

static void
mac1 (int index, WORD opword)
{
	/* MAC.W Rw,Rx */
	/* MAC.L Rw,Rx */
	/* MSAC.W Rw,Rx */
	/* MSAC.L Rw,Rx */
	int opword2;

	opword2 = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);

	if (opword2 & 0x0100)
	{
		if (opword2 & 0x0100)
			append_instruction(dstr,"MSAC.L");
		else
			append_instruction(dstr,"MSAC.W");
	}
	else
	{
		if (opword2 & 0x0100)
			append_instruction(dstr,"MAC.L");
		else
			append_instruction(dstr,"MAC.W");
	}

	append_register(dstr,opword,6,11);	/* Rw */
	if (opword2 & 0x0080)
		append_string(dstr,".U,");
	else
		append_string(dstr,".L,");

	append_register(dstr,opword,3,2);	/* Rx */
	if (opword2 & 0x0040)
		append_string(dstr,".U");
	else
		append_string(dstr,".L");

	if ((opword2 & 0x0600) == 0x0200)
		append_string(dstr,",<<");
	else if ((opword2 & 0x0600) == 0x0600)
		append_string(dstr,",>>");
}

static void
mac2 (int index, WORD opword)
{
	/* MACL.W Rw,Rx */
	/* MACL.L Rw,Rx */
	/* MSACL.W Rw,Rx */
	/* MSACL.L Rw,Rx */
	int opword2;

	opword2 = cpu_read_data((ADDRESS)disasm_pc,SIZE_WORD);
	inc_disasm_pc(2);

	if (opword2 & 0x0100)
	{
		if (opword2 & 0x0100)
			append_instruction(dstr,"MSACL.L");
		else
			append_instruction(dstr,"MSACL.W");
	}
	else
	{
		if (opword2 & 0x0100)
			append_instruction(dstr,"MACL.L");
		else
			append_instruction(dstr,"MACL.W");
	}

	append_register(dstr,opword2,15,14);	/* Rw */
	if (opword2 & 0x0080)
		append_string(dstr,".U,");
	else
		append_string(dstr,".L,");

	append_register(dstr,opword2,3,2);	/* Rx */
	if (opword2 & 0x0040)
		append_string(dstr,".U,");
	else
		append_string(dstr,".L,");

	if ((opword2 & 0x0600) == 0x0200)
		append_string(dstr,"<<,");
	else if ((opword2 & 0x0600) == 0x0600)
		append_string(dstr,">>,");

	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",");

	append_register(dstr,opword,6,11);	/* Ry */
}

static void
mac3 (int index, WORD opword)
{
	/* MOVE.L <ea>,ACC */
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",ACC,");
}

static void
mac4 (int index, WORD opword)
{
	/* MOVE.L <ea>,MACSR */
	append_ea(dstr,opword,5,isa[index].ea_mask);
	append_string(dstr,",MACSR,");
}

static void
mac5 (int index, WORD opword)
{
	/* MOVE.L ACC,Rx */
	append_string(dstr,"ACC,");
	append_register(dstr,opword,3,2);
}

static void
mac6 (int index, WORD opword)
{
	/* MOVE.L MACSR,Rx */
	append_string(dstr,"MACSR,");
	append_register(dstr,opword,3,2);
}

static void
mac7 (int index, WORD opword)
{
	/* MOVE.L MACSR,CCR */
	append_string(dstr,"MACSR,CCR");
}

#endif /* MCF5200M */
/*************************************************************/

static const INSTRENTRY isa[] = {

/* LINE 0000 */
#ifndef MCF5200
{0xFFFF,0x003C,"ORI.B",	EA_NONE,func2},	/* ORI.B #<data>,CCR */
{0xFFFF,0x007C,"ORI.W",	EA_NONE,func3},	/* ORI.W #<data>,SR */
{0xFFFF,0x023C,"ANDI.B",EA_NONE,func2},	/* ANDI.B #<data>,CCR */
{0xFFFF,0x027C,"ANDI.B",EA_NONE,func3},	/* ANDI.W #<data>,SR */
{0xFFFF,0x0A3C,"EORI.B",EA_NONE,func2},	/* EORI.B #<data>,CCR */
{0xFFFF,0x0A7C,"EORI.W",EA_NONE,func3},	/* EORI.W #<data>,SR */
{0xFFFF,0x0CFC,"CAS2",	EA_NONE,func40}, /*CAS2.W Dc1:Dc2,Du1,Du2,(Rn1):(Rn2)*/
{0xFFFF,0x0EFC,"CAS2",	EA_NONE,func40}, /*CAS2.L Dc1:Dc2,Du1,Du2,(Rn1):(Rn2)*/
{0xFFF0,0x06C0,"RTM",	EA_NONE,func4},	/* RTM Rn */
#endif
#ifndef MCF5200
{0xFFC0,0x0000,"ORI",	EA_DATALT1,func6},	/* ORI.B #<data>,<ea> */
{0xFFC0,0x0040,"ORI",	EA_DATALT1,func6},	/* ORI.W #<data>,<ea> */
{0xFFC0,0x0080,"ORI",	EA_DATALT1,func6},	/* ORI.L #<data>,<ea> */
#else
{0xFFF8,0x0080,"ORI",	DRD,func6},	/* ORI.L #<data>,Dn */
#endif
#ifndef MCF5200
{0xFFC0,0x0200,"ANDI",	EA_DATALT1,func6},	/* ANDI.B #<data>,<ea> */
{0xFFC0,0x0240,"ANDI",	EA_DATALT1,func6},	/* ANDI.W #<data>,<ea> */
{0xFFC0,0x0280,"ANDI",	EA_DATALT1,func6},	/* ANDI.L #<data>,<ea> */
#else
{0xFFF8,0x0280,"ANDI",	DRD,func6},	/* ANDI.L #<data>,Dn */
#endif
#ifndef MCF5200
{0xFFC0,0x0400,"SUBI",	EA_DATALT1,func6},	/* SUBI.B #<data>,<ea> */
{0xFFC0,0x0440,"SUBI",	EA_DATALT1,func6},	/* SUBI.W #<data>,<ea> */
{0xFFC0,0x0480,"SUBI",	EA_DATALT1,func6},	/* SUBI.L #<data>,<ea> */
#else
{0xFFF8,0x0480,"SUBI",	DRD,func6},	/* SUBI.L #<data>,Dn */
#endif
#ifndef MCF5200
{0xFFC0,0x0600,"ADDI",	EA_DATALT1,func6},	/* ADDI.B #<data>,<ea> */
{0xFFC0,0x0640,"ADDI",	EA_DATALT1,func6},	/* ADDI.W #<data>,<ea> */
{0xFFC0,0x0680,"ADDI",	EA_DATALT1,func6},	/* ADDI.L #<data>,<ea> */
#else
{0xFFF8,0x0680,"ADDI",	DRD,func6},	/* ADDI.L #<data>,Dn */
#endif
#ifndef MCF5200
{0xFFC0,0x06C0,"CALLM",	EA_CONTROL,func5},	/* CALLM #<data>,<ea> */
{0xFFC0,0x00C0,"CMP2",	EA_CONTROL,func19},	/* CMP2.B <ea>,Rn */
{0xFFC0,0x02C0,"CMP2",	EA_CONTROL,func19},	/* CMP2.W <ea>,Rn */
{0xFFC0,0x04C0,"CMP2",	EA_CONTROL,func19},	/* CMP2.L <ea>,Rn */
#endif
#ifndef MCF5200
{0xFFC0,0x0A00,"EORI",	EA_DATALT1,func6},	/* EORI.B #<data>,<ea> */
{0xFFC0,0x0A40,"EORI",	EA_DATALT1,func6},	/* EORI.W #<data>,<ea> */
{0xFFC0,0x0A80,"EORI",	EA_DATALT1,func6},	/* EORI.L #<data>,<ea> */
#else
{0xFFF8,0x0A80,"EORI",	DRD,func6},	/* EORI.L #<data>,Dn */
#endif
#ifndef MCF5200
{0xFFC0,0x0C00,"CMPI",	EA_DATA1,func6},	/* CMPI.B #<data>,<ea> */
{0xFFC0,0x0C40,"CMPI",	EA_DATA1,func6},	/* CMPI.W #<data>,<ea> */
{0xFFC0,0x0C80,"CMPI",	EA_DATA1,func6},	/* CMPI.L #<data>,<ea> */
#else
{0xFFF8,0x0C80,"CMPI",	DRD,func6},	/* CMPI.L #<data>,Dn */
#endif
#ifndef MCF5200
{0xFFC0,0x0800,"BTST",	EA_DATA1,func1},	/* BTST #<data>,<ea> */
{0xFFC0,0x0840,"BCHG",	EA_DATALT1,func1},	/* BCHG #<data>,<ea> */
{0xFFC0,0x08C0,"BSET",	EA_DATALT1,func1},	/* BSET #<data>,<ea> */
{0xFFC0,0x0880,"BCLR",	EA_DATALT1,func1},	/* BCLR #<data>,<ea> */
#else
{0xFFC0,0x0800,"BTST",	EA_DATA1,func1},	/* BTST #<data>,<ea> */
{0xFFC0,0x0840,"BCHG",	EA_DATA1,func1},	/* BCHG #<data>,<ea> */
{0xFFC0,0x08C0,"BSET",	EA_DATA1,func1},	/* BSET #<data>,<ea> */
{0xFFC0,0x0880,"BCLR",	EA_DATA1,func1},	/* BCLR #<data>,<ea> */
#endif
#ifndef MCF5200
{0xFFC0,0x0E00,"MOVES",	EA_MEMALT1,func30},
{0xFFC0,0x0E40,"MOVES",	EA_MEMALT1,func30},
{0xFFC0,0x0E80,"MOVES",	EA_MEMALT1,func30},
{0xFFC0,0x0AC0,"CAS",	EA_MEMALT1,func41},	/* CAS.B Dc,Du,<ea> */
{0xFFC0,0x0CC0,"CAS",	EA_MEMALT1,func41},	/* CAS.W Dc,Du,<ea> */
{0xFFC0,0x0EC0,"CAS",	EA_MEMALT1,func41},	/* CAS.L Dc,Du,<ea> */
{0xF1F8,0x0108,"MOVEP",	EA_NONE,func42},	/* MOVEP.W (d16,Ay),Dx */
{0xF1F8,0x0148,"MOVEP",	EA_NONE,func42},	/* MOVEP.L (d16,Ay),Dx */
{0xF1F8,0x0188,"MOVEP",	EA_NONE,func42},	/* MOVEP.W Dx,(d16,Ay) */
{0xF1F8,0x01C8,"MOVEP",	EA_NONE,func42},	/* MOVEP.L Dx,(d16,Ay) */
#endif
{0xF1C0,0x0100,"BTST",	EA_DATA,func7},		/* BTST Dn,<ea> */
{0xF1C0,0x0140,"BCHG",	EA_DATALT1,func7},	/* BCHG Dn,<ea> */
{0xF1C0,0x01C0,"BSET",	EA_DATALT1,func7},	/* BSET Dn,<ea> */
{0xF1C0,0x0180,"BCLR",	EA_DATALT1,func7},	/* BCLR Dn,<ea> */





/* LINE 0001 */
#ifndef MCF5200
{0xF000,0x1000,"MOVE",	EA_DATALT1 /* dest <ea> */,func8},	/* MOVE.B */
#else
{0xF000,0x1000,"MOVE",	EA_ALL /* dest <ea> */,func8},	/* MOVE.B */
#endif





/* LINE 0010 */
{0xF1C0,0x2040,"MOVEA",	EA_ALL,func26},	/* MOVEA.L */
#ifndef MCF5200
{0xF000,0x2000,"MOVE",	EA_DATALT1 /* dest <ea> */,func8},	/* MOVE.L */
#else
{0xF000,0x2000,"MOVE",	EA_ALL /* dest <ea> */,func8},	/* MOVE.L */
#endif





/* LINE 0011 */
{0xF1C0,0x3040,"MOVEA",	EA_ALL,func26},	/* MOVEA.W */
#ifndef MCF5200
{0xF000,0x3000,"MOVE",	EA_DATALT1 /* dest <ea> */,func8},	/* MOVE.W */
#else
{0xF000,0x3000,"MOVE",	EA_ALL /* dest <ea> */,func8},	/* MOVE.W */
#endif





/* LINE 0100 */
{0xFFFF,0x4AFC,"ILLEGAL",EA_NONE,func17},
{0xFFFF,0x4E71,"NOP",	EA_NONE,func17},
{0xFFFF,0x4AC8,"HALT",	EA_NONE,func17},
{0xFFFF,0x4BCC,"PULSE",	EA_NONE,func17},
#ifndef MCF5200
{0xFFFF,0x4E74,"RTD",	EA_NONE,func23},
{0xFFFF,0x4E77,"RTR",	EA_NONE,func17},
#endif
{0xFFFF,0x4E75,"RTS",	EA_NONE,func17},
#ifndef MCF5200
{0xFFFF,0x4E76,"TRAPV",	EA_NONE,func17},
{0xFFFF,0x4AFA,"BGND",	EA_NONE,func17},
{0xFFFF,0x4E70,"RESET",	EA_NONE,func17},
#endif
{0xFFFF,0x4E72,"STOP",	EA_NONE,func23},
{0xFFFF,0x4E73,"RTE",	EA_NONE,func17},
{0xFFFF,0x4E7A,"MOVEC",	EA_NONE,func36},
{0xFFFF,0x4E7B,"MOVEC",	EA_NONE,func36},
{0xFFF8,0x4880,"EXT.W",	EA_NONE,func13},	/* EXT.W Dn */
{0xFFF8,0x48C0,"EXT.L",	EA_NONE,func13},	/* EXT.L Dn */
{0xFFF8,0x49C0,"EXTB.L",EA_NONE,func13},	/* EXTB.L Dn */
#ifndef MCF5200
{0xFFF8,0x4808,"LINK.L",EA_NONE,func43},	/* LINK.L An,#<disp>*/
#endif
{0xFFF8,0x4840,"SWAP",	EA_NONE,func13},	/* SWAP.W Dn */
#ifndef MCF5200
{0xFFF8,0x4848,"BKPT",	EA_NONE,func44},	/* BKPT #<data> */
#endif
{0xFFF8,0x4E58,"UNLK",	EA_NONE,func21},	/* UNLK An */
{0xFFF8,0x4E50,"LINK",	EA_NONE,func16},	/* LINK.W An,#<disp>*/
#ifndef MCF5200
{0xFFF8,0x4E60,"MOVE",	EA_NONE,func60},	/* MOVE An,USP */
{0xFFF8,0x4E68,"MOVE",	EA_NONE,func61},	/* MOVE USP,An */
#endif
#ifndef MCF5200
{0xFFC0,0x40C0,"MOVE",	EA_DATALT1,func9},		/* MOVE.W SR,<ea> */
{0xFFC0,0x42C0,"MOVE",	EA_DATALT2,func11},		/* MOVE.B CCR,<ea> */
#else
{0xFFF8,0x40C0,"MOVE",	DRD,func9},			/* MOVE.W SR,<ea> */
{0xFFF8,0x42C0,"MOVE",	DRD,func11},		/* MOVE.B CCR,<ea> */
#endif
#ifndef MCF5200
{0xFFC0,0x4000,"NEGX",	EA_DATALT1,func15},	/* NEGX.B <ea> */
{0xFFC0,0x4040,"NEGX",	EA_DATALT1,func15},	/* NEGX.W <ea> */
{0xFFC0,0x4080,"NEGX",	EA_DATALT1,func15},	/* NEGX.L <ea> */
#else
{0xFFF8,0x4080,"NEGX",	DRD,func15},	/* NEGX.L Dn */
#endif
#ifndef MCF5200
{0xFFC0,0x4400,"NEG",	EA_DATALT1,func15},		/* NEG.B <ea> */
{0xFFC0,0x4440,"NEG",	EA_DATALT1,func15},		/* NEG.W <ea> */
{0xFFC0,0x4480,"NEG",	EA_DATALT1,func15},		/* NEG.L <ea> */
#else
{0xFFF8,0x4480,"NEG",	DRD,func15},		/* NEG.L Dn */
#endif
#ifndef MCF5200
{0xFFC0,0x4600,"NOT",	EA_DATALT1,func15},		/* NOT.B <ea> */
{0xFFC0,0x4640,"NOT",	EA_DATALT1,func15},		/* NOT.W <ea> */
{0xFFC0,0x4680,"NOT",	EA_DATALT1,func15},		/* NOT.L <ea> */
#else
{0xFFF8,0x4680,"NOT",	DRD,func15},		/* NOT.L Dn */
#endif
{0xFFF0,0x4E40,"TRAP",	EA_NONE,func22},	/* TRAP #<vector> */
#ifndef MCF5200
{0xFFC0,0x46C0,"MOVE",	EA_DATA,func10},		/* MOVE to SR */
#else
{0xFFC0,0x46C0,"MOVE",	(DRD | IM),func10},		/* MOVE to SR */
#endif
{0xFFC0,0x4200,"CLR",	EA_DATALT1,func15},		/* CLR.B <ea> */
{0xFFC0,0x4240,"CLR",	EA_DATALT1,func15},		/* CLR.W <ea> */
{0xFFC0,0x4280,"CLR",	EA_DATALT1,func15},		/* CLR.L <ea> */
#ifndef MCF5200
{0xFFC0,0x44C0,"MOVE",	EA_DATA,func12},		/* MOVE.B <ea>,CCR */
#else
{0xFFC0,0x44C0,"MOVE",	(DRD | IM),func12},		/* MOVE.B <ea>,CCR */
#endif
#ifndef MCF5200
{0xFFC0,0x4800,"NBCD",	EA_DATALT1,func18},		/* NBCD.B <ea> */
#endif
{0xFFC0,0x4840,"PEA",	EA_CONTROL,func18},		/* PEA <ea> */
#ifndef MCF5200
{0xFFC0,0x4AC0,"TAS",	EA_DATALT1,func18},	/* TAS.B <ea> */
#endif
{0xFFC0,0x4A00,"TST"	,EA_ALL,func15},		/* TST.B <ea> */
{0xFFC0,0x4A40,"TST"	,EA_ALL,func15},		/* TST.W <ea> */
{0xFFC0,0x4A80,"TST"	,EA_ALL,func15},		/* TST.L <ea> */
#ifndef MCF5200
{0xFFC0,0x4C00,"MULS"	,EA_DATA,func39},		/* MULS.L */
#else
{0xFFC0,0x4C00,"MULS"	,(DRD|ARI|ARIPO|ARIPR|ARID|ARII8),func39}, /* MULS.L */
#endif
#if 0
{0xFFC0,0x4C00,"MULU"	,EA_DATA,func39},		/* MULU.L */
#endif
#ifndef MCF5200
{0xFFC0,0x4C40,"DIVS"	,EA_DATA,func55},		/* DIVS.L */
#if 0
{0xFFC0,0x4C40,"DIVU"	,EA_DATA,func55},		/* DIVU.L */
#endif
#endif
{0xFFC0,0x4EC0,"JMP",	EA_CONTROL,func18},		/* JMP <ea>*/
{0xFFC0,0x4E80,"JSR",	EA_CONTROL,func18},		/* JSR <ea>*/
#ifndef MCF5200
{0xFFC0,0x4880,"MOVEM",	EA_CTRALT3,func27},	/* MOVEM.W <list>,<ea> */
{0xFFC0,0x48C0,"MOVEM",	EA_CTRALT3,func27},	/* MOVEM.L <list>,<ea> */
#else
{0xFFC0,0x48C0,"MOVEM",	(ARI | ARID),func27},	/* MOVEM.L <list>,<ea> */
#endif
#ifndef MCF5200
{0xFFC0,0x4C80,"MOVEM",	EA_CTRL1,func27},	/* MOVEM.W <ea>,<list> */
{0xFFC0,0x4CC0,"MOVEM",	EA_CTRL1,func27},	/* MOVEM.L <ea>,<list> */
#else
{0xFFC0,0x4CC0,"MOVEM",	(ARI | ARID),func27},	/* MOVEM.L <ea>,<list> */
#endif
{0xF1C0,0x41C0,"LEA",	EA_CONTROL,func32},		/* LEA <ea>*/
#ifndef MCF5200
{0xF1C0,0x4180,"CHK",	EA_DATA,func51},		/* CHK.W <ea>,Dn */
{0xF1C0,0x4100,"CHK",	EA_DATA,func51},		/* CHK.L <ea>,Dn */
#endif




/* LINE 0101 */
#ifndef MCF5200
{0xFFFF,0x50FC,"TRAPT"	,EA_NONE,func17},		/* TRAPcc */
{0xFFFF,0x51FC,"TRAPF"	,EA_NONE,func17},
{0xFFFF,0x52FC,"TRAPHI"	,EA_NONE,func17},
{0xFFFF,0x53FC,"TRAPLS"	,EA_NONE,func17},
{0xFFFF,0x54FC,"TRAPCC"	,EA_NONE,func17},
{0xFFFF,0x55FC,"TRAPCS"	,EA_NONE,func17},
{0xFFFF,0x56FC,"TRAPNE"	,EA_NONE,func17},
{0xFFFF,0x57FC,"TRAPEQ"	,EA_NONE,func17},
{0xFFFF,0x58FC,"TRAPVC"	,EA_NONE,func17},
{0xFFFF,0x59FC,"TRAPVS"	,EA_NONE,func17},
{0xFFFF,0x5AFC,"TRAPPL"	,EA_NONE,func17},
{0xFFFF,0x5BFC,"TRAPMI"	,EA_NONE,func17},
{0xFFFF,0x5CFC,"TRAPGE"	,EA_NONE,func17},
{0xFFFF,0x5DFC,"TRAPLT"	,EA_NONE,func17},
{0xFFFF,0x5EFC,"TRAPGT"	,EA_NONE,func17},
{0xFFFF,0x5FFC,"TRAPLE"	,EA_NONE,func17},
{0xFFFF,0x50FA,"TRAPT"	,EA_NONE,func59},		/* TRAPcc.W #<data> */
{0xFFFF,0x51FA,"TRAPF"	,EA_NONE,func59},
{0xFFFF,0x52FA,"TRAPHI"	,EA_NONE,func59},
{0xFFFF,0x53FA,"TRAPLS"	,EA_NONE,func59},
{0xFFFF,0x54FA,"TRAPCC"	,EA_NONE,func59},
{0xFFFF,0x55FA,"TRAPCS"	,EA_NONE,func59},
{0xFFFF,0x56FA,"TRAPNE"	,EA_NONE,func59},
{0xFFFF,0x57FA,"TRAPEQ"	,EA_NONE,func59},
{0xFFFF,0x58FA,"TRAPVC"	,EA_NONE,func59},
{0xFFFF,0x59FA,"TRAPVS"	,EA_NONE,func59},
{0xFFFF,0x5AFA,"TRAPPL"	,EA_NONE,func59},
{0xFFFF,0x5BFA,"TRAPMI"	,EA_NONE,func59},
{0xFFFF,0x5CFA,"TRAPGE"	,EA_NONE,func59},
{0xFFFF,0x5DFA,"TRAPLT"	,EA_NONE,func59},
{0xFFFF,0x5EFA,"TRAPGT"	,EA_NONE,func59},
{0xFFFF,0x5FFA,"TRAPLE"	,EA_NONE,func59},
{0xFFFF,0x50FB,"TRAPT"	,EA_NONE,func59},		/* TRAPcc.L #<data> */
{0xFFFF,0x51FB,"TRAPF"	,EA_NONE,func59},
{0xFFFF,0x52FB,"TRAPHI"	,EA_NONE,func59},
{0xFFFF,0x53FB,"TRAPLS"	,EA_NONE,func59},
{0xFFFF,0x54FB,"TRAPCC"	,EA_NONE,func59},
{0xFFFF,0x55FB,"TRAPCS"	,EA_NONE,func59},
{0xFFFF,0x56FB,"TRAPNE"	,EA_NONE,func59},
{0xFFFF,0x57FB,"TRAPEQ"	,EA_NONE,func59},
{0xFFFF,0x58FB,"TRAPVC"	,EA_NONE,func59},
{0xFFFF,0x59FB,"TRAPVS"	,EA_NONE,func59},
{0xFFFF,0x5AFB,"TRAPPL"	,EA_NONE,func59},
{0xFFFF,0x5BFB,"TRAPMI"	,EA_NONE,func59},
{0xFFFF,0x5CFB,"TRAPGE"	,EA_NONE,func59},
{0xFFFF,0x5DFB,"TRAPLT"	,EA_NONE,func59},
{0xFFFF,0x5EFB,"TRAPGT"	,EA_NONE,func59},
{0xFFFF,0x5FFB,"TRAPLE"	,EA_NONE,func59},
{0xFFF8,0x50C8,"DBT"	,EA_NONE,func53},
{0xFFF8,0x51C8,"DBF"	,EA_NONE,func53},
{0xFFF8,0x52C8,"DBHI"	,EA_NONE,func53},
{0xFFF8,0x53C8,"DBLS"	,EA_NONE,func53},
{0xFFF8,0x54C8,"DBCC"	,EA_NONE,func53},
{0xFFF8,0x55C8,"DBCS"	,EA_NONE,func53},
{0xFFF8,0x56C8,"DBNE"	,EA_NONE,func53},
{0xFFF8,0x57C8,"DBEQ"	,EA_NONE,func53},
{0xFFF8,0x58C8,"DBVC"	,EA_NONE,func53},
{0xFFF8,0x59C8,"DBVS"	,EA_NONE,func53},
{0xFFF8,0x5AC8,"DBPL"	,EA_NONE,func53},
{0xFFF8,0x5BC8,"DBMI"	,EA_NONE,func53},
{0xFFF8,0x5CC8,"DBGE"	,EA_NONE,func53},
{0xFFF8,0x5DC8,"DBLT"	,EA_NONE,func53},
{0xFFF8,0x5EC8,"DBGT"	,EA_NONE,func53},
{0xFFF8,0x5FC8,"DBLE"	,EA_NONE,func53},
#endif
#ifndef MCF5200
{0xFFC0,0x50C0,"ST",	EA_DATALT1,func18},
{0xFFC0,0x51C0,"SF",	EA_DATALT1,func18},
{0xFFC0,0x52C0,"SHI",	EA_DATALT1,func18},
{0xFFC0,0x53C0,"SLS",	EA_DATALT1,func18},
{0xFFC0,0x54C0,"SCC",	EA_DATALT1,func18},
{0xFFC0,0x55C0,"SCS",	EA_DATALT1,func18},
{0xFFC0,0x56C0,"SNE",	EA_DATALT1,func18},
{0xFFC0,0x57C0,"SEQ",	EA_DATALT1,func18},
{0xFFC0,0x58C0,"SVC",	EA_DATALT1,func18},
{0xFFC0,0x59C0,"SVS",	EA_DATALT1,func18},
{0xFFC0,0x5AC0,"SPL",	EA_DATALT1,func18},
{0xFFC0,0x5BC0,"SMI",	EA_DATALT1,func18},
{0xFFC0,0x5CC0,"SGE",	EA_DATALT1,func18},
{0xFFC0,0x5DC0,"SLT",	EA_DATALT1,func18},
{0xFFC0,0x5EC0,"SGT",	EA_DATALT1,func18},
{0xFFC0,0x5FC0,"SLE",	EA_DATALT1,func18},
#else
{0xFFC0,0x50C0,"ST",	DRD,func18},
{0xFFC0,0x51C0,"SF",	DRD,func18},
{0xFFC0,0x52C0,"SHI",	DRD,func18},
{0xFFC0,0x53C0,"SLS",	DRD,func18},
{0xFFC0,0x54C0,"SCC",	DRD,func18},
{0xFFC0,0x55C0,"SCS",	DRD,func18},
{0xFFC0,0x56C0,"SNE",	DRD,func18},
{0xFFC0,0x57C0,"SEQ",	DRD,func18},
{0xFFC0,0x58C0,"SVC",	DRD,func18},
{0xFFC0,0x59C0,"SVS",	DRD,func18},
{0xFFC0,0x5AC0,"SPL",	DRD,func18},
{0xFFC0,0x5BC0,"SMI",	DRD,func18},
{0xFFC0,0x5CC0,"SGE",	DRD,func18},
{0xFFC0,0x5DC0,"SLT",	DRD,func18},
{0xFFC0,0x5EC0,"SGT",	DRD,func18},
{0xFFC0,0x5FC0,"SLE",	DRD,func18},
#endif
#ifndef MCF5200
{0xF1C0,0x5000,"ADDQ",	EA_ALTER1,func29},	/* ADDQ.B #<data>,<ea> */
{0xF1C0,0x5040,"ADDQ",	EA_ALTER1,func29},	/* ADDQ.W #<data>,<ea> */
#endif
{0xF1C0,0x5080,"ADDQ",	EA_ALTER1,func29},	/* ADDQ.L #<data>,<ea> */
#ifndef MCF5200
{0xF1C0,0x5100,"SUBQ",	EA_ALTER1,func29},	/* SUBQ.B #<data>,<ea> */
{0xF1C0,0x5140,"SUBQ",	EA_ALTER1,func29},	/* SUBQ.W #<data>,<ea> */
#endif
{0xF1C0,0x5180,"SUBQ",	EA_ALTER1,func29},	/* SUBQ.L #<data>,<ea> */





/* LINE 0110 */
{0xFF00,0x6000,"BRA"	,EA_NONE,func25},
{0xFF00,0x6100,"BSR"	,EA_NONE,func25},
{0xFF00,0x6200,"BHI"	,EA_NONE,func25},
{0xFF00,0x6300,"BLS"	,EA_NONE,func25},
{0xFF00,0x6400,"BCC"	,EA_NONE,func25},
{0xFF00,0x6500,"BCS"	,EA_NONE,func25},
{0xFF00,0x6600,"BNE"	,EA_NONE,func25},
{0xFF00,0x6700,"BEQ"	,EA_NONE,func25},
{0xFF00,0x6800,"BVC"	,EA_NONE,func25},
{0xFF00,0x6900,"BVS"	,EA_NONE,func25},
{0xFF00,0x6A00,"BPL"	,EA_NONE,func25},
{0xFF00,0x6B00,"BMI"	,EA_NONE,func25},
{0xFF00,0x6C00,"BGE"	,EA_NONE,func25},
{0xFF00,0x6D00,"BLT"	,EA_NONE,func25},
{0xFF00,0x6E00,"BGT"	,EA_NONE,func25},
{0xFF00,0x6F00,"BLE"	,EA_NONE,func25},





/* LINE 0111 */
{0xF100,0x7000,"MOVEQ",	EA_NONE,func24},	/* MOVEQ.L #<data>,Dn */





/* LINE 1000 */
#ifndef MCF5200
{0xF1F8,0x8100,"SBCD",	EA_NONE,func45},	/* SBCD Dy,Dx */
{0xF1F8,0x8108,"SBCD",	EA_NONE,func46},	/* SBCD -(Ay),-(Ax) */
{0xF1F8,0x8140,"PACK",	EA_NONE,func58},	/* PACK Dx,Dy,#<adj> */
{0xF1F8,0x8148,"PACK",	EA_NONE,func57},	/* PACK -(Ax),-(Ay),#<adj> */
{0xF1F8,0x8180,"UNPK",	EA_NONE,func58},	/* UNPK Dx,Dy,#<adj> */
{0xF1F8,0x8188,"UNPK",	EA_NONE,func57},	/* UNPK -(Ax),-(Ay),#<adj> */
{0xF1C0,0x81C0,"DIVS.W",EA_DATA,func54},	/* DIVS.W <ea>,Dn */
{0xF1C0,0x80C0,"DIVU.W",EA_DATA,func54},	/* DIVU.W <ea>,Dn */
{0xF1C0,0x8000,"OR",	EA_DATA,func33},	/* OR.B <ea>,Dn */
{0xF1C0,0x8040,"OR",	EA_DATA,func33},	/* OR.W <ea>,Dn */
#endif
{0xF1C0,0x8080,"OR",	EA_DATA,func33},	/* OR.L <ea>,Dn */
#ifndef MCF5200
{0xF1C0,0x8100,"OR",	EA_MEMALT1,func31},	/* OR.B Dn,<ea> */
{0xF1C0,0x8140,"OR",	EA_MEMALT1,func31},	/* OR.W Dn,<ea> */
#endif
{0xF1C0,0x8180,"OR",	EA_MEMALT1,func31},	/* OR.L Dn,<ea> */






/* LINE 1001 */
#ifndef MCF5200
{0xF1F0,0x9100,"SUBX",	EA_NONE,func14},	/* SUBX.B */
{0xF1F0,0x9140,"SUBX",	EA_NONE,func14},	/* SUBX.W */
{0xF1F0,0x9180,"SUBX",	EA_NONE,func14},	/* SUBX.L */
#else
{0xF1F8,0x9180,"SUBX",	EA_NONE,func14},	/* SUBX.L */
#endif
#ifndef MCF5200
{0xF1C0,0x9000,"SUB",	EA_ALL,func33},	/* SUB.B <ea>,Dn */
{0xF1C0,0x9040,"SUB",	EA_ALL,func33},	/* SUB.W <ea>,Dn */
#endif
{0xF1C0,0x9080,"SUB",	EA_ALL,func33},	/* SUB.L <ea>,Dn */
#ifndef MCF5200
{0xF1C0,0x9100,"SUB",	EA_MEMALT1,func31},	/* SUB.B Dn,<ea> */
{0xF1C0,0x9140,"SUB",	EA_MEMALT1,func31},	/* SUB.W Dn,<ea> */
#endif
{0xF1C0,0x9180,"SUB",	EA_MEMALT1,func31},	/* SUB.L Dn,<ea> */
#ifndef MCF5200
{0xF1C0,0x90C0,"SUBA",	EA_ALL,func28},	/* SUBA.W <ea>,An */
#endif
{0xF1C0,0x91C0,"SUBA",	EA_ALL,func28},	/* SUBA.L <ea>,An */


/* Line 1010 */
#ifdef MCF5200M
{0xF1B0,0xA000,"MAC"	,EA_NONE,mac1},	/* MAC/MSAC */
{0xF180,0xA080,"MACL"	,EA_MAC1,mac2},	/* MACL/MSACL */
{0xFFC0,0xA100,"MOVE.L"	,EA_MAC2,mac3},	/* MOVE.L <ea>,ACC */
{0xFFC0,0xA900,"MOVE.L"	,EA_MAC2,mac4},	/* MOVE.L <ea>,MACSR */
{0xFFF0,0xA180,"MOVE.L"	,EA_NONE,mac5},	/* MOVE.L ACC,Rx */
{0xFFF0,0xA980,"MOVE.L"	,EA_NONE,mac6},	/* MOVE.L MACSR,Rx */
{0xFFFF,0xA9C0,"MOVE.L"	,EA_NONE,mac7},	/* MOVE.L MACSR,CCR */
#endif



/* LINE 1011 */
#ifndef MCF5200
{0xF1F8,0xB108,"CMPM"	,EA_NONE,func52},	/* CMPM.B (Ay)+,(Ax)+ */
{0xF1F8,0xB148,"CMPM"	,EA_NONE,func52},	/* CMPM.W (Ay)+,(Ax)+ */
{0xF1F8,0xB188,"CMPM"	,EA_NONE,func52},	/* CMPM.L (Ay)+,(Ax)+ */
#endif
#ifndef MCF5200
{0xF1C0,0xB000,"CMP"	,EA_ALL,func33},	/* CMP.B <ea>,Dn */
{0xF1C0,0xB040,"CMP"	,EA_ALL,func33},	/* CMP.W <ea>,Dn */
#endif
{0xF1C0,0xB080,"CMP"	,EA_ALL,func33},	/* CMP.L <ea>,Dn */
#ifndef MCF5200
{0xF1C0,0xB0C0,"CMPA"	,EA_ALL,func28},	/* CMPA.W <ea>,Dn */
#endif
{0xF1C0,0xB1C0,"CMPA"	,EA_ALL,func28},	/* CMPA.L <ea>,Dn */
#ifndef MCF5200
{0xF1C0,0xB100,"EOR"	,EA_DATALT1,func31},	/* EOR.B Dn,<ea> */
{0xF1C0,0xB140,"EOR"	,EA_DATALT1,func31},	/* EOR.W Dn,<ea> */
#endif
{0xF1C0,0xB180,"EOR"	,EA_DATALT1,func31},	/* EOR.L Dn,<ea> */





/* LINE 1100 */
#ifndef MCF5200
{0xF1F8,0xC140,"EXG.L",	EA_NONE,func56},	/* EXG.L Dx,Dy */
{0xF1F8,0xC148,"EXG.L",	EA_NONE,func56},	/* EXG.L Ax,Ay */
{0xF1F8,0xC188,"EXG.L",	EA_NONE,func56},	/* EXG.L Dx,Ay */
{0xF1F8,0xC100,"ABCD",	EA_NONE,func45},	/* ABCD Dy,Dx */
{0xF1F8,0xC108,"ABCD",	EA_NONE,func46},	/* ABCD -(Ay),-(Ax) */
#endif
{0xF1C0,0xC0C0,"MULU.W",EA_DATA,func38},	/* MULU.W <ea>,Dn */
{0xF1C0,0xC1C0,"MULS.W",EA_DATA,func38},	/* MULS.W <ea>,Dn */
#ifndef MCF5200
{0xF1C0,0xC000,"AND",	EA_DATA,func33},	/* AND.B <ea>,Dn */
{0xF1C0,0xC040,"AND",	EA_DATA,func33},	/* AND.W <ea>,Dn */
#endif
{0xF1C0,0xC080,"AND",	EA_DATA,func33},	/* AND.L <ea>,Dn */
#ifndef MCF5200
{0xF1C0,0xC100,"AND",	EA_MEMALT1,func31},	/* AND.B Dn,<ea> */
{0xF1C0,0xC140,"AND",	EA_MEMALT1,func31},	/* AND.W Dn,<ea> */
#endif
{0xF1C0,0xC180,"AND",	EA_MEMALT1,func31},	/* AND.L Dn,<ea> */






/* LINE 1101 */
#ifndef MCF5200
{0xF1F0,0xD100,"ADDX",	EA_NONE,func14},	/* ADDX.B */
{0xF1F0,0xD140,"ADDX",	EA_NONE,func14},	/* ADDX.W */
#endif
{0xF1F0,0xD180,"ADDX",	EA_NONE,func14},	/* ADDX.L */
#ifndef MCF5200
{0xF1C0,0xD0C0,"ADDA",	EA_ALL,func28},	/* ADDA.W <ea>,An */
#endif
{0xF1C0,0xD1C0,"ADDA",	EA_ALL,func28},	/* ADDA.L <ea>,An */
#ifndef MCF5200
{0xF1C0,0xD000,"ADD",	EA_ALL,func33},	/* ADD.B <ea>,Dn */
{0xF1C0,0xD040,"ADD",	EA_ALL,func33},	/* ADD.W <ea>,Dn */
#endif
{0xF1C0,0xD080,"ADD",	EA_ALL,func33},	/* ADD.L <ea>,Dn */
#ifndef MCF5200
{0xF1C0,0xD100,"ADD",	EA_MEMALT1,func47},	/* ADD.B Dn,<ea> */
{0xF1C0,0xD140,"ADD",	EA_MEMALT1,func47},	/* ADD.W Dn,<ea> */
#endif
{0xF1C0,0xD180,"ADD",	EA_MEMALT1,func47},	/* ADD.L Dn,<ea> */






/* LINE 1110 */
#ifndef MCF5200
{0xF1F8,0xE000,"ASR",	EA_NONE,func34},	/* ASR.B #<data>,Dy */
{0xF1F8,0xE040,"ASR",	EA_NONE,func34},	/* ASR.W #<data>,Dy */
#endif
{0xF1F8,0xE080,"ASR",	EA_NONE,func34},	/* ASR.L #<data>,Dy */
#ifndef MCF5200
{0xF1F8,0xE020,"ASR",	EA_NONE,func35},	/* ASR.B Dx,Dy */
{0xF1F8,0xE060,"ASR",	EA_NONE,func35},	/* ASR.W Dx,Dy */
#endif
{0xF1F8,0xE0A0,"ASR",	EA_NONE,func35},	/* ASR.L Dx,Dy */
#ifndef MCF5200
{0xFFC0,0xE0C0,"ASR.W",	EA_MEMALT1,func18},	/* ASR.W <ea> */
{0xF1F8,0xE100,"ASL",	EA_NONE,func34},	/* ASL.B #<data>,Dy */
{0xF1F8,0xE140,"ASL",	EA_NONE,func34},	/* ASL.W #<data>,Dy */
#endif
{0xF1F8,0xE180,"ASL",	EA_NONE,func34},	/* ASL.L #<data>,Dy */
#ifndef MCF5200
{0xF1F8,0xE120,"ASL",	EA_NONE,func35},	/* ASL.B Dx,Dy */
{0xF1F8,0xE160,"ASL",	EA_NONE,func35},	/* ASL.W Dx,Dy */
#endif
{0xF1F8,0xE1A0,"ASL",	EA_NONE,func35},	/* ASL.L Dx,Dy */
#ifndef MCF5200
{0xFFC0,0xE1C0,"ASL.W",	EA_MEMALT1,func18},	/* ASL.W <ea> */
{0xF1F8,0xE008,"LSR",	EA_NONE,func34},	/* LSR.B #<data>,Dy */
{0xF1F8,0xE048,"LSR",	EA_NONE,func34},	/* LSR.W #<data>,Dy */
#endif
{0xF1F8,0xE088,"LSR",	EA_NONE,func34},	/* LSR.L #<data>,Dy */
#ifndef MCF5200
{0xF1F8,0xE028,"LSR",	EA_NONE,func35},	/* LSR.B Dx,Dy */
{0xF1F8,0xE068,"LSR",	EA_NONE,func35},	/* LSR.W Dx,Dy */
#endif
{0xF1F8,0xE0A8,"LSR",	EA_NONE,func35},	/* LSR.L Dx,Dy */
#ifndef MCF5200
{0xFFC0,0xE2C0,"LSR.W",	EA_MEMALT1,func18},	/* LSR.W <ea> */
{0xF1F8,0xE108,"LSL",	EA_NONE,func34},	/* LSL.B #<data>,Dy */
{0xF1F8,0xE148,"LSL",	EA_NONE,func34},	/* LSL.W #<data>,Dy */
#endif
{0xF1F8,0xE188,"LSL",	EA_NONE,func34},	/* LSL.L #<data>,Dy */
#ifndef MCF5200
{0xF1F8,0xE128,"LSL",	EA_NONE,func35},	/* LSL.B Dx,Dy */
{0xF1F8,0xE168,"LSL",	EA_NONE,func35},	/* LSL.W Dx,Dy */
#endif
{0xF1F8,0xE1A8,"LSL",	EA_NONE,func35},	/* LSL.L Dx,Dy */
#ifndef MCF5200
{0xFFC0,0xE3C0,"LSL.W",	EA_MEMALT1,func18},	/* LSL.W <ea> */
{0xF1F8,0xE010,"ROXR",	EA_NONE,func34},	/* ROXR.B #<data>,Dy */
{0xF1F8,0xE050,"ROXR",	EA_NONE,func34},	/* ROXR.W #<data>,Dy */
{0xF1F8,0xE090,"ROXR",	EA_NONE,func34},	/* ROXR.L #<data>,Dy */
{0xF1F8,0xE030,"ROXR",	EA_NONE,func35},	/* ROXR.B Dx,Dy */
{0xF1F8,0xE070,"ROXR",	EA_NONE,func35},	/* ROXR.W Dx,Dy */
{0xF1F8,0xE0B0,"ROXR",	EA_NONE,func35},	/* ROXR.L Dx,Dy */
{0xFFC0,0xE4C0,"ROXR.W",EA_MEMALT1,func18},	/* ROXR.W <ea> */
{0xF1F8,0xE110,"ROXL",	EA_NONE,func34},	/* ROXL.B #<data>,Dy */
{0xF1F8,0xE150,"ROXL",	EA_NONE,func34},	/* ROXL.W #<data>,Dy */
{0xF1F8,0xE190,"ROXL",	EA_NONE,func34},	/* ROXL.L #<data>,Dy */
{0xF1F8,0xE130,"ROXL",	EA_NONE,func35},	/* ROXL.B Dx,Dy */
{0xF1F8,0xE170,"ROXL",	EA_NONE,func35},	/* ROXL.W Dx,Dy */
{0xF1F8,0xE1B0,"ROXL",	EA_NONE,func35},	/* ROXL.L Dx,Dy */
{0xFFC0,0xE5C0,"ROXL.W",EA_MEMALT1,func18},	/* ROXL.W <ea> */
{0xF1F8,0xE018,"ROR",	EA_NONE,func34},	/* ROR.B #<data>,Dy */
{0xF1F8,0xE058,"ROR",	EA_NONE,func34},	/* ROR.W #<data>,Dy */
{0xF1F8,0xE098,"ROR",	EA_NONE,func34},	/* ROR.L #<data>,Dy */
{0xF1F8,0xE038,"ROR",	EA_NONE,func35},	/* ROR.B Dx,Dy */
{0xF1F8,0xE078,"ROR",	EA_NONE,func35},	/* ROR.W Dx,Dy */
{0xF1F8,0xE0B8,"ROR",	EA_NONE,func35},	/* ROR.L Dx,Dy */
{0xFFC0,0xE6C0,"ROR.W",	EA_MEMALT1,func18},	/* ROR.W <ea> */
{0xF1F8,0xE118,"ROL",	EA_NONE,func34},	/* ROL.B #<data>,Dy */
{0xF1F8,0xE158,"ROL",	EA_NONE,func34},	/* ROL.W #<data>,Dy */
{0xF1F8,0xE198,"ROL",	EA_NONE,func34},	/* ROL.L #<data>,Dy */
{0xF1F8,0xE138,"ROL",	EA_NONE,func35},	/* ROL.B Dx,Dy */
{0xF1F8,0xE178,"ROL",	EA_NONE,func35},	/* ROL.W Dx,Dy */
{0xF1F8,0xE1B8,"ROL",	EA_NONE,func35},	/* ROL.L Dx,Dy */
{0xFFC0,0xE7C0,"ROL.W",	EA_MEMALT1,func18},	/* ROL.W <ea> */
{0xFFC0,0xEAC0,"BFCHG"	,EA_CTRALT1,func48},/* BFCHG <ea>{offset:width} */
{0xFFC0,0xECC0,"BFCLR"	,EA_CTRALT1,func48},/* BFCLR <ea>{offset:width} */
{0xFFC0,0xEBC0,"BFEXTS"	,EA_CTRALT2,func49},/* BFEXTS <ea>{offset:width},Dn */
{0xFFC0,0xE9C0,"BFEXTU"	,EA_CTRALT2,func49},/* BFEXTU <ea>{offset:width},Dn */
{0xFFC0,0xEDC0,"BFFFO"	,EA_CTRALT2,func49},/* BFFFO <ea>{offset:width},Dn */
{0xFFC0,0xEFC0,"BFINS"	,EA_CTRALT1,func50},/* BFINS Dn,<ea>{offset:width} */
{0xFFC0,0xEEC0,"BFSET"	,EA_CTRALT1,func48},/* BFSET <ea>{offset:width} */
{0xFFC0,0xE8C0,"BFTST"	,EA_CTRALT2,func48},/* BFTST <ea>{offset:width} */
#endif





/* LINE 1111 */

#ifdef MCF5200
{0xFFC0,0xFB00,"WDDATA",EA_MEMALT1,func15},	/* WDDATA.b <ea> */
{0xFFC0,0xFB40,"WDDATA",EA_MEMALT1,func15},	/* WDDATA.w <ea> */
{0xFFC0,0xFB80,"WDDATA",EA_MEMALT1,func15},	/* WDDATA.l <ea> */
{0xFFC0,0xFBC0,"WDEBUG",(ARI | ARID),func37},	/* WDEBUG <ea> */
#endif

#ifdef CPU32
{0xFFFF,0xF800,"LPSTOP",EA_NONE,func63},	/* LPSTOP #<data> */
{0xFFC0,0xF800,"TBL",	EA_NONE,func62},	/* TBL <ea>,Dx */
#endif

#if 0
{0xFFF8,0xF620,"MOVE16",	EA_NONE,func},	/* MOVE16 (Ax)+,(Ay)+ */
{0xFFF8,0xF600,"MOVE16",	EA_NONE,func},	/* MOVE16 (Ay)+,(xxx).L */
{0xFFF8,0xF608,"MOVE16",	EA_NONE,func},	/* MOVE16 (xxx).L,(Ay)+ */
{0xFFF8,0xF610,"MOVE16",	EA_NONE,func},	/* MOVE16 (Ay).L,(xxx).L */
{0xFFF8,0xF618,"MOVE16",	EA_NONE,func},	/* MOVE16 (xxx).L,(Ay) */
#endif

} ;
static const int
ISASIZE = sizeof(isa)/sizeof(INSTRENTRY);

/********************************************************/
static void
disasm_display (char *stmt, ADDRESS init_pc, ADDRESS next_pc)
{
	int nw, ad, done, i, spaces;
	ADDRESS pc;
#ifdef SYMBOL_TABLE
	char sstr[100];
#endif

	pc = init_pc;
	ad = FALSE;
	done = FALSE;

#ifdef SYMBOL_TABLE
	if (symtab_convert_address(pc,sstr))
	{
		printf("%08X:\n%08X: %s:\n",
			(unsigned int)pc,
			(unsigned int)pc,
			sstr);
	}
#endif
	
	while (!done)
	{
		nw = 0;
		printf("%08lX: ",(unsigned long)pc);
		spaces = 10;	/* used 10 chars up so far on screen */
		while (pc < next_pc)
		{
			printf("%04X ",cpu_read_data((ADDRESS)pc,16));
			pc = (ADDRESS)((unsigned int)pc + (unsigned int)2);
			spaces += 5;
			if (++nw >= 4)
			{
				break;
			}
		}

		if (!ad)
		{
			for (i = spaces; i < 31; i++)
				printf(" ");
			printf("%s",stmt);
			ad = TRUE;
		}
		printf("\n");

		if (pc >= next_pc)
			done = TRUE;
	}
}

char *DisHelp[] = {
	"Disassemble binary",
	"{address} [lines]",
	" -m   query for more after each block disassembled",
	0,
	
};

int
Dis(int argc, char *argv[])
{
	int	i, opt, more, lines;
	ADDRESS addr;

	more = 0;
	while((opt=getopt(argc,argv,"m")) != -1) {
		switch(opt) {
		case 'm':
			more = 1;
			break;
		default:
			return(0);
		}
	}

	if ((argc < optind+1) || (argc > optind+2))
		return(-1);

	if (argc == optind+2)
		lines = (int)strtol(argv[optind+1],(char **)0,0);
	else
		lines = 8;

	addr = (ADDRESS)strtoul(argv[optind],(char **)0,0);
	
	do {
		for(i=0;i<lines;i++)
			addr = cpu_disasm(addr,1);
		if (more)
			more = More();
	}
	while(more);
	return(0);
}

/********************************************************/

ADDRESS
cpu_disasm (ADDRESS init_pc, int display)
{
	/*
	 * This routine disassembles one ColdFire instruction, and
	 * returns the address of the next instruction.
	 */
	int index, opword;

	disasm_pc = init_pc;
	opword = (int)((WORD)cpu_read_data((ADDRESS)disasm_pc,16));
	valid_instruction = FALSE;
	dstr[0] = '\0';

	for (index = 0; index < ISASIZE; index++)
	{
		if ((opword & isa[index].keepers) == isa[index].match)
		{
			valid_instruction = TRUE;
			append_instruction(dstr,isa[index].instruction);
			inc_disasm_pc(2);
			isa[index].handler(index,opword);	/* disassemble */
			break;
		}
	}
	if (valid_instruction)
	{
	}
	else
	{
		append_instruction(dstr,"DC.W");
		append_value(dstr,opword,16);
		/* inc_disasm_pc(2); */
		disasm_pc = (ADDRESS)((LONG)init_pc + 2);
	}
	if (display)
	{
		disasm_display(dstr,init_pc,disasm_pc);
	}

	return disasm_pc;
}
#endif
