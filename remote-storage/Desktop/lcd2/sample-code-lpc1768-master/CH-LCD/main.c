#include <lpc17xx.h>

#define LCD_LPC1768       /* Char LCD definition */
#define LCD_PORT_1
#define LCD_RS     18
#define LCD_RW     19
#define LCD_E      20
#define LCD_DB4    22
#define LCD_DB5    23
#define LCD_DB6    24
#define LCD_DB7    25

#include "lcd.h"


char ch[]={'*','C','H','A','R',' ','L','C','D',' ','T','e','s','t'};

int delay(int Del){
	long int i;
	for (i=0;i<Del*100000;i++);
}


int main()
{
	int j;
	lcd_init();
	lcd_clear();
	
	while (1) {
		lcd_gotoxy(0,2);
		lcd_putsf("Hello-World");
		lcd_gotoxy(2,2);
		
		for(j=0;j<14;j++){
			lcd_putchar(ch[j]);
			delay(35);
		}
		delay(200);
		
		for(j=0;j<16;j++){
		lcd_gotoxy(1,j+1);
		lcd_putchar(' ');	
		lcd_gotoxy(2,j+1);	
		lcd_putchar(' ');
		delay(25);	
		}
	}
}

