#include "iostream"
#include "CpuFunctions.h"
#include "CPU.h"
#include <chrono>
#include <thread>
#include <fstream>


 uint8_t z = 0; //True (1) when equal to zero.
 uint8_t s = 0; //True when MSB is set.
 uint8_t p = 0;  //True when result is of even parity.
 uint8_t cy = 0; //True when instruction resulted in a carry-out or borrow into the high order bit.
 uint8_t ac = 0; //Not used by Space Invaders, still needs to be implemented for any 8080 games using it.
 uint8_t pad = 0;

 uint8_t a = 0;
 uint8_t b = 0;
 uint8_t c = 0;
 uint8_t d = 0;
 uint8_t e = 0;
 uint8_t h = 0;
 uint8_t l = 0;
 uint16_t sp = 0;
 uint16_t pc = 0;
 uint8_t int_enable = 0;
 int interrupt_num = 1;
 
 long long int lastInterrupt;
 
 uint8_t controllerByte;
 
 uint16_t ret;
 time_t timer;
 uint8_t port;

std::ofstream out("debug.txt");

 
 
 CpuFunctions cpuFunctions;

void CPU::Start(uint8_t *&memory, uint8_t *&framebuffer, bool firstRun)
 {
     std::string assemblyCode;
     uint16_t answer16;
     uint16_t offset16;
     uint16_t vramoffset16;
     auto debug = true;
    
     if (firstRun)
     {
         lastInterrupt = time(&timer);
         std::this_thread::sleep_for(std::chrono::seconds(2/60));
     }
     else
     {
         if (time(&timer) - lastInterrupt >= 1.0/60) //1 frame passed
         {
             if (int_enable)
             {
                 pc = cpuFunctions.Interrupt(interrupt_num, pc);
                 if (interrupt_num == 1)
                 {
                     interrupt_num = 2;
                 }
                 else
                 {
                     interrupt_num = 1;
                 }
                 lastInterrupt = time(&timer);
             }
         }
     }
     
     
     switch (memory[pc])
     {
         case 0x00:
             assemblyCode = "NOP";
             break;
         case 0x01:
             c = memory[pc + 1];
             b = memory[pc + 2];
             pc +=2;
             assemblyCode = "LXI B,D16";
             break;
         case 0x02:
             offset16 = (b << 8) | c;
             if (offset16 >= 0x2000)
             {
                vramoffset16 = offset16 - 0x2000;
                framebuffer[vramoffset16] = a;
                memory[offset16] = a;
             }
             else
             {
                 memory[offset16] = a;
             }
             break;
         case 0x03:
             answer16 = ((b << 8) | c) + 1;
             b = (answer16 >> 8);
             c = (answer16 & 0xff);
             assemblyCode = "INX B";
             break;
         case 0x04: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x05:
             b -= 1;
             z = (b == 0);
             s = ((b & 0x80) != 0);
             cy = (b > 0xff);
             p = cpuFunctions.checkParity(b);
             assemblyCode = "DCR B";
             break;
         case 0x06:
             b = memory[pc + 1];
             pc+=1;
             assemblyCode = "MVI B, D8";
             break;
         case 0x07: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x08: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x09: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x0a: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x0b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x0c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x0d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x0e: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x0f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x10: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x11:
             d = memory[pc + 2];
             e = memory[pc + 1];
             pc+=2;
             assemblyCode = "LXI D,D16";
             break;
         case 0x12: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x13:
             answer16 = ((d << 8) | e) + 1;
             d = (answer16 >> 8);
             e = (answer16 & 0xff);
             assemblyCode = "INX D";
             break;
         case 0x14: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x15: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x16: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x17: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x18: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x19:
             answer16 = ((h<<8) | l) + ((d<<8) | e);
             cy = (answer16 > 0xff);
             h = (answer16 >> 8);
             l = (answer16 & 0xff);
             assemblyCode = "DAD D";
             break;
         case 0x1a:
             offset16 = (d << 8) | e;
             a = memory[offset16];
             assemblyCode = "LDAX D";
             break;
         case 0x1b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x1c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x1d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x1e: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x1f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x20: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x21:
             h = memory[pc + 2];
             l = memory[pc + 1];
             pc +=2;
             assemblyCode = "LXI H,D16";
             break;
         case 0x22: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x23:
             answer16 = ((h<<8) | l) + 1;
             h = (answer16>>8);
             l = (answer16 & 0xff);
             assemblyCode = "INX H";
             break;
         case 0x24: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x25: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x26: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x27: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x28: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x29: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x2a: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x2b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x2c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x2d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x2e: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x2f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x30: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x31:
             sp = (memory[pc + 2]<< 8) | memory[pc + 1];
             pc+=2;
             assemblyCode = "LXI SP, D16";
             break;
         case 0x32:
             offset16 = (memory[pc + 2] << 8) | memory[pc + 1];
             if (offset16 >= 0x2000)
             {
                 vramoffset16 = offset16 - 0x2000;
                 framebuffer[vramoffset16] = a;
                 memory[offset16] = a;
             }
             else
             {
                 memory[offset16] = a;
             }
             pc+=2;
             assemblyCode = "STA adr";
             break;
         case 0x33: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x34: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x35: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x36:
             offset16 = (h << 8) | l;
             if (offset16 >= 0x2000)
             {
                 vramoffset16 = offset16 - 0x2000;
                 framebuffer[vramoffset16] = memory[pc + 1];;
                 memory[offset16] = memory[pc + 1];;
             }
             else
             {
                 memory[offset16] = memory[pc + 1];;
             }
             pc+=1;
             assemblyCode = "MVI M,D8";
             break;
         case 0x37: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x38: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x39: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x3a:
             offset16 = (memory[pc + 2] << 8) | memory[pc + 1];
             a = memory[offset16];
             pc+=2;
             assemblyCode = "LDA adr";
             break;
         case 0x3b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x3c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x3d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x3e:
             a = memory[pc + 1];
             assemblyCode = "MVI A,D8";
             pc+=1;
             break;
         case 0x3f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x40: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x41: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x42: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x43: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x44: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x45: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x46:
             offset16 = (h << 8) | l;
             b = memory[offset16];
             assemblyCode = "MOV B,M";
             break;
         case 0x47: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x48: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x49: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x4a: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x4b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x4c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x4d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x4e:
             offset16 = (h << 8) | l;
             c = memory[offset16];
             assemblyCode = "MOV C,M";
             break;
         case 0x4f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x50: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x51: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x52: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x53: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x54: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x55: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x56:
             offset16 = (h <<8) | l;
             d = (memory[offset16]);
             assemblyCode = "MOV D,M";
             break;
         case 0x57: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x58: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x59: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x5a: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x5b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x5c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x5d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x5e:
             offset16 = (h <<8) | l;
             e = (memory[offset16]);
             assemblyCode = "MOV E,M";
             break;
         case 0x5f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x60: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x61:
             h = c;
             assemblyCode = "MOV H,C";
             break;
         case 0x62: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x63: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x64: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x65: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x66: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x67: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x68: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x69: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x6a: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x6b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x6c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x6d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x6e: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x6f:
             l = a;
             assemblyCode = "MOV L,A";
             break;
         case 0x70: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x71: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x72: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x73: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x74: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x75: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x76: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x77:
             offset16 = (h<<8) | l;
             if (offset16 >= 0x2000)
             {
                 vramoffset16 = offset16 - 0x2000;
                 framebuffer[vramoffset16] = a;
             }
             else
             {
                 memory[offset16] = a;
             }
             assemblyCode = "MOV M,A";
             break;
         case 0x78: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x79: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x7a: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x7b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x7c:
             a = h;
             assemblyCode = "MOV A,H";
             break;
         case 0x7d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x7e:
             offset16 = (h << 8) | l;
             a = memory[offset16];
             assemblyCode = "MOV A,M";
             break;
         case 0x7f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x80:
             answer16 = (uint16_t) a + (uint16_t) b;
             z = ((answer16 & 0xff) == 0);
             s = ((answer16 & 0x80) != 0);
             cy = (answer16 > 0xff);
             p = cpuFunctions.checkParity(answer16 & 0xff);
             a = answer16 & 0xff;
             assemblyCode = "ADD B";
             break;
         case 0x81: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x82: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x83: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x84: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x85: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x86:
             offset16 = (uint16_t) (h<<8) | (l);
             answer16 = (uint16_t) a + memory[offset16];
             z = ((answer16 & 0xff) == 0);
             s = ((answer16 & 0x80) != 0);
             cy = (answer16 > 0xff);
             p = cpuFunctions.checkParity(answer16 & 0xff);
             a = answer16 & 0xff;
             assemblyCode = "ADD M";
             break;
         case 0x87: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x88: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x89: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x8a: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x8b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x8c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x8d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x8e: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x8f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x90: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x91: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x92: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x93: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x94: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x95: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x96: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x97: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x98: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x99: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x9a: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x9b: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x9c: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x9d: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x9e: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0x9f: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa0: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa1: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa2: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa3: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa4: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa5: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa6: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa7:
             a = (a & a);
             z = (a == 0);
             s = ((a & 0x80) != 0);
             p = cpuFunctions.checkParity(a);
             cy = (a > 0xff);
             assemblyCode = "ANA A";
             break;
         case 0xa8: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xa9: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xaa: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xab: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xac: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xad: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xae: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xaf:
             a = (a ^ a);
             z = (a == 0);
             s = ((a & 0x80) != 0);
             p = cpuFunctions.checkParity(a);
             cy = (a > 0xff);
             assemblyCode = "XRA A";
             break;
         case 0xb0: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb1: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb2: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb3: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb4: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb5: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb6: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb7: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb8: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xb9: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xba: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xbb: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xbc: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xbd: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xbe: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xbf: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xc0: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xc1:
             c = memory[sp];
             sp += 1;
             b = memory[sp];
             sp += 1;
             assemblyCode = "POP B";
             break;
         case 0xc2:
             if (z == 0) //Jump when flag is not set.
             {
                 pc = (memory[pc + 2] << 8) | memory[pc + 1];
                 pc-=1;
             }
             else pc+=2;
             assemblyCode = "JNZ adr";
             break;
         case 0xc3:
             pc = (memory[pc + 2] << 8) | memory[pc + 1];
             pc-=1;
             assemblyCode = "JMP adr";
             break;
         case 0xc4: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xc5:
             sp -=1;
             memory[sp] = b;
             sp -= 1;
             memory[sp] = c;
             assemblyCode = "PUSH B";
             break;
         case 0xc6:
             answer16 = (uint16_t) a + (uint16_t) memory[pc + 1];
             z = ((answer16 & 0xff) == 0);
             s = ((answer16 & 0x80) != 0);
             cy = (answer16 > 0xff);
             p = cpuFunctions.checkParity(answer16 & 0xff);
             a = answer16 & 0xff;
             assemblyCode = "ADI D8";
             break;
         case 0xc7: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xc8: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xc9:
             pc = memory[sp] | (memory[sp+1] << 8);
             sp += 2;
             pc -=1;
             assemblyCode = "RET";
             break;
         case 0xca:
             if (z == 1) //Jump when flag is set.
             {
                 pc = (memory[pc + 2] << 8) | memory[pc + 1];
                 pc-=1;
             }
             else pc+=2;
             assemblyCode = "JZ adr";
             break;
         case 0xcb: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xcc: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xcd:
             ret = pc+2;
             memory[sp - 1] = (ret >> 8) & 0xff;
             memory[sp - 2] = (ret & 0xff);
             sp = sp - 2;
             pc = (memory[pc + 2] << 8) | memory[pc + 1];
             pc -=1;
             assemblyCode = "CALL adr";
             break;
         case 0xce: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xcf: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xd0: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xd1:
             e = memory[sp];
             sp += 1;
             d = memory[sp];
             sp += 1;
             assemblyCode = "POP D";
             break;
         case 0xd2: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xd3:
             port = memory[pc + 1]; //OUT
             cpuFunctions.MachineOut(port, a);
             assemblyCode = "OUT D8";
             pc +=1;
             break;
         case 0xd4: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xd5:
             sp -=1;
             memory[sp] = d;
             sp -= 1;
             memory[sp] = e;
             assemblyCode = "PUSH D";
             break;
         case 0xd6: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xd7: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xd8: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xd9: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xda: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xdb:
             port = memory[pc + 1]; //IN
             a = cpuFunctions.MachineIn(port);
             assemblyCode = "IN D8";
             pc +=1;
             break;
         case 0xdc: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xdd: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xde: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xdf: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xe0: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xe1:
             l = memory[sp];
             sp += 1;
             h = memory[sp];
             sp += 1;
             assemblyCode = "POP H";
             break;
         case 0xe2: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xe3: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xe4: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xe5:
             sp -=1;
             memory[sp] = h;
             sp -= 1;
             memory[sp] = l;
             assemblyCode = "PUSH H";
             break;
         case 0xe6: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xe7: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xe8: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xe9: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xea: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xeb: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xec: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xed: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xee: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xef: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xf0: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xf1:
             std::cout << unsigned(memory[sp]);
             s = (memory[sp] & 0x80) >> 7;
             z = (memory[sp] & 0x40) >> 6;
             ac = (memory[sp] & 0x10) >> 4;
             p = (memory[sp] & 0x04) >> 2;
             cy = (memory[sp] & 0x01);
             sp += 1;
             a = memory[sp];
             assemblyCode = "POP PSW";
             break;
         case 0xf2: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xf3: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xf4: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xf5:
             sp = sp -= 1;
             memory[sp] = a;
             sp = sp -= 1;
             memory[sp] = ((((((((((((((s << 1) | z) << 1) | 0) << 1) | ac) << 1) | 0) << 1) | p) << 1) | 1) << 1) | cy);
             assemblyCode = "PUSH PSW";                                 //  s  z     ac    p     cy
                                                                        // 0  0  0  0  0  0  0  0
             break;
         case 0xf6: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xf7: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xf8: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xf9: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xfa: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xfb:
             int_enable = 1;
             assemblyCode = "EI";
             break;
         case 0xfc: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xfd: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xfe: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
         case 0xff: cpuFunctions.UnimplementedInstruction(memory[pc]); break;
     }
     if (debug)
     {
         //Debug logging to see current state of registers and flags.
         std::cout << " Assembly code: " << assemblyCode << " Flags: Z: " << unsigned (z) << " S: "<<
                   unsigned (s) << " P: " << unsigned (p) << " CY: " <<
                   unsigned(cy) << "     " << "Registers: a: " <<
                   unsigned (a) << " b: " << unsigned (b) << " c: " << unsigned (c) << " d: " <<
                   unsigned(d) << " e: " << unsigned(e) << " h: " << unsigned(h) << " l: " <<
                   unsigned(l) << " sp: " << unsigned(sp) << " pc: " << unsigned(pc)
                   << std::endl;
         
         out.open("debug.txt", std::ios_base::app);
         out << " Assembly code: " << assemblyCode << " Flags: Z: " << unsigned (z) << " S: "<<
                   unsigned (s) << " P: " << unsigned (p) << " CY: " <<
                   unsigned(cy) << "     " << "Registers: a: " <<
                   unsigned (a) << " b: " << unsigned (b) << " c: " << unsigned (c) << " d: " <<
                   unsigned(d) << " e: " << unsigned(e) << " h: " << unsigned(h) << " l: " <<
                   unsigned(l) << " sp: " << unsigned(sp) << " pc: " << unsigned(pc)
                   << std::endl;
         out.close();
     }
     pc+=1;
     if (pc >= 0x4000)
     {
         pc = pc - 0x4000;
     }
 }
 
void CPU::ParseKeyPress(const std::string& input, uint8_t *&memory)
{
    if (input == "LEFT")
    {
        controllerByte |= 0x20;
    }
    else if (input == "RIGHT")
    {
        controllerByte |= 0x40;
    }
    else if (input == "START")
    {
        controllerByte |= 0x04;
    }
    else if (input == "COIN")
    {
        controllerByte |= 0x01;
    }
    else if (input == "SHOOT")
    {
        controllerByte |= 0x10;
    }
}

void CPU::ParseKeyLift(const std::string& input, uint8_t *&memory)
{
    if (input == "LEFT")
    {
        controllerByte &= 0xDF;
    }
    else if (input == "RIGHT")
    {
        controllerByte &= 0xBF;
    }
    else if (input == "START")
    {
        controllerByte &= 0x0B;
    }
    else if (input == "COIN")
    {
        controllerByte &= 0x0E;
    }
    else if (input == "SHOOT")
    {
        controllerByte &= 0xEF;
    }
}
