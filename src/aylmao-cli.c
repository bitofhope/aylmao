/*
 * This program is free software. See COPYING for details.
 */

// Standard includes
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// System includes
#include <getopt.h>
#include <unistd.h>
#include <string.h>

// Local includes
#include "extern.h"
#include "types.h"
#include "kone.h"
#include "util.h"

bool read01(FILE* f);
Instruction nullinst(void);
Instruction parse01inst(FILE* f);
void usage(void);

int main(int argc, char* argv[])
{
	bool verbose = false;
	bool step = false;

	int opt;
	while ((opt = getopt(argc, argv, "hsv")) != -1)
	{
		switch (opt)
		{
			case 'h':
				usage();
				break;
			case 's':
				step = true;
				break;
			case 'v':
				verbose = true;
				break;
			default:
				//usage();
				break;
		}
	}

	DLOG("Parsed opts: (-s %s) (-v %s)", step ? "true" : "false",
	                                     verbose ? "true" : "false");

	if (optind >= argc)
		usage();

	char* fpath = argv[optind];
	FILE* f = fopen(fpath, "r");
	if (!f)
	{
		eprintf("Could not read file %s\n", fpath);
		exit(EXIT_FAILURE);
	}

	DLOG("Read file\n");
	Instruction mpm[256];
	int16_t mm[4096] = {0};

	for (int i = 0; i < 256; i++)
	{
		mpm[i] = parse01inst(f);
	}

	DLOG("Parsed microcode\n");

	Machine m;
	char quit = '\0';

	initMachine(&m, mpm, mm);

	DLOG("Initialized machine\n");

	do
	{
		clock1(&m);
		clock2(&m);
		clock3(&m);
		clock4(&m);
		clock5(&m);
		if (verbose)
		{
			printState(m);
		}
		if (step)
			quit = getchar();
	} while (quit != 'q');
	
	printState(m);
}

Instruction parse01inst(FILE* f)
{
	Instruction ret = {
		.wA =       read01(f),
		.wB =       read01(f),
		.wC =       read01(f),
		.wD =       read01(f),
		.w1 =       read01(f),
		.wMDR =     read01(f),
		.compl =    read01(f),
		.lshift =   read01(f),
		.rtoA =     read01(f),
		.rtoB =     read01(f),
		.rtoC =     read01(f),
		.rtoD =     read01(f),
		.rtoMDR =   read01(f),
		.rtoMAR =   read01(f),
		.memtoMDR = read01(f),
		.MDRtomem = read01(f),
		.MPCw1 =    read01(f),
		.MPCwMIR8 = read01(f),
		.MPCA0 =    read01(f),
		.MPCAneg =  read01(f),
		.MPCwMDR4 = read01(f),
		.MPCwMPC =  read01(f)
	};

	return ret;
}

bool read01(FILE* f)
{
	bool done = false;
	char c;
	while (!done)
	{
		switch ((c = fgetc(f)))
		{
			case '1':
				return true;
				break; // Notreached
			case '0':
				return false;
				break; // Notreached
			case EOF:
				return false;
				break; // Notreached
		}
	}
	return false; // Notreached
}

void usage(void)
{
	eprintf(
"aylmao-cli [-h] | [-vs] FILE\n"
"\t-h\tDisplay this help and exit\n"
"\n"
"\t-s\tStep, wait for newline after every clock\n"
"\t-v\tVerbose, show machine state after every clock\n");
	exit (EXIT_FAILURE);
}

Instruction nullinst(void)
{
	Instruction i;
	memset(&i, 0, sizeof(i));
	return i;
}

