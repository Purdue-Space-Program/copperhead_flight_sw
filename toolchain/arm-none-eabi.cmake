set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TARGET_FLAGS       "-mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard")
set(COMMON_WARN_FLAGS  "-Wall -fdata-sections -ffunction-sections")

set(_C_INC_LIST
  " -I /usr/lib/arm-none-eabi/include"
  " -I /usr/lib/gcc/arm-none-eabi/*/include"
  " -I /usr/lib/gcc/arm-none-eabi/*/include-fixed"
)
set(_CXX_INC_LIST
  ${_C_INC_LIST}
  " -I /usr/lib/arm-none-eabi/include/c++/*"
  " -I /usr/lib/arm-none-eabi/include/c++/*/arm-none-eabi"
  " -I /usr/lib/arm-none-eabi/include/c++/*/backward"
)
string(JOIN "" ARM_GCC_C_INC_STR   ${_C_INC_LIST})
string(JOIN "" ARM_GCC_CXX_INC_STR ${_CXX_INC_LIST})

string(APPEND CMAKE_C_FLAGS_INIT
  " ${TARGET_FLAGS} ${COMMON_WARN_FLAGS}"
  "${ARM_GCC_C_INC_STR}"
)
string(APPEND CMAKE_CXX_FLAGS_INIT
  " ${TARGET_FLAGS} ${COMMON_WARN_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics"
  "${ARM_GCC_CXX_INC_STR}"
)
string(APPEND CMAKE_ASM_FLAGS_INIT
  " ${TARGET_FLAGS} ${COMMON_WARN_FLAGS} -x assembler-with-cpp"
)

set(CMAKE_C_FLAGS_DEBUG_INIT     "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE_INIT   "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG_INIT   "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Os -g0")

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(LDSCRIPT "${CMAKE_SOURCE_DIR}/hal/startup/stm32h730/STM32H730XX_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS_INIT
  "--sysroot=/usr/lib/arm-none-eabi ${TARGET_FLAGS} -Wl,-T,${LDSCRIPT} --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections -Wl,--print-memory-usage"
)
