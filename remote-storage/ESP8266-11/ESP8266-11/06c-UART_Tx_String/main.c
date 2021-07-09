///***************************************************************************************************
//                                   ExploreEmbedded
//****************************************************************************************************
// * File:   main.c
// * Version: 16.0
// * Author: ExploreEmbedded
// * Website: http://www.exploreembedded.com/wiki
// * Description: This file contains the program to transmit a string on UART at 9600 baud rate

//The libraries have been tested on ExploreEmbedded development boards. We strongly believe that the
//library works on any of development boards for respective controllers. However, ExploreEmbedded
//disclaims any kind of hardware failure resulting out of usage of libraries, directly or indirectly.
//Files may be subject to change without prior notice. The revision history contains the information
//related to updates.


//GNU GENERAL PUBLIC LICENSE:
//    Copyright (C) 2012  ExploreEmbedded

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.


//Errors and omissions should be reported to codelibraries@exploreembedded.com
//**************************************************************************************************/
//#include "LPC17xx.h"
//#include "uart.h"
//#include "delay.h"
////#include "stdlib.h"

//int len;
//int main() 
//{
//	char RECstr[2000];
//char REC0str[2000];
//    SystemInit();	
//	  UART0_Init(115200);
//	  UART2_Init(115200);
//    
////	while(strcmp(RECstr,"END/r"))
////	{
//  // UART2_Printf("Welcome to ARM Serial Programming by ExploreEmbedded\n\r");
////	  DELAY_ms(500);   
////	  UART2_Printf("please config your module with proper AT command");
////	  DELAY_ms(500);
////	  UART2_RxString(RECstr);
////		UART0_TxString(RECstr);
////		UART0_RxString(REC0str);
////		UART2_Printf(REC0str);	
////		sprintf (str,"AT+CWMODE=3%c%c",0x0d,0x0a);
////	sendstring0(str);
////	delay();
////	
////	sprintf (str,"AT+CIPMUX=1%c%c",0x0d,0x0a);
////	sendstring0(str);
////	delay();
////	
////	sprintf (str,"AT+CIPSERVER=1,8888%c%c",0x0d,0x0a);
////	sendstring0(str);
////	delay();	

////	sprintf (str,"AT+CWSAP?%c%c",0x0d,0x0a);
////	sendstring0(str);
////	delay();

//		
////	}
//    while(1)
//    {
//     //   UART2_Printf("Welcome to ARM Serial Programming by ExploreEmbedded\n\r");
////			  DELAY_ms(500);
// 			  UART2_RxString(RECstr);
//		  	//UART2_TxString(RECstr);
//			  DELAY_ms(500);
//		    UART0_TxString(RECstr);
//        DELAY_ms(500);			
//	    	UART0_RxString(REC0str);
//	    	UART2_Printf(REC0str);
////			  UART0_RxString(REC0str);
////			  UART2_TxString(REC0str);
//	 //		 UART2_TxString("Welcome to ARM Serial Programming by ExploreEmbedded\n\r");
//	//	  	DELAY_ms(500);
////			if(UART0_RxString(RECstr)!=0)
////			{
////				UART0_TxString(RECstr);
////			}
//	
//				
//    }
//    
//}
/***************************************************************************************************
                                   ExploreEmbedded
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to read 10bit ADC value from channel 0 and send it on UART.

The libraries have been tested on ExploreEmbedded development boards. We strongly believe that the
library works on any of development boards for respective controllers. However, ExploreEmbedded
disclaims any kind of hardware failure resulting out of usage of libraries, directly or indirectly.
Files may be subject to change without prior notice. The revision history contains the information
related to updates.


GNU GENERAL PUBLIC LICENSE:
    Copyright (C) 2012  ExploreEmbedded
  
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


Errors and omissions should be reported to codelibraries@exploreembedded.com
**************************************************************************************************/
#include "LPC17xx.h"
#include "adc.h"
#include "uart.h"
#include "delay.h"
#include "stdutils.h"
#include "gpio.h"
#include "timer.h"
#include "serial.h"
char str[200];
char str2[200];
volatile  int adcValue;
volatile  float H_PERCENT;
int cnt=0;
void  myTimerIsr_0(void)
{
//   adcValue = ADC_GetAdcValue(3); // Read the ADC value of channel zero
//	 H_PERCENT=100-(adcValue*100)/4096 ;
//   UART2_Printf("ADC0 Value:%4d \n\r",H_PERCENT);     // Send the value on UART
	
//	sprintf(str,"AT%c%c",0x0d,0x0a);
	//UART0_TxString(str);
//	UART0_RxString(str2);
//	DELAY_ms(200);
//	UART2_TxString(str2);
	
//	DELAY_ms(200);
//	UART0_RxString(str2);
//	DELAY_ms(200);
//	sprintf (str,"AT+CWJAP=MPLab,MpProject1400%c%c",0x0d,0x0a);
//	UART0_TxString(str);
	
	
//	while(UART0_RxString(str2)!=0)
//	{ 
//		if ((str2[cnt]=UART0_RxChar())!=0x0a && str2[cnt]!=0x0d)
//		UART0_RxString(str2);	
//		
//	UART2_TxString(str2);
//	DELAY_ms(200);
//	}
	
//	sprintf (str,"AT+CWMODE=3%c%c",0x0d,0x0a);
//	UART0_TxString(str);
//	DELAY_ms(200);
//	UART0_RxString(str2);
//	DELAY_ms(200);
//	
//		UART2_TxString(str2);
//	DELAY_ms(200);
//	

//	
//	sprintf (str,"AT+CIPMUX=1%c%c",0x0d,0x0a);
//	UART0_TxString(str);
//	DELAY_ms(200);
//		UART0_RxString(str2);
//	DELAY_ms(200);
//	
//		UART2_TxString(str2);
//	DELAY_ms(200);
//	
//	sprintf (str,"AT+CIPSERVER=1,8888%c%c",0x0d,0x0a);
//	UART0_TxString(str);
//	DELAY_ms(200);
//		UART0_RxString(str2);
//	DELAY_ms(200);
//	
//		UART2_TxString(str2);
//	DELAY_ms(200);

//	sprintf (str,"AT+CWSAP?%c%c",0x0d,0x0a);
//	UART0_TxString(str);
//	DELAY_ms(200);
//		UART0_RxString(str2);
//	DELAY_ms(200);
//	
//		UART2_TxString(str2);
//	DELAY_ms(200);
}
///*******SEVEN-SEGMENT IMPLEMENTATION****///////
//void  myTimerIsr_1(void)
//{
//    GPIO_PinToggle(P2_0);
//	  DELAY_ms(100);
//	  GPIO_PinToggle(P2_1);
//	  DELAY_ms(100);
//	  GPIO_PinToggle(P2_2);
//	  DELAY_ms(100);
//	  GPIO_PinToggle(P2_3);
//	  DELAY_ms(100);
//	  GPIO_PinToggle(P2_4);
//	  DELAY_ms(100);
//	  GPIO_PinToggle(P2_5);
//	  DELAY_ms(100);
//	  GPIO_PinToggle(P2_6);
//	  DELAY_ms(100);
//	  GPIO_PinToggle(P2_7);
//	  DELAY_ms(100);
//	
//}
//void  myuartIsr_2(void)
//{

