#include "CPU.h"

CPU::CPU(const char* path)
{
	std::cout << "CPU Created" << std::endl;
	_RAM = new byte[4096];
	std::fill(_RAM, _RAM + 4096, 0);


	_GPR = new byte[16];
	std::fill(_GPR, _GPR + 16, 0);

	_soundReg = new Uint16();
	*_soundReg = 0x0;

	_timerReg = new Uint16();
	*_timerReg = 0x0;
	
	_PC = new Uint16();
	*_PC = 0x200;
	
	_SP = new byte();
	*_SP = 0x0;

	_stack = new Uint16[16];
	std::fill(_stack, _stack + 16, 0);

	_I = new Uint16();
	*_I = 0;


	byte* program = ProgramLoader::LoadProgram(path, &_programSize);
	
	if (_programSize <= (4096 - 0x200))
	{
		// Copy program to memory starting at address 0x200
		std::copy(program, program + _programSize, _RAM + 0x200);
	}
	else
	{
		std::cerr << "Error: Program size exceeds available memory." << std::endl;
	}
	free(program);

}

void CPU::Execute()
{
	/*************************************************************************************************************************\
	*	Instructions are ARM equivalent for comparision
	* ------------------------------------------------------------------------------------------------------------------------
	* ADD x,y		|		adds x and y registers and put the result in x register without carry flag being set
	* ADDS x,y		|		performs same as the ADD but sets the flag of carry
	/*************************************************************************************************************************/


	Uint16 instruction = (_RAM[*_PC] << 8) | _RAM[*_PC+1];
	bool shouldChangePC = true;
	Uint16 x, y, i, kk, nnn, result;
	byte random;
	switch (instruction) {
		case 0xe0:
			_displayStatus = DisplayOut::CLEAR;
			break;
		case 0xee:
			//return from subroutine
			//set PC to top of stack and subtract from stack pointer
			*_PC = _stack[*_SP];
			*_SP = *_SP-1;
			break;

		default:
			switch (instruction & 0xF000) {
				
				case 0x1000:
					//Instruction: 0nnn
					//bring PC to nnn
					*_PC = instruction & 0x0fff;
					shouldChangePC = false;
					break;
				case 0x2000:
					//Instruction: 2nnn
					//call a subroutine
					//increase SP, put PC on stack, set PC to nnn
					(*_SP)++;
					_stack[*_SP] = *_PC;
					*_PC = instruction & 0x0fff;
					shouldChangePC = false;
					break;
				case 0x3000:
					//Instruction: 3xkk
					//skip instruction if Vx == kk
					x = (instruction & 0x0f00)>>8;
					kk = (instruction & 0x00ff);
					if (_GPR[x] == kk)
						*_PC += 2;
					break;
				case 0x4000:
					//Instruction: 4xkk
					//skip instruction if Vx != kk
					x = (instruction & 0x0f00) >> 8;
					kk = (instruction & 0x00ff);
					if (_GPR[x] != kk)
						*_PC += 2;
					break;
				case 0x5000:
					//Instruction: 5xy0
					//skip instruction if Vx == Vy
					x = (instruction & 0x0f00) >> 8;
					y = (instruction & 0x00f0) >> 4;
					if (_GPR[x] == _GPR[y])
						*_PC += 2;
					break;
				case 0x6000:
					//Instruction: 6xkk
					//LD Vx, kk
					x = (instruction & 0x0f00) >> 8;
					kk = (instruction & 0x00ff);
					_GPR[x] = kk;
					break;
				case 0x7000:
					//Instruction: 7xkk
					//ADD Vx, kk
					x = (instruction & 0x0f00) >> 8;
					kk = (instruction & 0x00ff);
					_GPR[x] += kk;
					break;
				case 0x8000:
					//Instruction: 8xyi
					x = (instruction & 0x0f00) >> 8;
					y = (instruction & 0x00f0) >> 4;
					i = (instruction & 0x000f);
					switch (i) {
					case 0x0:
						//Instruction: 8xy0
						//LD Vx, Vy
						_GPR[x] = _GPR[y];
						break;
					case 0x1:
						//Instruction: 8xy1
						//OR Vx, Vy
						_GPR[x] |= _GPR[y];
						break;
					case 0x2:
						//Instruction: 8xy2
						//AND Vx, Vy
						_GPR[x] &= _GPR[y];
						break;
					case 0x3:
						//Instruction: 8xy3
						//XOR Vx, Vy
						_GPR[x] ^= _GPR[y];
						break;
					case 0x4:
						//Instruction: 8xy4
						//ADDS Vx, Vy
						result = (Uint16)_GPR[x] + (Uint16)_GPR[y];
						_GPR[GPRs::Vf] = 0x100 & result;
						//_GPR[GPRs::Vf] = 0;
						//if (result > 255)
						//	_GPR[GPRs::Vf] = 1;
						_GPR[x] = 0xff & result;
						break;
					case 0x5:
						//Instruction: 8xy5
						//SUBS Vx, Vy
						result = (Uint16)_GPR[x] - (Uint16)_GPR[y];
						_GPR[GPRs::Vf] = 0;
						if (_GPR[x] >= _GPR[y])
							_GPR[GPRs::Vf] = 1;
						_GPR[x] = 0xff & result;
						break;
					case 0x6:
						//Instruction: 8xy6
						//LSR Vx, #1
						_GPR[GPRs::Vf] = (_GPR[x] & 0x1);
						_GPR[x] >>= 1;
						break;
					case 0x7:
						//Instruction: 8xy7
						//SUBN Vx,Vy
						result = _GPR[y] - _GPR[x];
						_GPR[GPRs::Vf] = 0;
						if (_GPR[y] >= _GPR[x])
							_GPR[GPRs::Vf] = 1;
						_GPR[x] = 0xff & result;
						break;
					case 0xe:
						//Instruction: 8xye
						//LSL Vx,Vy
						_GPR[GPRs::Vf] = (_GPR[x] & 0x80);
						_GPR[x] <<= 1;
						break;
					}
					break;

				case 0x9000:
					//Instruction: 9xy0
					//skip instruction if Vx != Vy
					x = (instruction & 0x0f00) >> 8;
					y = (instruction & 0x00f0) >> 4;
					if (_GPR[x] != _GPR[y])
						*_PC += 2;
					break;

				case 0xa000:
					//Instruction: Annn
					//LD I,#nnn
					nnn = instruction & 0xfff;
					*_I = nnn;
					break;
				case 0xb000:
					//Instruction: Bnnn
					//ADD PC,V0,#nnn
					nnn = instruction & 0xfff;
					*_PC = nnn + _GPR[GPRs::V0];
					shouldChangePC = false;
					break;
				case 0xc000:
					//Instruction: Cxkk
					//Vx = random(0,255) + kk
					x = (instruction & 0x0f00) >> 8;
					kk = (instruction & 0x00ff);
					random = rand() % 256;
					_GPR[x] = kk + random;
					break;
				case 0xd000:
					//Instruction: Dxyi
					x = (instruction & 0x0f00) >> 8;
					y = (instruction & 0x00f0) >> 4;
					i = (instruction & 0x000f);
					_displayStatus = DisplayOut::DRAW;
					_drawCommand._x = _GPR[x];
					_drawCommand._y = _GPR[y];
					_drawCommand._n = i;
					break;
				case 0xe000:
					//TODO: Keyboard Instructions to be handled soon
					break;
				case 0xf000:
					//Instruction: Fxyi
					x = (instruction & 0x0f00) >> 8;
					y = (instruction & 0x00f0) >> 4;
					i = (instruction & 0x000f);
					switch (((y << 8) | i)) {
						case 0x07:
							//Instruction: Load Vx with the timer value
							//LD Vx, DT
							_GPR[x] = *_timerReg;
							break;
						case 0x0a:
							//Instruction: Wait for keypress and store key in Vx
							//TODO: Soon to be done
							break;
						case 0x15:
							//Set delay timer to Vx
							//LD DT, Vx
							*_timerReg = _GPR[x];
							break;
						case 0x18:
							//Set sound timer to Vx
							//LD ST, Vx
							*_soundReg = _GPR[x];
							break;
						case 0x1e:
							//set I to I + Vx
							//ADD I,Vx
							*_I += _GPR[x];
							break;
						case 0x29:
							//Set I to location of sprite of digit Vx
							//TODO: Add font in the interpreter area
							break;
						case 0x33:
							/*************************************************************
							*Stores BCD representation of Vx in consecutive I locations
							*			 I	 |	hundreds
							*			I+1	 |	twos
							*			I+2	 |	ones											
							***************************************************************/
							//TODO: Add font in the interpreter area
							break;
						case 0x55:
							//Instruction: Fx55
							//Store register V0 to Vx in memory starting from location I
							x = (instruction & 0xf00) >> 8;
							for (int i{}; i <= x; i++)
							{
								_RAM[*_I + i] = _GPR[i];
							}
							break;
						case 0x65:
							//Instruction: Fx65
							//Load register V0 to Vx in memory starting from location I
							x = (instruction & 0xf00) >> 8;
							for (int i{}; i <= x; i++)
							{
								_GPR[i] = _RAM[*_I + i];
							}
							break;
					}
					break;

			}
			break;
	}
	if (shouldChangePC)
		(*_PC)+=2;
}

