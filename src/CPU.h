#pragma once
#include "chip8.h"

typedef enum {
	V0,
	V1,
	V2,
	V3,
	V4,
	V5,
	V6,
	V7,
	V8,
	V9,
	Va,
	Vb,
	Vc,
	Vd,
	Ve,
	Vf
} GPRs;



class CPU
{
	CPU();

	~CPU();

private:
	byte *_RAM;		//Must be 4096 byte long
	byte *_GPR;		//Must be 16 byte long
	Uint16 *_soundReg, *_timerReg;
	Uint16 *_PC;
	byte *_SP;
	Uint16 *_stack;	//Must be 16 16-bit long
};

