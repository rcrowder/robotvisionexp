	.file	"vectors.s"

/*****************************************************************************
 *
 * Register definitions correspond to offsets into the
 * regtbl[] array...
 */
	.equ PC,	0
	.equ SR,	4
	.equ A0,	8
	.equ A1,	12
	.equ A2,	16
	.equ A3,	20
	.equ A4,	24
	.equ A5,	28
	.equ A6,	32
	.equ SP,	36
	.equ D0,	40
	.equ D1,	44
	.equ D2,	48
	.equ D3,	52
	.equ D4,	56
	.equ D5,	60
	.equ D6,	64
	.equ D7,	68


/*****************************************************************************
 #
 # Vectors:
 # The code beginning at vbase is used by execption.c as a table of
 # vector handlers where the size of each member is "vnext - vbase".
 # This scheme keeps the C-code that initializes the vector handlers to
 # a minimum ; but still provides full-and-unique coverage of every
 # possible exception on the 68k family.
 # THe function gotoexception is used to stuff values into regtbl[] for
 # use by the general purpose exception() function.
 */
	.global gotoexception
gotoexception:
	move.l	%a0,(regtbl+A0)
	move.l	%a1,(regtbl+A1)
	move.l	%a2,(regtbl+A2)
	move.l	%a3,(regtbl+A3)
	move.l	%a4,(regtbl+A4)
	move.l	%a5,(regtbl+A5)
	move.l	%a6,(regtbl+A6)
	move.l	%d0,(regtbl+D0)
	move.l	%d1,(regtbl+D1)
	move.l	%d2,(regtbl+D2)
	move.l	%d3,(regtbl+D3)
	move.l	%d4,(regtbl+D4)
	move.l	%d5,(regtbl+D5)
	move.l	%d6,(regtbl+D6)
	move.l	%d7,(regtbl+D7)
	move.l	%sp,%a0
	move.l	%a0,(regtbl+SP)
	move.l	%sp@,%a0
	move.l	%a0,(regtbl+SR)
	move.l	%sp@(4),%a0
	move.l	%a0,(regtbl+PC)
 	jsr	exception	


/*****************************************************************************/

	.global	vbase, vnext, vbase128, vnext128
vbase:
	move.b	#2,%d0
	jmp gotoexception
