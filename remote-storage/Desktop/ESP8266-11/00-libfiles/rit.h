#ifndef _RIT_H_
#define _RIT_H_

#include <lpc17xx.h>
#include "ritDefine.h"

#define Enable 	1
#define Disable 0

void rit_enable_counter( void );
void rit_disable_counter( void );

void rit_clear_interrupt_flag( void );
char rit_read_interrupt_flag( void );

void rit_set_zero_counter_in_occur_match( char EnableDisable );
void rit_reset( void );
void rit_set_value_counter( unsigned int Value );
void rit_init( unsigned int TimerInterval, char rit_Enable_Disable );

void rit_set_value_counter( unsigned int Value );

#endif
