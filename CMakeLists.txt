cmake_minimum_required(VERSION 3.10)

project(
    MineSweep
    VERSION 1.0.0
    LANGUAGES CXX
)

include(cmake/utility.cmake)
include(cmake/FindSDL2.cmake)
include(cmake/copydll.cmake)

IsMSVCBackend(is_msvc_backend)

aux_source_directory(src SRC)
add_executable(mine-sweep ${SRC})
target_include_directories(mine-sweep PRIVATE include)
target_link_libraries(mine-sweep PRIVATE SDL2)
target_compile_features(mine-sweep PRIVATE cxx_std_17)

if (${is_msvc_backend})
    target_link_options(mine-sweep PRIVATE $<IF:$<CONFIG:Release>,/SUBSYSTEM:WINDOWS,/SUBSYSTEM:CONSOLE>)
endif()

CopyDLL(mine-sweep)

install(PROGRAMS $<TARGET_FILE:mine-sweep>
DESTINATION "${CMAKE_BINARY_DIR}/mine-sweep-${PROJECT_VERSION}")
install(DIRECTORY resources
DESTINATION "${CMAKE_BINARY_DIR}/mine-sweep-${PROJECT_VERSION}")
install(FILES $CACHE{SDL2_DYNAMIC_LIB_DIR}/SDL2.dll
DESTINATION "${CMAKE_BINARY_DIR}/mine-sweep-${PROJECT_VERSION}")