vnext:
	move.b	#3,%d0
	jmp gotoexception
	move.b	#4,%d0
	jmp gotoexception
	move.b	#5,%d0
	jmp gotoexception
	move.b	#6,%d0
	jmp gotoexception
	move.b	#7,%d0
	jmp gotoexception
	move.b	#8,%d0
	jmp gotoexception
	move.b	#9,%d0
	jmp gotoexception
	move.b	#10,%d0
	jmp gotoexception
	move.b	#11,%d0
	jmp gotoexception
	move.b	#12,%d0
	jmp gotoexception
	move.b	#13,%d0
	jmp gotoexception
	move.b	#14,%d0
	jmp gotoexception
	move.b	#15,%d0
	jmp gotoexception
	move.b	#16,%d0
	jmp gotoexception
	move.b	#17,%d0
	jmp gotoexception
	move.b	#18,%d0
	jmp gotoexception
	move.b	#19,%d0
	jmp gotoexception
	move.b	#20,%d0
	jmp gotoexception
	move.b	#21,%d0
	jmp gotoexception
	move.b	#22,%d0
	jmp gotoexception
	move.b	#23,%d0
	jmp gotoexception
	move.b	#24,%d0
	jmp gotoexception
	move.b	#25,%d0
	jmp gotoexception
	move.b	#26,%d0
	jmp gotoexception
	move.b	#27,%d0
	jmp gotoexception
	move.b	#28,%d0
	jmp gotoexception
	move.b	#29,%d0
	jmp gotoexception
	move.b	#30,%d0
	jmp gotoexception
	move.b	#31,%d0
	jmp gotoexception
	move.b	#32,%d0
	jmp gotoexception
	move.b	#33,%d0
	jmp gotoexception
	move.b	#34,%d0
	jmp gotoexception
	move.b	#35,%d0
	jmp gotoexception
	move.b	#36,%d0
	jmp gotoexception
	move.b	#37,%d0
	jmp gotoexception
	move.b	#38,%d0
	jmp gotoexception
	move.b	#39,%d0
	jmp gotoexception
	move.b	#40,%d0
	jmp gotoexception
	move.b	#41,%d0
	jmp gotoexception
	move.b	#42,%d0
	jmp gotoexception
	move.b	#43,%d0
	jmp gotoexception
	move.b	#44,%d0
	jmp gotoexception
	move.b	#45,%d0
	jmp gotoexception
	move.b	#46,%d0
	jmp gotoexception
	move.b	#47,%d0
	jmp gotoexception
	move.b	#48,%d0
	jmp gotoexception
	move.b	#49,%d0
	jmp gotoexception
	move.b	#50,%d0
	jmp gotoexception
	move.b	#51,%d0
	jmp gotoexception
	move.b	#52,%d0
	jmp gotoexception
	move.b	#53,%d0
	jmp gotoexception
	move.b	#54,%d0
	jmp gotoexception
	move.b	#55,%d0
	jmp gotoexception
	move.b	#56,%d0
	jmp gotoexception
	move.b	#57,%d0
	jmp gotoexception
	move.b	#58,%d0
	jmp gotoexception
	move.b	#59,%d0
	jmp gotoexception
	move.b	#60,%d0
	jmp gotoexception
	move.b	#61,%d0
	jmp gotoexception
	move.b	#62,%d0
	jmp gotoexception
	move.b	#63,%d0
	jmp gotoexception
	move.b	#64,%d0
	jmp gotoexception
	move.b	#65,%d0
	jmp gotoexception
	move.b	#66,%d0
	jmp gotoexception
	move.b	#67,%d0
	jmp gotoexception
	move.b	#68,%d0
	jmp gotoexception
	move.b	#69,%d0
	jmp gotoexception
	move.b	#70,%d0
	jmp gotoexception
	move.b	#71,%d0
	jmp gotoexception
	move.b	#72,%d0
	jmp gotoexception
	move.b	#73,%d0
	jmp gotoexception
	move.b	#74,%d0
	jmp gotoexception
	move.b	#75,%d0
	jmp gotoexception
	move.b	#76,%d0
	jmp gotoexception
	move.b	#77,%d0
	jmp gotoexception
	move.b	#78,%d0
	jmp gotoexception
	move.b	#79,%d0
	jmp gotoexception
	move.b	#80,%d0
	jmp gotoexception
	move.b	#81,%d0
	jmp gotoexception
	move.b	#82,%d0
	jmp gotoexception
	move.b	#83,%d0
	jmp gotoexception
	move.b	#84,%d0
	jmp gotoexception
	move.b	#85,%d0
	jmp gotoexception
	move.b	#86,%d0
	jmp gotoexception
	move.b	#87,%d0
	jmp gotoexception
	move.b	#88,%d0
	jmp gotoexception
	move.b	#89,%d0
	jmp gotoexception
	move.b	#90,%d0
	jmp gotoexception
	move.b	#91,%d0
	jmp gotoexception
	move.b	#92,%d0
	jmp gotoexception
	move.b	#93,%d0
	jmp gotoexception
	move.b	#94,%d0
	jmp gotoexception
	move.b	#95,%d0
	jmp gotoexception
	move.b	#96,%d0
	jmp gotoexception
	move.b	#97,%d0
	jmp gotoexception
	move.b	#98,%d0
	jmp gotoexception
	move.b	#99,%d0
	jmp gotoexception
	move.b	#100,%d0
	jmp gotoexception
	move.b	#101,%d0
	jmp gotoexception
	move.b	#102,%d0
	jmp gotoexception
	move.b	#103,%d0
	jmp gotoexception
	move.b	#104,%d0
	jmp gotoexception
	move.b	#105,%d0
	jmp gotoexception
	move.b	#106,%d0
	jmp gotoexception
	move.b	#107,%d0
	jmp gotoexception
	move.b	#108,%d0
	jmp gotoexception
	move.b	#109,%d0
	jmp gotoexception
	move.b	#110,%d0
	jmp gotoexception
	move.b	#111,%d0
	jmp gotoexception
	move.b	#112,%d0
	jmp gotoexception
	move.b	#113,%d0
	jmp gotoexception
	move.b	#114,%d0
	jmp gotoexception
	move.b	#115,%d0
	jmp gotoexception
	move.b	#116,%d0
	jmp gotoexception
	move.b	#117,%d0
	jmp gotoexception
	move.b	#118,%d0
	jmp gotoexception
	move.b	#119,%d0
	jmp gotoexception
	move.b	#120,%d0
	jmp gotoexception
	move.b	#121,%d0
	jmp gotoexception
	move.b	#122,%d0
	jmp gotoexception
	move.b	#123,%d0
	jmp gotoexception
	move.b	#124,%d0
	jmp gotoexception
	move.b	#125,%d0
	jmp gotoexception
	move.b	#126,%d0
	jmp gotoexception
	move.b	#127,%d0
	jmp gotoexception
