add_subdirectory(glm)

add_subdirectory(ink-stroke-modeler)


set(SDL_SHARED ON CACHE BOOL "Build SDLs shared lib")
set(SDL_STATIC OFF CACHE BOOL "Build static lib")
set(SDL_TEST OFF CACHE BOOL "Build SDL tests")
add_subdirectory(sdl)

# set(SDLSHADERCROSS_SHARED ON CACHE BOOL "Build SDL_shadercrosses shared lib")
set(SDLSHADERCROSS_CLI OFF CACHE BOOL "Build SDL_shadercrosses cli")
set(SDLSHADERCROSS_VENDORED ON CACHE BOOL "Use SDL_shadercrosses vendored libs")
set(ENABLE_SPIRV_CODEGEN ON CACHE BOOL "Enable spirv codegen")
set(SPIRV_BUILD_TESTS OFF CACHE BOOL "Build SDL_shadercrosses tests")
add_subdirectory(sdl-shadercross)

set(SDLTTF_VENDORED ON CACHE BOOL "Use SDL_ttfs vendored libs")
add_subdirectory(sdl-ttf)

add_library(external INTERFACE)

target_include_directories(external INTERFACE
    glm
    sdl/include
    sdl-shadercross/include
    pfr/include
)

target_link_libraries(external INTERFACE
    glm::glm-header-only
    SDL3::SDL3
    SDL3_shadercross-shared
    SDL3_ttf::SDL3_ttf
)
