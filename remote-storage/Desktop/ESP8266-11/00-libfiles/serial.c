#include <lpc17xx.h>
#include "serial.h"
#include "rit.h"
#include <math.h>
#include <stdarg.h>

//****************************************************
uint8_t numberDigits( uint32_t Number ) {
	
	uint8_t Digits = 0;
	
	while( Number != 0 ) {
		Number /= 10;
		Digits++;
	}	
	
	return Digits;
}
//****************************************************


//**************************************************** UART0
/*
uartNumber : 0, 2, 3
NumberCharRxTrigger : _1Character, _4Character, _8Character, _14Character
TxEnable, RxEnable : 1=Enable, 0=Disable
*/
void  uart0_init(char TxEnable, char RxEnable, uint32_t baudrate, char use_Recive_Interrupt, char autoBaud_Enable, char use_AutoBaud_Finish_Interrupt, char NumberCharRxTrigger) {

	uint32_t var_UART0ClockDivider_u32, var_UART0Clock_u32, var_RegValue_u32;

	LPC_SC->PCONP |= (1 << 3);	// Enable CLOCK for internal UART0 controller
	
	// UART0 ---> P0.2(TXD0), Function 01 ----> P0.3(RXD0), Function 01
	if( TxEnable ) 	{ LPC_PINCON->PINSEL0 |= (1<<4);	LPC_PINCON->PINSEL0 &= ~(1<<5); }
	if( RxEnable ) 	{ LPC_PINCON->PINSEL0 |= (1<<6);	LPC_PINCON->PINSEL0 &= ~(1<<7); }

	// Enable FIFO & Reset Rx/Tx FIFO buffers
	LPC_UART0->FCR = (NumberCharRxTrigger<<_RxTriggerLevel) | (0<<_DmaModeSelect) | (1<<_TxFifoReset) | (1<<_RxFifoReset) | (1<<_FifoEnable); 

	if( use_Recive_Interrupt ) {
		LPC_UART0->IER = (1 << _RbrInterruptEnable); 
	}
	
	if(!autoBaud_Enable) {
	
		// 8bit data, 1Stop bit, No parity
		LPC_UART0 -> LCR = (1<<_DLAB) | (0<<_BreakControl) | (0<<_ParitySelect) | (0<<_ParityEnable) | (0<<_StopBitSelect) | (3<<_WordLengthSelect);

		// Get the "UART0 Clock Divider" from PCLKSELx register 7-6 bits
		var_UART0ClockDivider_u32 = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
	
		switch( var_UART0ClockDivider_u32 ) {
			case 0x00:	var_UART0Clock_u32 = SystemCoreClock/4;	break;
			case 0x01:	var_UART0Clock_u32 = SystemCoreClock/1;	break; 
			case 0x02:	var_UART0Clock_u32 = SystemCoreClock/2;	break; 
			case 0x03:	var_UART0Clock_u32 = SystemCoreClock/8;	break;
		}

		var_RegValue_u32 = ( var_UART0Clock_u32 / (16 * baudrate ));

		LPC_UART0->DLL = var_RegValue_u32 & 0xFF;
		LPC_UART0->DLM = (var_RegValue_u32 >> 8) & 0xFF;

		// Clear DLAB after setting DLL,DLM
		LPC_UART0->LCR &= ~(1<<_DLAB);
	}
	else {
		// 8bit data, 1Stop bit, No parity
		LPC_UART0 -> LCR = (0<<_DLAB) | (0<<_BreakControl) | (0<<_ParitySelect) | (0<<_ParityEnable) | (0<<_StopBitSelect) | (3<<_WordLengthSelect);

		LPC_UART0->ACR = (1<<_Start); // Strt Auto Bauding	
		
		if( !use_AutoBaud_Finish_Interrupt ) {
			while( LPC_UART0->ACR & (1<<_Start) ); // Wait for End Auto Bauding
		}
	}
}
void  uart0_write(char data) {
	while( !((LPC_UART0 -> LSR)	& (1 << _THRE)) ); // ??? ?????? ?? ????? ???? ????? ???
	LPC_UART0 -> THR = data; // ???? ??????? ?? ???? ?????? ????? ???? ????? ????
}
void  uart0_print(char *data) {
	unsigned char i;
	
	for(i=0; data[i]!='\0'; i++)
		uart0_write( data[i] );
}
void  uart0_println(char *data) {
	uart0_print(data);
	uart0_write('\n');
}
void  uart0_printNumber(int32_t data, unsigned int base) {
  char buf[8 * sizeof(long) + 1];
  char *str = &buf[sizeof(buf) - 1];
	char c;
	unsigned long m;
  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

	if( base == 10 ) { // ديسيمال
		if(data<0) {
			data = -data;
			uart0_write('-');
		}
	}
	
  do {
    m = data;
    data /= base;
    c = m - base * data;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(data);

  uart0_print( str );
}
void  uart0_printNumberln(int32_t data, unsigned int base) {
	uart0_printNumber(data, base);
	uart0_write('\n');
}
/*
	Send 0 if Character Have Eror
*/
char  uart0_readData(void) {

	char LsrRegisterValue;

	do {
		LsrRegisterValue = LPC_UART0 -> LSR;
	}
	while( !(LsrRegisterValue & (1<<_RDR)) ); // Wait While UARTn receiver FIFO is empty

	if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
		LPC_UART0 -> RBR; // Read Data then This Data With Eror Will Clear
		return 0;
	}
	else {
		return LPC_UART0 -> RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char )
	}
}
/*
Farz Bar ine ke as "RxTriggerLevel" va Interrupt EstefadeKardim
va Megdar ("RxTriggerLevel" = "NumberDataShouldRead") Hastesh.
*/
char uart0_DataStoreArray[14];
char* uart0_readData2(unsigned char NumberDataShouldRead) {

	char LsrRegisterValue, i;

	for( i=0; i<NumberDataShouldRead; i++ ) {

		LsrRegisterValue = LPC_UART0 -> LSR;

		if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
			LPC_UART0 -> RBR; // Read Data then This Data With Eror Will Clear
			uart0_DataStoreArray[i] = 255; // Error Data
		}
		else {
			uart0_DataStoreArray[i] = LPC_UART0 -> RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char )
		}

	}

	return uart0_DataStoreArray;
}
/*
دريافت يه عدد چند رقمي از سريال
*/
unsigned int uart0_readNumber( unsigned int MaxTimeOut, unsigned char MaxNumber ) {

	int Data[6], DataEnd = 0;
	unsigned char NumberData = 0, i;
	unsigned char LsrRegisterValue;


	rit_init( MaxTimeOut, Disable/*rit_Enable_Disable*/ ); // 300,000,000 = 300M ---> 3Sec

	while( NumberData < MaxNumber ) {

		if( NumberData == 0) { // رقم اول رو ميخوايم دريافت کينم - لذا تايمر رو براي گرفتن رقم اول غير فعال ميکنيم
			Data[ 0 ] = uart0_readData(); // گرفتن اولين ديتا		
		}
		else { /// گرفتن ديتاي دوم به بعد
			// فعال کردن شمارنده
			rit_clear_interrupt_flag();
			rit_set_value_counter(0);
			rit_enable_counter();

			do { // گرفتن ديتا		
				LsrRegisterValue = LPC_UART0->LSR;	
			} while( !(LsrRegisterValue & (1<<_RDR)) && rit_read_interrupt_flag() == 0 ); // Wait While UARTn receiver FIFO is empty && TimeOut RIT
		
			//	بررسي اين که خطاي صحت دريافت رخ داده يا نه - اگه رخ داده بود که 0 بفرست و در غير اين صورت ديتا رو ذخيره کن
			if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
				LPC_UART0->RBR; // Read Data then This Data With Eror Will Clear
				return 0; // در حين دريافت ديتا - خطاي صحت ديتا رخ داده است
			}
			else {
				Data[ NumberData ] = LPC_UART0->RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char ) = ذخيره ديتا
			}
		}

		if( Data[NumberData] != 0 ) { // افزايش شمارنده تعداد ديتا - در صورت دريافت ديتا
			NumberData++; // ديتا دريافت کرديم لذا بيا شمارنده ديتا رو يه واحد زياد کن
		}

		// در دو صورت از حلقه وايل بالا خارج ميشيم - يا زمان تموم شده و يا ديتا گرفتيم
		if( rit_read_interrupt_flag() == 1 ) { // ديتايي در مدت زمان مد نظر دريافت نشده است

			if( NumberData == 0 ) { // کلا ديتايي دريافت نکرديم - با توجه به نهوه برنامه نوسي اين مشکل هيچوقت رخ نميده
				rit_clear_interrupt_flag();
				rit_set_value_counter(0);
				rit_disable_counter();

				return 0;
			}
			else { // ديتايي قبلا دريافت کرديم هر چند که در آخرين بررسي ديتايي دريافت نکرديم
				rit_clear_interrupt_flag();
				rit_set_value_counter(0);
				rit_disable_counter();

				for( i=0; i<NumberData; i++ ) DataEnd += (Data[ i ]-48) * pow(10, (NumberData-1)-i);

				return DataEnd;
			}
		}
		else { // در مدت زمان قانوني ديتا وارد شده

			rit_clear_interrupt_flag();
			rit_set_value_counter(0);
			rit_disable_counter();
		}
	}

	// شش ديتا دريافت کرديم - حداکثر ديتايي که متونيم بگيريم 6 تا هستش
	rit_clear_interrupt_flag();
	rit_set_value_counter(0);
	rit_disable_counter();
	for( i=0; i<NumberData; i++ ) DataEnd += (Data[ i ]-48) * pow(10, (NumberData-1)-i);
	
	return DataEnd;
}


