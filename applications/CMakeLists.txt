# Usage:
#
# ```cmake
# add_app(bruh src...)
# ```
#
# `<app_name>` should be the the directory name
function(add_app name)
    message("Adding application: \"${name}\"")

    add_executable(${name} 
        ${CMAKE_CURRENT_SOURCE_DIR}/${name}/${ARGN}
    )

    target_include_directories(${name} INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/${name}
    )

    target_link_libraries(${name} etl freertos_kernel)

    if(NOT "${TARGET}" STREQUAL "Native")
        target_link_libraries(${name} stm_hal cmsis stm32_system)

        if("${TARGET}" STREQUAL "STM32H753")
            target_sources(example PRIVATE 
                ${CMAKE_SOURCE_DIR}/hal/startup/stm32h753/startup_stm32h753xx.s
            )       
        endif()

    endif()

endfunction()

add_app(example main.cpp)

