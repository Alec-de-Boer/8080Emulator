#include "CpuFunctions.h"
#include "iostream"

uint8_t shift0;
uint8_t shift1;
uint8_t shift_offset;

//Calculates the parity of an 8-bit number.
int CpuFunctions::checkParity(uint8_t toCalculate)
{
        toCalculate ^= toCalculate >> 4;
        toCalculate ^= toCalculate >> 2;
        toCalculate ^= toCalculate >> 1;
        return (~toCalculate) & 1;
}

//Logs an error if an opcode not implemented is executed (in decimal).
void CpuFunctions::UnimplementedInstruction(uint8_t memory)
{
    std::cout << "Error: Unimplemented instruction at: " << memory;
}

uint8_t CpuFunctions::MachineIn(uint8_t port)
{
    uint8_t a;
    switch (port) //READ
    {
        case 3:
            uint16_t v = (shift1<<8) | shift0;
            a = ((v >> (8-shift_offset)) & 0xff);
    }
    return a;
}

void CpuFunctions::MachineOut(uint8_t port, uint8_t value)
{
    switch (port) //WRITE
    {
        case 2:
            shift_offset = value & 0x7;
            break;
        case 4:
            shift0 = shift1;
            shift1 = value;
            break;
    }
}

uint16_t CpuFunctions::Interrupt(int interrupt_num, uint16_t pc)
{
    pc = (pc & 0);
    pc = 8 * interrupt_num;
    return pc;
}