vbase128:
	move.b	#128,%d0
	jmp gotoexception
vnext128:
	move.b	#129,%d0
	jmp gotoexception
	move.b	#130,%d0
	jmp gotoexception
	move.b	#131,%d0
	jmp gotoexception
	move.b	#132,%d0
	jmp gotoexception
	move.b	#133,%d0
	jmp gotoexception
	move.b	#134,%d0
	jmp gotoexception
	move.b	#135,%d0
	jmp gotoexception
	move.b	#136,%d0
	jmp gotoexception
	move.b	#137,%d0
	jmp gotoexception
	move.b	#138,%d0
	jmp gotoexception
	move.b	#139,%d0
	jmp gotoexception
	move.b	#140,%d0
	jmp gotoexception
	move.b	#141,%d0
	jmp gotoexception
	move.b	#142,%d0
	jmp gotoexception
	move.b	#143,%d0
	jmp gotoexception
	move.b	#144,%d0
	jmp gotoexception
	move.b	#145,%d0
	jmp gotoexception
	move.b	#146,%d0
	jmp gotoexception
	move.b	#147,%d0
	jmp gotoexception
	move.b	#148,%d0
	jmp gotoexception
	move.b	#149,%d0
	jmp gotoexception
	move.b	#150,%d0
	jmp gotoexception
	move.b	#151,%d0
	jmp gotoexception
	move.b	#152,%d0
	jmp gotoexception
	move.b	#153,%d0
	jmp gotoexception
	move.b	#154,%d0
	jmp gotoexception
	move.b	#155,%d0
	jmp gotoexception
	move.b	#156,%d0
	jmp gotoexception
	move.b	#157,%d0
	jmp gotoexception
	move.b	#158,%d0
	jmp gotoexception
	move.b	#159,%d0
	jmp gotoexception
	move.b	#160,%d0
	jmp gotoexception
	move.b	#161,%d0
	jmp gotoexception
	move.b	#162,%d0
	jmp gotoexception
	move.b	#163,%d0
	jmp gotoexception
	move.b	#164,%d0
	jmp gotoexception
	move.b	#165,%d0
	jmp gotoexception
	move.b	#166,%d0
	jmp gotoexception
	move.b	#167,%d0
	jmp gotoexception
	move.b	#168,%d0
	jmp gotoexception
	move.b	#169,%d0
	jmp gotoexception
	move.b	#170,%d0
	jmp gotoexception
	move.b	#171,%d0
	jmp gotoexception
	move.b	#172,%d0
	jmp gotoexception
	move.b	#173,%d0
	jmp gotoexception
	move.b	#174,%d0
	jmp gotoexception
	move.b	#175,%d0
	jmp gotoexception
	move.b	#176,%d0
	jmp gotoexception
	move.b	#177,%d0
	jmp gotoexception
	move.b	#178,%d0
	jmp gotoexception
	move.b	#179,%d0
	jmp gotoexception
	move.b	#180,%d0
	jmp gotoexception
	move.b	#181,%d0
	jmp gotoexception
	move.b	#182,%d0
	jmp gotoexception
	move.b	#183,%d0
	jmp gotoexception
	move.b	#184,%d0
	jmp gotoexception
	move.b	#185,%d0
	jmp gotoexception
	move.b	#186,%d0
	jmp gotoexception
	move.b	#187,%d0
	jmp gotoexception
	move.b	#188,%d0
	jmp gotoexception
	move.b	#189,%d0
	jmp gotoexception
	move.b	#190,%d0
	jmp gotoexception
	move.b	#191,%d0
	jmp gotoexception
	move.b	#192,%d0
	jmp gotoexception
	move.b	#193,%d0
	jmp gotoexception
	move.b	#194,%d0
	jmp gotoexception
	move.b	#195,%d0
	jmp gotoexception
	move.b	#196,%d0
	jmp gotoexception
	move.b	#197,%d0
	jmp gotoexception
	move.b	#198,%d0
	jmp gotoexception
	move.b	#199,%d0
	jmp gotoexception
	move.b	#200,%d0
	jmp gotoexception
	move.b	#201,%d0
	jmp gotoexception
	move.b	#202,%d0
	jmp gotoexception
	move.b	#203,%d0
	jmp gotoexception
	move.b	#204,%d0
	jmp gotoexception
	move.b	#205,%d0
	jmp gotoexception
	move.b	#206,%d0
	jmp gotoexception
	move.b	#207,%d0
	jmp gotoexception
	move.b	#208,%d0
	jmp gotoexception
	move.b	#209,%d0
	jmp gotoexception
	move.b	#210,%d0
	jmp gotoexception
	move.b	#211,%d0
	jmp gotoexception
	move.b	#212,%d0
	jmp gotoexception
	move.b	#213,%d0
	jmp gotoexception
	move.b	#214,%d0
	jmp gotoexception
	move.b	#215,%d0
	jmp gotoexception
	move.b	#216,%d0
	jmp gotoexception
	move.b	#217,%d0
	jmp gotoexception
	move.b	#218,%d0
	jmp gotoexception
	move.b	#219,%d0
	jmp gotoexception
	move.b	#220,%d0
	jmp gotoexception
	move.b	#221,%d0
	jmp gotoexception
	move.b	#222,%d0
	jmp gotoexception
	move.b	#223,%d0
	jmp gotoexception
	move.b	#224,%d0
	jmp gotoexception
	move.b	#225,%d0
	jmp gotoexception
	move.b	#226,%d0
	jmp gotoexception
	move.b	#227,%d0
	jmp gotoexception
	move.b	#228,%d0
	jmp gotoexception
	move.b	#229,%d0
	jmp gotoexception
	move.b	#230,%d0
	jmp gotoexception
	move.b	#231,%d0
	jmp gotoexception
	move.b	#232,%d0
	jmp gotoexception
	move.b	#233,%d0
	jmp gotoexception
	move.b	#234,%d0
	jmp gotoexception
	move.b	#235,%d0
	jmp gotoexception
	move.b	#236,%d0
	jmp gotoexception
	move.b	#237,%d0
	jmp gotoexception
	move.b	#238,%d0
	jmp gotoexception
	move.b	#239,%d0
	jmp gotoexception
	move.b	#240,%d0
	jmp gotoexception
	move.b	#241,%d0
	jmp gotoexception
	move.b	#242,%d0
	jmp gotoexception
	move.b	#243,%d0
	jmp gotoexception
	move.b	#244,%d0
	jmp gotoexception
	move.b	#245,%d0
	jmp gotoexception
	move.b	#246,%d0
	jmp gotoexception
	move.b	#247,%d0
	jmp gotoexception
	move.b	#248,%d0
	jmp gotoexception
	move.b	#249,%d0
	jmp gotoexception
	move.b	#250,%d0
	jmp gotoexception
	move.b	#251,%d0
	jmp gotoexception
	move.b	#252,%d0
	jmp gotoexception
	move.b	#253,%d0
	jmp gotoexception
	move.b	#254,%d0
	jmp gotoexception
	move.b	#255,%d0
	jmp gotoexception


