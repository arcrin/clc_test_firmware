#include "PWMDriver.h"
#include "IOMAP.h"

void PWMDriver::DriverInit() {
}

void PWMDriver::configure() const {
	// NOTE This HAL makes no attempt to control the PWM frequency. It will depend on the peripheral clock frequency
	hal.info.ENR_REG() |= 1 << hal.info.ENR_POS;

	pin.configure();
	*hal.CCMR() |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE) << hal.CCMR_Offset;
	hal.timer()->CCER |= (TIM_CCER_CC1E) << hal.CCER_Offset;
	hal.timer()->ARR = 4095;
	*hal.CCR() = 0;

	hal.timer()->PSC = 0;
	hal.timer()->BDTR |= TIM_BDTR_MOE;
	hal.timer()->EGR |= TIM_EGR_UG;
	hal.timer()->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
}

void PWMDriver::Write(uint8_t Percent) const {
	Percent = Percent > 100 ? 100 : Percent;
	int32_t PWMValue = 4095 * Percent / 100;

	if (PWMValue < 0) {
		PWMValue = 0;
	} else if (PWMValue > 4095) {
		PWMValue = 4095;
	}

	*hal.CCR() = PWMValue;
}
