add_library(${PROJECT_NAME}_test SHARED test.cpp)
add_library(${PROJECT_NAME}_test2 SHARED test2.cpp)

target_link_libraries(${PROJECT_NAME}_test PUBLIC external)
target_link_libraries(${PROJECT_NAME}_test2 PUBLIC external)

target_sources(${PROJECT_NAME} PUBLIC
    main.cpp
    Object.hpp
    Node.hpp
)