#if INCLUDE_AT
/************************************************************************/
/* at_hdlr:																*/
/* Save registers and jump to attrap.									*/
/* Note: %a7 and %sp are the same thing.								*/
/************************************************************************/
	.text
	.global at_hdlr
at_hdlr:
	mov.l	%a0,(regtbl+A0)	/* Store all registers */
	mov.l	%a1,(regtbl+A1)
	mov.l	%a2,(regtbl+A2)
	mov.l	%a3,(regtbl+A3)
	mov.l	%a4,(regtbl+A4)
	mov.l	%a5,(regtbl+A5)
	mov.l	%a6,(regtbl+A6)
	mov.l	%d0,(regtbl+D0)
	mov.l	%d1,(regtbl+D1)
	mov.l	%d2,(regtbl+D2)
	mov.l	%d3,(regtbl+D3)
	mov.l	%d4,(regtbl+D4)
	mov.l	%d5,(regtbl+D5)
	mov.l	%d6,(regtbl+D6)
	mov.l	%d7,(regtbl+D7)
	mov.l	%sp,(regtbl+SP)
	mov.l	2(%a7),%a0
	mov.l	%a0,(regtbl+PC)
	mov.w	(%a7),%a0
	mov.l	%a0,(regtbl+SR)
 	jsr	attrap				/* Execute high level trap handler. */

