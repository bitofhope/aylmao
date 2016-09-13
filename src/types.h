/*
 * This program is free software. See COPYING for details.
 */

typedef uint8_t byte;

// Contain's a single instruction's bits
typedef struct instruction
{
	// Clock 1
	// Writing to DC2
	bool wA       : 1;
	bool wB       : 1;
	bool wC       : 1;
	bool wD       : 1;
	//Writing to DC1
	bool w1       : 1;
	bool wMDR     : 1;
	bool compl    : 1;
	bool lshift   : 1;
	
	// Clock 2
	bool rtoA     : 1;
	bool rtoB     : 1;
	bool rtoC     : 1;
	bool rtoD     : 1;
	bool rtoMDR   : 1;
	bool rtoMAR   : 1;

	// Clock 3
	bool memtoMDR : 1;
	bool MDRtomem : 1;

	// Clock 4
	// Writing to DC1
	bool MPCw1    : 1;
	bool MPCwMIR8 : 1;
	bool MPCA0    : 1;
	bool MPCAneg  : 1;
	bool MPCwMDR4 : 1;
	// Writing to DC2
	bool MPCwMPC  : 1;
} Instruction;

typedef struct machine
{
	// Data buses
	int16_t DC1;
	int16_t DC2;
	int16_t DC3;
	Instruction CC;

	
	// GP Registers
	int16_t reg_A;
	int16_t reg_B;
	int16_t reg_C;
	int16_t reg_D;

	// Special registers
	Instruction   reg_MIR;
	int16_t       reg_MDR;
	uint8_t       reg_MPC;
	uint16_t reg_MAR : 12;

	// Memories
	Instruction* MPM; // Size 256
	int16_t*     MM;  // Size 4096
} Machine;

