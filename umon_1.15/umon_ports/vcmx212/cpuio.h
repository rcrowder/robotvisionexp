#ifndef CPUIO
#define CPUIO


#define MONARGV0 "umon"

//Setup for LAN91C111
#define LAN_REG_ADD				0xD1000000	// CS3 
#define LAN_REG(_x_)  			*(vushort *)(LAN_REG_ADD | _x_)
#define LAN_STEP				1		// LAN91C11x is on 16-bit boundries

// Console Serial Port Setup
#define DEFAULT_BAUD_RATE 	230400


#endif //CPUIO
