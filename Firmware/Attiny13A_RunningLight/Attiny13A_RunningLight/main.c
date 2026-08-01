/*
 * Attiny13A_Blinky_4_LEDS.c
 *
 * Created: 09.07.2026 18:42:30
 * Author : alexa
 */

#define F_CPU 1200000UL  ///< 9,6 Mhz internal CLK, DIV 8, not changed


#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "gpio_functions.h"


#define LED_BOARD PORTB4
#define LED_DELAY_ms 500	///< time a LED lights before switched to the next

void init(void);

int main(void)
{
    init();    
    while (1) 
    {
		set_PortPin(LED_BOARD);
		clear_PortPin(PORTB3);
		_delay_ms(LED_DELAY_ms);
		
		set_PortPin(PORTB0);
		clear_PortPin(LED_BOARD);
		_delay_ms(LED_DELAY_ms);
		
		set_PortPin(PORTB1);
		clear_PortPin(PORTB0);
		_delay_ms(LED_DELAY_ms);
		
		set_PortPin(PORTB2);
		clear_PortPin(PORTB1);
		_delay_ms(LED_DELAY_ms);
		
		set_PortPin(PORTB3);
		clear_PortPin(PORTB2);
		_delay_ms(LED_DELAY_ms);
    }
}

void init(void)
{
	DDRB = 0x0;		///< all pins as Input
	DDRB |= (1 << LED_BOARD) | (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);	///< set the 4 led ports + onboard led  as output
	PORTB = 0x0;	///< set all pins low
}