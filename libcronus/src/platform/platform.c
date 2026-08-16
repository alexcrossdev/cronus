#include "cronus/types.h"

#if defined(PWINDOWS)
	#include "windows_platform.c"
#elif defined(PLINUX)
	#include "linux_platform.c"
#elif defined(PMAC)
	#include "mac_platform.c"

#else
	#error "Unsupported platform!"
#endif
