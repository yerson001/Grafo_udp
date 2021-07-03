project("Grafo_udp")

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories($(CMAKE_CURRENT_SOURCE_DIR)/source)

file(GLOB SLAVE_SOURCE_FILES
  "source/SQlite/*.h"
)
find_package (Threads)
add_executable(client source/cliente.cpp)
add_executable(master source/master.cpp)
add_executable(repositorio source/repositorio.cpp ${SLAVE_SOURCE_FILES})
add_executable(main source/main.cpp source/SQlite.h ${SLAVE_SOURCE_FILES})
find_package (SQLite3)
if (SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIRS})
  target_link_libraries (main sqlite3${SQLITE3_LIBRARIES})
  target_link_libraries (repositorio sqlite3${SQLITE3_LIBRARIES})
endif (SQLITE3_FOUND)

target_link_libraries(main sqlite3)
target_link_libraries (client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (master ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (repositorio ${CMAKE_THREAD_LIBS_INIT})