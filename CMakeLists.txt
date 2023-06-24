cmake_minimum_required(VERSION 3.10)

project(
    MineSweep
    VERSION 1.0.0
    LANGUAGES CXX
)

include(cmake/utility.cmake)
include(cmake/FindSDL2.cmake)
include(cmake/copydll.cmake)

aux_source_directory(src SRC)
add_executable(mine-sweep ${SRC})
target_include_directories(mine-sweep PRIVATE include)
target_link_libraries(mine-sweep PRIVATE SDL2)
target_compile_features(mine-sweep PRIVATE cxx_std_17)
CopyDLL(mine-sweep)