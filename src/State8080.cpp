#include <fstream>
#include "State8080.h"
#include "iostream"
#include <cstring>


uint8_t * State8080::LoadFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cout << "couldn't open rom file";
        throw std::runtime_error("rom not found");
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    auto* buffer = new uint8_t[65000];
    memset(buffer, 0, 65000);
    file.read((char*)buffer, 65000);
    uint8_t *memory = buffer;
    return memory;
}

uint8_t& State8080::ReadMemory(unsigned int index, uint8_t *&memory)
{
    if (index > 4096)
    {
        throw std::runtime_error("index exceeds allocated memory size");
    }
    else
    {
        return memory[index];
    }
}

void State8080::WriteMemory(unsigned int index, int value, uint8_t *&memory)
{
    if (index > 4096)
    {
        throw std::runtime_error("index exceeds allocated memory size");
    }
    else
    {
        memory[index] = value;
        return;
    }
}
