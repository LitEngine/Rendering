cmake_minimum_required(VERSION 3.19)
project(main)

set(CMAKE_CXX_STANDARD 20)

set(
        HEADERS
        include/lit/gl/shader.hpp
        include/lit/gl/program.hpp
        include/lit/gl/frame_buffer.hpp
        include/lit/gl/uniform.hpp
        include/lit/gl/texture.hpp
        include/lit/gl/shader.hpp
        include/lit/rendering/pipeline_node.hpp
        include/lit/rendering/pipeline.hpp)

set(
        SOURCES
        src/gl/shader.cpp
        src/gl/program.cpp
        src/gl/texture.cpp
        src/gl/frame_buffer.cpp
        src/gl/shader.cpp
        src/rendering/pipeline.cpp
        src/rendering/shaderx.cpp
        src/rendering/pipeline_node.cpp)

include(FetchContent)

if (NOT TARGET glm)
    FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm GIT_TAG master)
    FetchContent_MakeAvailable(glm)
endif ()

if (NOT TARGET application)
    FetchContent_Declare(application GIT_REPOSITORY https://github.com/LiteEngine/Application GIT_TAG master)
    FetchContent_MakeAvailable(application)
endif ()

add_executable(main main.cpp ${HEADERS} ${SOURCES})
target_include_directories(main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(main glm application)