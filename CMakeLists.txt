cmake_minimum_required(VERSION 3.10)

project(
    MineSweep
    VERSION 1.0.0
    LANGUAGES CXX
)

include(cmake/utility.cmake)
if (NOT EMSCRIPTEN)
    include(cmake/FindSDL2.cmake)
endif()
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

if (EMSCRIPTEN)
    message("-- build for wasm")
    CopyResForWASM(mine-sweep)    # NOTE: em++ work under ${CMAKE_BINARY_DIR}, so we copy resources dir to there for resources path correctly
    set(USE_SDL "-sUSE_SDL=2" )
    set(PRELOAD "--preload-file resources --use-preload-plugins")
    set(OTHER_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_SDL}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_SDL}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_SDL} ${PRELOAD} ${OTHER_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

CopyDLL(mine-sweep)

install(PROGRAMS $<TARGET_FILE:mine-sweep>
DESTINATION "${CMAKE_BINARY_DIR}/mine-sweep-${PROJECT_VERSION}")
install(DIRECTORY resources
DESTINATION "${CMAKE_BINARY_DIR}/mine-sweep-${PROJECT_VERSION}")
install(FILES $CACHE{SDL2_DYNAMIC_LIB_DIR}/SDL2.dll
DESTINATION "${CMAKE_BINARY_DIR}/mine-sweep-${PROJECT_VERSION}")