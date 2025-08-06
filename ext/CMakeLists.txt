add_library(etl INTERFACE)
target_include_directories(etl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/etl/include)


add_library(cmsis INTERFACE)
target_include_directories(cmsis INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Include)


add_subdirectory(freertos)
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_POSIX" CACHE STRING "" FORCE)
if (CMAKE_CROSSCOMPILING)
    # presumed STM32H7xx
    set(FREERTOS_PORT "GCC_ARM_CM7" CACHE STRING "" FORCE)
endif()
