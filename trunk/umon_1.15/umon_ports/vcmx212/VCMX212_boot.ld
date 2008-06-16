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

MEMORY 
{
	rom :		org = 0xc8000000,	len = 0x7ffff
	ram :		org = 0xc0000000,	len = 0x7ffff	
}

SECTIONS
{
	.text	:
	{
		boot_base = .;
		BOOT_reset.o(.text)
		*(.glue_7t)
		*(.glue_7)		
	} >rom

	.data	:
	{
		*(.data)
	} >rom

	.sdata	:
	{
		*(.sdata)
	} >rom

	.sdata2	:
	{
		*(.sdata2)
	} >rom

	.rodata	:
	{
		*(.rodata)
	} >rom

	.got	:
	{
		*(.got)
	} >rom

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
