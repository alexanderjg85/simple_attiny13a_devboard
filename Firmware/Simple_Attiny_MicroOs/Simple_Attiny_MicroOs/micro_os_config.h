/*
 * micro_os_config.h
 *
 * Created: 01.08.2026 18:52:23
 *  Author: alexa
 * Provides configuration and functions the MicroOS Core Configuration and Initialization
 */ 


#ifndef MICRO_OS_CONFIG_H_
#define MICRO_OS_CONFIG_H_

#include "custom_types.h"
#include <assert.h>


///< Helper Macro to print values that are define by a #define in an assert output string
#define STR_HELPER(x) #x
#define TO_STRING(x) STR_HELPER(x)

///< Defines events a task may react to
typedef enum {
	TASK_EVENT_INIT,
	TASK_EVENT_LOOP
} TaskEvent;

///< defines a typedef for function pointers with one argument used by the OS
typedef void (*TaskFunc)(TaskEvent event);

#define ms_PER_min 60000	///< number of ms in a minute
#define SCHEDULER_PERIOD_MS ms_PER_min	///< Time until the systicktimer starts counting from zero again
#define MAX_TASKS 6	///< maximum allowed number of taks, don't change
#define TASK_CNT 6	///< actual used number of tasks, change to adjust the os, set a value from 1 to 6
static_assert(TASK_CNT<= MAX_TASKS, "No more than " TO_STRING(MAX_TASKS) " tasks are supported!");
#define MAX_TASK_INTERVALL_ms 200	///< maximum task interval the OS supports

///< definition of the struct that contains the task and task information during runtime
struct scheduler_event
{
	uint8 interval_ms;
	uint8 offset_ms;	
	TaskFunc task_main;
	bool triggered;
};

///< type definition of the struct that contains the task and task information during initialization
typedef struct {
	uint8 interval_ms;
	uint8 offset_ms;
	TaskFunc task_main;
} TaskConfig;

///< enum that defines the interval of all tasks, change this values to adjust the operating system
///< if number of tasks is less than the maximum of 6, the unused tasks still need a valid interval otherwise the assert will fail
enum TASK_INTERVALS {
	TASK_1_INTERVAL = 200,
	TASK_2_INTERVAL = 200,
	TASK_3_INTERVAL = 100,
	TASK_4_INTERVAL = 100,
	TASK_5_INTERVAL = 100,
	TASK_6_INTERVAL = 100
};
///< make sure task interval are not set to a number that isn't a divisor of the scheduler period, otherwise the tasks will jitter
static_assert((SCHEDULER_PERIOD_MS % TASK_1_INTERVAL) == 0, "Task 1 Interval invalid!");
static_assert((SCHEDULER_PERIOD_MS % TASK_2_INTERVAL) == 0, "Task 2 Interval invalid!");
static_assert((SCHEDULER_PERIOD_MS % TASK_3_INTERVAL) == 0, "Task 3 Interval invalid!");
static_assert((SCHEDULER_PERIOD_MS % TASK_4_INTERVAL) == 0, "Task 4 Interval invalid!");
static_assert((SCHEDULER_PERIOD_MS % TASK_5_INTERVAL) == 0, "Task 5 Interval invalid!");
static_assert((SCHEDULER_PERIOD_MS % TASK_6_INTERVAL) == 0, "Task 6 Interval invalid!");

///< enum that defines the offsets of all tasks, change this values to adjust the operating system
///< if number of tasks is less than the maximum of 6, the unused tasks still need a valid offset otherwise the assert will fail
enum TASK_OFFSETS {
	TASK_1_OFFSET = 0,
	TASK_2_OFFSET = 100,
	TASK_3_OFFSET = 10,
	TASK_4_OFFSET = 20,
	TASK_5_OFFSET = 30,
	TASK_6_OFFSET = 40
};
///< make sure task offsets don't exceed the maximum task interval
static_assert((TASK_1_OFFSET <= MAX_TASK_INTERVALL_ms), "Task 1 Offset too large!");
static_assert((TASK_2_OFFSET <= MAX_TASK_INTERVALL_ms), "Task 2 Offset too large!");
static_assert((TASK_3_OFFSET <= MAX_TASK_INTERVALL_ms), "Task 3 Offset too large!");
static_assert((TASK_4_OFFSET <= MAX_TASK_INTERVALL_ms), "Task 4 Offset too large!");
static_assert((TASK_5_OFFSET <= MAX_TASK_INTERVALL_ms), "Task 5 Offset too large!");
static_assert((TASK_6_OFFSET <= MAX_TASK_INTERVALL_ms), "Task 6 Offset too large!");


///< define the task functions
void task1_main(TaskEvent event);
void task2_main(TaskEvent event);
void task3_main(TaskEvent event);
void task4_main(TaskEvent event);
void task5_main(TaskEvent event);
void task6_main(TaskEvent event);

///< array that will be used during startup to initialize all the task, don't change, use the enums above to adjust the OS
const TaskConfig TASK_CONFIG[MAX_TASKS] PROGMEM = {
	{TASK_1_INTERVAL, TASK_1_OFFSET, task1_main}, {TASK_2_INTERVAL, TASK_2_OFFSET, task2_main}, {TASK_3_INTERVAL, TASK_3_OFFSET, task3_main},
	{TASK_4_INTERVAL, TASK_4_OFFSET, task4_main}, {TASK_5_INTERVAL, TASK_5_OFFSET, task5_main}, {TASK_6_INTERVAL, TASK_6_OFFSET, task6_main}
};

///< create the struct that contains the tasks
struct scheduler_event events[TASK_CNT];

#endif /* MICRO_OS_CONFIG_H_ */