cmake_minimum_required(VERSION 3.20)


set(PROJECT ranger)
set(PICO_BOARD pico) # pico or pico_w
# set(PICO_BOARD pico_w)
set(PICO_SDK_PATH "$ENV{HOME}/github/pico-sdk")
set(ENV{PATH} "$ENV{HOME}/github/gcc-arm/bin:$ENV{PATH}")
set(PICO_SDK_PATH "${PICO_SDK_PATH}" CACHE PATH "Path to the PICO SDK")
set(PICO_CXX_ENABLE_EXCEPTIONS 1) # for gciGPS

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)


# pico_set_program_name(foo "not foo") # as "foo" would be the default
# pico_set_program_description(foo "this is a foo")
# pico_set_program_version(foo "0.00001a")
# pico_set_program_url(foo "www.plinth.com/foo")

include(FetchContent)

project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O3 ")
string(APPEND CMAKE_C_FLAGS_RELEASE " -O3 ")
string(PREPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--print-memory-usage ")


# Initialize the SDK
pico_sdk_init()

if(PICO_SDK)
    add_definitions(-DPICO_SDK=1)
endif()

# message(STATUS ">> test: ${PICO_SDK}  ${PICO_BOARD} ${ARDUINO}")

if(PROJECT_IS_TOP_LEVEL)
    cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
    # cmake_host_system_information(RESULT CORES QUERY NUMBER_OF_LOGICAL_CORES)
    cmake_host_system_information(RESULT OSN QUERY OS_NAME)
    cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
    cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

    message(STATUS "-------------------------------------")
    message(STATUS "  Project: ${PROJECT_NAME}")
    message(STATUS "  Hardware: ${PICO_BOARD}")
    message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
    message(STATUS "  C ${CMAKE_C_STANDARD}")
    message(STATUS "-------------------------------------")
    message(STATUS " ${HOST}")
    message(STATUS " ${OSN}: ${OS_VERSION}")
    message(STATUS " ${PROC}")
    message(STATUS "-------------------------------------")

    # set(BUILD_EXAMPLES ON)
    # set(BUILD_DEV ON)
else()
    message(STATUS "-> ${PROJECT_NAME} is submodule")
    # set(BUILD_DEV OFF)
    # set(BUILD_GTESTS OFF)
endif()

# squaterion ------------
# FetchContent_Declare(squaternion
#     GIT_REPOSITORY "https://github.com/MomsFriendlyRobotCompany/squaternion.git"
#     GIT_TAG "origin/master"
#     SOURCE_SUBDIR "cpp"
# )

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


# list(APPEND
#     libs
#         squaternion
#         # yivo
#         # messages
#         # gcisensors
#         # gcigps
# )

# FetchContent_MakeAvailable( ${libs} ) # only call this once with all libs


# include(secrets.cmake)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/robotlib
        ${CMAKE_SOURCE_DIR}/src/gci_sensors
        ${CMAKE_SOURCE_DIR}/src/gci_gps
        ${CMAKE_SOURCE_DIR}/src/yivo
        ${CMAKE_SOURCE_DIR}/src/picolib
        ${CMAKE_SOURCE_DIR}/src/squaternion
        ${CMAKE_SOURCE_DIR}/../../c_library_v2/)

target_link_libraries(
    ${PROJECT_NAME}
        pico_stdlib
        pico_multicore
        hardware_exception
        hardware_adc
        hardware_i2c
        hardware_uart
        hardware_pio
        hardware_pwm
        hardware_dma
        hardware_irq
        hardware_spi
        hardware_rtc
        ${libs}
)

# stdio -> USB --------------------------------------------------------
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# PIO state machine ---------------------------------------------------
# pico_generate_pio_header(${PROJECT_NAME}
#     ${CMAKE_SOURCE_DIR}/src/blink.pio
#     # OUTPUT_DIR ${CMAKE_SOURCE_DIR}
# )

# pico_generate_pio_header(${PROJECT_NAME}
#     ${CMAKE_SOURCE_DIR}/src/servo_pwm.pio
#     # OUTPUT_DIR ${CMAKE_SOURCE_DIR}
# )

# create uf2 output --------------------------------------------------
pico_add_extra_outputs(${PROJECT_NAME})

# fix cleanup command ------------------------------------------------
set(clean_files
    ${PROJECT_NAME}.bin
    ${PROJECT_NAME}.hex
    ${PROJECT_NAME}.elf
    ${PROJECT_NAME}.dis
    ${PROJECT_NAME}.elf.map
    ${PROJECT_NAME}.uf2)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        ADDITIONAL_CLEAN_FILES
            "${clean_files}")

# Show info after project bulid ---------------------------------------
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND echo -------------------------------------
            # && ls -al ${PROJECT_NAME}.uf2
            # && stat -f "%N %z / 2097152B of Flash" ${PROJECT_NAME}.uf2
            # && echo -------------------------------------
            && picotool info ${PROJECT_NAME}.uf2 -a
            && echo -------------------------------------
)

# if(${BUILD_DEV})
#     add_subdirectory(dev)
# endif()
