### Usage:
### ```cmake
### add_driver(<driver_name> <driver source files>)
### ```
### `<driver_name>` should be the the directory name
function(add_simulated_driver name)
    message("Adding simulated driver: \"${name}\"")

    add_library(${name} OBJECT 
    ${CMAKE_CURRENT_SOURCE_DIR}/${name}/${ARGN}
    )

    target_include_directories(${name} INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/${name}
    )

endfunction()

add_simulated_driver(adc adc.cpp)
