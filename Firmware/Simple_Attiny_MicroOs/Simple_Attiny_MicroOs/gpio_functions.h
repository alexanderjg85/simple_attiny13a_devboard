/*
 * GPIO_Functions.h
 *
 * Created: 01.08.2026 18:04:06
 *  Author: alexa
 * Provides functions for bit manipulation of GPIO Pins of PortB
 * Attiny13A only has PortB
 */ 



#ifndef GPIO_FUNCTIONS_H_
#define GPIO_FUNCTIONS_H_

#include "custom_types.h"

///< sets the Port pin specified by the argument
static inline void set_PortPin(uint8 portpin) {
	PORTB |= (1 << portpin);
}

///< clears the Port pin specified by the argument
static inline void clear_PortPin(uint8 portpin) {
	PORTB &= ~(1 << portpin);
}

///< toggles the Port pin specified by the argument
static inline void toggle_PortPin(uint8 portpin) {
	PORTB ^= (1 << portpin);
}



#endif /* GPIO_FUNCTIONS_H_ */