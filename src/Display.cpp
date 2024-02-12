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
    
    for (size_t i{}; i < 32; i++)
    {
        for (size_t j{}; j < 8; j++)
        {
            _data[i][j] = 0x0;
        }
    }
}

byte A[5]{ 0x18, 0x24, 0x3c, 0x24, 0x24 };
int x = 25, y = 29, h=5;

void Display::UpdateData()
{
    byte* DataBuffer = A;


    bool flag = false;
    size_t xReg1 = x / 8;
    size_t bitsInXReg1 = 8 - x % 8;
    size_t xReg2 = xReg1 + 1;
    size_t bitsInXReg2 = x % 8;

    xReg1 %= 8;
    xReg2 %= 8;


    for (size_t i{}; i < h; i++)
    {
        size_t yPos = (y + i) % 32;

        byte mask1Reg = (0x1 << bitsInXReg1) - 1;
        byte input1Reg = (DataBuffer[i] & (mask1Reg << bitsInXReg2))>>bitsInXReg2;

        if(_data[yPos][xReg1]!= input1Reg)
        {
            _data[yPos][xReg1] ^= input1Reg;
        }

        byte mask2Reg = (0x1 << bitsInXReg2)-1;
        byte input2Reg = (DataBuffer[i] & mask2Reg)<<bitsInXReg1;

        if (_data[yPos][xReg2] != input2Reg)
        {
            _data[yPos][xReg2] ^= input2Reg;
        }

        //std::cout<<"Height:" << i << "Mask1: " << (int)mask1Reg << " Reg1: " << (int)input1Reg << " Mask2: " << (int)mask2Reg << " Reg2: " << (int)input2Reg << "\n";
    }


}


void Display::display()
{
    
    while (_active)
    {
        SDL_Event event;
        long long diff{};

        while (SDL_PollEvent(&event))
        {
            auto start = std::chrono::high_resolution_clock::now().time_since_epoch().count();

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

            diff = std::chrono::high_resolution_clock::now().time_since_epoch().count() - start;
        }
#ifdef LOG_FPS
        if(diff>0)
            std::cout << (1e9/(double)diff)<<"\r";
#endif
    }
    SDL_DestroyWindow(_window);
    SDL_Quit();
        
}

void Display::LogDisplay()
{
    for (int p{}; p < 32; p++)
    {
        for (int j{}; j < 8; j++)
        {
            int num = 0b10000000;
            for (int i{}; i < 8; i++)
            {
                if (_data[p][j] & num) {
                    std::cout << 1;
                }
                else {
                    std::cout << 0;
                }
                num >>= 1;
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }

}

void Display::DrawPixel(size_t x, size_t y)
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
    for (size_t i{}; i < 32; i++)
    {
        for (size_t j{}; j < 8; j++)
        {
            Uint32 bitVal = 0b10000000;

            for (size_t k{}; k < 8; k++)
            {
                if (bitVal & _data[i][j])
                {
                    DrawPixel(j*8+k,i);     
                }
                bitVal >>= 1;
            }
        }
    }
}