/*ارسال عدد اعشاري با قابليت تعيين تعداد ارقام اعشار*/
void uart0_printFloat( double NumberSend_Float, uint8_t FloatDigit ) {

	int32_t NumberSend_Int;
	
	// مثبت کردن عدد در صورت منفي بودن
	if( NumberSend_Float < 0 ) {
		uart0_write('-');
		NumberSend_Float = -NumberSend_Float;
	}

	uart0_printNumber( (uint32_t)NumberSend_Float, 10 );

	uart0_write('.');

	NumberSend_Int = NumberSend_Float;	NumberSend_Int *= pow(10,FloatDigit);
	NumberSend_Float = (int32_t)(NumberSend_Float * pow(10,FloatDigit));
	NumberSend_Int = NumberSend_Float - NumberSend_Int;
	
	uart0_printNumber( NumberSend_Int, 10 );
}
void uart0_printFloatln( double NumberSend_Float, uint8_t FloatDigit ) {
	uart0_printFloat( NumberSend_Float, FloatDigit );
	uart0_write('\n');
}
/*PrintF*/
void uart0_Printf(const char *argList, ...) {
	const char *ptr;
	double v_floatNum_f32;
	va_list argp;
	int16_t v_num_s16;
	int32_t v_num_s32;
	uint16_t v_num_u16;
	uint32_t v_num_u32;
	char *str, ch;
	uint8_t v_numOfDigitsToTransmit_u8;
	
	va_start(argp, argList);
	
	// Loop through the list to extract all the input arguments
	for(ptr = argList; *ptr != '\0'; ptr++) {
		ch= *ptr;
		if(ch == '%') { // Check for '%' as there will be format specifier after it
			ptr++;
			ch = *ptr;
			
			if((ch>=0x30) && (ch<=0x39)) {
				v_numOfDigitsToTransmit_u8 = 0;
				while((ch>=0x30) && (ch<=0x39)) {
					v_numOfDigitsToTransmit_u8 = (v_numOfDigitsToTransmit_u8 * 10) + (ch-0x30);
					ptr++;
					ch = *ptr;
				}
			}
			else {
				v_numOfDigitsToTransmit_u8 = 0xffu;
			}                

			switch(ch) {      // Decode the type of the argument
				case 'C':
				
				case 'c': // Argument type is of char, hence read char data from the argp
					ch = va_arg(argp, int);
					uart0_write(ch);
				break;
				
				case 'd': // Argument type is of signed integer, hence read 16bit data from the argp
					v_num_s16 = va_arg(argp, int);
					if(v_num_s16<0) { // If the number is -ve then display the 2's complement along with '-' sign
						v_num_s16 = -v_num_s16;
						uart0_write('-');
					}
					uart0_printNumber(v_num_s16,10);
				break;
				
				case 'D': // Argument type is of integer, hence read 16bit data from the argp
					v_num_s32 = va_arg(argp, int32_t);
					if(v_num_s32<0) { // If the number is -ve then display the 2's complement along with '-' sign
						v_num_s32 = -v_num_s32;
						uart0_write('-');
					}
					uart0_printNumber(v_num_s32,10);
				break;    

				case 'u': // Argument type is of unsigned integer, hence read 16bit unsigned data
					v_num_u16 = va_arg(argp, int);
					uart0_printNumber(v_num_u16,10);
				break;

				case 'U': // Argument type is of integer, hence read 32bit unsigend data
					v_num_u32 = va_arg(argp, uint32_t);
					uart0_printNumber(v_num_u32,10);
				break;            

				case 'x':  // Argument type is of hex, hence hexadecimal data from the argp
					v_num_u16 = va_arg(argp, int);
					uart0_printNumber(v_num_u16, 10);              
				break;

				case 'X':  // Argument type is of hex, hence hexadecimal data from the argp
					v_num_u32 = va_arg(argp, uint32_t);
					uart0_printNumber(v_num_u32,8);                
				break;

				case 'b':  // Argument type is of binary,Read int and convert to binary
					v_num_u16 = va_arg(argp, int);                    
					if(v_numOfDigitsToTransmit_u8 == 0xffu) v_numOfDigitsToTransmit_u8 = 16;
					uart0_printNumber(v_num_u16, 2);               
				break;

				case 'B':  // Argument type is of binary,Read int and convert to binary
					v_num_u32 = va_arg(argp, uint32_t);
					if(v_numOfDigitsToTransmit_u8 == 0xffu) v_numOfDigitsToTransmit_u8 = 16;                
					uart0_printNumber(v_num_u32,2);    
				break;

				case 'F':

				case 'f': // Argument type is of float, hence read double data from the argp
					v_floatNum_f32 = va_arg(argp, double);  
					uart0_printFloat(v_floatNum_f32,3);
				break;

				case 'S':

				case 's': // Argument type is of string, hence get the pointer to sting passed
					str = va_arg(argp, char *);
					uart0_print(str);                
				break;

				case '%':
					uart0_write('%');
				break;
			}
		}
		else {
			uart0_write(ch); // As '%' is not detected transmit the char passed
		}
	}

	va_end(argp);
}
//**************************************************** UART2
/*
uartNumber : 0, 2, 3
NumberCharRxTrigger : _1Character, _4Character, _8Character, _14Character
TxEnable, RxEnable : 1=Enable, 0=Disable
*/
void  uart2_init(char TxEnable, char RxEnable, uint32_t baudrate, char use_Recive_Interrupt, char autoBaud_Enable, char use_AutoBaud_Finish_Interrupt, char NumberCharRxTrigger) {

	uint32_t var_UART2ClockDivider_u32, var_UART2Clock_u32, var_RegValue_u32;

	LPC_SC->PCONP |= (1 << 24);	// Enable CLOCK for internal UART2 controller
	
	// UART2 ---> P0.10(TXD2), Function 01 ----> P0.11(RXD2), Function 01
	if( TxEnable ) 	{ LPC_PINCON->PINSEL0 |= (1<<20);	LPC_PINCON->PINSEL0 &= ~(1<<21); }
	if( RxEnable ) 	{ LPC_PINCON->PINSEL0 |= (1<<22);	LPC_PINCON->PINSEL0 &= ~(1<<23); }

	// Enable FIFO & Reset Rx/Tx FIFO buffers
	LPC_UART2->FCR = (NumberCharRxTrigger<<_RxTriggerLevel) | (0<<_DmaModeSelect) | (1<<_TxFifoReset) | (1<<_RxFifoReset) | (1<<_FifoEnable); 


	if( use_Recive_Interrupt ) {
		LPC_UART2->IER = (1 << _RbrInterruptEnable); 
	}
	
	
	if(!autoBaud_Enable) {
	
		// 8bit data, 1Stop bit, No parity
		LPC_UART2 -> LCR = (1<<_DLAB) | (0<<_BreakControl) | (0<<_ParitySelect) | (0<<_ParityEnable) | (0<<_StopBitSelect) | (3<<_WordLengthSelect);

		// Get the "UART2 Clock Divider" from PCLKSELx register 7-6 bits
		var_UART2ClockDivider_u32 = (LPC_SC->PCLKSEL1 >> 16) & 0x03;
	
		switch( var_UART2ClockDivider_u32 ) {
			case 0x00:	var_UART2Clock_u32 = SystemCoreClock/4;	break;
			case 0x01:	var_UART2Clock_u32 = SystemCoreClock/1;	break; 
			case 0x02:	var_UART2Clock_u32 = SystemCoreClock/2;	break; 
			case 0x03:	var_UART2Clock_u32 = SystemCoreClock/8;	break;
		}

		var_RegValue_u32 = ( var_UART2Clock_u32 / (16 * baudrate ));

		LPC_UART2->DLL = var_RegValue_u32 & 0xFF;
		LPC_UART2->DLM = (var_RegValue_u32 >> 8) & 0xFF;

		// Clear DLAB after setting DLL,DLM
		LPC_UART2->LCR &= ~(1<<_DLAB);
	}
	else {
		// 8bit data, 1Stop bit, No parity
		LPC_UART2 -> LCR = (0<<_DLAB) | (0<<_BreakControl) | (0<<_ParitySelect) | (0<<_ParityEnable) | (0<<_StopBitSelect) | (3<<_WordLengthSelect);

		LPC_UART2->ACR = (1<<_Start); // Strt Auto Bauding	
		
		if( !use_AutoBaud_Finish_Interrupt ) {
			while( LPC_UART2->ACR & (1<<_Start) ); // Wait for End Auto Bauding
		}
	}
}
void  uart2_write(char data) {
	while( !((LPC_UART2 -> LSR)	& (1 << _THRE)) ); // ??? ?????? ?? ????? ???? ????? ???
	LPC_UART2 -> THR = data; // ???? ??????? ?? ???? ?????? ????? ???? ????? ????
}
void  uart2_print(char *data) {
	unsigned char i;
	
	for(i=0; data[i]!='\0'; i++)
		uart2_write( data[i] );
}
void  uart2_println(char *data) {
	uart2_print(data);
	uart2_write('\n');
}
void  uart2_printNumber(int32_t data, unsigned int base) {
  char buf[8 * sizeof(long) + 1];
  char *str = &buf[sizeof(buf) - 1];
	char c;
	unsigned long m;
  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

	if( base == 10 ) { // ديسيمال
		if(data<0) {
			data = -data;
			uart2_write('-');
		}
	}
	
  do {
    m = data;
    data /= base;
    c = m - base * data;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(data);

  uart2_print( str );
}
void  uart2_printNumberln(int32_t data, unsigned int base) {
	uart2_printNumber(data, base);
	uart2_write('\n');
}
/*
	Send 0 if Character Have Eror
*/
char  uart2_readData(void) {

	char LsrRegisterValue;

	do {
		LsrRegisterValue = LPC_UART2 -> LSR;
	}
	while( !(LsrRegisterValue & (1<<_RDR)) ); // Wait While UARTn receiver FIFO is empty

	if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
		LPC_UART2 -> RBR; // Read Data then This Data With Eror Will Clear
		return 0;
	}
	else {
		return LPC_UART2 -> RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char )
	}
}
/*
Farz Bar ine ke as "RxTriggerLevel" va Interrupt EstefadeKardim
va Megdar ("RxTriggerLevel" = "NumberDataShouldRead") Hastesh.
*/
char uart2_DataStoreArray[14];
char* uart2_readData2(unsigned char NumberDataShouldRead) {

	char LsrRegisterValue, i;

	for( i=0; i<NumberDataShouldRead; i++ ) {

		LsrRegisterValue = LPC_UART2 -> LSR;

		if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
			LPC_UART2 -> RBR; // Read Data then This Data With Eror Will Clear
			uart2_DataStoreArray[i] = 255; // Error Data
		}
		else {
			uart2_DataStoreArray[i] = LPC_UART2 -> RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char )
		}

	}

	return uart2_DataStoreArray;
}
/* دريافت يه عدد چند رقمي از سريال */
unsigned int uart2_readNumber( unsigned int MaxTimeOut, unsigned char MaxNumber ) {

	int Data[6], DataEnd = 0;
	unsigned char NumberData = 0, i;
	unsigned char LsrRegisterValue;


	rit_init( MaxTimeOut, Disable/*rit_Enable_Disable*/ ); // 300,000,000 = 300M ---> 3Sec

	while( NumberData < MaxNumber ) {

		if( NumberData == 0) { // رقم اول رو ميخوايم دريافت کينم - لذا تايمر رو براي گرفتن رقم اول غير فعال ميکنيم
			Data[ 0 ] = uart2_readData(); // گرفتن اولين ديتا		
		}
		else { /// گرفتن ديتاي دوم به بعد
			// فعال کردن شمارنده
			rit_clear_interrupt_flag();
			rit_set_value_counter(0);
			rit_enable_counter();
			
			do { // گرفتن ديتا		
				LsrRegisterValue = LPC_UART2->LSR;	
			} while( !(LsrRegisterValue & (1<<_RDR)) && rit_read_interrupt_flag() == 0 ); // Wait While UARTn receiver FIFO is empty && TimeOut RIT
		
			//	بررسي اين که خطاي صحت دريافت رخ داده يا نه - اگه رخ داده بود که 0 بفرست و در غير اين صورت ديتا رو ذخيره کن
			if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
				LPC_UART2->RBR; // Read Data then This Data With Eror Will Clear
				return 0; // در حين دريافت ديتا - خطاي صحت ديتا رخ داده است
			}
			else {
				Data[ NumberData ] = LPC_UART2->RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char ) = ذخيره ديتا
			}
		}

		if( Data[NumberData] != 0 ) { // افزايش شمارنده تعداد ديتا - در صورت دريافت ديتا
			NumberData++; // ديتا دريافت کرديم لذا بيا شمارنده ديتا رو يه واحد زياد کن
		}

		// در دو صورت از حلقه وايل بالا خارج ميشيم - يا زمان تموم شده و يا ديتا گرفتيم
		if( rit_read_interrupt_flag() == 1 ) { // ديتايي در مدت زمان مد نظر دريافت نشده است

			if( NumberData == 0 ) { // کلا ديتايي دريافت نکرديم - با توجه به نهوه برنامه نوسي اين مشکل هيچوقت رخ نميده
				rit_clear_interrupt_flag();
				rit_set_value_counter(0);
				rit_disable_counter();

				return 0;
			}
			else { // ديتايي قبلا دريافت کرديم هر چند که در آخرين بررسي ديتايي دريافت نکرديم
				rit_clear_interrupt_flag();
				rit_set_value_counter(0);
				rit_disable_counter();

				for( i=0; i<NumberData; i++ ) DataEnd += (Data[ i ]-48) * pow(10, (NumberData-1)-i);

				return DataEnd;
			}
		}
		else { // در مدت زمان قانوني ديتا وارد شده

			rit_clear_interrupt_flag();
			rit_set_value_counter(0);
			rit_disable_counter();
		}
	}

	// شش ديتا دريافت کرديم - حداکثر ديتايي که متونيم بگيريم 6 تا هستش
	rit_clear_interrupt_flag();
	rit_set_value_counter(0);
	rit_disable_counter();
	for( i=0; i<NumberData; i++ ) DataEnd += (Data[ i ]-48) * pow(10, (NumberData-1)-i);
	
	return DataEnd;
}


