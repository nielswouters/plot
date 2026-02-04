cmake_minimum_required(VERSION 3.20)

include(FetchContent)

project(Plot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable pkgconfig for Windows
if (WIN32)
    set(CMAKE_DISABLE_FIND_PACKAGE_PkgConfig TRUE)
endif()

# Grab all .cpp files in src/
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Build SDL as static, not shared
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

# Fetch SDL
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.0
)
FetchContent_MakeAvailable(SDL3)

add_executable(Plot ${PROJECT_SOURCES})

# Set as startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT Plot
)

target_sources(Plot PRIVATE ${PROJECT_SOURCES})

target_link_libraries(Plot PRIVATE SDL3::SDL3)

# So headers in src/ can be included like: #include "foo.h"
target_include_directories(Plot PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
