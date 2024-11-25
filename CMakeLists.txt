cmake_minimum_required(VERSION 3.10)

project(anki_app_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/database.cpp
)

find_package(SQLite3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3)

target_include_directories(${PROJECT_NAME} PRIVATE src)
