set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Specify the paths to the SQLite3 library and include files
set(SQLite3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/_deps/sqlite3/include")
set(SQLite3_LIBRARY "${CMAKE_SOURCE_DIR}/_deps/sqlite3/lib/sqlite3.lib")

# Specify the search paths for the cross-compilation environment
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32 ${CMAKE_SOURCE_DIR}/_deps/sqlite3)

# Adjust the search behavior
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)