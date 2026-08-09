/*
 * systicktimer_config.h
 *
 * Created: 01.08.2026 19:01:44
 *  Author: alexa
 * Provides configuration and functions for Timer0 Configuration to use it in CTC Mode (Clear Timer on Compare Match) as Systicktimer
 */ 


#ifndef SYSTICKTIMER_CONFIG_H_
#define SYSTICKTIMER_CONFIG_H_

#include "custom_types.h"

///< Bit masks TCCR0A register
#define COM0A_MASK	0b00000011	///> Compare Match Output A Mode
#define COM0B_MASK	0b00000011	///> Compare Match Output B Mode
#define WGM1_0_MASK 0b00000011	///> WGM-Bit[1:0]

///< Function that sets the TIMER0 TCCR0A register according to the arguments
static inline void set_TIMER0_TCCR0A(uint8 com0a, uint8 com0b, uint8 wgm1_0) {
	TCCR0A = ((com0a & COM0A_MASK) << COM0A0) | ((com0b & COM0B_MASK) << COM0B0) | ((wgm1_0 & WGM1_0_MASK) << WGM00);
}

///< Bit masks TCCR0B register
#define FOC0_MASK   0b00000011  ///>  FOC0A / FOC0B
#define WGM2_MASK   0b00000001  ///>  WGM-Bit[2]
#define CS0_MASK    0b00000111  ///> Prescaler

///< Function that sets the TIMER0 TCCR0B register according to the arguments
static inline void set_TIMER0_TCCR0B(uint8 foc0, uint8 wgm2, uint8 cs0) {
	TCCR0B = ((foc0  & FOC0_MASK) << FOC0B) | ((wgm2 & WGM2_MASK) << WGM02) | ((cs0 & CS0_MASK)   << CS00);
}

// Bit masks TIMSK0 register
#define OCIE0B_MASK   0b00000001  ///> Output Compare Match B Interrupt Enable
#define OCIE0A_MASK   0b00000001  ///> Output Compare Match A Interrupt Enable
#define TOIE0_MASK    0b00000001  ///> Overflow Interrupt Enable

///< Function that sets the TIMER0 TIMSK0 register according to the arguments
static inline void set_TIMER0_TIMSK0(uint8 ocie0b, uint8 ocie0a, uint8 toie0) {
	TIMSK0 = ((ocie0b & OCIE0B_MASK) << OCIE0B) | ((ocie0a & OCIE0A_MASK) << OCIE0A) | ((toie0 & TOIE0_MASK)  << TOIE0);
}

///< timer prescaler 64 => 4.8 Mhz / 64 = 75 kHz, Interrupt every 1 ms -> compare at 75
///< use CTC mode Clear timer on compare match WGM0[2:0] = 2
#define TIMER0_COMPARE_VAL 74	///< 75 timer ticks, 0..74, value to be written in to compare register
#define TIMER0_PRESCALER 0x3  ///< set prescaler to 64

#define TIMER0_OVERFLOW_INT_EN 0x0			///< define Overflow interrupt not enabled
#define TIMER0_COMPARE_MATCH_A_INT_EN 0x1	///< define Compare Match Interrupt A interrupt  enabled
#define TIMER0_COMPARE_MATCH_B_INT_EN 0x0	///< define Compare Match Interrupt B interrupt not enabled

#define TIMER0_FORCE_AB 0					///< don't force output compare A and B (FOC0A/FOC0B)
#define TIMER0_WAVEFORM_GEN_MODE 0x2		///< set waveform generation mode to CTC 
#define TIMER0_COMPARE_MATCH_MODE_A 0x0		///< set Compare Match Output A Mode to normal operation
#define TIMER0_COMPARE_MATCH_MODE_B  0x0	///< set Compare Match Output B Mode to normal operation



#endif /* SYSTICKTIMER_CONFIG_H_ */