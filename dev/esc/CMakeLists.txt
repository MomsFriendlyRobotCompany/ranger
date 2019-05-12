cmake_minimum_required(VERSION 3.7)
PROJECT("esc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")

# Threads ---------------------------------------------------------
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS ">> Threads found")
    if(CMAKE_USE_PTHREADS_INIT)
        message(STATUS " -> Using pthreads")
    endif()
    # list(APPEND ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
else()
    message(WARNING "*** Could not find Threads ***")
endif()

add_executable(esc esc.cpp)
target_link_libraries(esc ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(esc pigpio)
