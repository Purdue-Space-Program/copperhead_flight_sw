### Usage:
### ```cmake
### add_driver(<driver_name> <driver source files>)
### ```
### `<driver_name>` should be the the directory name
function(add_driver name)
    message("Adding driver: \"${name}\"")

    add_library(${name} OBJECT 
    ${CMAKE_CURRENT_SOURCE_DIR}/${name}/${ARGN}
    )

    target_include_directories(${name} INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/${name}
    )

    target_link_libraries(${name} stm_hal cmsis)
endfunction()

if(NOT "${TARGET}" STREQUAL NATIVE)
    add_driver(phy phy.cpp)
endif()
