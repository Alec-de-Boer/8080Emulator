#pragma once
#include <iostream>

class CPU
{
    public:
    void Start(uint8_t *&memory, uint8_t *&framebuffer, bool firstRun);
    
    void ParseKeyPress(const std::string& input, uint8_t *&memory);
    
    void ParseKeyLift(const std::string &input, uint8_t *&memory);
};
