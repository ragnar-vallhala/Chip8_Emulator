#include "Display.h"

Display::Display(unsigned int width, unsigned int height, SDL_Color drawColor, SDL_Color backgroundColor)
{

    if (SDL_Init(0xFFFFFF) != 0)
    {
        std::cout << "Failed to start SDL " << SDL_GetError() << std::endl;
        return;
    }

    _window = SDL_CreateWindow(
        "Chip 8",
        width,
        height,
        0);
    _renderer = SDL_CreateRenderer(
        _window, 
        NULL, 
        SDL_RENDERER_ACCELERATED
    );

    _drawColor = drawColor;
    _backgroundColor = backgroundColor;
    _width = width;
    _height = height;
}

void Display::UpdateData()
{


    for (int i{}; i < 32 -2; i += 5)
    {
        for (int j{}; j < 64 - 1; j += 5)
        {
            _data[i + 1][j + 1] = true;
            _data[i + 1][j + 3] = true;
            _data[i + 3][j + 1] = true;
            _data[i + 3][j + 3] = true;
            _data[i + 4][j + 1] = true;
            _data[i + 4][j + 2] = true;
            _data[i + 4][j + 3] = true;
        }
    }

}


void Display::display()
{
    
    while (_active)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type){
                case SDL_EVENT_QUIT:
                    _active = false;
                    break;
            }
            SDL_SetRenderDrawColor(_renderer, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
            SDL_RenderClear(_renderer);
            SDL_SetRenderDrawColor(_renderer, _drawColor.r, _drawColor.g, _drawColor.b, _drawColor.a);
            UpdateData();
            Draw();
            SDL_RenderPresent(_renderer);
            
        }
    }
    SDL_DestroyWindow(_window);
    SDL_Quit();
        
}

void Display::DrawPixel(int x, int y)
{
    int pixelEdge = 16;
    x = x % 64, y = y % 32;
    SDL_FRect rect{
        (float)x*pixelEdge,
        (float)y*pixelEdge,
        (float)pixelEdge,
        (float)pixelEdge
    };
    SDL_RenderFillRect(_renderer, &rect);

}

void Display::Draw()
{
    for (int i{}; i < 32; i++)
    {
        for (int j{}; j < 64; j++)
        {
            if (_data[i][j])
                DrawPixel(j,i);
        }
    }

}
