#include "config.h"
#include "ctype.h"
#include "genlib.h"
#include "stddefs.h"

/* memcpy():
 *  Copy n bytes from 'from' to 'to'; return 'to'.
 *	This version of memcpy() tries to take advantage of address alignment.
 *  The goal is to do as many of the copies on 4-byte aligned addresses,
 *  falling back to 2-byte alignment, and finally, if there is no other
 *  way, simple byte-by-byte copy.
 *  Note that there is some point where the amount of overhead may exceed
 *  the byte count; hence, this will take longer for small byte counts.
 *  The assumption here is that small byte count memcpy() calls don't really
 *  care.
 */
char *
memcpy(char *to,char *from,int count)
{
	char	*to_copy, *end;

	to_copy = to;

#if INCLUDE_QUICKMEMCPY
	/* If count is greater than 8, get fancy, else just do byte-copy... */
	if (count > 8) {
		/* Attempt to optimize the transfer here... */
		if (((int)to & 3) && ((int)from & 3)) {
			/* If from and to pointers are both unaligned to the
			 * same degree then we can do a few char copies to get them
			 * 4-byte aligned and then do a lot of 4-byte aligned copies.
			 */
			if (((int)to & 3) == ((int)from & 3)) {
				while((int)to & 3) {
					*to++ = *from++;
					count--;
				}
			}
			/* If from and to pointers are both odd, but different, then
			 * we can increment them both by 1 and do a bunch of 2-byte
			 * aligned copies...
			 */
			else if (((int)to & 1) && ((int)from & 1)) {
				*to++ = *from++;
				count--;
			}
		}
	
		/* If both pointers are now 4-byte aligned or 2-byte aligned,
		 * take advantage of that here...
		 */
		if (!((int)to & 3) && !((int)from & 3)) {
			end = to + (count & ~3);
			count = count & 3;
			while(to < end) {
				*(ulong *)to = *(ulong *)from;
				from += 4;
				to += 4;
			}
		}
		else if (!((int)to & 1) && !((int)from & 1)) {
			end = to + (count & ~1);
			count = count & 1;
			while(to < end) {
				*(ushort *)to = *(ushort *)from;
				from += 2;
				to += 2;
			}
		}
	}
#endif

	if (count) {
		end = to + count;
		while(to < end)
			*to++ = *from++;
	}
	return(to_copy);
}

void
bcopy(char *from, char *to, int size)
{
	memcpy(to,from,size);
}
