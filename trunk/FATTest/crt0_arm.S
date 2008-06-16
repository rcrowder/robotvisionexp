#include "cfg.h"

    .extern AppStack
	.extern Cstart

	.text
	.code 32
	.align 	0

	.global	_mainCRTStartup
	.global	_start
	.global	start

    /* Set stack pointer to end of AppStack and jump to Cstart:
	 */
start:
_start:
_mainCRTStartup:
    ldr sp, =(AppStack + APPSTACKSIZE - 4)

jump_to_c:
    bl Cstart

exit:
	b exit
