Here is a more structured and detailed version of the documentation for the HAL (Processor Hardware Abstraction Library). This documentation provides clarity on the various components, their usage, and how to integrate and configure the HAL in your projects.

---

## HAL - Processor Hardware Abstraction Library

### Overview
The HAL library provides drivers for the hardware peripherals of various processors. The library is designed to be reusable across multiple MCUs, organized in a layered fashion to minimize code duplication and memory usage.

### Types of Drivers
1. **Processor-specific Drivers**:
   - Control hardware peripherals of the processor.
   - Must be re-implemented for each new processor.
   - Examples: UART, ADC, GPIODriver, PWMDriver.

2. **Processor-agnostic / Utility Drivers**:
   - Do not directly rely on processor peripherals.
   - Use other drivers to implement their functionality.
   - Examples: 
     - I2C (uses GPIODriver)
     - DF (uses SPI)
     - UPDI (uses UART)

3. **MemProg**:
   - `*Prog` drivers are `MemProg` subclasses for use with `MemProgLib`.

4. **IOResource**:
   - Classes in the `IOResource` folder are `IOResource` subclasses for use with `ResourceShell`.

### Firmware Application Integration
Configuration and integration of the HAL are achieved by defining CMake variables and including CMake recipes. The CMake dependency graph follows this structure:

1. **Project Root CMakeList**:
   - `CMakeLists.txt`: Includes `${TARGET_NAME}.cmake` for each target.

2. **Target-specific CMake File**:
   - `${TARGET_NAME}.cmake`: Defines target-specific HAL and HAL_CONFIG. Includes `target_common.cmake`.

3. **Common Target CMake File**:
   - `target_common.cmake`: Includes `${HAL_PATH}/HALConfig.cmake` and performs target build steps.

4. **HAL Configuration File**:
   - `${HAL_PATH}/HALConfig.cmake`: Includes `${HAL_PATH}/${HAL}.cmake` and populates `HAL_FILES`, `HAL_FLAGS`, `HAL_INCLUDES`, `HAL_DEFINES`, and `HAL_DIRS`.

5. **Processor-specific HAL File**:
   - `${HAL_PATH}/${HAL}.cmake`: Populates processor-specific `HAL_FLAGS` and defines the source directories for this HAL.

### Application CMake Project Update
Add the following changes to the application's `CMakeLists.txt`:

```cmake
# `HAL_PATH` is the relative path from the application CMake project root to the HAL project root
# It is used by the included CMake recipes
set(HAL_PATH firmwareutil/hal)

# If there are multiple targets, repeat the rest for each target (typically this is done in a `foreach` loop)

# Set `TARGET_NAME` to something representative of the current target
# NOTE: This is not strictly part of the HAL configuration, as it is only used by the applications `target_common.cmake`.
set(TARGET_NAME ...)

# Include the target-specific CMake recipe, which will define which drivers are enabled
include(${TARGET_NAME}.cmake)
```

### Creating Target-specific CMake Recipes
For each build target, create a `${TARGET_NAME}.cmake` with the following content:

```cmake
# Set `HAL` to one of the available HAL targets (See CMakeLists.txt ${TARGETS})
# This is used to select the correct HAL source directories
set(HAL K20)

# `HAL_CONFIG` is a list of drivers to enable.
# Globbing expressions are used, so for example adding `SPI` to the list will enable
# `SPIDriver`, `SPIDevice`, and `SPIResource`
set(HAL_CONFIG
    SPI
    I2C
    #...
    )

# Include the recipe common to all targets.
include(target_common.cmake)

# Any other target-specific stuff can go here as well
```

### Creating/Updating the Common Target CMake Recipe
Each target-specific recipe includes `target_common.cmake`, which contains all the boilerplate to set up source directories, include directories, compiler flags, compiler definitions, linker scripts, etc. A template is available in `target_common_template.cmake`.

### HAL Implementation Details

#### Directory Structure
- `hal/`
  - `linker/`: Linker scripts
  - `src/`: Driver source code
    - `cmsis/`: ARM CMSIS header files
    - `IOResource/`: IOResource subclasses
    - `STM32x/K20/RA6M4/...`: Processor-specific HAL implementation directories
      - `vectors.c, .h`: Processor interrupt vector declarations
      - `${HAL}.h`: Processor header file, defining peripheral base addresses and register layout
      - `*Driver.cpp`: Processor-specific driver implementation
      - `*HAL.h`: Processor-specific hardware abstraction structure
    - `*Driver.h`: Driver API declaration
    - `*Driver.cpp`: Common driver implementation
  - `test/`: Test suite containing sample firmware to exercise each HAL variant
  - `CMakeLists.txt`: CMakeList for the test suite
  - `HALConfig.cmake`: Main recipe to be included by test firmwares

