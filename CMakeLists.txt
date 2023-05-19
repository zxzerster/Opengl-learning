cmake_minimum_required(VERSION 3.10)

project(HelloOpengl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    shader/shader_util.cpp
    shader/shader_util.h
    textures/stb_image.cpp
    Cube.cpp
    Cube.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)