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

Instruction parse01inst(FILE* f);
void usage(void);
Instruction nullinst(void);

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
	Instruction i;
	char c = '\0';

	// Brace yourself, this is gonna be ugly.
	
	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.wA = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.wB = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.wC = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.wD = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.w1 = (c == '1') ? true : false;
	
	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.wMDR = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.compl = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.lshift = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.rtoA = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.rtoB = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.rtoC = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.rtoD = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.rtoMDR = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.rtoMAR = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.memtoMDR = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.MDRtomem = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.MPCw1 = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.MPCwMIR8 = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.MPCA0 = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.MPCAneg = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.MPCwMDR4 = (c == '1') ? true : false;

	while ((c = fgetc(f)) != '1' && (c != '0') && (c != EOF))
		if (c == EOF)
			return nullinst();
	i.MPCwMPC = (c == '1') ? true : false;

	// Finally over
	return i;
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