### Driver Classes
Drivers are represented by C++ classes, where each instance of a hardware peripheral is controlled by a separate instance of the class.

#### Driver Class Declaration
- The API for a driver class is declared in a single `*Driver.h` file, located in the `src` directory.
- Wherever possible, data and methods in the driver should be `const`/`constexpr` to allow for compile-time evaluation.

At a minimum, the driver class must contain the following functions:

- **Constructor**:
  ```cpp
  constexpr ABCDriver::ABCDriver(...) { /* Constructor implementation */ }
  ```
- **Driver Initialization**:
  ```cpp
  static void DriverInit() { /* Global peripheral subsystem configuration */ }
  ```
- **Configuration Method**:
  ```cpp
  void configure(...) const { /* Specific peripheral instance configuration */ }
  ```

Processor-specific configuration is abstracted to a `*HAL` class.

#### Example: GPIODriver
```cpp
struct GPIOHAL {
    uint32_t gpio_address;
    uint8_t pin_number;
};
static constexpr const GPIODriver gpio {{PTD_BASE, 13}, GPIODriver::MODE::INPUT, 0, false, false};

struct GPIOHAL {
    uint8_t port_number;
    uint8_t pin_number;
};
static constexpr const GPIODriver gpio {{2, 9}, GPIODriver::MODE::OUTPUT, 0, false, true};
```

#### Driver Class Definition
For each processor, create a separate `*Driver.cpp` file in its respective source directory, e.g., `src/STM32F427`. This file contains processor-specific implementations for the functions declared in the driver header file. If the implementation is common to multiple processors, place the file in one of the 'common' source directories, e.g., `src/STM32Fx`.

#### Driver Source Directories
When building the project, the correct source directories are selected based on the `HAL_DIRS` CMake variable, defined in `${HAL}.cmake`.

#### Shared Driver Code
To manage code shared between multiple processors, a macro definition is automatically created for each source directory, e.g., `HAL_{$DIRECTORY}`. These can be used in the source code.

```cpp
// STM32x compatible code
#if defined(HAL_STM32Fx)
// STM32Fx specific part
#elif defined(HAL_STM32F4x)
// STM32F4x specific part
#elif defined(HAL_STM32F427)
// STM32F427 specific part
#else
#error "Not implemented"
#endif
```

### Adding a New Processor
To add support for a new processor named ABC123:

1. Create a directory `src/ABC123`.
2. Create `vectors.c` and `vectors.h` in the directory, following the format of an existing implementation.
3. Add the processor register map header file (obtained from the manufacturer) to this directory.
4. If needed, modify `enum IRQInterruptIndex` to map each interrupt to its interrupt index.
5. Update `src/IOMAP.h` to include this header file based on the auto-generated macro `HAL_ABC123`. Implement `IRQn_Type ToIRQ(IRQInterruptIndex index)` for this processor.
6. Create `ABC123.cmake`. Set `HAL_FLAGS` to flags that need to be passed to the compiler. Set `HAL_DIRS` to the list of source directories to build (ABC123).
7. If available, add an SVD file for the processor to the `svd` directory.
8. Update the test suite:
   - Add the new processor to `TARGETS` in `CMakeLists.txt`.
   - Copy and modify one of the existing test suites in the `test` directory to fully test the capabilities of any drivers implemented for the new processor.

### Examples
#### Create an Output Pin
```cpp
#include "GPIODriver.h"
static constexpr const GPIODriver NFC_PWR {{PTB_BASE, 17}, GPIODriver::MODE::OUTPUT};
int main() {
    GPIODriver::DriverInit();
    NFC_PWR.configure();
    NFC_PWR.set_level(true);
}
```

#### Create an I2C Port
```cpp
#include "I2CDriver.h"
static constexpr const I2CDriver SENSOR {
    {{4, 0}, GPIODriver::MODE::OUTPUT, 0, false, true, GPIODriver::TYPE::OPEN_DRAIN},
    {{4, 1}, GPIODriver::MODE::OUTPUT,