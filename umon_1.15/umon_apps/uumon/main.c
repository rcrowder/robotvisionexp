#include "config.h"
#include "stddefs.h"
#include "genlib.h"

/* MonStack:
 * The monitor's stack is declared within the monitor's own .bss space.
 * This keeps the memory map simple, the only thing that needs to be
 * accounted for is that in the bss init loop, this section should not
 * be initialized.  MONSTACKSIZE must be defined in config.h.  
 */
ulong	MonStack[MONSTACKSIZE/4];

int
main(void)
{
	static	char cmdline[CMDLINESIZE];

	ioInit();

	while(1) {
		puts("uuMON>");
		memset(cmdline,0,CMDLINESIZE);	/* Clear command line buffer */
		if (getline(cmdline,CMDLINESIZE) == 0)
			continue;
		docommand(cmdline);
	}
	return(0);
}
