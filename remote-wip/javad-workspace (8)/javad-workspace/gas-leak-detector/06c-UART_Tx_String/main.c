
#include "LPC17xx.h"
#include "adc.h"
#include "uart.h"
#include "delay.h"
#include "stdutils.h"
#include "gpio.h"
#include "timer.h"
#include <stdio.h>
#include "gateway.h"


void  ADC_ISR(void)
{
	 int adc_value;
	 float humidity_percentage;
	
   adc_value = ADC_GetAdcValue(3); 	
	 humidity_percentage= 100-(adc_value*100)/4096 ;
	
   UART2_Printf("ADC0 Value:%4d \n\r",humidity_percentage);
	 Send_Humidity_Status((int) humidity_percentage);
}

void  Valve_Command_ISR(void)
{
	int valve_cmd = Get_Valve_Open_Cmd();
	
}


int main() 
{	
	//***********************Initialization********************************////////
	
	  SystemInit();
    TIMER_Init(0,1000000);                  /* Configure timer0 to generate 100ms(100000us) delay*/
    TIMER_Init(1,500000);                   /* Configure timer1 to generate 500ms(500000us) delay*/
	  ADC_Init();                             /* Initialize the ADC module */
	  UART2_Init(115200);                     /* Initialize UART2 for serial commmunication with PC */
	  UART1_Init(115200);											/* Initialize UART1 for ESP */
		Initialize_Wifi();										  /* Initialize Wifi */
	
	
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
	
		

	//***********************Timers Interrupt Initialization****************************///

    TIMER_AttachInterrupt(0,ADC_ISR);       
    TIMER_AttachInterrupt(1,Valve_Command_ISR);     
		
		
		TIMER_Start(0);                           
		TIMER_Start(1);

//*****************************END*************************************************///////// 
    

}

