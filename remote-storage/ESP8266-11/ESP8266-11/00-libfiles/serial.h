#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <lpc17xx.h>
#include "uart023Define.h"

void  uart0_init(char TxEnable, char RxEnable, uint32_t baudrate, char use_Recive_Interrupt, char autoBaud_Enable, char use_AutoBaud_Finish_Interrupt, char NumberCharRxTrigger);
void  uart0_write(char data);
void  uart0_print(char *data);
void  uart0_println(char *data);
void  uart0_printNumber(int32_t data, unsigned int base);
void  uart0_printNumberln(int32_t data, unsigned int base);
char  uart0_readData(void);
char* uart0_readData2(unsigned char NumberDataShouldRead);
unsigned int uart0_readNumber( unsigned int MaxTimeOut, unsigned char MaxNumber );
void uart0_printFloat( double NumberSend_Float , uint8_t FloatDigit );
void uart0_printFloatln( double NumberSend_Float, uint8_t FloatDigit );
void uart0_Printf( const char *argList, ...);

void  uart2_init(char TxEnable, char RxEnable, uint32_t baudrate, char use_Recive_Interrupt, char autoBaud_Enable, char use_AutoBaud_Finish_Interrupt, char NumberCharRxTrigger);
void  uart2_write(char data);
void  uart2_print(char *data);
void  uart2_println(char *data);
void  uart2_printNumber(int32_t data, unsigned int base);
void  uart2_printNumberln(int32_t data, unsigned int base);
char  uart2_readData(void);
char* uart2_readData2(unsigned char NumberDataShouldRead);
unsigned int uart2_readNumber( unsigned int MaxTimeOut, unsigned char MaxNumber );
void uart2_printFloat( double NumberSend_Float , uint8_t FloatDigit );
void uart2_printFloatln( double NumberSend_Float, uint8_t FloatDigit );
void uart2_Printf( const char *argList, ...);

void  uart3_init(char TxEnable, char RxEnable, uint32_t baudrate, char use_Recive_Interrupt, char autoBaud_Enable, char use_AutoBaud_Finish_Interrupt, char NumberCharRxTrigger);
void  uart3_write(char data);
void  uart3_print(char *data);
void  uart3_println(char *data);
void  uart3_printNumber(int32_t data, unsigned int base);
void  uart3_printNumberln(int32_t data, unsigned int base);
char  uart3_readData(void);
char* uart3_readData2(unsigned char NumberDataShouldRead);
unsigned int uart3_readNumber( unsigned int MaxTimeOut, unsigned char MaxNumber );
void uart3_printFloat( double NumberSend_Float , uint8_t FloatDigit );
void uart3_printFloatln( double NumberSend_Float, uint8_t FloatDigit );
void uart3_Printf( const char *argList, ...);

#endif
