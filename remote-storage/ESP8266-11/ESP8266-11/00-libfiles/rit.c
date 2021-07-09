#include <lpc17xx.h>
#include "rit.h"

unsigned int rit_counter = 0;
/*
rit_Enable_Disable : چون بعضي وقتا نيازه که بعد از راه اندازي اين واحد، اين واحد شروع به کار نکه
و بعدا هر موقع نياز داشتيم شروعش کنيم و غير فعالش کنيم - لذا اين قابليت فعال بودن يا 
نبودن اين واحد در هنگام پيکربندي رو به اين تابع اضافه کردم
Enable : فعال کردن
Disable : غير فعال کردن
*/
void rit_init( unsigned int TimerInterval, char rit_Enable_Disable ) {
	
	LPC_SC->PCONP |= (1 << 16); // Repetitive Interrupt Timer power/clock control bit.
	
	// بعد از روشن شدن ميکرو، اين واحد هم فعال ميشه به صورت خودکار
	// ما غير فعالش ميکنيم تا مقدار اوليه شمارنده ها رو 0 کنيم و بعد
	// دو تا کار انجام ميديم و در نهايت اين واحد رو فعال ميکنيم
	rit_disable_counter();

	// صفر کردن رجيستر و متغير شمارنده
	rit_counter = 0;
	rit_set_value_counter(0);

	rit_set_zero_counter_in_occur_match( 1/*Enable*/ );

	LPC_RIT->RICOMPVAL = TimerInterval; // RI Compare Value register

	if( rit_Enable_Disable == Enable ) rit_enable_counter();
}

/*-------------------------------------------*/
void rit_clear_interrupt_flag( void ) {

	LPC_RIT->RICTRL |= (1<<_RICTRL_RITINT);
}
char rit_read_interrupt_flag( void ) {
	return LPC_RIT->RICTRL & (1<<_RICTRL_RITINT);
}
/*-------------------------------------------*/
// 0 : Disable, 1 : Enable
void rit_set_zero_counter_in_occur_match( char EnableDisable ) {
	
	if( EnableDisable ) { // Enable
		LPC_RIT->RICTRL |= (1<<_RICTRL_RITENCLR);
	}
	else { // Disable
		LPC_RIT->RICTRL &= ~(1<<_RICTRL_RITENCLR);
	}
}
/*-------------------------------------------*/
void rit_enable_counter( void ) {

	LPC_RIT->RICTRL |= (1<<_RICTRL_RITEN);
}
void rit_disable_counter( void ) {

	LPC_RIT->RICTRL &= ~(1<<_RICTRL_RITEN);
}
/*-------------------------------------------*/

void rit_reset( void ) {

	LPC_RIT->RICTRL |= (1<<_RICTRL_RITENCLR);
}

void rit_set_value_counter( unsigned int Value ) {

	LPC_RIT->RICOUNTER = Value;
}
