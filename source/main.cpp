#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "test.hpp"

#include "Node.hpp"
#include "Object.hpp"

#include <iostream>
#include <memory>
#include <print>
#include <string>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    std::println("Hallo");

    std::string name;

    std::getline(std::cin, name);

    Node::load(name);
    auto test = Node::create(name);

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
    std::println("Tschüss");
}