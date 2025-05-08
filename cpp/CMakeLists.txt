cmake_minimum_required(VERSION 3.28)
project(ranger VERSION 2024.09.30.0 LANGUAGES CXX C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")
# string(PREPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--print-memory-usage ")

cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
cmake_host_system_information(RESULT OSN QUERY OS_NAME)
cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "  C ${CMAKE_C_STANDARD}")
message(STATUS "-------------------------------------")
message(STATUS " ${HOST}")
message(STATUS " ${OSN}: ${OS_VERSION}")
message(STATUS " ${PROC}")
message(STATUS "-------------------------------------")

# set(BUILD_EXAMPLES ON)
set(BUILD_TESTS ON)


# Add Homebrew libraries ------------------------------------------
link_directories(/opt/homebrew/lib)
include_directories(/opt/homebrew/include)
# Threads ---------------------------------------------------------
find_package(Threads REQUIRED)
# absl ------------------------------------------------------------
find_package(absl REQUIRED)
message(STATUS "=>> ${absl_LIBRARY}")
# MCAP ------------------------------------------------------------
include(FetchContent)
set(MCAP_VERSION v0.0.53)
FetchContent_Declare(
  mcap
  URL https://github.com/foxglove/mcap/archive/refs/tags/releases/mcap-cli/${MCAP_VERSION}.zip
)
FetchContent_MakeAvailable(mcap) # this downloads it
message(STATUS "-> mcap version: ${MCAP_VERSION}")
set(MCAP_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/_deps/mcap-src/cpp/mcap/include)
# External Libraries ----------------------------------------------
add_subdirectory(extlibs/squaternion/cpp)
add_subdirectory(extlibs/ipc/cpp)
add_subdirectory(extlibs/yivo/c)
add_subdirectory(extlibs/serialcomm)
add_subdirectory(extlibs/ulog)
add_subdirectory(extlibs/messages)
# add_subdirectory(extlibs/gciSensors/linux)

if (1)
LIST(APPEND APPS
    # main
    # dummy_imu
    # dummy_lidar
    # read_msg
    # lidar
)
else()
LIST(APPEND APPS
    main
    imu
    # read_msg
    # lidar
    noise
)
endif()

foreach(MAIN ${APPS})
    add_executable(${MAIN} src/${MAIN}.cpp)
    target_include_directories(${MAIN} 
      PUBLIC
        extlibs/ipc/cpp/
        # serialcomm
        # extlibs/gciSensors/src
        # squaternion
    )
    target_link_libraries(${MAIN} PUBLIC
        ipc
        serialcomm
        squaternion
        Threads::Threads
        ulog
        messages
        absl::log_internal_message
        absl::log_internal_check_op
        absl::status
        zstd
        protobuf
        lz4
    )
    if(LINUX)
        target_link_libraries(${MAIN} PUBLIC i2c)
    endif()

    # add_custom_command(TARGET ${MAIN} POST_BUILD
    #     # du is human readable, wc gives bytes
    #     # COMMAND du -sh ${CMAKE_CURRENT_BINARY_DIR}/${MAIN}
    #     COMMAND wc -c ${CMAKE_CURRENT_BINARY_DIR}/${MAIN}
    # )
endforeach()

add_subdirectory(dev/protobuf)