//   UART2_Printf("Hello world \n\r");     
//}
//void UART2_IRQHandler(void)
//{
//	 UART2_Printf("Hello world \n\r");
//}





void UART2_IRQHandler(void) {
 //uart2_write( uart2_readData() + 2 );
//	UART0_TxChar(UART2_RxChar());
//	DELAY_ms(100);
//	UART2_TxChar(UART2_RxChar());
	UART2_RxString(str);
	UART2_TxString(str);
	DELAY_ms(100);
	UART1_TxString(str);

	//DELAY_ms(100);
//	UART1_TxString(str);
}
void UART1_IRQHandler(void) {

	UART1_RxString(str);
	UART2_TxString(str);
	
	
//	UART0_RxString(str2);
//	UART2_TxString(str2);

	
	
}
int main() 
{
	 SystemInit();
	  UART2_Init(115200);
	  UART1_Init(115200);
	 // uart2_init(0,1,9600,1, 1/*autoBaud_Enable*/,1,1);
	  NVIC_EnableIRQ(UART2_IRQn);
	  NVIC_EnableIRQ(UART1_IRQn);
    LPC_UART2->IER = (1 << _RbrInterruptEnable);
	  LPC_UART1->IER = (1 << _RbrInterruptEnable);
    
	
//	sprintf(str,"AT%c%c",0x0d,0x0a);
//	UART0_TxString(str);
//	UART0_RxString(str2);
//	DELAY_ms(200);
//	UART2_TxString(str2);
	 
//	  UART0_Init(115200);
//	  UART2_Init(115200);  /* Initialize UART at 9600 baud rate */
    ADC_Init();                               /* Initialize the ADC module */
    TIMER_Init(0,1000000);                  /* Configure timer0 to generate 100ms(100000us) delay*/
 //   TIMER_Init(1,500000);                  /* Configure timer1 to generate 500ms(500000us) delay*/
	  
	//GPIO_PinWrite(P0_17,1);
	GPIO_PinDirection(P2_0,OUTPUT);
	GPIO_PinDirection(P2_1,OUTPUT);
	GPIO_PinDirection(P2_2,OUTPUT);
	GPIO_PinDirection(P2_3,OUTPUT);
	GPIO_PinDirection(P2_4,OUTPUT);
	GPIO_PinDirection(P2_5,OUTPUT);
	GPIO_PinDirection(P2_6,OUTPUT);
	GPIO_PinDirection(P2_7,OUTPUT);

   TIMER_AttachInterrupt(0,myTimerIsr_0);  /* myTimerIsr_0 will be called by TIMER0_IRQn */
 //   TIMER_AttachInterrupt(1,myTimerIsr_1);  /* myTimerIsr_1 will be called by TIMER1_IRQn */

    TIMER_Start(0);                         /* Start the Timers */
 //   TIMER_Start(1);

  
   
//		NVIC_EnableIRQ(UART0_IRQn);
//	  NVIC_EnableIRQ(UART2_IRQn);

	//	UART_AttachInterrupt(2,myuartIsr_2); 
    
    while(1)
    {
//	sprintf(str,"AT%c%c",0x0d,0x0a);
//	UART0_TxString(str);
//	if (UART0_RxString(str2)!=0)
//			{
//	           UART2_TxString(str);
//			}
////        adcValue = ADC_GetAdcValue(3); // Read the ADC value of channel zero
////        UART2_Printf("ADC0 Value:%4d \n\r",adcValue);     // Send the value on UART
//			DELAY_ms(500);
    }
    

}


