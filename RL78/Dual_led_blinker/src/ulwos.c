/*
 * ULWOS - Ultra Lightweight Operating System
 *
 * ULWOS is not a real operating system but a pre-emptive task switcher. It was designed to
 * demonstrate the basics of a multi-tasking system with minimum memory footprint.
 *
 * The following code makes heavy use of assembly language!!!The code is compatible with GCC compiler only!
 *
 * Copyright: Fábio Pereira 2016
 *
 */

#include "iodefine.h"
#include "iodefine_ext.h"
#include "myrl78.h"
#include "interrupt_handlers.h"
#include "ulwos.h"

static unsigned volatile int tempSP;
static unsigned volatile int task_context[ULWOS_NUM_TASKS][12];
static unsigned volatile int taskSP[ULWOS_NUM_TASKS];
static volatile ULWOS_TASKHANDLER current_task=0;
static volatile char num_tasks=0;
volatile char task_stack[ULWOS_NUM_TASKS][ULWOS_TASK_STACK_SIZE];

#define save_regs()	asm volatile ("push AX\n\tpush BC\n\tpush DE\n\tpush HL\n\t")
#define restore_regs() asm volatile ("pop HL\n\tpop DE\n\tpop BC\n\tpop AX\n\t")

void inline save_context(void){
	asm volatile (
		"sel RB3\n\t"		// We are going to use bank3 as it is reserved for isr code
		"mov X,%0\n\t"		// X = current_task
		"clrb A\n\t"		// A = 0 (AX = current_task)
		"shlw AX,1\n\t"		// AX = current_task*2
		"movw BC,%2\n\t"	// BC = taskSP
		"addw AX,BC\n\t"	// AX = taskSP+(current_task*2) => taskSP[current_task]
		"movw DE,AX\n\t"	// DE = taskSP[current_task]
		"movw AX,SP\n\t"	// AX = task's SP
		"movw [DE],AX\n\t"	// task_SP[current_task] = task's SP
		// now we set the pointer to the context stack, so we can save current context
		"mov A,#24\n\t"		// A = 24
		"mov X,%0\n\t"		// X = current_task
		"inc X\n\t"			// X = current_task+1 (so we to the top of the context stack)
		"mulu X\n\t"		// AX = 24 * (current_task+1)
		"addw AX,%1\n\t"	// AX = address of task_context[current_task]
		"movw SP,AX\n\t"	// set the SP to the top of task's context stack
		:
		:"m"(current_task),"i"(task_context),"i"(taskSP)
	);
	// now we save registers from banks 2,1 and 0 into the context stack
	asm ("sel RB2");
	save_regs();		// save regs from bank 2
	asm ("sel RB1");
	save_regs();		// save regs from bank 1
	asm ("sel RB0");
	save_regs();		// save regs from bank 0
	asm ("sel RB3");
}

void inline restore_context(void){
	asm volatile (
		// now we set the pointer to the context stack, so we can save current context
		"mov A,#24\n\t"		// A = 24
		"mov X,%0\n\t"		// X = current_task
		"mulu X\n\t"		// AX = 24 * (current_task)
		"addw AX,%1\n\t"	// AX = address of task_context[current_task]
		"movw SP,AX\n\t"	// set the SP to the top of task's context stack
		:
		:"m"(current_task),"i"(task_context)
	);
	// now we restore registers from banks 0,1 and 2 from the stack
	asm ("sel RB0");
	restore_regs();		// restore regs from bank 0
	asm ("sel RB1");
	restore_regs();		// restore regs from bank 1
	asm ("sel RB2");
	restore_regs();		// restore regs from bank 2
	asm volatile (
		"sel RB3\n\t"
		"mov X,%0\n\t"		// X = current_task
		"clrb A\n\t"		// A = 0 (AX = current_task)
		"shlw AX,1\n\t"		// AX = current_task*2
		"movw BC,%1\n\t"	// BC = taskSP
		"addw AX,BC\n\t"	// AX = address of taskSP+(current_task*2) => taskSP[current_task]
		"movw DE,AX\n\t"	// DE = address of taskSP[current_task]
		"movw AX,[DE]\n\t"	// AX = taskSP[current_task]
		"movw SP,AX\n\t"	// set the SP for the current task
		"reti\n\t"			// return from this interrupt (change the context)
		:
		:"m"(current_task),"i"(taskSP)
	);
}

ULWOS_TASKHANDLER ulwos_create_task(void * task_address){
	if (num_tasks>=ULWOS_NUM_TASKS) return -1;
	tempSP = (int) task_address;
	current_task = num_tasks;
	num_tasks++;
	asm volatile (
		"sel RB3\n\t"
		"movw HL,SP\n\t"	// HL has the old SP
		"mov A,%0\n\t"		// A = ULWOS_TASK_STACK_SIZE
		"mov X,%1\n\t"		// X = current_task
		"inc X\n\t"			// X = current_task+1 (so we can calculate the top of the current stack)
		"mulu X\n\t"		// AX = ULWOS_TASK_STACK_SIZE * (current_task+1)
		"movw BC,AX\n\t"
		"movw AX,%2\n\t"
		"addw AX,BC\n\t"	// AX = pointer to task_stack[current_stack]
		"movw SP,AX\n\t"	// set the SP to the top of the new task stack
		"movw DE,AX\n\t"	// DE has the TOS for this task
		"movw AX,#0x8600\n\t"
		"push AX\n\t"		// save PSW and PCH (all 0)
		"movw AX,%3\n\t"	// AX = task function address (stored in tempSP)
		"push AX\n\t"		// save the function address onto stack
		:
		:"i"(ULWOS_TASK_STACK_SIZE),"m"(current_task),"i"(task_stack),"m"(tempSP)
	);
	asm volatile (
		"mov X,%1\n\t"		// X = current_task
		"clrb A\n\t"		// A = 0 (AX = current_task)
		"shlw AX,1\n\t"		// AX = current_task*2
		"movw BC,%0\n\t"	// BC = taskSP
		"addw AX,BC\n\t"	// AX = taskSP+(current_task*2) => taskSP[current_task]
		"movw DE,AX\n\t"	// DE = taskSP[current_task]
		"movw AX,SP\n\t"	// AX = task's SP
		"movw [DE],AX\n\t"	// task_SP[current_task] = task's SP
		"movw AX,HL\n\t"
		"movw SP,AX\n\t"	// restore the old SP
		"sel RB0\n\t"
		:
		:"i"(taskSP),"m"(current_task)
	);
	return current_task;
}

void inline ulwos_start(void){
	current_task = 0;
	OSMC = bWUTMMCK0;		// set LOCO (15kHz) as the IT/RTC clock source
	RTCEN = 1;				// enables the RTC and IT
	ITMC = bRINTE | 14;		// IT enabled, interval = 1ms
	ITMK = 0;				// IT interrupt enabled
	asm volatile (
		"sel RB3\n\t"		// select register bank 3
		"movw AX,%0\n\t"	// AX = taskSP[0]
		"movw SP,AX\n\t"	// SP = AX = taskSP[0]
		// this is a fake return from interrupt, it restores the PSW and PC from the stack
		"reti\n\t"
		"nop\n\t"
		:
		:"m"(taskSP)
	);
}

void __attribute__ ((naked)) INT_IT(void) {
	save_context();
	current_task++;
	if (current_task>=num_tasks) current_task=0;
	restore_context();
}
