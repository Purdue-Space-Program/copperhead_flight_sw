### Usage:
### ```cmake
### add_simulated_driver(<simulated_driver_name> <simulated_driver_source_file>)
### ```
### `<simulated_drvier_name>` should be the the directory name
function(add_simulated_driver name)
    message("Adding simulated driver: \"${name}\"")
    
    add_library(${name} OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/${name}/${ARGN}
    )

    target_include_directories(${name} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/${name}
    )

endfunction()

if("${TARGET}" STREQUAL NATIVE)
    add_simulated_driver(mag mag.cpp)
endif()
