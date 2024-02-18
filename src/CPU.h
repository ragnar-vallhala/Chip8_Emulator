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
};

