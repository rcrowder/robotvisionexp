/* Not all CPUs refer to the register pointing to the next instruction
 * address as "PC", so this definition allows it to be cpu specific...
 */
#define CPU_PC_REG	"PC"

/* List of the register names for the ColdFire in the order they
 * are expected by the GDB 'g' command (read all registers).
 * Not sure why I need to pad the end of the table, but GDB appears
 * to want to see more registers than are actually in the CPU.
 */
static char *gdb_regtbl[] = {
	"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
	"A0", "A1", "A2", "A3", "A4", "A5", "A6", "SP",
	"SR", "PC", "D0", "D0", "D0", 0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

