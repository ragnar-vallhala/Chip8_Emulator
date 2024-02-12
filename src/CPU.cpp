#include "CPU.h"

CPU::CPU()
{
	_RAM = (byte*)malloc(4096);
	_GPR = (byte*)malloc(16);
	_soundReg = new Uint16();
	_timerReg = new Uint16();
	_PC = new Uint16();
	_SP = new byte();
	_stack = (Uint16*)malloc(sizeof(Uint16)*16);
}

CPU::~CPU()
{
	delete _soundReg, _timerReg, _PC, _SP;
	free(_RAM);
	free(_GPR);
	free(_stack);
}
