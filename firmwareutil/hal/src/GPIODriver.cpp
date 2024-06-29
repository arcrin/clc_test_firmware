#include "GPIODriver.h"

void GPIODriver::configure() const {
	configure(mode, alternate, type, speed, pull);
	set_level(initial_level);
}

void GPIODriver::set_mode(MODE m) const {
	configure(m, alternate, type, speed, pull);
}

void GPIODriver::set_pull(PULL p) const {
	configure(mode, alternate, type, speed, p);
}

void GPIODriver::set_type(TYPE t) const {
	configure(mode, alternate, t, speed, pull);
}

void GPIODriver::set_speed(SPEED s) const {
	configure(mode, alternate, type, s, pull);
}

void GPIODriver::set_alternate(uint8_t a) const {
	configure(mode, a, type, speed, pull);
}

void GPIODriver::set_polarity(bool p) {
	polarity = p;
	set_level(current_level);
}

void GPIODriver::set_level(bool state) const {
	current_level = state;
	set_physical_level(current_level ^ polarity);
}

bool GPIODriver::get_level() const {
	return get_physical_level() ^ polarity;
}