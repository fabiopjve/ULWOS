/*
 * ULWOS - Ultra Light Weight Operating System
 * Author: Fábio Pereira fabio@sctec.com.br or fabio.jve@gmail.com
 *
 * RL78 Port in 02/26/2016
 */

#include "iodefine.h"
#include "iodefine_ext.h"
#include "myrl78.h"
#include "interrupt_handlers.h"

// the following line defines the stack size for each task
#define ULWOS_TASK_STACK_SIZE	128

// the following line defines the maximum number of tasks
#define ULWOS_NUM_TASKS			8

// the handler for each ULWOS task
typedef signed char ULWOS_TASKHANDLER;
