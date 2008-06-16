/* Linker file for building monitor using GNU toolset on PC...

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
etheraddr   		= 0xc0000020;
alt_tfsdevtbl_base	= 0xc8000040;

MEMORY 
{
	ram :		org = 0xc0000000,	len = 0x7ffff
}

SECTIONS
{
	.text	:
	{
		boot_base = .;
		RAMTST_reset.o(.text)
		*(.glue_7t)		
		*(.glue_7)		
	} >ram

	.data	:
	{
		*(.data)
	} >ram

	.sdata	:
	{
		*(.sdata)
	} >ram

	.sdata2	:
	{
		*(.sdata2)
	} >ram

	.rodata	:
	{
		*(.rodata)
	} >ram

	.got	:
	{
		*(.got)
	} >ram

	.bss	:
	{
		bss_start = .;
		*(.bss) *(COMMON)
	} >ram

	.sbss	:
	{
		*(.sbss)
		bss_end = .;
	} >ram
}
