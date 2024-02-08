#pragma once
#include "SDL3/SDL.h"
#include <iostream>
class Display
{
public:
	Display(unsigned int width, unsigned int height, SDL_Color drawColor, SDL_Color backgroundColor);
	void UpdateData();
	void display();
	
private:
	void DrawPixel(int x, int y);
	void Draw();

	bool _active = true;
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	SDL_Color _drawColor = SDL_Color{};
	SDL_Color _backgroundColor = SDL_Color{};
	unsigned int _width = 0;
	unsigned int _height = 0;
	bool _data[32][64]{};
	
};
