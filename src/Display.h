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
#include "CPU.h"


#define LOG_FPS

class Display
{
public:
	Display(unsigned int width, unsigned int height, SDL_Color drawColor, SDL_Color backgroundColor, const char* path);
	~Display();
	void UpdateData();
	void ClearDisplay();
	void display();
	void LogDisplay();
	
private:
	void DrawPixel(size_t x, size_t y);
	void Draw();
	void Keyboard(SDL_Event &event);
	bool _active = true;
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	SDL_Color _drawColor = SDL_Color{};
	SDL_Color _backgroundColor = SDL_Color{};
	byte _width = 0;
	byte _height = 0;
	byte _data[32][8]{};
	byte _keyPressed = 0x10;		//No key pressed
	CPU *_cpu;
};
