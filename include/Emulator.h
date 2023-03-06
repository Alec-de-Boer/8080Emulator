#pragma once
#include "../include//SDL.h"

enum class EmulatorState {PLAY, EXIT};

class Emulator
{
    public:
    Emulator();
    void initSystems();
    void DrawImage(uint8_t *&framebuffer);
    void processInput(uint8_t *&memory);
    EmulatorState _emulatorState;
    SDL_Window* _window;
    int _screenWidth;
    int _screenHeight;
    SDL_Renderer* renderer;

};
