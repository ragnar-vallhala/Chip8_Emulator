#pragma once
#include "chip8.h"

class ProgramLoader
{
public:
	static byte* LoadProgram(const char* path, long *size);
};