void CPU::LogRAM()
{
	for (size_t i{}; i < 4096; i+=2)
	{
		size_t inst = (_RAM[i]<<8)| _RAM[i+1];
		std::cout << "Address: " << std::hex << "0x" << i << "\tInstruction: " << std::hex<<"0x"<< inst << std::endl;
	}
}

void CPU::LogGPR()
{
	for (size_t i{}; i < 16; i++)
	{	
		std::cout << "GPR: V" << i << "\t\tValue: " << std::hex << "0x" <<(int)_GPR[i] << std::endl;
	}
}

void CPU::LogSpcReg()
{
	std::cout << "PC: " << "\t\tValue: " << std::hex << "0x" << (int)*_PC << std::endl;
	std::cout << "I:  " << "\t\tValue: " << std::hex << "0x" << (int)*_I << std::endl;
	std::cout << "SP: " << "\t\tValue: " << std::hex << "0x" << (int)*_SP << std::endl;
	std::cout << "SR: " << "\t\tValue: " << std::hex << "0x" << (int)*_soundReg << std::endl;
	std::cout << "TR: " << "\t\tValue: " << std::hex << "0x" << (int)*_timerReg << std::endl;
}

CPU::~CPU()
{
	std::cout << "CPU Destroyed" << std::endl;
	delete _soundReg;
	delete _timerReg;
	delete _PC;
	delete _SP;
	delete[] _RAM;
	delete[] _GPR;
	delete[]  _stack;
}
