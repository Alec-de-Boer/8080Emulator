#include "SDL.h"
#include "../include/Emulator.h"
#include "../include/State8080.h"
#include "../include/CPU.h"


int main(int argv, char** args)
{
    uint8_t *memory = State8080::LoadFile("rom/rom.h");
    auto *framebuffer = new uint8_t[0x1BFF];
    memset(framebuffer, 0, 0x1BFF);
    time_t timer;
    
    
    Emulator emulator;
    CPU cpu;
    emulator.initSystems();
    auto firstRun = true;
    auto frameready = true;
    
    while (emulator._emulatorState != EmulatorState::EXIT)
    {
        //auto start = time(timer);
        //int n = (2 * 1000000);
        //while (n > 0)
        //{
            emulator.processInput(memory);
            if (frameready)
            {
                frameready = true;
                emulator.DrawImage(framebuffer);
            }
            cpu.Start(memory, framebuffer, firstRun);
            firstRun = false;
            //n = n - cycles;
        //}
        //sleep time amount
    }
    return 0;
}
