#include "lpc17xx.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"


 void delay(void){
  uint32_t i=0;
  for(i=0;i<9999999;i++);
 }
uint16_t read_adc(adc_channel_number)
{
	uint16_t adc_value;

	//adc init
	ADC_Init(LPC_ADC, 100000);
	//adc interrupt DISABLE
	ADC_IntConfig(LPC_ADC,adc_channel_number,DISABLE);
	//adc channel on
	ADC_ChannelCmd(LPC_ADC,adc_channel_number,ENABLE);

		// Start conversion
		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		//Wait conversion complete
		while (!(ADC_ChannelGetStatus(LPC_ADC,adc_channel_number,ADC_DATA_DONE)));
		//read adc channel
		adc_value = ADC_ChannelGetData(LPC_ADC,adc_channel_number);
	return adc_value;
}


int main (void){
    uint16_t V;
	//begin pinsel confing
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
	PinCfg.Pinnum = 26;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	 //end pinsel confing 
	 SystemInit();
	 	debug_frmwrk_init();
		
		while(1){

		H=read_adc(3);
		  
		_DBG("ADC= ");
	   	_DBD32(H);
		_DBG("\n");
		delay();
		}

   return 1;
 }
