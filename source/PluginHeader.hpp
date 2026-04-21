#pragma once

#include <set>
#include <string>

enum PluginVersionFlags {
    NONE    = 0,
    LESS    = 1 << 0,
    EQUAL   = 1 << 1,
    GREATER = 1 << 2,
};

struct PluginDependency {
    std::string                        name;
    std::tuple<size_t, size_t, size_t> version;
    PluginVersionFlags                 version_flags;
};

struct PluginHeader {
    std::string                name;
    std::tuple<size_t, size_t> version;
    std::set<PluginDependency> dependencies;
};

#ifdef _WIN32
#define RUNA_API __declspec(dllexport)
#else
#define RUNA_API __attribute__((visibility("default")))
#endif

#define RUNA_API_INIT(code)       \
    extern "C" {                  \
    RUNA_API void init() { code } \
    }

#define RUNA_API_HEADER(name, version_major, version_minor, dependencies) \
    extern "C" {                                                          \
    RUNA_API struct PluginHeader header() {                               \
        return {                                                          \
            name, {version_major, version_minor}, \
             dependencies \
        };          \
    }                                                                     \
    }