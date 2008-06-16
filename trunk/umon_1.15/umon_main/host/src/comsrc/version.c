#include <stdio.h>
#include <stdlib.h>

void
showVersion(void)
{
	printf(" Built: %s @ %s\n",__DATE__,__TIME__);
	exit(1);
}
