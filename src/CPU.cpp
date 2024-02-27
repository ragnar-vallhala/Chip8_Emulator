/*
 * Chip-8 Interpreter
 * Author: Ashutosh Vishwakarma
 * Year: 2024
 *
 * Description:
 *   This C++ program is a simple Chip-8 interpreter, capable of running Chip-8 programs.
 *
 * License:
 *   MIT License
 *
 *   Copyright (c) 2024 Ashutosh Vishwakarma
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */


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


	_symbols = new byte[16 * 5];	//16 symbols; 5 byte long each
	{

		//0							//1							//2									//3							//4							//5
		_symbols[0] = 0xf0;			_symbols[5] = 0x20;			_symbols[10] = 0xf0;			_symbols[15] = 0xf0;			_symbols[20] = 0x90;			_symbols[25] = 0xf0;
		_symbols[1] = 0x90;			_symbols[6] = 0x60;			_symbols[11] = 0x10;			_symbols[16] = 0x10;			_symbols[21] = 0x90;			_symbols[26] = 0x80;
		_symbols[2] = 0x90;			_symbols[7] = 0x20;			_symbols[12] = 0xf0;			_symbols[17] = 0xf0;			_symbols[22] = 0xf0;			_symbols[27] = 0xf0;
		_symbols[3] = 0x90;			_symbols[8] = 0x20;			_symbols[13] = 0x80;			_symbols[18] = 0x10;			_symbols[23] = 0x10;			_symbols[28] = 0x10;
		_symbols[4] = 0xf0;			_symbols[9] = 0x70;			_symbols[14] = 0xf0;			_symbols[19] = 0xf0;			_symbols[24] = 0x10;			_symbols[29] = 0xf0;


		//6									//7								//8								//9							//A								//B
		_symbols[30] = 0xf0;			_symbols[35] = 0xf0;			_symbols[40] = 0xf0;			_symbols[45] = 0xf0;			_symbols[50] = 0xf0;			_symbols[55] = 0xe0;
		_symbols[31] = 0x80;			_symbols[36] = 0x10;			_symbols[41] = 0x90;			_symbols[46] = 0x90;			_symbols[51] = 0x90;			_symbols[56] = 0x90;
		_symbols[32] = 0xf0;			_symbols[37] = 0x20;			_symbols[42] = 0xf0;			_symbols[47] = 0xf0;			_symbols[52] = 0xf0;			_symbols[57] = 0xe0;
		_symbols[33] = 0x90;			_symbols[38] = 0x40;			_symbols[43] = 0x90;			_symbols[48] = 0x10;			_symbols[53] = 0x90;			_symbols[58] = 0x90;
		_symbols[34] = 0xf0;			_symbols[39] = 0x40;			_symbols[44] = 0xf0;			_symbols[49] = 0xf0;			_symbols[54] = 0x90;			_symbols[59] = 0xe0;


		//C									//D									//E							//F							
		_symbols[60] = 0xf0;			_symbols[65] = 0xe0;			_symbols[70] = 0xf0;			_symbols[75] = 0xf0;			
		_symbols[61] = 0x80;			_symbols[66] = 0x90;			_symbols[71] = 0x80;			_symbols[76] = 0x80;			
		_symbols[62] = 0x80;			_symbols[67] = 0x90;			_symbols[72] = 0xf0;			_symbols[77] = 0xf0;			
		_symbols[63] = 0x80;			_symbols[68] = 0x90;			_symbols[73] = 0x80;			_symbols[78] = 0x80;			
		_symbols[64] = 0xf0;			_symbols[69] = 0xe0;			_symbols[74] = 0xf0;			_symbols[79] = 0x80;			

	}

	for (size_t i{}; i < 80; i++)
	{
		_RAM[100 + i] = _symbols[i];		//symbols start in RAM from address 0x100
	}

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
	byte ones;
	byte twos;
	byte hundreds;

	/******************************************************************************************************\
	*	Sound and Timer Registers are upadted below at a 60Hz frequency
	*	---------------------------------------------------------------------------------------------------
	*	_soundReg	|	ST
	*	_timerReg	|	DT
	\******************************************************************************************************/

	std::chrono::steady_clock::time_point currTime = std::chrono::high_resolution_clock::now();
	if (*_soundReg > 0 && std::chrono::duration<double>((_lastSoundUpd - currTime)).count() > 16.66667)
	{
		_lastSoundUpd = currTime;
		(*_soundReg)--;
		std::cout << "Sound: " << (int)*_soundReg << "\n";
	}

	if (*_timerReg > 0 && std::chrono::duration<double>((_lastTimerUpd - currTime)).count() > 16.66667)
	{
		_lastTimerUpd = currTime;
		(*_timerReg)--;
		std::cout << "Timer: " << (int)*_timerReg << "\n";
	}


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
					x = (instruction & 0x0f00) >> 8;
					nnn = (instruction & 0x00ff);
					switch (nnn)
					{
						case 0x9e:
							if (_GPR[x] == _keyDown)
							{
								(*_PC) += 2;
								shouldChangePC = false;
							}
							break;
						case 0xa1:
							if (_GPR[x] != _keyDown)
							{
								(*_PC) += 2;
								shouldChangePC = false;
							}
							break;
					}
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
							if (_keyDown != 0x10)
								_GPR[x] = _keyDown;
							else
								shouldChangePC = false;
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
							//digit sprite starts at 0x100
							*_I = 100 + _GPR[x] * 5;			// offset 100 and factor 5

							break;
						case 0x33:
							/*************************************************************
							*Stores BCD representation of Vx in consecutive I locations
							*			 I	 |	hundreds
							*			I+1	 |	twos
							*			I+2	 |	ones											
							***************************************************************/
							kk = _GPR[x];
							ones = kk % 10;
							twos = (kk / 10) % 10;
							hundreds = (kk / 100) % 10;
							_RAM[*_I]	=	_RAM[100 + 5 * hundreds];
							_RAM[*_I + 1] = _RAM[100 + 5 * twos];
							_RAM[*_I + 2] = _RAM[100 + 5 * ones];
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
	delete[] _symbols;
}
