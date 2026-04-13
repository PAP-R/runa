add_subdirectory(glm)



add_subdirectory(sdl)

add_subdirectory(sdl-shadercross)

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
    SDL3_shadercross
    SDL3_ttf::SDL3_ttf
)
