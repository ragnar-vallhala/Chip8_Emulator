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