cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(clc_test ASM C CXX)

set(HAL_PATH firmwareutil/hal)

set(TARGETS
    CLC:STM32F103xB
)

# NOTE: define F_CPU
# The clock speed of the STM32F103V8 might be different

add_compile_definitions(
    F_CPU=8000000
)

include_directories(
    src
    firmwareutil/memproglib
    firmwareutil/resourceshell
)

# Collect the application source files
file(GLOB APP_FILES
    src/*                           
    firmwareutil/resourceshell/*    
)

message(STATUS "All source files ${APP_FILES}")

# Compiler and linker flags
SET(DEBUG_OPTIMIZATION false)
#SET(TRACE_MALLOC true)
if(${DEBUG_OPTIMIZATION})
    set(OPTIMIZATION_FLAGS "-g3 -O0")
    set(LINKER_SCRIPT_PREFIX Debug_)
else()
    # set(OPTIMIZATION_FLAGS "-g3 -O3")
    set(OPTIMIZATION_FLAGS "-Os -flto")
    set(LINKER_SCRIPT_PREFIX "")
endif()

message(STATUS "OPTIMIZATION_FLAGS= ${OPTIMIZATION_FLAGS}")
message(STATUS "LINKER_SCRIPT_PREFIX= ${LINKER_SCRIPT_PREFIX}")

SET(COMMON_FLAGS "${OPTIMIZATION_FLAGS} -Wall -Wundef -Wdouble-promotion -Wno-unknown-pragmas -fstack-usage -Wstack-usage=1024 -g3 -mthumb -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wuninitialized -Wmissing-declarations -Wlogical-op -Wfloat-equal -MD -MP -MF \"$$(@:%.o=%.d)\" -MT\"$$(@:%.o=%.d)\" -MT\"$$(@:%.o=%.o)\"")
SET(CMAKE_C_FLAGS "${COMMON_FLAGS}")
SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -Wodr -Wno-volatile -Wno-register -fno-rtti -fno-exceptions")

message("COMMON_FLAGS= ${COMMON_FLAGS}")

SET(LINKER_SCRIPT_DIR ${CMAKE_SOURCE_DIR}/linker/)
SET(COMMON_LINKER_FLAGS "--specs=nano.specs --specs=nosys.specs -Wl,--gc-sections")
if(${TRACE_MALLOC})
	SET(COMMON_LINKER_FLAGS "${COMMON_LINKER_FLAGS} -Xlinker --wrap=malloc -Xlinker --wrap=free -Xlinker --wrap=_sbrk")
endif()


foreach(TARGET_DATA_STR ${TARGETS})
    string(REPLACE ":" ";" TARGET_DATA ${TARGET_DATA_STR})
    list(GET TARGET_DATA 0 DELTA_BOARD)
    list(GET TARGET_DATA 1 HAL)
    set(TARGET_NAME ${DELTA_BOARD}_${HAL})

    set(HAL_CONFIG
        GPIO
        AFIO
        UART
        FLASHProg
        SYSTICK
    )
    message(STATUS "HAL_CONFIG= ${HAL_CONFIG}")
    include(${TARGET_NAME}.cmake)
endforeach()