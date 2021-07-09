#ifndef _UART023DEFINE_H_
#define _UART023DEFINE_H_



// UART0 FIFO Control Register(FCR) - LPC_UART0 -> FCR
#define _FifoEnable			0
#define _RxFifoReset 		1
#define _TxFifoReset 		2
#define _DmaModeSelect 	3
#define _RxTriggerLevel	6

// UART0 Line Control Register(LCR) - LPC_UART0 -> LCR
#define _WordLengthSelect	0
#define _StopBitSelect		2
#define _ParityEnable			3
#define _ParitySelect			4
#define _BreakControl			6
#define _DLAB							7 // DivisorLatchAccessBit

// UART0 Line Status Register(LSR) - LPC_UART0 -> LSR
#define _RDR	0 // ReceiverDataReady
#define _OE		1 // OverrunError
#define _PE		2 // ParityError
#define _FE		3 // FramingError
#define _BI		4 // BreakInterrupt
#define _THRE	5 // TransmitterHoldingRegisterEmpty
#define _TEMT	6 // TransmitterEmpty
#define _RXFE	7 // ErrorInRXFIFO

// UART0 Fractional Divider Register(FDR) - LPC_UART0 -> FDR
#define _DIVADDVAL 0
#define _MULVAL		4

// UART0 Divisor Latch LSB register(DLL) - LPC_UART0 -> DLL
#define _DLLSB 0

// UART0 Divisor Latch MSB register(DLM) - LPC_UART0 -> DLM
#define _DLMSB 0

// UART0 Transmit Holding Register(THR) - LPC_UART0 -> THR
#define _THR 0

// UART0 Receiver Buffer Register(RBR) - LPC_UART0 -> RBR
#define _RBR 0

//LPC_UART0 -> IER
#define _RbrInterruptEnable						0
#define _ThreInterruptEnable					1
#define _RxLineStatusInterruptEnable	2
#define _ABEOIntEn										8
#define _ABTOIntEn										9

//LPC_UART0 -> IIR
#define _IntStatus 			0
#define	_IntId					1
#define	_FifoEnable_IIR	6
#define _ABEOInt				8
#define	_ABTOInt				9
//-----------------------
#define	RDA_Interrupt		0x04 // 0b0100 = 0x04
//-----------------------

//LPC_UART0 -> ACR
#define _Start 				0
#define _Mode 				1
#define _AutoRestart	2
#define _ABEOIntClr		8
#define _ABTOIntClr		9

//LPC_UART0 -> ICR
#define _IrDAEn 			0
#define _IrDAInv 			1
#define _FixPulseEn		2
#define _PulseDiv 		3

//LPC_UART0 -> TER
#define _TXEN					7


// Define for "RX Trigger Level", UARTn FIFO Control Register (UnFCR)
#define _1Character		0
#define _4Character		1
#define _8Character		2
#define _14Character	3




#endif
