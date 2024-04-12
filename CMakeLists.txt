cmake_minimum_required(VERSION 3.27)
project(fluid_simulation)

set(CMAKE_CXX_STANDARD 23)

set(SOURCE_DIR src)
set(VENDOR_DIR vendor)

add_subdirectory("${VENDOR_DIR}/raylib")

set(COMPILE_FLAGS "-stdlib=libc++")

file(GLOB_RECURSE SOURCE_FILES 
    "${SOURCE_DIR}/**.cpp" 
    "${SOURCE_DIR}/**.h" 
)

add_executable(fluid_simulation "${SOURCE_FILES}")

target_include_directories(fluid_simulation PUBLIC 
    "${SOURCE_DIR}"
    "${VENDOR_DIR}/raylib/src"
)

target_link_libraries(fluid_simulation PUBLIC raylib)