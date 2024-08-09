#include "CANDriver.h"
#include "IOMAP.h"

void CANDriver::DriverInit() {
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
}

void CANDriver::configure() const {
    hal.TX.configure();
    hal.RX.configure();
    
    hal.can()->MCR = CAN_MCR_RESET;

    hal.can()->MCR |= CAN_MCR_AWUM  // Automatic Wakeup Mode
                    | CAN_MCR_ABOM  // Automatic Bus-Off Management
                    | CAN_MCR_NART  // No Automatic ReTransmission
                    | CAN_MCR_INRQ; // Request Initialization
    
    while (!(hal.can()->MSR & CAN_MSR_INAK));



    static_assert(F_CPU == 72000000);
    /********************************************************************************************
     * Using HSI with no prescaler
     * APB1 bus clock is 8MHz
     * Bit Rate     accuracy    Pres-scaler     Number of time quanta   Seg1       Seg2     SJW
     * 500k         0.0000      1               16                      13         2        1
     ********************************************************************************************/
    hal.can()->BTR = (3 << CAN_BTR_BRP_Pos)
                     | (10 << CAN_BTR_TS1_Pos)
                     | (5 << CAN_BTR_TS2_Pos)
                     | (1 << CAN_BTR_SJW_Pos);

    // Loopback mode for testing
    hal.can()->BTR |= CAN_BTR_LBKM;

    hal.can()->FMR = CAN_FMR_FINIT;
    hal.can()->FM1R &= 0;
    hal.can()->FS1R |= 0x7FFFFFF;
    hal.can()->FA1R = 0;
    hal.can()->FA1R = CAN_FA1R_FACT0;
    hal.can()->sFilterRegister[0].FR1 = 0;
    hal.can()->sFilterRegister[0].FR2 = 0;
    hal.can()->FMR = 0;

    hal.can()->MCR &= ~CAN_MCR_INRQ;
    while (hal.can()->MSR & CAN_TSR_TME0);
}

void CANDriver::Write(uint8_t Data[8], uint8_t Length, uint32_t ID) const {
    while (!(hal.can()->TSR & CAN_TSR_TME0));
    hal.can()->sTxMailBox[0].TIR = (ID << CAN_TI0R_STID_Pos);
    hal.can()->sTxMailBox[0].TDTR = Length;
    hal.can()->sTxMailBox[0].TDLR = (Data[3] << 24) | (Data[2] << 16) | (Data[1] << 8) | Data[0];
    hal.can()->sTxMailBox[0].TDHR = (Data[7] << 24) | (Data[6] << 16) | (Data[5] << 8) | Data[4];   
    hal.can()->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
    while (!(hal.can()->TSR & CAN_TSR_TXOK0));
}

bool CANDriver::Read(uint8_t Data[8], uint8_t &Length, uint32_t &ID, uint16_t &Timestamp, bool &Remote) const {
	if (!(hal.can()->RF0R & CAN_RF0R_FMP0)) {
		return false;
	}

	if (hal.can()->sFIFOMailBox[0].RIR & CAN_RI0R_IDE) {
		ID = (hal.can()->sFIFOMailBox[0].RIR & (CAN_RI0R_EXID_Msk | CAN_RI0R_STID_Msk)) >> CAN_RI0R_EXID_Pos;
	} else {
		ID = (hal.can()->sFIFOMailBox[0].RIR & CAN_RI0R_STID_Msk) >> CAN_RI0R_STID_Pos;
	}
	Remote = hal.can()->sFIFOMailBox[0].RIR & CAN_RI0R_RTR;
	Timestamp = (hal.can()->sFIFOMailBox[0].RDTR & CAN_RDT0R_TIME_Msk) >> CAN_RDT0R_TIME_Pos;
	Length = (hal.can()->sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos;

	uint8_t x;
	for (x = 0; x < Length; x++) {
		Data[x] = hal.can()->sFIFOMailBox[0].RDLR >> (x * 8);
	}
	for (x = 4; x < Length; x++) {
		Data[x] = hal.can()->sFIFOMailBox[0].RDHR >> ((x - 4) * 8);
	}

	hal.can()->RF0R |= CAN_RF0R_RFOM0;
	return true;
}

void CANDriver::Enable() const {
}

void CANDriver::Disable() const {
}

void CANDriver::Flush() const {
	uint8_t Data[8];
	uint8_t Length;
	uint32_t ID;
	uint16_t Timestamp;
	bool Remote;
	while (Read(Data, Length, ID, Timestamp, Remote));
}