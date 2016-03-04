/*
 * ULWOS - Ultra Lightweight Operating System
 * Author:
 * 	Fábio Pereira
 * 	fabio@sctec.com.br or fabio.jve@gmail.com
 *
 *
 * RL78 Port on 02/26/2016
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 */

#include "iodefine.h"
#include "iodefine_ext.h"
#include "myrl78.h"
#include "interrupt_handlers.h"

// the following line defines the stack size for each task
#ifndef ULWOS_TASK_STACK_SIZE

#define ULWOS_TASK_STACK_SIZE	128

#endif

// the following line defines the maximum number of tasks
#ifndef ULWOS_NUM_TASKS

#define ULWOS_NUM_TASKS			4

#endif

// the handler for each ULWOS task
typedef signed char ULWOS_TASKHANDLER;
