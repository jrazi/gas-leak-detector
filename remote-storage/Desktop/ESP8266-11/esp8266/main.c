#include <lpc17XX.h>
#include "stdio.h"
#include "uart.h"
#include "delay.h"


int CounterWiFi=0;
//char REC2str[20000];
char str[2000];	
#ifdef __cplusplus
extern "C" {
#endif	
	void UART1_IRQHandler (void);
#ifdef __cplusplus
}
#endif


void delay ()  // ???? ????
{
	int a= 7000000;
	while (a--);
}
unsigned char getchar0 (void)  // ?????? ??????? ?? ?????0
{
	while (!(LPC_UART1->LSR & 0X1));  
	return LPC_UART1->RBR;   
}

unsigned char getchar2 (void)  // ?????? ??????? ?? ?????2
{
	while (!(LPC_UART2->LSR & 0X1));  
	return LPC_UART2->RBR;  
}


void sendchar0 (unsigned char ch) // ????? ?????? ?? ????? 0
{
	while (!(LPC_UART1->LSR & 1<<5));
	LPC_UART1->THR=ch;
	LPC_UART1->TER=0X80;
}

void sendchar2 (unsigned char ch) // ????? ?????? ?? ????? 2
{
	while (!(LPC_UART2->LSR & 1<<5));
	LPC_UART2->THR=ch;
	LPC_UART2->TER=0X80;
}

void sendstring0 (*str)  // ????? ???? ?? ????? 0
{
	for (int i=0;str[i]!='\0';i++)
	sendchar0(str[i]);
	
}

void sendstring2 (*str)   // ????? ???? ?? ????? 2
{
	for (int i=0;str[i]!='\0';i++)
	sendchar2 (str[i]);
	
}



#ifdef __cplusplus
extern "C" {
#endif
	 
void UART2_IRQHandler (void) // تابع مروبط به وقفه UART0
{
	sendchar0(getchar2());
//	UART2_RxString(REC2str);
//	UART0_TxString(REC2str);
}

#ifdef __cplusplus
}
#endif
 
 char RECstr[2000]; // Recive String
//char RECstr1[200]="Hello";

int main (){
	
	SystemInit();
//	UART0_Init(115200);
//	UART2_Init(115200);
	
	
//	LPC_GPIO2->FIODIR=0XFF;	// FOR LED
	
//	// init UART0
	NVIC_EnableIRQ(UART1_IRQn);
	LPC_SC->PCLKSEL0|=0X0;     //SET CLOCK OF UART0    CPUCLK/4=24MHZ
	LPC_UART1->LCR=0X83;      //SET 8bit data & enable dlab
	LPC_UART1->DLL=156;        // SET BAUD RATE = 9600///13 for 115200
	LPC_UART1->DLM=0;
	LPC_UART1->LCR=0X3;      // DESABLE DLAB
	LPC_UART1->FCR=0X7;      // SET FIFO AND CLAER
	LPC_PINCON->PINSEL0=0X50;// SET PIN FOR UART0
	LPC_UART1->IER=0X01;
	
	// init UART2
	NVIC_EnableIRQ(UART2_IRQn);
	LPC_SC->PCLKSEL0|=0X0;     //SET CLOCK OF UART0    CPUCLK/4=24MHZ
	LPC_PINCON->PINSEL0 |= (1 << 20);             // Pin P0.10 used as TXD2 (Com2) 
  LPC_PINCON->PINSEL0 |= (1 << 22);             // Pin P0.11 used as RXD2 (Com2) 	
	LPC_SC->PCONP = LPC_SC->PCONP|(1<<24);	      //Open UART2 power control bit	           
	LPC_UART2->LCR=0X83;      //SET 8bit data & enable dlab
	LPC_UART2->DLL=156;        // SET BAUD RATE = 115200
	LPC_UART2->DLM=0;
	LPC_UART2->LCR=0X3;      // DESABLE DLAB
	LPC_UART2->FCR=0X7;      // SET FIFO AND CLAER
	LPC_UART2->IER=0X01;

	// init ESP8266 ESP-01
	sprintf (str,"AT+CWMODE=3%c%c",0x0d,0x0a);
	sendstring0(str);
	delay();
	
	sprintf (str,"AT+CIPMUX=1%c%c",0x0d,0x0a);
	sendstring0(str);
	delay();
	
	sprintf (str,"AT+CIPSERVER=1,8888%c%c",0x0d,0x0a);
	sendstring0(str);
	delay();	

	sprintf (str,"AT+CWSAP?%c%c",0x0d,0x0a);
	sendstring0(str);
	delay();

	CounterWiFi=0;
	while (1){ 		
	
//	LPC_GPIO2->FIOPIN=0XFF;
//	delay();		
//	LPC_GPIO2->FIOPIN=0X00;
//	delay();

	sendstring2(RECstr);

		//sendchar2('A');
	//	sendstring2("Hello");

}
}

		
#ifdef __cplusplus
extern "C" {
#endif
	 

void UART1_IRQHandler (void) // ???? ???? ????? 0 ???? ?????? ??????? ?? ?????
{	

	if ((RECstr[CounterWiFi]=getchar0())!=0x0a && RECstr[CounterWiFi]!=0x0d)
	{
		RECstr[++CounterWiFi]='\0';
   }
	
	else{
		if(RECstr[CounterWiFi]==0x0d){
		RECstr[CounterWiFi]='\0';
		CounterWiFi++;	
		}
		else
		CounterWiFi=0;
	}
	
//	UART0_RxString(RECstr);
//	UART2_TxString(RECstr);

	
}

#ifdef __cplusplus
}
#endif

