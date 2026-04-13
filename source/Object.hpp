#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <print>

struct Object {
    virtual ~Object() {}

    virtual int test(int new_number) = 0;

    static std::shared_ptr<Object> load(const char *name) {
        std::shared_ptr<Object> obj = nullptr;

        auto test                   = SDL_LoadObject(name);

        if (test) {
            auto create = (Object * (*)()) SDL_LoadFunction(test, "create");
            if (create) {
                obj = std::shared_ptr<Object>(create());
            } else {
                std::println("Failed to find function: {}", SDL_GetError());
            }
        } else {
            std::println("Failed to find library: {}", SDL_GetError());
        }

        return obj;
    }
};