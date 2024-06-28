#pragma once
#include "memprog.hpp"

class FLASHProg : public MemProg {
public:
	/// Initialize the hardware
	void Init() override;

	/// Transition from reading to program/erase
	static void ROM_READ_to_ROM_PE();

	// TODO this should be combined with `ROM_READ_to_ROM_PE`, which should take the address as a parameter and act accordingly
	/// Transition from reading to program/erase of the data area
	static void ROM_READ_to_DATA_PE();
	/// Transition from program/erase to reading
	static void PE_to_ROM_READ();

	/// Return the byte alignment and programming width needed to program at a certain address
	/// Note: it's assumed that the alignment and programming width are equal. If not, this function should return
	///  the greater of the two.
	/// \param Address The address to start programming
	/// \return The byte alignment needed at this address
	static uint32_t GetAlignment(uint32_t Address);

	/// Program data to the flash
	/// \param Destination The address to program to. Must be aligned per GetAlignment(Destination)
	/// \param Source The data to program. This buffer must be at least as long as specified by GetAlignment(Destination)
	/// \return 0 on success
	static uint32_t PGM(uint32_t Destination, const uint8_t * Source);

private:
	void CMD_MASS_ERASE() override;
	void CMD_PROG_VERIFY() override;
//	void CMD_PROG() override;
//	void CMD_CRC() override;
};
