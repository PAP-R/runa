add_library(${PROJECT_NAME}_lib SHARED 
    Object.hpp
	Node.cpp
    Node.hpp
    Plugin.hpp
	Log.hpp
)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC external)

add_library(${PROJECT_NAME}_test SHARED test.cpp)
add_library(${PROJECT_NAME}_test2 SHARED test2.cpp)

target_link_libraries(${PROJECT_NAME}_test PUBLIC ${PROJECT_NAME}_lib)
target_link_libraries(${PROJECT_NAME}_test2 PUBLIC ${PROJECT_NAME}_lib)

set_target_properties(${PROJECT_NAME}_test ${PROJECT_NAME}_test2 PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins" 
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
)

target_sources(${PROJECT_NAME} PUBLIC
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)
