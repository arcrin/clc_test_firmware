unset(HAL_FILES)
unset(HAL_FLAGS)
unset(HAL_INCLUDES)
unset(HAL_DEFINES)
unset(HAL_INC_DIRS)
unset(HAL_SRC_GLOBS)
unset(HAL_DIRS)

if(HAL_PTH STREQUAL "")
    message(FATAL_ERROR "HAL_PATH not defined")
endif()

# HAL specific cmake file contains HAL_DIRS
# HAL_DIRS indicate where the hardware abstraction layer codes is located
# These codes are usually organized in a hierarchical manner
# For example, the HAL for STM32F103xB can be organized as follows:
# - STM32x
# - STM32Fx
# - STM32F1x
# - STM32F103xB
# This is due the fact certain peripheral memories are shared among the STM32 family
# and some are specific to the STM32F103xB
# Organizing the HAL in this manner allows for code reuse
include(${HAL_PATH}/${HAL}.cmake)


# Here we go through all HAL related directories and define three lists:
# - HAL_SRC_GLOBS: the list of source files used to build final executable
# - HAL_INC_DIRS: the list of include directories, where the compiler will look for header files
# - HAL_DEFINES: the list of preprocessor definitions
foreach(DIR ${HAL_DIRS})
    foreach(RESOURCE ${HAL_CONFIG})
        list(APPEND HAL_SRC_GLOBS ${HAL_PATH}/src/${DIR}/${RESOURCE}*)
    endforeach()
    list(APPEND HAL_INC_DIRS ${HAL_PATH}/src/${DIR})
    list(APPEND HAL_SRC_GLOBS ${HAL_PATH}/src/${DIR}/*.c)
    # Add MCU flags (e.g. HAL_STM32x, HAL_STM32Fx, HAL_STM32F1x, HAL_STM32F103xB)
    list(APPEND HAL_DEFINES HAL_${DIR})
endforeach()


# Here we further expands HAL_SRC_GLOBS and HAL_DEFINES lists 
foreach(RESOURCE ${HAL_CONFIG})
    # Expand HAL_SRC_GLOBS to contain source files from the root HAL src directory
    # Here we can find the interface definitions (header and sometimes source files) for most common peripherals
    # For example, the GPIO
    list(APPEND HAL_SRC_GLOBS ${HAL_PATH}/src/${RESOURCE}*)
    # Expand HAL_SRC_GLOBS to contain source files from the IOResource directory
    # The code IOResource directory defines the general command interface we use for the final test firmware
    # Refer to the documentaion of "resourceshell" for more information on command interface
    list(APPEND HAL_SRC_GLOBS ${HAL_PATH}/src/IOResource/${RESOURCE}*)
    # Expand HAL_DEFINES peripheral related flags
    list(APPEND HAL_DEFINES HAL_CONFIG_${RESOURCE})
endforeach()

# I believe this line is meant to specifically include the startup.c in the HAL_SRC_GLOBS list
list(APPEND HAL_SRC_GLOBS ${HAL_PATH}/src/*.c)
# Expand HAL_INC_DIRS to include headers from IOResource directory
list(APPEND HAL_INC_DIRS ${HAL_PATH}/src/IOResource)

message("HAL_SRC_GLOBS: ${HAL_SRC_GLOBS}")

# Essentially renaming the HAL_SRC_GLOBS list to HAL_FILES
file(GLOB HAL_FILES ${HAL_SRC_GLOBS})

# Include the rest of the source files that is not include in MCU or peripheral specific directories
list(APPEND HAL_FILES
    ${HAL_PATH}/src/RingBUffer.cpp
    ${HAL_PATH}/src/SYSTICK.cpp
    ${HAL_PATH}/src/util.cpp
    ${HAL_PATH}/src/HAL.CPP    
)

# Include the rest of the header files, and put them all in HAL_INCLUDES list
list(APPEND HAL_INCLUDES ${HAL_PATH}/src ${HAL_PATH}/src/${HAL} ${HAL_INC_DIRS} ${HAL_PATH}/src/cmsis)