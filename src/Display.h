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

	bool _active = true;
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	SDL_Color _drawColor = SDL_Color{};
	SDL_Color _backgroundColor = SDL_Color{};
	byte _width = 0;
	byte _height = 0;
	byte _data[32][8]{};

	CPU *_cpu;
	
};
