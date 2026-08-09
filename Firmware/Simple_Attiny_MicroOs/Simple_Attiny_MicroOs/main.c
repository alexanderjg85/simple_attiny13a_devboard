/*
 * Simple_Attiny_MicroOs.c
 *
 * Created: 31.07.2026 22:14:31 
 * Author : alexa
 * Description:
 * Simple Time Triggered OS Core with Cooperative Multitasking, supports task intervals up to 200 ms
 * Every Task task has an Offset, so they can shifted in time. A task Interval must be a divisor of the scheduler cycle time,
 * which is configured to 60.000 which equals 1 Minute. This is enforced by assert statements, otherwise the tasks will jitter after 
 * system time overrun. The OS Core was inspired by the Autosar OS logic, which is a highly deterministic automotive grade operating system.
 *
 * Design Goals & Compromises:
 * This core was designed with 2 goals in mind and compromises had to be made. 
 * 1. Scalable Design that is easily portable to other architecture
 * 2. A strict limit of 500 bytes of Flash and less than 40 bytes of RAM, ideally less than that. 
 *
 * Compromise 1: Scheduler uses modulo logic because its easy scalable and the code for the scheduler logic doesn't grow, when adding additional tasks, 
 * although the modulo logic takes a lot of calculation power on systems without hardware support
 *
 * Compromise 2: To ensure the RAM limits are met, specific AVR commands for memory handling are used (pgm_read). Therefore these have to be removed when 
 * porting the OS to another architecture. 
 
 * Dependency:
 * The OS requires access to a systick timer, this systick timer has to be implemented for every CPU the OS is ported to.
 * Alternative:
 * If processing load of this OS core is too high for your application, use the Simple_Attiny_NanoOS, it supports 4 task and doesn't use
 * modulo logic and solely relies on cycle counter / timer decrementing to handle the scheduling and is highly recommended to use for small 8 Bit Cores
 * without hardware support for modulo.
 * Stats:
 *			Program Memory Usage 	:	466 bytes   45,5 % Full
 *			Data Memory Usage 		:	34 bytes   53,1 % Full
 *			Compiler				:	avr-gcc 5.4.0
 *			Important Compiler Flags:	-std=gnu11 -Os
 *			IDE						:	Atmel Studio 7.0.2389
 */ 

#include <avr/io.h>
#include "gpio_functions.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "systicktimer_config.h"
#include "micro_os_config.h"
#include "main.h"


volatile uint16 timerticks;		///< global systick timer
static uint16 timerticks_last;	///< last timer tick a scheduling event has taken place

static void initialize_events(void);	///< function to initialize the OS tasks 
static void schedule_events(uint16 current_tick);		///< function that calls the tasks that are ready to run
static void dispatch_events(void);		///< function that determines which tasks are ready to run

void init(void);			///< main init function
void init_systick_timer(void);	///< initializes the sys tick timer 

int main(void)
{
	
	init();
	
	while (1)
	{
		///< call the dispatcher every time the timertick has changed
		cli(); ///< lock interrupts to access the variable 
		uint16 timerticks_current = timerticks;
		sei();	///< unlock interrupts 
		while(timerticks_current != timerticks_last)
		{			
			if(timerticks_last >= SCHEDULER_PERIOD_MS)
			{
				timerticks_last = 0;
			}else {
				timerticks_last += 1;
			}
			schedule_events(timerticks_last);
			dispatch_events();			
		}		
	}
}

void init(void)
{
	///< initialize variables
	timerticks = 0;
	timerticks_last = 0;
	
	///< initialize GPIOs
	PORTB = 0x0;	///< set all pins low
	DDRB = 0x0;		///< all pins as Input
		
	DDRB |= (1 << LED_BOARD);	///< set LED Pins as Output
	
	///< initialize OS core
	init_systick_timer();
	
	initialize_events();
	
	sei();	///enable interrupts for the systick timer
}

