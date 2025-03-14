cmake_minimum_required(VERSION 3.7.0)

project(3d-placer CXX)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Prefix prepended to install directories")
# set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -O3
    -fomit-frame-pointer
    -funroll-loops
    -ffast-math
    -funit-at-a-time
    -fPIC
)

add_definitions(-DNDEBUG)

add_subdirectory(src)

install(TARGETS 3d_placer_lib
    EXPORT 3d_placer_targets
    LIBRARY DESTINATION thirdparty/3d-placer/lib
    ARCHIVE DESTINATION thirdparty/3d-placer/lib
    RUNTIME DESTINATION thirdparty/3d-placer/bin
    INCLUDES DESTINATION thirdparty/3d-placer/include
)

add_executable(3d-placer src/main/main.cpp)
target_link_libraries(3d-placer PRIVATE 3d_placer_lib)

