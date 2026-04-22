#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <memory>
#include <print>
#include <string>

#include "Log.hpp"
#include "Node.hpp"
#include "Object.hpp"
#include "Plugin.hpp"

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

    appstate->device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan");

    auto window      = Node::create("window", appstate, {
                                                            {{"WindowTitle", std::string("Test")}, {"WindowWidth", 400}, {"WindowHeight", 400}, {"WindowFlags", SDL_WINDOW_RESIZABLE}}
    });

    Log::println("Created {} as temporary variable", window->to_string());

    auto test  = window->create_child("test2", appstate);
    auto test2 = test->create_child("test", appstate);

    test->disable();
    test2->disable();

    *appstate_ptr = appstate;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate_ptr) {
    auto appstate            = (AppState *)appstate_ptr;
    appstate->command_buffer = SDL_AcquireGPUCommandBuffer(appstate->device);

    Node::update(appstate);

    SDL_SubmitGPUCommandBuffer(appstate->command_buffer);

    // auto fence = SDL_SubmitGPUCommandBufferAndAcquireFence(appstate->command_buffer);
    // SDL_WaitForGPUFences(appstate->device, true, &fence, 1);
    // SDL_ReleaseGPUFence(appstate->device, fence);

    if (appstate->should_quit) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate_ptr, SDL_Event *event) {
    auto appstate = (AppState *)appstate_ptr;

    Node::event(event, appstate);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate_ptr, SDL_AppResult result) {
    auto appstate = (AppState *)appstate_ptr;
    Node::terminate();
    Plugin::terminate();

    SDL_DestroyGPUDevice(appstate->device);
    delete appstate;
    SDL_Quit();
}