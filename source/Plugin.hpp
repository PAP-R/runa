#pragma once

#include <map>
#include <string>

#include <SDL3/SDL.h>

#include "Log.hpp"
#include "PluginHeader.hpp"

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
        if (!obj) {
            Log::println(Log::ERROR, "Failed to load {}: {}", name, SDL_GetError());
            return false;
        }

        auto header_func = (PluginHeader (*)())SDL_LoadFunction(obj, "header");
        if (!header_func) {
            Log::println(Log::ERROR, "Failed to get header for {}: {}", name, SDL_GetError());
            return false;
        }

        auto header = header_func();
        for (auto &d : header.dependencies) {
            objects.at(d.name);
            if (objects.contains(d.name)) {
            }
        }

        Log::println(Log::INFO, "Loaded obj {}", name);
        objects.insert(std::make_pair(name, obj));
        auto init = (void (*)())SDL_LoadFunction(obj, "init");
        if (init != nullptr) {
            init();
        }

        return true;
    }

    inline static SDL_EnumerationResult init_callback(void *userdata, const char *dirname, const char *fname) {
        std::string name = fname;
        if (name.ends_with(LIBFILE)) {
            Log::println(Log::INFO, "{}{} :", dirname, fname);
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