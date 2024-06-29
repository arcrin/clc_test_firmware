#include "ADCDriver.h"

void ADCDriver::DriverInit() {
}

void ADCDriver::configure() const {
	hal.adc_pin.configure();
	// NOTE this is run for every ADCDriver instance, many of which share the same ADC address.
	//  It only needs to run once per ADC address
	RCC->APB2ENR |= 1 << hal.info.ENR_POS;

	// Not tested above 168 MHz
	static_assert(F_CPU <= 168000000);
	// 84 MHz divide by 4 = 21 MHz
	ADC->CCR |= ADC_CCR_ADCPRE_0;

	// set sampling time to maximum
	if (hal.channel < 10) {
		hal.adc()->SMPR2 |= 7 << (3 * hal.channel);
	} else {
		hal.adc()->SMPR1 |= 7 << (3 * (hal.channel - 10));
	}
	// regular channel sequence length = 1
	hal.adc()->SQR1 = 0;
	// clear out sequence
	hal.adc()->SQR3 = 0;
	// turn on ADC
	hal.adc()->CR2 |= ADC_CR2_ADON;
}

bool ADCDriver::Read(uint16_t &result) const {
	hal.adc()->SQR3 = (uint8_t)hal.channel;

	// start conversion
	hal.adc()->CR2 |= ADC_CR2_SWSTART;
	// wait for conversion to finish
	while (!(hal.adc()->SR & ADC_SR_EOC));
	result = hal.adc()->DR;
	return true;
}
