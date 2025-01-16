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
)

find_package(SQLite3 REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Enable testing
enable_testing()

# Add Catch2
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.7 # or the latest version
)
FetchContent_MakeAvailable(catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)

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
target_link_libraries(runTests Catch2::Catch2 SQLite::SQLite3)
add_test(NAME runTests COMMAND runTests)