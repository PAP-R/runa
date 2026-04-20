#pragma once

#include <map>
#include <string>

#include <SDL3/SDL.h>

#include "Log.hpp"

#ifdef _WIN32
#define LIBFILE ".dll"
#else
#define LIBFILE ".so"
#endif

#define PLUGIN_DIR "plugins"

namespace Plugin {
    inline static std::map<std::string, SDL_SharedObject *> objects;

    inline static bool load(const std::string &name) {
        auto obj = SDL_LoadObject(name.c_str());
        if (obj) {
            Log::println("Loaded obj {}", name);

            objects.insert(std::make_pair(name, obj));
            auto init = (void (*)())SDL_LoadFunction(obj, "init");
            if (init != nullptr) {
                init();
            }

            return true;
        } else {
            Log::println("Failed to load {}: ", name, SDL_GetError());
        }

        return false;
    }

    inline static SDL_EnumerationResult init_callback(void *userdata, const char *dirname, const char *fname) {
        std::string name = fname;
        if (name.ends_with(LIBFILE)) {
            Log::println("{}{} :", dirname, fname);
        }

        std::string path = std::format("{}{}", dirname, fname);

        load(path);

        return SDL_ENUM_CONTINUE;
    }

    inline static void
    init() {
        std::string path = SDL_GetBasePath();
        path.append(PLUGIN_DIR);
        SDL_EnumerateDirectory(path.c_str(), init_callback, nullptr);
    }

    inline static void terminate() {
        for (auto o : objects) {
            SDL_UnloadObject(o.second);
        }

        objects.clear();
    }
} // namespace Plugin