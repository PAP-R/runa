set(SOURCES
    Object.hpp
    Node.cpp
    Node.hpp
    Plugin.hpp
    PluginHeader.hpp
    Log.hpp
)

add_library(${PROJECT_NAME}_lib SHARED
    ${SOURCES}
    ArgMap.hpp
)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC external)

add_library(${PROJECT_NAME}_test SHARED test.cpp)
add_library(${PROJECT_NAME}_test2 SHARED test2.cpp)
add_library(${PROJECT_NAME}_window SHARED Window.cpp)

target_link_libraries(${PROJECT_NAME}_test PUBLIC ${PROJECT_NAME}_lib)
target_link_libraries(${PROJECT_NAME}_test2 PUBLIC ${PROJECT_NAME}_lib)
target_link_libraries(${PROJECT_NAME}_window PUBLIC ${PROJECT_NAME}_lib)

set_target_properties(
    ${PROJECT_NAME}_test
    ${PROJECT_NAME}_test2
    ${PROJECT_NAME}_window

    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

target_sources(${PROJECT_NAME} PUBLIC
    main.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)

add_library(main SHARED
    main.cpp
)
target_link_libraries(main PUBLIC ${PROJECT_NAME}_lib)

