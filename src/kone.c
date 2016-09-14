/*
 * This program is free software. See COPYING for details.
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "kone.h"

void initMachine(Machine* m, Instruction* mpm, int16_t* mm)
{
	memset(m, 0, sizeof(Machine));
	m->MPM = mpm;
	m->MM  = mm;
}

void clock1(Machine* m)
{
	m->DC1 = 0;
	m->DC2 = 0;
	m->DC3 = 0;

	if (m->reg_MIR.w1)
		m->DC1 = 1;
	if (m->reg_MIR.wMDR)
		m->DC1 = m->reg_MDR;
	
	if (m->reg_MIR.wA)
		m->DC2 = m->reg_A;
	if (m->reg_MIR.wB)
		m->DC2 = m->reg_B;
	if (m->reg_MIR.wC)
		m->DC2 = m->reg_C;
	if (m->reg_MIR.wD)
		m->DC2 = m->reg_D;

	if (m->reg_MIR.compl)
		m->DC1 = -(m->DC1);

	m->DC3 = m->DC1 + m->DC2;

	if (m->reg_MIR.lshift)
		m->DC3 = m->DC3 << 1;
}

void clock2(Machine* m)
{
	if (m->reg_MIR.rtoA)
		m->reg_A = m->DC3;
	if (m->reg_MIR.rtoB)
		m->reg_B = m->DC3;
	if (m->reg_MIR.rtoC)
		m->reg_C = m->DC3;
	if (m->reg_MIR.rtoD)
		m->reg_D = m->DC3;

	if (m->reg_MIR.rtoMDR)
		m->reg_MDR = m->DC3;
	if (m->reg_MIR.rtoMAR)
		m->reg_MAR = m->DC3;
}

void clock3(Machine* m)
{
	if (m->reg_MIR.MDRtomem)
		m->MM[m->reg_MAR] = m->reg_MDR;

	if (m->reg_MIR.memtoMDR)
		m->reg_MDR = m->MM[m->reg_MAR];
}

void clock4(Machine* m)
{
	m->DC1 = 0;
	m->DC2 = 0;
	m->DC3 = 0;

	if (m->reg_MIR.MPCw1)
		m->DC1 = 1;
	if (m->reg_MIR.MPCwMIR8)
		m->DC1 = instrtoaddr(m->reg_MIR);
	if (m->reg_MIR.MPCA0)
		m->DC1 = (m->reg_A == 0) ? 1 : 2;
	if (m->reg_MIR.MPCAneg)
		m->DC1 = (m->reg_A < 0)  ? 1 : 2;
	if (m->reg_MIR.MPCwMDR4)
		m->DC1 = MDRtoaddr(m->reg_MDR);

	if (m->reg_MIR.MPCwMPC)
		m->DC2 = m->reg_MPC;

	m->DC3 = m->DC1 + m->DC2;
}

void clock5(Machine* m)
{
	m->reg_MPC = m->DC3;
	m->reg_MIR = m->MPM[m->reg_MPC];
}

int16_t MDRtoaddr(int16_t mdr)
{
	return mdr >> 12;
}

uint8_t instrtoaddr(Instruction i)
{
	uint8_t ret = 0;
	ret += i.wA << 7;
	ret += i.wB << 6;
	ret += i.wC << 5;
	ret += i.wD << 4;
	ret += i.w1 << 3;
	ret += i.wMDR << 2;
	ret += i.compl << 1;
	ret += i.lshift;
	return ret;
}

void printState(Machine m)
{
	char* inst = insttostr(m.reg_MIR);
	printf(
"Current instruction: %s\n"
"Registers:\n"        \
"----------\n"        \
"A:\t%" PRId16 "\n"   \
"B:\t%" PRId16 "\n"   \
"C:\t%" PRId16 "\n"   \
"D:\t%" PRId16 "\n"   \
"\n"                  \
"MPC:\t%" PRIu8  "\n" \
"MDR:\t%" PRId16 "\n" \
"MAR:\t%" PRIu16 "\n" \
"----------\n"        \
"\n",
	    inst, m.reg_A, m.reg_B, m.reg_C, m.reg_D,
		m.reg_MPC, m.reg_MDR, m.reg_MAR);

	free(inst);
}

char* insttostr(Instruction i)
{
	char* ret = (char*)calloc(sizeof('0'), 23);
	
	ret[0]  = i.wA       ? '1' : '0';
	ret[1]  = i.wB       ? '1' : '0';
	ret[2]  = i.wC       ? '1' : '0';
	ret[3]  = i.wD       ? '1' : '0';
	ret[4]  = i.w1       ? '1' : '0';
	ret[5]  = i.wMDR     ? '1' : '0';
	ret[6]  = i.compl    ? '1' : '0';
	ret[7]  = i.lshift   ? '1' : '0';
	ret[8]  = i.rtoA     ? '1' : '0';
	ret[9]  = i.rtoB     ? '1' : '0';
	ret[10] = i.rtoC     ? '1' : '0';
	ret[11] = i.rtoD     ? '1' : '0';
	ret[12] = i.rtoMDR   ? '1' : '0';
	ret[13] = i.rtoMAR   ? '1' : '0';
	ret[14] = i.memtoMDR ? '1' : '0';
	ret[15] = i.MDRtomem ? '1' : '0';
	ret[16] = i.MPCw1    ? '1' : '0';
	ret[17] = i.MPCwMIR8 ? '1' : '0';
	ret[18] = i.MPCA0    ? '1' : '0';
	ret[19] = i.MPCAneg  ? '1' : '0';
	ret[20] = i.MPCwMDR4 ? '1' : '0';
	ret[21] = i.MPCwMPC  ? '1' : '0';

	return ret;
}

