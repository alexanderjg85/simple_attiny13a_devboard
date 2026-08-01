/*
 * GPIO_Functions.h
 *
 * Created: 01.08.2026 18:04:06
 *  Author: alexa
 */ 


#ifndef GPIO_FUNCTIONS_H_
#define GPIO_FUNCTIONS_H_

#include "main.h"

static inline void set_PortPin(uint8 portpin) {
	PORTB |= (1 << portpin);
}

static inline void clear_PortPin(uint8 portpin) {
	PORTB &= ~(1 << portpin);
}

static inline void toggle_PortPin(uint8 portpin) {
	PORTB ^= (1 << portpin);
}



#endif /* GPIO_FUNCTIONS_H_ */