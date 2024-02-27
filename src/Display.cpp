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


#include "Display.h"

Display::Display(unsigned int width, unsigned int height, SDL_Color drawColor, SDL_Color backgroundColor, const char* path)
{
    std::cout << "Display Created" << std::endl;

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
    _cpu = new CPU(path);
    ClearDisplay();
}

Display::~Display()
{
    std::cout << "Display Destroyed" << std::endl;
    delete _cpu;
}


void Display::UpdateData()
{
    size_t DataBuffer = _cpu->getI();
    int x, y, h;
    x = _cpu->_drawCommand._x;
    y = _cpu->_drawCommand._y;
    h = _cpu->_drawCommand._n;

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
        byte input1Reg = (_cpu->GetRAM(DataBuffer + i) & (mask1Reg << bitsInXReg2)) >> bitsInXReg2;

        size_t checker = 0b1;
        for (size_t r{}; r < 8; r++)
        {
            if ((_data[yPos][xReg1] & checker) && (input1Reg & checker))
            {
                flag = 1;
                break;
            }
            checker <<= 1;
        }
        
        _data[yPos][xReg1] ^= input1Reg;

        byte mask2Reg = (0x1 << bitsInXReg2)-1;
        byte input2Reg = (_cpu->GetRAM(DataBuffer + i) & mask2Reg)<<bitsInXReg1;

        size_t checker2 = 0b1;
        for (size_t r{}; r < 8; r++)
        {
            if ((_data[yPos][xReg2] & checker2) && (input2Reg & checker2))
            {
                flag = 1;
                break;
            }
            checker2 <<= 1;
        }

        _data[yPos][xReg2] ^= input2Reg;
        _cpu->_displayStatus = DisplayOut::STANDBY;
        //std::cout<<"Inst: "<< std::hex <<(int)_cpu->GetRAM(DataBuffer+i) << " Height:" << i << "Mask1: " << (int)mask1Reg << " Reg1: " << (int)input1Reg << " Mask2: " << (int)mask2Reg << " Reg2: " << (int)input2Reg << "\n";
        /*LogDisplay();*/
    }
    if (flag)
        _cpu->setFlag(1);
    else
        _cpu->setFlag(0);


}

void Display::ClearDisplay()
{
    for (size_t i{}; i < 32; i++)
    {
        for (size_t j{}; j < 8; j++)
        {
            _data[i][j] = 0x0;
        }
    }
    _cpu->_displayStatus = DisplayOut::STANDBY;
}


void Display::display()
{
    while (_active)
    {
        SDL_Event event;
        long long diff{};
        UpdateData();
        
        bool isRunning = true;

        while (isRunning)
        {
            SDL_PollEvent(&event);
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    exit(0);
                    isRunning = false;
                    break;
            }

            Keyboard(event);
            
            auto start = std::chrono::high_resolution_clock::now().time_since_epoch().count();

            switch(event.type){
                case SDL_EVENT_QUIT:
                    _active = false;
                    break;
            }
            SDL_SetRenderDrawColor(_renderer, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
            SDL_RenderClear(_renderer);
            SDL_SetRenderDrawColor(_renderer, _drawColor.r, _drawColor.g, _drawColor.b, _drawColor.a);
            _cpu->Execute();
            if (_cpu->_displayStatus == DisplayOut::DRAW)
                UpdateData();
            if (_cpu->_displayStatus == DisplayOut::CLEAR) 
                ClearDisplay();
            Draw();
            SDL_Delay(2);
            //std::system("cls");
            /*std::cout << "************************************ RAM Coming ************************************\n";
            _cpu->LogRAM();*/
            /*std::cout << "************************************ GPR Coming ************************************\n";
            _cpu->LogGPR();
            std::cout << "********************************** SPReg Coming ************************************\n";
            _cpu->LogSpcReg();*/

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
    SDL_RenderPresent(_renderer);
}

void Display::Keyboard(SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) 
    {
        const char* key = SDL_GetKeyName(event.key.keysym.sym);
        if (key[0] == '2')
            _keyPressed = PhysicalKEY::PK2;
        else if (key[0] == '3')
            _keyPressed = PhysicalKEY::PK3;
        else if (key[0] == '4')
            _keyPressed = PhysicalKEY::PK4;
        else if (key[0] == '5')
            _keyPressed = PhysicalKEY::PK5;
        else if (key[0] == 'Q')
            _keyPressed = PhysicalKEY::PKQ;
        else if (key[0] == 'W')
            _keyPressed = PhysicalKEY::PKW;
        else if (key[0] == 'E')
            _keyPressed = PhysicalKEY::PKE;
        else if (key[0] == 'R')
            _keyPressed = PhysicalKEY::PKR;
        else if (key[0] == 'A')
            _keyPressed = PhysicalKEY::PKA;
        else if (key[0] == 'S')
            _keyPressed = PhysicalKEY::PKS;
        else if (key[0] == 'D')
            _keyPressed = PhysicalKEY::PKD;
        else if (key[0] == 'F')
            _keyPressed = PhysicalKEY::PKF;
        else if (key[0] == 'Z')
            _keyPressed = PhysicalKEY::PKZ;
        else if (key[0] == 'X')
            _keyPressed = PhysicalKEY::PKX;
        else if (key[0] == 'C')
            _keyPressed = PhysicalKEY::PKC;
        else if (key[0] == 'V')
            _keyPressed = PhysicalKEY::PKV;
    }
    else 
    {
        _keyPressed = 0x10;
    }

    _cpu->SetKeyDown(_keyPressed);
}
