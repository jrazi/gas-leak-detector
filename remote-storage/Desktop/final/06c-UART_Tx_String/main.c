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

volatile  int adcValue;
volatile  float H_PERCENT;
char str[200];
char apikeys[1000]="alshldhgashg";
void  myTimerIsr_0(void)
{
   adcValue = ADC_GetAdcValue(3); // Read the ADC value of channel zero
	 H_PERCENT= 100-(adcValue*100)/4096 ;
//	 sprintf (str,"AT+CWMODE=3%c%c",0x0d,0x0a);
//	 UART0_TxString(str);
//	 DELAY_ms(100);
	
   UART2_Printf("ADC0 Value:%4d \n\r",H_PERCENT);     // Send the value on UART
	
}
///*******SEVEN-SEGMENT IMPLEMENTATION****///////
void  myTimerIsr_1(void)
{
	
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
	
}

int main() 
{
	//***********************Initialization********************************////////
	
	  SystemInit();
    TIMER_Init(0,1000000);                  /* Configure timer0 to generate 100ms(100000us) delay*/
    TIMER_Init(1,500000);                  /* Configure timer1 to generate 500ms(500000us) delay*/
	  ADC_Init();                               /* Initialize the ADC module */
	  UART2_Init(115200);                          /* Initialize UART at 9600 baud rate */
	  UART0_Init(115200); 
	
	//*******************************END******************************************/////
	
	//*********************GPIO-Seven segment init**************************************////////
	 
	GPIO_PinDirection(P2_0,OUTPUT);
	GPIO_PinDirection(P2_1,OUTPUT);
	GPIO_PinDirection(P2_2,OUTPUT);
	GPIO_PinDirection(P2_3,OUTPUT);
	GPIO_PinDirection(P2_4,OUTPUT);
	GPIO_PinDirection(P2_5,OUTPUT);
	GPIO_PinDirection(P2_6,OUTPUT);
	GPIO_PinDirection(P2_7,OUTPUT);
	
	//********************************END**************************************************///////////
	
	//*********************Config ESP8266************************************/////////
	
	sprintf (str,"AT+CWMODE=3%c%c",0x0d,0x0a);
	UART0_TxString(str);
	DELAY_ms(100);
	
	sprintf (str,"AT+CIPMUX=1%c%c",0x0d,0x0a);
	UART0_TxString(str);
	DELAY_ms(100);

	sprintf (str,"AT+CWJAP=MPLab,MpProject1400%c%c",0x0d,0x0a);
	UART0_TxString(str);
	DELAY_ms(100);
	
	//****************************END******************************////////////////////
	
	//***********************Timers Intialization****************************///

    TIMER_AttachInterrupt(0,myTimerIsr_0);       /* myTimerIsr_0 will be called by TIMER0_IRQn */
    TIMER_AttachInterrupt(2,myTimerIsr_1);       /* myTimerIsr_1 will be called by TIMER1_IRQn */

      TIMER_Start(0);                            /* Start the Timers */
      TIMER_Start(1);

//*****************************END*************************************************///////// 

    while(1)
    {

			
    }
    

}


