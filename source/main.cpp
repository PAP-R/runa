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

SDL_Window *create_window(const char *title, int width, int height, SDL_WindowFlags flags) {
    auto window = SDL_CreateWindow(title, width, height, SDL_WINDOW_VULKAN | flags);

    return window;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        Log::println("Failed to initialize SDL:\n{}", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        Log::println("Failed to initialize SDL TTF:\n{}", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Plugin::init();

    auto test = Node::create("test2");

    test->create_child("test");

    if (test) {
        test->test(7);
        test->test(12);
    }

    Node::root()->update();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    return SDL_APP_SUCCESS;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    Node::terminate();
    Plugin::terminate();
    SDL_Quit();
}