/************************************************************************/
/*  async_brk_hdlr:														*/
/*  Save registers and jump to asyncbrk.								*/
/*  Used by the debug uart's break interrupt and any other externally	*/
/*  generated asynchronous breakpoint.									*/
/************************************************************************/

	.text
	.global async_brk_hdlr
async_brk_hdlr:
	mov.l	%a0,(regtbl+A0)	/* Store all registers */
	mov.l	%a1,(regtbl+A1)
	mov.l	%a2,(regtbl+A2)
	mov.l	%a3,(regtbl+A3)
	mov.l	%a4,(regtbl+A4)
	mov.l	%a5,(regtbl+A5)
	mov.l	%a6,(regtbl+A6)
	mov.l	%d0,(regtbl+D0)
	mov.l	%d1,(regtbl+D1)
	mov.l	%d2,(regtbl+D2)
	mov.l	%d3,(regtbl+D3)
	mov.l	%d4,(regtbl+D4)
	mov.l	%d5,(regtbl+D5)
	mov.l	%d6,(regtbl+D6)
	mov.l	%d7,(regtbl+D7)
	mov.l	%a7,(regtbl+SP)
	mov.l	2(%a7),%a0
	mov.l	%a0,(regtbl+PC)
	mov.w	(%a7),%a0
	mov.l	%a0,(regtbl+SR)
 	jsr	asyncbreak			/* Execute high level trap handler. */


/****************************************************************************
  trace_hdlr:
  Save registers and jump to tracetrap.
  Identical to trap_hdlr except that it calls trace trap...
*/ 
	.global trace_hdlr
trace_hdlr:
	mov.l	%a0,(regtbl+A0)		/* Store all registers */
	mov.l	%a1,(regtbl+A1)
	mov.l	%a2,(regtbl+A2)
	mov.l	%a3,(regtbl+A3)
	mov.l	%a4,(regtbl+A4)
	mov.l	%a5,(regtbl+A5)
	mov.l	%a6,(regtbl+A6)
	mov.l	%d0,(regtbl+D0)
	mov.l	%d1,(regtbl+D1)
	mov.l	%d2,(regtbl+D2)
	mov.l	%d3,(regtbl+D3)
	mov.l	%d4,(regtbl+D4)
	mov.l	%d5,(regtbl+D5)
	mov.l	%d6,(regtbl+D6)
	mov.l	%d7,(regtbl+D7)
	mov.l	%a7,(regtbl+SP)
	mov.l	2(%a7),%a0
	mov.l	%a0,(regtbl+PC)
	mov.w	(%a7),%a0
	mov.l	%a0,(regtbl+SR)
 	jsr	tracetrap			/* Execute high level trap handler. */


/****************************************************************************
  resume:
  Restore registers and issue rte.
*/ 

	.global	resume
resume:
	mov.l	(regtbl+SP),%a7	/* Restore registers */
	mov.l	(regtbl+SR),%a0
	mov.w	%a0,(%a7)
	mov.l	(regtbl+A0),%a0
	mov.l	(regtbl+A1),%a1
	mov.l	(regtbl+A2),%a2
	mov.l	(regtbl+A3),%a3
	mov.l	(regtbl+A4),%a4
	mov.l	(regtbl+A5),%a5
	mov.l	(regtbl+A6),%a6
	mov.l	(regtbl+D0),%d0
	mov.l	(regtbl+D1),%d1
	mov.l	(regtbl+D2),%d2
	mov.l	(regtbl+D3),%d3
	mov.l	(regtbl+D4),%d4
	mov.l	(regtbl+D5),%d5
	mov.l	(regtbl+D6),%d6
	mov.l	(regtbl+D7),%d7
	rte
#endif
