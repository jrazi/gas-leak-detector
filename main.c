#include "LPC17xx.h"

int main() {
	
	while (1) {
		LPC_GPIO0->FIODIR0 = 0xFF;
		
	}
}