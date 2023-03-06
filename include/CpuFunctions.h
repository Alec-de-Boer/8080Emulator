#pragma once
#include "iostream"


class CpuFunctions
{
    public:
    int checkParity(uint8_t toCalculate);
    void UnimplementedInstruction(uint8_t memory);
    uint8_t MachineIn(uint8_t port);
    void MachineOut(uint8_t port, uint8_t value);
    
    uint16_t Interrupt(int interrupt_num, uint16_t pc);
};
