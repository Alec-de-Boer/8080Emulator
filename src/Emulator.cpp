#include "../include/Emulator.h"
#include "iostream"
#include "CPU.h"

CPU cpu;

void fatalError(const std::string& errorString)
{
    std::cout << errorString << std::endl;
    std::cout << "Enter any key to quit...";
    int tmp;
    std::cin >> tmp;
    SDL_Quit();
}

Emulator::Emulator()
{
    _window = nullptr;
    renderer = nullptr;
    _screenWidth = 256;
    _screenHeight = 224;
    _emulatorState = EmulatorState::PLAY;
}

void Emulator::initSystems()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    _window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth * 2, _screenHeight * 2, SDL_WINDOW_RESIZABLE);
    if (_window == nullptr)
    {
        fatalError("SDL Window could not be created.");
    }
    renderer = SDL_CreateRenderer(_window, -1, 0);
    if (renderer == nullptr)
    {
        fatalError("SDL Renderer could not be created.");
    }
}

void Emulator::processInput(uint8_t *&memory)
{
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt))
    {
        switch (evnt.type)
        {
            case SDL_QUIT:
                _emulatorState = EmulatorState::EXIT;
                break;
                
            case SDL_KEYDOWN:
                switch (evnt.key.keysym.sym)
                {
                    case SDLK_a:
                        cpu.ParseKeyPress("LEFT", memory);
                        break;
                    case SDLK_d:
                        cpu.ParseKeyPress("RIGHT", memory);
                        break;
                    case SDLK_c:
                        cpu.ParseKeyPress("COIN", memory);
                        break;
                    case SDLK_w:
                        cpu.ParseKeyPress("SHOOT", memory);
                        break;
                    case SDLK_SPACE:
                        cpu.ParseKeyPress("START", memory);
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (evnt.key.keysym.sym)
                {
                    case SDLK_a:
                        cpu.ParseKeyLift("LEFT", memory);
                        break;
                    case SDLK_d:
                        cpu.ParseKeyLift("RIGHT", memory);
                        break;
                    case SDLK_c:
                        cpu.ParseKeyLift("COIN", memory);
                        break;
                    case SDLK_w:
                        cpu.ParseKeyLift("SHOOT", memory);
                        break;
                    case SDLK_SPACE:
                        cpu.ParseKeyLift("START", memory);
                        break;
                }
                break;
        }
    }
}

void Emulator::DrawImage(uint8_t *&framebuffer)
{
    auto *convertedFramebuffer = new uint8_t[0xDFF8];
    memset(convertedFramebuffer, 0, 0xDFF8);
    int framebufferCounter = 0;
    for (int i = 0; i <= 224 ; ++i)
    {
        convertedFramebuffer[framebufferCounter] = (framebuffer[i] & 0x80);
        if (convertedFramebuffer[framebufferCounter] == 0x01)
        {
            convertedFramebuffer[framebufferCounter] = 0xFF;
        }
        framebufferCounter +=1;
    
        convertedFramebuffer[framebufferCounter] = (framebuffer[i] & 0x40);
        if (convertedFramebuffer[framebufferCounter] == 0x01)
        {
            convertedFramebuffer[framebufferCounter] = 0xFF;
        }
        framebufferCounter +=1;
    
        convertedFramebuffer[framebufferCounter] = (framebuffer[i] & 0x20);
        if (convertedFramebuffer[framebufferCounter] == 0x01)
        {
            convertedFramebuffer[framebufferCounter] = 0xFF;
        }
        framebufferCounter +=1;
    
        convertedFramebuffer[framebufferCounter] = (framebuffer[i] & 0x10);
        if (convertedFramebuffer[framebufferCounter] == 0x01)
        {
            convertedFramebuffer[framebufferCounter] = 0xFF;
        }
        framebufferCounter +=1;
    
        convertedFramebuffer[framebufferCounter] = (framebuffer[i] & 0x08);
        if (convertedFramebuffer[framebufferCounter] == 0x01)
        {
            convertedFramebuffer[framebufferCounter] = 0xFF;
        }
        framebufferCounter +=1;
    
        convertedFramebuffer[framebufferCounter] = (framebuffer[i] & 0x04);
        if (convertedFramebuffer[framebufferCounter] == 0x01)
        {
            convertedFramebuffer[framebufferCounter] = 0xFF;
        }
        framebufferCounter +=1;
    
        convertedFramebuffer[framebufferCounter] = (framebuffer[i] & 0x02);
        if (convertedFramebuffer[framebufferCounter] == 0x01)
        {
            convertedFramebuffer[framebufferCounter] = 0xFF;
        }
        framebufferCounter +=1;
    
        convertedFramebuffer[framebufferCounter] = (framebuffer[i] & 0x01);
        if (convertedFramebuffer[framebufferCounter] == 0x01)
        {
            convertedFramebuffer[framebufferCounter] = 0xFF;
        }
        framebufferCounter += 1;
    }
    
    //set texture template
    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, _screenWidth, _screenHeight);
    SDL_UpdateTexture(texture, NULL, convertedFramebuffer, _screenWidth);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}