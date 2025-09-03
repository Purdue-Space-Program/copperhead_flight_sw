cmake_minimum_required(VERSION 3.25)

## Default target
if(NOT "${TARGET}")
    set(TARGET "STM32H730" CACHE STRING "Build target")
endif()

if("${TARGET}" STREQUAL "Native")
    set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
    set(CMAKE_C_FLAGS 
        "${CMAKE_C_FLAGS} -Wall -fdata-sections -ffunction-sections"
    )

    set(CMAKE_C_FLAGS_DEBUG     "-O0 -g3")
    set(CMAKE_C_FLAGS_RELEASE   "-Os -g0")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

    set(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics"
    )
    set(TOOLCHAIN_LINK_LIBRARIES "${TOOLCHAIN_LINK_LIBRARIES} m pthread rt")
else()
    set(CMAKE_TOOLCHAIN_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/toolchain/arm-none-eabi.cmake
        CACHE FILEPATH "toolchain file"
        FORCE
    )
endif()

project(copperhead C CXX ASM)

# must come after `project()`
string(APPEND CMAKE_EXE_LINKER_FLAGS " -Wl,-Map=${PROJECT_NAME}.map")

## Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})
message("Target: " ${TARGET})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
if("${TARGET}" STREQUAL STM32H730)
    add_subdirectory(ext)
    add_subdirectory(drivers)
    add_subdirectory(hal)
    add_subdirectory(applications)
elseif("${TARGET}" STREQUAL Native)
    add_subdirectory(ext)
    add_subdirectory(applications)
    add_subdirectory(simulations)
endif()
