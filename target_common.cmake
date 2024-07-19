set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/${TARGET_NAME})

include(${HAL_PATH}/HALConfig.cmake)

message("HAL_FILES= ${HAL_FILES}")
message("HAL_FLAGS= ${HAL_FLAGS}")
message("HAL_INCLUDES= ${HAL_INCLUDES}")
message("HAL_DEFINES= ${HAL_DEFINES}")
message("HAL_DIRS= ${HAL_DIRS}")

# Directory name transformation
# E.g. STM32x -> src/STM32x
list(TRANSFORM HAL_DIRS PREPEND src/ OUTPUT_VARIABLE APP_HAL_INCLUDES)
list(TRANSFORM APP_HAL_INCLUDES APPEND /* OUTPUT_VARIABLE APP_HAL_GLOBS)

# These are the product (board) specific source files. 
file(GLOB TARGET_FILES
    src/${DELTA_BOARD}/*
    src/${TARGET_NAME}/*
    ${APP_HAL_GLOBS}
)

message(STATUS "APP_FILES: ${APP_FILES}")
message(STATUS "TARGET_FIELS: ${TARGET_FIELS}")
message(STATUS "HAL_FILES: ${HAL_FILES}")

add_executable(${TARGET_NAME}
    ${APP_FILES}
    ${TARGET_FILES}
    ${HAL_FILES}
)

# Preprocessor definitions (i.e. #define)
target_compile_definitions(${TARGET_NAME} PRIVATE
    DELTA_BOARD_${DELTA_BOARD}
    TARGET_${TARGET_NAME}    
    ${HAL_DEFINES}
)

target_include_directories(${TARGET_NAME} PUBLIC
    src/${DELTA_BOARD}
    src/${TARGET_NAME}
    ${APP_HAL_INCLUDES}
    ${HAL_INCLUDES}
)

set_target_properties(${TARGET_NAME} PROPERTIES
	SUFFIX ".elf"
	RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
	COMPILE_FLAGS "${TARGET_FLAGS} ${HAL_FLAGS}"
	LINK_FLAGS "${COMMON_LINKER_FLAGS} ${TARGET_FLAGS} ${HAL_FLAGS} -Wl,-Map,\"${OUTPUT_DIR}/${TARGET_NAME}.map\" -L ${LINKER_SCRIPT_DIR} -T ${LINKER_SCRIPT_DIR}/${LINKER_SCRIPT_PREFIX}${HAL}.ld"
)

add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -Dsource="${TARGET_NAME}.map" -Ddestination="${TARGET_NAME}_prev.map" -P "${CMAKE_SOURCE_DIR}/copy.cmake"
	WORKING_DIRECTORY ${OUTPUT_DIR}
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
	COMMAND arm-none-eabi-objcopy -O ihex ${TARGET_NAME}.elf ${TARGET_NAME}.hex
	COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_NAME}.hex ${CMAKE_SOURCE_DIR}/../firmware/test_firmware/${TARGET_NAME}.hex
	COMMAND arm-none-eabi-objcopy -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin
	COMMAND arm-none-eabi-nm -C -l -n -S ${TARGET_NAME}.elf > ${TARGET_NAME}.sym
	COMMAND arm-none-eabi-nm -C -l --size-sort -S ${TARGET_NAME}.elf > ${TARGET_NAME}.symsize
	COMMAND arm-none-eabi-objdump -S -C ${TARGET_NAME}.elf > ${TARGET_NAME}.lst
	COMMAND ${CMAKE_COMMAND} -Dsource="${TARGET_NAME}.siz" -Ddestination="${TARGET_NAME}_prev.siz" -P "${CMAKE_SOURCE_DIR}/copy.cmake"
	COMMAND arm-none-eabi-size ${TARGET_NAME}.elf > ${TARGET_NAME}.siz
	COMMAND python ${CMAKE_SOURCE_DIR}/diff_size.py ${TARGET_NAME}_prev.siz ${TARGET_NAME}.siz
	WORKING_DIRECTORY ${OUTPUT_DIR}
)
