#pragma once
#include <iostream>

namespace State8080
{
    //loads chip8 file into memory
    uint8_t * LoadFile(const std::string& filename);
    
    //functions related to manipulation of allocated memory
    uint8_t& ReadMemory(unsigned int index, uint8_t *&memory);
    void WriteMemory(unsigned int index, int value, uint8_t *&memory);
}
