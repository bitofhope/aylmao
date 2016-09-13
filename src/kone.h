/*
 * This program is free software. See COPYING for details.
 */

void initMachine(Machine* m, Instruction* mpm, int16_t* mm);

void clock1(Machine* m);
void clock2(Machine* m);
void clock3(Machine* m);
void clock4(Machine* m);
void clock5(Machine* m);

int16_t MDRtoaddr(int16_t);
uint8_t instrtoaddr(Instruction);

void printState(Machine m);