/*ارسال عدد اعشاري با قابليت تعيين تعداد ارقام اعشار*/
void uart2_printFloat( double NumberSend_Float, uint8_t FloatDigit ) {

	int32_t NumberSend_Int;
	
	// مثبت کردن عدد در صورت منفي بودن
	if( NumberSend_Float < 0 ) {
		uart2_write('-');
		NumberSend_Float = -NumberSend_Float;
	}

	uart2_printNumber( (uint32_t)NumberSend_Float, 10 );

	uart2_write('.');

	NumberSend_Int = NumberSend_Float;	NumberSend_Int *= pow(10,FloatDigit);
	NumberSend_Float = (int32_t)(NumberSend_Float * pow(10,FloatDigit));
	NumberSend_Int = NumberSend_Float - NumberSend_Int;
	
	uart2_printNumber( NumberSend_Int, 10 );
}
void uart2_printFloatln( double NumberSend_Float, uint8_t FloatDigit ) {
	uart2_printFloat( NumberSend_Float, FloatDigit );
	uart2_write('\n');
}
/*PrintF*/
void uart2_Printf(const char *argList, ...) {
	const char *ptr;
	double v_floatNum_f32;
	va_list argp;
	int16_t v_num_s16;
	int32_t v_num_s32;
	uint16_t v_num_u16;
	uint32_t v_num_u32;
	char *str, ch;
	uint8_t v_numOfDigitsToTransmit_u8;
	
	va_start(argp, argList);
	
	// Loop through the list to extract all the input arguments
	for(ptr = argList; *ptr != '\0'; ptr++) {
		ch= *ptr;
		if(ch == '%') { // Check for '%' as there will be format specifier after it
			ptr++;
			ch = *ptr;
			
			if((ch>=0x30) && (ch<=0x39)) {
				v_numOfDigitsToTransmit_u8 = 0;
				while((ch>=0x30) && (ch<=0x39)) {
					v_numOfDigitsToTransmit_u8 = (v_numOfDigitsToTransmit_u8 * 10) + (ch-0x30);
					ptr++;
					ch = *ptr;
				}
			}
			else {
				v_numOfDigitsToTransmit_u8 = 0xffu;
			}                

			switch(ch) {      // Decode the type of the argument
				case 'C':
				
				case 'c': // Argument type is of char, hence read char data from the argp
					ch = va_arg(argp, int);
					uart2_write(ch);
				break;
				
				case 'd': // Argument type is of signed integer, hence read 16bit data from the argp
					v_num_s16 = va_arg(argp, int);
					if(v_num_s16<0) { // If the number is -ve then display the 2's complement along with '-' sign
						v_num_s16 = -v_num_s16;
						uart2_write('-');
					}
					uart2_printNumber(v_num_s16,10);
				break;
				
				case 'D': // Argument type is of integer, hence read 16bit data from the argp
					v_num_s32 = va_arg(argp, int32_t);
					if(v_num_s32<0) { // If the number is -ve then display the 2's complement along with '-' sign
						v_num_s32 = -v_num_s32;
						uart2_write('-');
					}
					uart2_printNumber(v_num_s32,10);
				break;    

				case 'u': // Argument type is of unsigned integer, hence read 16bit unsigned data
					v_num_u16 = va_arg(argp, int);
					uart2_printNumber(v_num_u16,10);
				break;

				case 'U': // Argument type is of integer, hence read 32bit unsigend data
					v_num_u32 = va_arg(argp, uint32_t);
					uart2_printNumber(v_num_u32,10);
				break;            

				case 'x':  // Argument type is of hex, hence hexadecimal data from the argp
					v_num_u16 = va_arg(argp, int);
					uart2_printNumber(v_num_u16, 10);              
				break;

				case 'X':  // Argument type is of hex, hence hexadecimal data from the argp
					v_num_u32 = va_arg(argp, uint32_t);
					uart2_printNumber(v_num_u32,8);                
				break;

				case 'b':  // Argument type is of binary,Read int and convert to binary
					v_num_u16 = va_arg(argp, int);                    
					if(v_numOfDigitsToTransmit_u8 == 0xffu) v_numOfDigitsToTransmit_u8 = 16;
					uart2_printNumber(v_num_u16, 2);               
				break;

				case 'B':  // Argument type is of binary,Read int and convert to binary
					v_num_u32 = va_arg(argp, uint32_t);
					if(v_numOfDigitsToTransmit_u8 == 0xffu) v_numOfDigitsToTransmit_u8 = 16;                
					uart2_printNumber(v_num_u32,2);    
				break;

				case 'F':

				case 'f': // Argument type is of float, hence read double data from the argp
					v_floatNum_f32 = va_arg(argp, double);  
					uart2_printFloat(v_floatNum_f32,3);
				break;

				case 'S':

				case 's': // Argument type is of string, hence get the pointer to sting passed
					str = va_arg(argp, char *);
					uart2_print(str);                
				break;

				case '%':
					uart2_write('%');
				break;
			}
		}
		else {
			uart2_write(ch); // As '%' is not detected transmit the char passed
		}
	}

	va_end(argp);
}
//**************************************************** UART3
/*
uartNumber : 0, 2, 3
NumberCharRxTrigger : _1Character, _4Character, _8Character, _14Character
TxEnable, RxEnable : 1=Enable, 0=Disable
*/
void  uart3_init(char TxEnable, char RxEnable, uint32_t baudrate, char use_Recive_Interrupt, char autoBaud_Enable, char use_AutoBaud_Finish_Interrupt, char NumberCharRxTrigger) {

	uint32_t var_UART3ClockDivider_u32, var_UART3Clock_u32, var_RegValue_u32;

	LPC_SC->PCONP |= (1 << 25);	// Enable CLOCK for internal UART3 controller

	// UART3 ---> P0.0(TXD3), Function 10 ----> P0.1(RXD3), Function 10
	if( TxEnable ) { LPC_PINCON->PINSEL0 &= ~(1<<0);	LPC_PINCON->PINSEL0 |= (1<<1); }
	if( RxEnable ) { LPC_PINCON->PINSEL0 &= ~(1<<2);	LPC_PINCON->PINSEL0 |= (1<<3); }

	// Enable FIFO & Reset Rx/Tx FIFO buffers
	LPC_UART3->FCR = (NumberCharRxTrigger<<_RxTriggerLevel) | (0<<_DmaModeSelect) | (1<<_TxFifoReset) | (1<<_RxFifoReset) | (1<<_FifoEnable); 

	if( use_Recive_Interrupt ) {
		LPC_UART3->IER = (1 << _RbrInterruptEnable); 
	}
	
	if(!autoBaud_Enable) {

		// 8bit data, 1Stop bit, No parity
		LPC_UART3 -> LCR = (1<<_DLAB) | (0<<_BreakControl) | (0<<_ParitySelect) | (0<<_ParityEnable) | (0<<_StopBitSelect) | (3<<_WordLengthSelect);

		// Get the "UART0 Clock Divider" from PCLKSELx register 7-6 bits
		var_UART3ClockDivider_u32 = (LPC_SC->PCLKSEL1 >> 18) & 0x03;

		switch( var_UART3ClockDivider_u32 ) {
			case 0x00:	var_UART3Clock_u32 = SystemCoreClock/4;	break;
			case 0x01:	var_UART3Clock_u32 = SystemCoreClock/1;	break; 
			case 0x02:	var_UART3Clock_u32 = SystemCoreClock/2;	break; 
			case 0x03:	var_UART3Clock_u32 = SystemCoreClock/8;	break;
		}

		var_RegValue_u32 = ( var_UART3Clock_u32 / (16 * baudrate ));

		LPC_UART3->DLL = var_RegValue_u32 & 0xFF;
		LPC_UART3->DLM = (var_RegValue_u32 >> 8) & 0xFF;

		// Clear DLAB after setting DLL,DLM
		LPC_UART3->LCR &= ~(1<<_DLAB);
	}
	else {
		// 8bit data, 1Stop bit, No parity
		LPC_UART3 -> LCR = (0<<_DLAB) | (0<<_BreakControl) | (0<<_ParitySelect) | (0<<_ParityEnable) | (0<<_StopBitSelect) | (3<<_WordLengthSelect);

		LPC_UART3->ACR = (1<<_Start); // Strt Auto Bauding	
		
		if( !use_AutoBaud_Finish_Interrupt ) {
			while( LPC_UART3->ACR & (1<<_Start) ); // Wait for End Auto Bauding
		}
	}
}
void  uart3_write(char data) {
	while( !((LPC_UART3 -> LSR)	& (1 << _THRE)) ); // ??? ?????? ?? ????? ???? ????? ???
	LPC_UART3 -> THR = data; // ???? ??????? ?? ???? ?????? ????? ???? ????? ????
}
void  uart3_print(char *data) {
	unsigned char i;
	
	for(i=0; data[i]!='\0'; i++)
		uart3_write( data[i] );
}
void  uart3_println(char *data) {
	uart3_print(data);
	uart3_write('\n');
}
void  uart3_printNumber(int32_t data, unsigned int base) {
  char buf[8 * sizeof(long) + 1];
  char *str = &buf[sizeof(buf) - 1];
	char c;
	unsigned long m;
  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

	if( base == 10 ) { // ديسيمال
		if(data<0) {
			data = -data;
			uart3_write('-');
		}
	}
	
  do {
    m = data;
    data /= base;
    c = m - base * data;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(data);

  uart3_print( str );
}
void  uart3_printNumberln(int32_t data, unsigned int base) {
	uart3_printNumber(data, base);
	uart3_write('\n');
}
/*
	Send 0 if Character Have Eror
*/
char  uart3_readData(void) {

	char LsrRegisterValue;

	do {
		LsrRegisterValue = LPC_UART3 -> LSR;
	}
	while( !(LsrRegisterValue & (1<<_RDR)) ); // Wait While UARTn receiver FIFO is empty

	if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
		LPC_UART3 -> RBR; // Read Data then This Data With Eror Will Clear
		return 0;
	}
	else {
		return LPC_UART3 -> RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char )
	}
}
/*
Farz Bar ine ke as "RxTriggerLevel" va Interrupt EstefadeKardim
va Megdar ("RxTriggerLevel" = "NumberDataShouldRead") Hastesh.
*/
char uart3_DataStoreArray[14];
char* uart3_readData2(unsigned char NumberDataShouldRead) {

	char LsrRegisterValue, i;

	for( i=0; i<NumberDataShouldRead; i++ ) {

		LsrRegisterValue = LPC_UART3 -> LSR;

		if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
			LPC_UART3 -> RBR; // Read Data then This Data With Eror Will Clear
			uart3_DataStoreArray[i] = 255; // Error Data
		}
		else {
			uart3_DataStoreArray[i] = LPC_UART3 -> RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char )
		}

	}

	return uart3_DataStoreArray;
}
/* دريافت يه عدد چند رقمي از سريال */
unsigned int uart3_readNumber( unsigned int MaxTimeOut, unsigned char MaxNumber ) {

	int Data[6], DataEnd = 0;
	unsigned char NumberData = 0, i;
	unsigned char LsrRegisterValue;


	rit_init( MaxTimeOut, Disable/*rit_Enable_Disable*/ ); // 300,000,000 = 300M ---> 3Sec

	while( NumberData < MaxNumber ) {

		if( NumberData == 0) { // رقم اول رو ميخوايم دريافت کينم - لذا تايمر رو براي گرفتن رقم اول غير فعال ميکنيم
			Data[ 0 ] = uart3_readData(); // گرفتن اولين ديتا		
		}
		else { /// گرفتن ديتاي دوم به بعد
			// فعال کردن شمارنده
			rit_clear_interrupt_flag();
			rit_set_value_counter(0);
			rit_enable_counter();
			
			do { // گرفتن ديتا		
				LsrRegisterValue = LPC_UART3 -> LSR;	
			} while( !(LsrRegisterValue & (1<<_RDR)) && rit_read_interrupt_flag() == 0 ); // Wait While UARTn receiver FIFO is empty && TimeOut RIT
		
			//	بررسي اين که خطاي صحت دريافت رخ داده يا نه - اگه رخ داده بود که 0 بفرست و در غير اين صورت ديتا رو ذخيره کن
			if( LsrRegisterValue & ((1<<_OE)|(1<<_PE)|(1<<_FE)) ) {
				LPC_UART3 -> RBR; // Read Data then This Data With Eror Will Clear
				return 0; // در حين دريافت ديتا - خطاي صحت ديتا رخ داده است
			}
			else {
				Data[ NumberData ] = LPC_UART3 -> RBR; // Read Data From RBR ( Top Byte of RX FIFO - Oldest Char ) = ذخيره ديتا
			}
		}

		if( Data[NumberData] != 0 ) { // افزايش شمارنده تعداد ديتا - در صورت دريافت ديتا
			NumberData++; // ديتا دريافت کرديم لذا بيا شمارنده ديتا رو يه واحد زياد کن
		}

		// در دو صورت از حلقه وايل بالا خارج ميشيم - يا زمان تموم شده و يا ديتا گرفتيم
		if( rit_read_interrupt_flag() == 1 ) { // ديتايي در مدت زمان مد نظر دريافت نشده است

			if( NumberData == 0 ) { // کلا ديتايي دريافت نکرديم - با توجه به نهوه برنامه نوسي اين مشکل هيچوقت رخ نميده
				rit_clear_interrupt_flag();
				rit_set_value_counter(0);
				rit_disable_counter();

				return 0;
			}
			else { // ديتايي قبلا دريافت کرديم هر چند که در آخرين بررسي ديتايي دريافت نکرديم
				rit_clear_interrupt_flag();
				rit_set_value_counter(0);
				rit_disable_counter();

				for( i=0; i<NumberData; i++ ) DataEnd += (Data[ i ]-48) * pow(10, (NumberData-1)-i);

				return DataEnd;
			}
		}
		else { // در مدت زمان قانوني ديتا وارد شده

			rit_clear_interrupt_flag();
			rit_set_value_counter(0);
			rit_disable_counter();
		}
	}

	// شش ديتا دريافت کرديم - حداکثر ديتايي که متونيم بگيريم 6 تا هستش
	rit_clear_interrupt_flag();
	rit_set_value_counter(0);
	rit_disable_counter();
	for( i=0; i<NumberData; i++ ) DataEnd += (Data[ i ]-48) * pow(10, (NumberData-1)-i);
	
	return DataEnd;
}
/*ارسال عدد اعشاري با قابليت تعيين تعداد ارقام اعشار*/
void uart3_printFloat( double NumberSend_Float, uint8_t FloatDigit ) {

	int32_t NumberSend_Int;
	
	// مثبت کردن عدد در صورت منفي بودن
	if( NumberSend_Float < 0 ) {
		uart3_write('-');
		NumberSend_Float = -NumberSend_Float;
	}

	uart3_printNumber( (uint32_t)NumberSend_Float, 10 );

	uart3_write('.');

	NumberSend_Int = NumberSend_Float;	NumberSend_Int *= pow(10,FloatDigit);
	NumberSend_Float = (int32_t)(NumberSend_Float * pow(10,FloatDigit));
	NumberSend_Int = NumberSend_Float - NumberSend_Int;
	
	uart3_printNumber( NumberSend_Int, 10 );
}

