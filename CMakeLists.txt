cmake_minimum_required(VERSION 3.10)

project(anki_app_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Main executable
add_executable(${PROJECT_NAME}
    src/core/main.cpp
    src/core/database.cpp
    src/core/database_initializer.cpp
    src/core/app_logic.cpp
    src/cli/cli.cpp
    src/cli/study_session.cpp
    src/cli/ascii_art.cpp
    src/file/file_handler.cpp
    src/sound/sound_manager.cpp
)

# Manually specify the paths to SDL2 and SDL2_mixer
set(SDL2_INCLUDE_DIR "/usr/include/SDL2")
set(SDL2_LIBRARY "/usr/lib/x86_64-linux-gnu/libSDL2.so")
set(SDL2_MIXER_INCLUDE_DIR "/usr/include/SDL2")
set(SDL2_MIXER_LIBRARY "/usr/lib/x86_64-linux-gnu/libSDL2_mixer.so")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})
link_directories("/usr/lib/x86_64-linux-gnu")

find_package(SQLite3 REQUIRED)

# Use pkg-config to find Wayland libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-cursor wayland-egl xkbcommon libdecor-0 wayland-server expat)

target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3 ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} ${WAYLAND_LIBRARIES_LDFLAGS} -lpthread -ldl)
target_include_directories(${PROJECT_NAME} PRIVATE src ${WAYLAND_INCLUDE_DIRS})

enable_testing()

include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.0
)
FetchContent_MakeAvailable(catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Test executable
add_executable(runTests
    tests/test_main.cpp
    tests/test_app_logic.cpp
    tests/test_database.cpp
    tests/test_utils.cpp
    src/core/database.cpp
    src/core/app_logic.cpp
    src/core/database_initializer.cpp
    src/file/file_handler.cpp
)

target_include_directories(runTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(runTests PRIVATE SQLite::SQLite3 ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} ${WAYLAND_LIBRARIES_LDFLAGS} Catch2::Catch2 -lpthread -ldl nlohmann_json::nlohmann_json)
add_test(NAME runTests COMMAND runTests)