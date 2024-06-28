#pragma once
#include "memprog.hpp"
#include "DFDriver.h"

class DFProg : public MemProg {
public:
	constexpr DFProg(const DFDriver & df) :
	MemProg(),
	df(df)
	{}

private:
	const DFDriver & df;

	void CMD_MASS_ERASE() override;
	void CMD_PROG_VERIFY() override;
	void CMD_READ() override;
};
