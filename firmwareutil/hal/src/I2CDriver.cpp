#include "I2CDriver.h"
#include "util.h"

void I2CDriver::configure() const {
	SCL.configure();
	SDA.configure();
}

bool I2CDriver::WriteRead(uint8_t Address, const uint8_t * WData, uint32_t WriteLen, const uint8_t *RData, uint32_t NumBytes) const {
	if (!Start(Address, RW::WRITE)) {
		Stop();
		return false;
	}
	if (!TransferBytes(RW::WRITE, Address, WData, WriteLen)) {
		Stop();
		return false;
	}

	if (!Restart(Address, RW::READ)) {
		return false;
	}

	bool Result = TransferBytes(RW::READ, Address, RData, NumBytes);
	Stop();
	return Result;
}

bool I2CDriver::Write(uint8_t Address, const uint8_t * Data, uint32_t NumBytes) const {
	if (!Start(Address, RW::WRITE)) {
		Stop();
		return false;
	}
	bool Result = TransferBytes(RW::WRITE, Address, Data, NumBytes);
	Stop();
	return Result;
}

bool I2CDriver::Read(uint8_t Address, const uint8_t * Data, uint32_t NumBytes) const {
	if (!Start(Address, RW::READ)) {
		Stop();
		return false;
	}
	bool Result = TransferBytes(RW::READ, Address, Data, NumBytes);
	Stop();
	return Result;
}

bool I2CDriver::TransferBytes(RW rw, uint8_t Address, const uint8_t * Data, uint32_t NumBytes) const {
	bool Result = true;

	uint32_t x = 0;
	for (x = 0; x < NumBytes; x++) {
		if (RW::WRITE == rw) {
			Transfer(RW::WRITE, Data[x], Result);
		} else {
			//send ack on all bytes but the last
			bool SendAck = x != (NumBytes - 1); //TODO do something with ACK?
			((uint8_t*)Data)[x] = Transfer(RW::READ, SendAck, Result);
		}

		if (!Result) {
			break;
		}
	}
	return Result;
}

void I2CDriver::Delay() const {
	_delay_us(ClockDelayus);
}

[[nodiscard]] bool I2CDriver::Start(uint8_t Address, RW rw) const {
	SCL.set_level(true);
	SDA.set_level(true);
	Delay();
	SDA.set_level(false);
	Delay();
	SCL.set_level(false);
	Delay();
	bool Result = true;
	Transfer(RW::WRITE, (Address<<1) | (uint8_t)rw, Result);
	return Result;
}

[[nodiscard]] bool I2CDriver::Restart(uint8_t Address, RW rw) const {
	SDA.set_level(true);
	if (!ITERATION_TIMEOUT(SDA.get_level(), true, BusTimeout, 1)) {
		Stop();
		return false;
	}
	Delay();

	SCL.set_level(true);
	if (!ITERATION_TIMEOUT(SCL.get_level(), true, BusTimeout, 1)) {
		Stop();
		return false;
	}
	Delay();

	if (!Start(Address, rw)) {
		Stop();
		return false;
	}
	return true;
}

void I2CDriver::Stop() const {
	SDA.set_level(false);
	Delay();
	SCL.set_level(true);
	Delay();
	SDA.set_level(true);
	Delay();
	// TODO wait until SCL and SDA are release by slave
}

uint8_t I2CDriver::Transfer(RW rw, uint8_t Param, bool &Result) const {
	uint8_t Data = (RW::READ == rw) ? 0 : Param;
	bool ACK = Param;
	int8_t x = 0;
//	volatile Safety s;
	Result = true;

	for (x = 7; x >= 0; x--) {
		if (RW::WRITE == rw) {
			SDA.set_level((Data >> x) & 0x01);
		} else {
			SDA.set_level(true);
		}

		Delay();
		SCL.set_level(true);
		//wait until slave releases clock (clock stretching)
		if (!ITERATION_TIMEOUT(SCL.get_level(), true, BusTimeout, 1)) {
			Result = false;
			Stop();
			return 0;
		}

		Delay();
		if (RW::READ == rw) {
			Data |= (SDA.get_level() << x);
		}
		SCL.set_level(false);
	}

	if (RW::WRITE == rw) {
		Result = WaitACK();
	} else {
		Result = SendACK(ACK);
	}
	return Data;
}

[[nodiscard]] bool I2CDriver::SendACK(bool ACK) const {
	bool Result = true;
	SDA.set_level(!ACK);// maybe better?
	Delay();
	SCL.set_level(true);
	//wait until slave releases clock (clock stretching)
	if (!ITERATION_TIMEOUT(SCL.get_level(), true, BusTimeout, 1)) {
		Result = false;
	}

	//provide full high clock
	Delay();
	SCL.set_level(false);
	Delay();

	SDA.set_level(true);
	return Result;
}

[[nodiscard]] bool I2CDriver::WaitACK() const {
	bool Result = true;
	//release SDA for ACK
	SDA.set_level(true);
	Delay();
	SCL.set_level(true);
	//wait until slave releases clock (clock stretching)
	if (!ITERATION_TIMEOUT(SCL.get_level(), true, BusTimeout, 1)) {
		Result = false;
	}
	if (Result) {
		//if SDA is low, we have ACK
		Result = !SDA.get_level();
	}

	//provide full high clock
	Delay();
	SCL.set_level(false);
	return Result;
}
