#include "cfg.h"
    
    .extern	_AppStack
	.extern	_Cstart
    .global start

    .text

start:
    sp.h = _AppStack+(APPSTACKSIZE-32)
    sp.l = _AppStack+(APPSTACKSIZE-32)
    jump	_Cstart
