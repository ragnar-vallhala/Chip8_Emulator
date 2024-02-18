#include <iostream>
#include "Display.h"
#include "SDL3\SDL.h"

int main(int argc, char* args[]){

	std::cout << "Chip_8" << std::endl;
	
	SDL_Color c1{ 0,0,0,255 }, c2{100,150,50,255};
	Display display = Display(1024, 512, c2, c1, "C:\\Users\\Asus\\Documents\\Projects\\Chip8_Emulator\\roms\\BRIX");

	display.display();

	return 0;
}