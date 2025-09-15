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
        target_link_libraries(${name} stm_hal)
    endif()

    add_custom_command(TARGET ${name} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${name}> ${name}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${name}> ${name}.bin
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${name}>
        COMMENT "Building hex and bin files"
    )

endfunction()

add_app(example main.cpp)

