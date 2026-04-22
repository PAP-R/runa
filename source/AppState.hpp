#pragma once

#include <SDL3/SDL.h>

struct AppState {
    SDL_GPUDevice        *device         = nullptr;
    SDL_GPUCommandBuffer *command_buffer = nullptr;
    bool                  should_quit    = false;
};