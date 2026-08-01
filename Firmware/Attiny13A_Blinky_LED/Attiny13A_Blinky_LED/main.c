/*
 * Attiny13A_Blinky_LED.c
 *
 * Created: 08.07.2026 23:11:35
 * Author : alexa
 */ 

#define F_CPU 1200000UL  ///< 9,6 Mhz internal CLK, DIV 8, not changed

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "gpio_functions.h"


#define LED PORTB4

void init(void);

int main(void)
{
    init();
	set_PortPin(LED);
	
    while (1) 
    {
		_delay_ms(500);
		toggle_PortPin(LED);
    }
}

void init(void)
{
	DDRB = 0x0;
	DDRB |= (1 << LED);	
	PORTB = 0x0;
}