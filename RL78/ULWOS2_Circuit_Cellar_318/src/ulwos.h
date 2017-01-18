/*
 * ULWOS - Ultra Lightweight Operating System
 * Author: Fábio Pereira fabio@sctec.com.br or fabio.jve@gmail.com
 *
 * RL78 Port in 02/26/2016
 */

#include "iodefine.h"
#include "iodefine_ext.h"
#include "myrl78.h"
#include "interrupt_handlers.h"

// the following line defines the stack size for each task
#ifndef ULWOS_TASK_STACK_SIZE

#define ULWOS_TASK_STACK_SIZE	256

#endif

// the following line defines the maximum number of tasks
#ifndef ULWOS_NUM_TASKS

#define ULWOS_NUM_TASKS			8

#endif

// idle task ID
#define ULWOS_IDLETASK_ID		-1
// the handler for each ULWOS task
typedef signed char ULWOS_TASKHANDLER;
// task status flags
typedef enum {ULWOS_TASK_EMPTY,ULWOS_TASK_RUNNING,ULWOS_TASK_DELAY} ULWOS_TASKSTATUS;
// simple semaphore
typedef volatile unsigned char ULWOS_SEMAPHORE;
