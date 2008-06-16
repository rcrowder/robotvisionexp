#include "config.h"
#include "ctype.h"
#include "genlib.h"
#include "stddefs.h"

/* memcmp():
 * Compare n bytes:  s1>s2: >0  s1==s2: 0  s1<s2: <0
 */
int
memcmp(register char *s1,register char *s2,register int n)
{
	int diff;

	if (s1 != s2)
		while (--n >= 0)
			if ((diff = (*s1++ - *s2++)))
				return (diff);
	return (0);
}
