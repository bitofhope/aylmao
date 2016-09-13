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
	m->DC2 = 0;

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
	m->reg_MPC = m->DC1;
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
	printf(
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
	    m.reg_A, m.reg_B, m.reg_C, m.reg_D, m.reg_MPC, m.reg_MDR, m.reg_MAR);
}

