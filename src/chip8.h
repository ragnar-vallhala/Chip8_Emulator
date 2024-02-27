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

typedef unsigned char byte;
#include <iostream>
#include <cstdlib> 
#include <math.h>
#include <chrono>
#include "SDL3/SDL.h"


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


typedef enum chipKey {
	VK0,			//X
	VK1,			//2
	VK2,			//3
	VK3,			//4
	VK4,			//Q
	VK5,			//W
	VK6,			//E
	VK7,			//A
	VK8,			//S
	VK9,			//D
	VKa,			//Z
	VKb,			//C
	VKc,			//5
	VKd,			//R
	VKe,			//F
	VKf				//V
}VirtualKEY;

typedef enum Key {
	PKX,			//K0	
	PK2,			//K1	
	PK3,			//K2	
	PK4,			//K3	
	PKQ,			//K4	
	PKW,			//K5	
	PKE,			//K6	
	PKA,			//K7	
	PKS,			//K8	
	PKD,			//K9	
	PKZ,			//Ka	
	PKC,			//Kb	
	PK5,			//Kc	
	PKR,			//Kd	
	PKF,			//Ke	
	PKV				//Kf	
}PhysicalKEY;

typedef enum disp{
	CLEAR,
	STANDBY,
	DRAW,
}DisplayOut;

typedef enum sound{

}SoundOut;

typedef enum timer{

}TimerOut;


typedef struct draw{
	Uint16 _x{}, _y{}, _n{};
}DrawCommand;