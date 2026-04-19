add_subdirectory(glm)

add_subdirectory(ink-stroke-modeler)

option(LIEF_DOC "Build LIEF docs" OFF)
option(LIEF_PYTHON_API "Build LIEF Python API" OFF)
option(LIEF_EXAMPLES "Build LIEF examples" OFF)
option(LIEF_TESTS "Build LIEF tests" OFF)
add_subdirectory(lief)

option(SDL_SHARED "Build SDLs shared lib" ON)
option(SDL_STATIC "Build static lib" OFF)
option(SDL_TEST "Build SDL tests" OFF)
add_subdirectory(sdl)

# set(SDLSHADERCROSS_SHARED ON CACHE BOOL "Build SDL_shadercrosses shared lib")
option(SDLSHADERCROSS_VENDORED "Use SDL_shadercrosses vendored libs" ON)
option(ENABLE_SPIRV_CODEGEN "Enable spirv codegen" ON)
option(SDLSHADERCROSS_CLI "Build SDL_shadercrosses cli" OFF)
option(SPIRV_BUILD_TESTS "Build SDL_shadercrosses tests" OFF)
add_subdirectory(sdl-shadercross)

option(SDLTTF_VENDORED "Use SDL_ttfs vendored libs" ON)
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
	LIEF::LIEF
)