void uart3_printFloatln( double NumberSend_Float, uint8_t FloatDigit ) {
	uart3_printFloat( NumberSend_Float, FloatDigit );
	uart3_write('\n');
}
void uart3_Printf(const char *argList, ...) {
	const char *ptr;
	double v_floatNum_f32;
	va_list argp;
	int16_t v_num_s16;
	int32_t v_num_s32;
	uint16_t v_num_u16;
	uint32_t v_num_u32;
	char *str, ch;
	uint8_t v_numOfDigitsToTransmit_u8;
	
	va_start(argp, argList);
	
	// Loop through the list to extract all the input arguments
	for(ptr = argList; *ptr != '\0'; ptr++) {
		ch= *ptr;
		if(ch == '%') { // Check for '%' as there will be format specifier after it
			ptr++;
			ch = *ptr;
			
			if((ch>=0x30) && (ch<=0x39)) {
				v_numOfDigitsToTransmit_u8 = 0;
				while((ch>=0x30) && (ch<=0x39)) {
					v_numOfDigitsToTransmit_u8 = (v_numOfDigitsToTransmit_u8 * 10) + (ch-0x30);
					ptr++;
					ch = *ptr;
				}
			}
			else {
				v_numOfDigitsToTransmit_u8 = 0xffu;
			}                

			switch(ch) {      // Decode the type of the argument
				case 'C':
				
				case 'c': // Argument type is of char, hence read char data from the argp
					ch = va_arg(argp, int);
					uart3_write(ch);
				break;
				
				case 'd': // Argument type is of signed integer, hence read 16bit data from the argp
					v_num_s16 = va_arg(argp, int);
					if(v_num_s16<0) { // If the number is -ve then display the 2's complement along with '-' sign
						v_num_s16 = -v_num_s16;
						uart3_write('-');
					}
					uart3_printNumber(v_num_s16,10);
				break;
				
				case 'D': // Argument type is of integer, hence read 16bit data from the argp
					v_num_s32 = va_arg(argp, int32_t);
					if(v_num_s32<0) { // If the number is -ve then display the 2's complement along with '-' sign
						v_num_s32 = -v_num_s32;
						uart3_write('-');
					}
					uart3_printNumber(v_num_s32,10);
				break;    

				case 'u': // Argument type is of unsigned integer, hence read 16bit unsigned data
					v_num_u16 = va_arg(argp, int);
					uart3_printNumber(v_num_u16,10);
				break;

				case 'U': // Argument type is of integer, hence read 32bit unsigend data
					v_num_u32 = va_arg(argp, uint32_t);
					uart3_printNumber(v_num_u32,10);
				break;            

				case 'x':  // Argument type is of hex, hence hexadecimal data from the argp
					v_num_u16 = va_arg(argp, int);
					uart3_printNumber(v_num_u16, 10);              
				break;

				case 'X':  // Argument type is of hex, hence hexadecimal data from the argp
					v_num_u32 = va_arg(argp, uint32_t);
					uart3_printNumber(v_num_u32,8);                
				break;

				case 'b':  // Argument type is of binary,Read int and convert to binary
					v_num_u16 = va_arg(argp, int);                    
					if(v_numOfDigitsToTransmit_u8 == 0xffu) v_numOfDigitsToTransmit_u8 = 16;
					uart3_printNumber(v_num_u16, 2);               
				break;

				case 'B':  // Argument type is of binary,Read int and convert to binary
					v_num_u32 = va_arg(argp, uint32_t);
					if(v_numOfDigitsToTransmit_u8 == 0xffu) v_numOfDigitsToTransmit_u8 = 16;                
					uart3_printNumber(v_num_u32,2);    
				break;

				case 'F':

				case 'f': // Argument type is of float, hence read double data from the argp
					v_floatNum_f32 = va_arg(argp, double);  
					uart3_printFloat(v_floatNum_f32,3);
				break;

				case 'S':

				case 's': // Argument type is of string, hence get the pointer to sting passed
					str = va_arg(argp, char *);
					uart3_print(str);                
				break;

				case '%':
					uart3_write('%');
				break;
			}
		}
		else {
			uart3_write(ch); // As '%' is not detected transmit the char passed
		}
	}

	va_end(argp);
}
