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




#pragma once
#include "chip8.h"
#include "ProgramLoader.h"


class CPU
{
public:
	CPU(const char* program);
	CPU()=default;
	~CPU();

	void Execute();
	void LogRAM();

	void LogGPR();
	void LogSpcReg();

	Uint16 getI() {
		return *_I;
	}

	byte GetRAM(size_t i)
	{
		return _RAM[i];
	}
	
	void setFlag(byte f) 
	{
		_GPR[GPRs::Vf] = f;
	}

	void SetKeyDown(byte key)
	{
		_keyDown = key;
	}

	DisplayOut _displayStatus{STANDBY};
	SoundOut _soundStatus{};
	TimerOut _timerOut{};
	DrawCommand _drawCommand{};

private:
	byte *_RAM;		//Must be 4096 byte long
	byte *_GPR;		//Must be 16 byte long
	Uint16 *_soundReg, *_timerReg;
	Uint16 *_PC;
	Uint16* _I;
	byte *_SP;
	Uint16 *_stack;	//Must be 16 16-bit long
	long _programSize{};
	std::chrono::steady_clock::time_point _lastSoundUpd = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point _lastTimerUpd = std::chrono::high_resolution_clock::now();
	byte* _symbols;		//A total of 16 symbols from 0 to F. Each symbol is 5 byte long.
	byte _keyDown = 0x10;
};

