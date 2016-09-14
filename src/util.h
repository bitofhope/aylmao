/*
 * This program is free software. See COPYING for details.
 */

#define NOP while(0);

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#ifdef DEBUG
	#define DLOG(...) eprintf(__VA_ARGS__)
#else
	#define DLOG(...) NOP
#endif

