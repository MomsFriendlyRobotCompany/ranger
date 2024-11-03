cmake_minimum_required(VERSION 3.30)
project(ranger VERSION 2024.09.30.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
cmake_host_system_information(RESULT OSN QUERY OS_NAME)
cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")
message(STATUS " ${HOST}")
message(STATUS " ${OSN}: ${OS_VERSION}")
message(STATUS " ${PROC}")
message(STATUS "-------------------------------------")

# set(BUILD_EXAMPLES ON)
set(BUILD_TESTS ON)

include(FetchContent)

# gtests ---------
# FetchContent_Declare(
#     gtest
#     URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
# )

# ipc ------------
# FetchContent_Declare(ipc
#     GIT_REPOSITORY "git@github.com:MomsFriendlyRobotCompany/ipc.git"
#     GIT_TAG "origin/main"
#     SOURCE_SUBDIR "cpp"
# )

# squaterion ------------
FetchContent_Declare(squaternion
    GIT_REPOSITORY "https://github.com/MomsFriendlyRobotCompany/squaternion.git"
    GIT_TAG "origin/master"
    SOURCE_SUBDIR "cpp"
)

# messages -------------
# FetchContent_Declare(messages
#     GIT_REPOSITORY "https://github.com/gecko-robotics/messages.git"
#     GIT_TAG "origin/main"
#     SOURCE_SUBDIR "cpp"
# )

# yivo -----------------
# FetchContent_Declare(yivo
#     GIT_REPOSITORY "https://github.com/MomsFriendlyRobotCompany/yivo.git"
#     GIT_TAG "origin/main"
#     SOURCE_SUBDIR "cpp"
# )

# gcigps -----------------
# FetchContent_Declare(gcigps
#     GIT_REPOSITORY "https://github.com/the-guild-of-calamitous-intent/gciGps.git"
#     GIT_TAG "origin/main"
#     SOURCE_SUBDIR "pico"
# )

# gcisensors -----------------
# FetchContent_Declare(gcisensors
#     GIT_REPOSITORY "https://github.com/the-guild-of-calamitous-intent/gciSensors.git"
#     GIT_TAG "origin/main"
# )

FetchContent_MakeAvailable(
    squaternion
    # ipc
)

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

LIST(APPEND APPS
    main
    imu
    # read_msg
    lidar
)

foreach(MAIN ${APPS})
    add_executable(${MAIN} src/${MAIN}.cpp)
    target_include_directories(${MAIN} PUBLIC
        ipc/src
        serialcomm
    )
    target_link_libraries(${MAIN} PUBLIC
        # ipc
        squaternion
        Threads::Threads
    )

    add_custom_command(TARGET ${MAIN} POST_BUILD
        # du is human readable, wc gives bytes
        # COMMAND du -sh ${CMAKE_CURRENT_BINARY_DIR}/${MAIN}
        COMMAND wc -c ${CMAKE_CURRENT_BINARY_DIR}/${MAIN}
    )
endforeach()