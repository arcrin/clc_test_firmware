#pragma once
#include "IOMAP.h"

class AFIODriver {
    public:
        enum class PERIPHERAL_MAP : uint8_t {
            CAN = 13,
        };
        constexpr AFIODriver(
            PERIPHERAL_MAP peripheral, 
            uint8_t remap_value) : 
            peripheral(peripheral), 
            remap_value(remap_value) {}
        AFIODriver() =  delete;
        constexpr ~AFIODriver() = default;

        static void DriverInit();

        /// @brief Remap peripheral based on the settings provided in the constructor
        void configure() const;

        /// @brief  Reset the peripheral mapping to its default state
        void reset() const;

    private:
        AFIO_TypeDef * const afio() const {return reinterpret_cast<AFIO_TypeDef *>(AFIO_BASE);};
        PERIPHERAL_MAP peripheral;
        uint8_t remap_value;
};