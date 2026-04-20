#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <memory>
#include <print>
#include <string>

#include "test.hpp"

#include "Log.hpp"
#include "Node.hpp"
#include "Object.hpp"
#include "Plugin.hpp"

SDL_Window *create_window(AppState *appstate, const char *title, int width, int height, SDL_WindowFlags flags = 0) {
    auto window = SDL_CreateWindow(title, width, height, SDL_WINDOW_VULKAN | flags);

    SDL_ClaimWindowForGPUDevice(appstate->device, window);

    appstate->windows.insert(window);

    return window;
}

void destroy_window(AppState *appstate, SDL_Window *window) {
    appstate->windows.erase(window);
    SDL_ReleaseWindowFromGPUDevice(appstate->device, window);
    SDL_DestroyWindow(window);
}

SDL_AppResult SDL_AppInit(void **appstate_ptr, int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        Log::println("Failed to initialize SDL:\n{}", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        Log::println("Failed to initialize SDL TTF:\n{}", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    auto appstate = new AppState;

    Plugin::init();

    auto test  = Node::create("test2");

    auto test2 = test->create_child("test");

    if (test) {
        test->test(7);
        test->test(12);
    }

    test->disable();
    test2->disable();

    appstate->device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan");

    auto window      = create_window(appstate, "Hallo", 400, 400);

    *appstate_ptr    = appstate;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate_ptr) {
    auto appstate = (AppState *)appstate_ptr;

    Node::root()->update();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate_ptr, SDL_Event *event) {
    auto appstate = (AppState *)appstate_ptr;

    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
        auto window = SDL_GetWindowFromEvent(event);

        SDL_DestroyWindow(window);
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate_ptr, SDL_AppResult result) {
    auto appstate = (AppState *)appstate_ptr;
    Node::terminate();
    Plugin::terminate();

    for (auto w : appstate->windows) {
        destroy_window(appstate, w);
    }

    SDL_DestroyGPUDevice(appstate->device);
    delete appstate;
    SDL_Quit();
}