///< set Timer0 register to configure Timer0 as sys tick timer
void init_systick_timer(void)
{
	OCR0A = TIMER0_COMPARE_VAL;		///< set compare value to the value defined in micro_os_config	
	
	set_TIMER0_TCCR0A(TIMER0_COMPARE_MATCH_MODE_A, TIMER0_COMPARE_MATCH_MODE_B,	TIMER0_WAVEFORM_GEN_MODE);
	
	set_TIMER0_TCCR0B(TIMER0_FORCE_AB, TIMER0_WAVEFORM_GEN_MODE, TIMER0_PRESCALER);
	
	set_TIMER0_TIMSK0(TIMER0_COMPARE_MATCH_B_INT_EN, TIMER0_COMPARE_MATCH_A_INT_EN, TIMER0_OVERFLOW_INT_EN);
}

///< scheduler function that determines which tasks are ready to run
///< uses the modulo operator as mathematical operation, this is performance critical on
///< small core likes ATTiny or PIC12, that don't have hardware support for modulo calculation
///< CPU Load measurement with 6 tasks in the simulator is around 23% running on ATTiny Core with 4,8 Mhz for the os core
///< which equals  around 1100 clock cycles
static void schedule_events(uint16 current_tick)
{
	uint8 task_interval;
	uint8 task_offset;
	uint8 time_modulo;
	for(uint8 i=0; i< TASK_CNT;i++)
	{
		task_interval = events[i].interval_ms;
		task_offset = events[i].offset_ms;
		///< if the remainder of the division is zero a task is ready to run, offsets are taken into account
		///< in order to prevent jitter 
		time_modulo = (current_tick - task_offset) % task_interval;
		if(!time_modulo)
		{
			events[i].triggered = true;
		}
	}
	return;
}

///< dispatch function, checks for ready to run tasks
static void dispatch_events(void)
{
	for(uint8 i = 0;i< TASK_CNT;i++)
	{
		if(events[i].triggered)
		{
			events[i].task_main(TASK_EVENT_LOOP);
			events[i].triggered = false;
		}
	}
}

///< function to initialize the OS tasks 
///< after initialization of the events structs, all taks will be called once so they can perform their own initialization
///< uses AVR specific functions, so that the values for the initialization are only stored in flash in and not in the RAM
///< Porting to another CPU architecture requires to remove the pgm_read commands
static void initialize_events(void)
{
	for(uint8 i=0;i<TASK_CNT;i++)
	{
	
		events[i].interval_ms = pgm_read_word(&(TASK_CONFIG[i].interval_ms));
		events[i].offset_ms = pgm_read_word(&(TASK_CONFIG[i].offset_ms));
		events[i].task_main   = (void (*)(uint8))pgm_read_ptr(&(TASK_CONFIG[i].task_main));
		events[i].triggered = false;	
	
		events[i].task_main(TASK_EVENT_INIT);		 
	}
}

///< skeleton for task 1, place your code here
void task1_main(TaskEvent event)
{
	if (event == TASK_EVENT_INIT) {
		
	}
	else
	{
		PORTB |= (1 << LED_BOARD);	///< led blinky example, can be removed
	}
}

///< skeleton for task 2, place your code here
void task2_main(TaskEvent event)
{
	if (event == TASK_EVENT_INIT) {
		
	}
	else
	{
		PORTB &= ~(1 << LED_BOARD);	///< led blinky example, can be removed
	}
}

///< skeleton for task 3, place your code here
void task3_main(TaskEvent event)
{
	if (event == TASK_EVENT_INIT) {
		
	}
	else
	{
		
	}
}

///< skeleton for task 4, place your code here
void task4_main(TaskEvent event)
{
	if (event == TASK_EVENT_INIT) {
		
	}
	else
	{
		
	}
}

///< skeleton for task 5, place your code here
void task5_main(TaskEvent event)
{
	if (event == TASK_EVENT_INIT) {
		
	}
	else
	{
		
	}
}

///< skeleton for task 6, place your code here
void task6_main(TaskEvent event)
{
	if (event == TASK_EVENT_INIT) {
		
	}
	else
	{
		
	}
}

///< ISR for Compare Match A Timer0 used as system timer
///< calls the scheduler every systick
ISR(TIM0_COMPA_vect) {
	timerticks++;
	if(timerticks >= SCHEDULER_PERIOD_MS)
	{
		timerticks = 0;
	} else {} ///< nothing to do		
}