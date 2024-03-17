# Chip8 Emulator
This is a fun project to learn and practise the basics of emulation and hands on with some machine code. The emulator is built in C++ using SDL. The project uses SDL for all system operation, windowing, handling sound ( *not yet implemented* ), input handling, etc.

The project built on VS Community 2022.

Example ROMs are the roms directory. The required dll is attached in the release section.  You can use any other rom.

## Build and Run the Project:
You need Visual Studio to build and run the project.

- Clone the respository
```
git clone https://github.com/ragnar-vallhala/Chip8_Emulator.git
```
- Open the sln file of the project.
```
cd Chip8_Emulator
./Chip8_Emulator.sln
```
- Build the project with Debug or Release configuration ( *depends on your needs* ).
- You neet to add the ```SDL3.dll``` to the directory containing the ```Chip8_Emulator.exe```. You may find it online, build it from source or download from the release section of the repository.
- To run any rom on emulator, drag and drop the rom the ```Chip8_Emulator.exe``` or use command line as ```./Chip8_Emulator.exe [path to ROM]```. 

## Key mapping

### Chip8 Keypad

| 1 | 2 | 3 | C |
| - |:-:|:-:|:-:|     
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |

### Keyboard map
| 2 | 3 | 4 | 5 |
| - |:-:|:-:|:-:|     
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |

# Refrences
[CHIP8 - Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)\
[Cowgod's Chip-8](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

# Developer
Ashutosh Vishwakarma\
BTech  CSE Student, IIT Jammu\
[LinkdIn](https://www.linkedin.com/in/ashutosh-vishwakarma-083305257/)\
[E-Mail](mailto:ashutoshvishwakarma208@gmail.com)