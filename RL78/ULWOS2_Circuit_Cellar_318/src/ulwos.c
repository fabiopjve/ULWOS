/*
 * ULWOS - Ultra Lightweight Operating System
 *
 * ULWOS is not a real operating system but a pre-emptive task switcher. It was designed to
 * demonstrate the basics of a multi-tasking system with minimum memory footprint.
 *
 * The following code makes heavy use of assembly language!!!
 * The code is compatible with GCC compiler only!
 *
 * Copyright: Fábio Pereira 2016
 * fabio.jve@gmail.com
 */

#include "iodefine.h"
#include "iodefine_ext.h"
#include "myrl78.h"
#include "interrupt_handlers.h"
#include "ulwos.h"

static unsigned volatile int ulwos_tempSP, ulwos_idleSP;
static unsigned volatile int ulwos_intcounter;
static unsigned volatile int ulwos_idlecounter, ulwos_counter;
static unsigned volatile int ulwos_task_context[ULWOS_NUM_TASKS][12];
static unsigned volatile int ulwos_taskSP[ULWOS_NUM_TASKS];
static unsigned volatile int ulwos_task_delay[ULWOS_NUM_TASKS];
static volatile ULWOS_TASKSTATUS ulwos_task_status[ULWOS_NUM_TASKS];
static volatile ULWOS_TASKHANDLER ulwos_current_task=0, ulwos_old_task=0;
static volatile char ulwos_num_tasks=0;
volatile char ulwos_task_stack[ULWOS_NUM_TASKS][ULWOS_TASK_STACK_SIZE];

#define save_regs()	asm volatile ("push AX\n\tpush BC\n\tpush DE\n\tpush HL\n\t")
#define restore_regs() asm volatile ("pop HL\n\tpop DE\n\tpop BC\n\tpop AX\n\t")
#define ulwos_halt() asm ("halt")
#define ulwos_critical_start() asm("di")
#define ulwos_critical_end() asm ("ei")

void inline save_context(void){
	asm volatile (
		"sel RB3\n\t"		// We are going to use bank3 as it is reserved for isr code
		"mov X,%[ct]\n\t"	// X = current_task
		"clrb A\n\t"		// A = 0 (AX = current_task)
		"shlw AX,1\n\t"		// AX = current_task*2
		"movw BC,%[tsp]\n\t"// BC = taskSP
		"addw AX,BC\n\t"	// AX = taskSP+(current_task*2) => taskSP[current_task]
		"movw DE,AX\n\t"	// DE = taskSP[current_task]
		"movw AX,SP\n\t"	// AX = task's SP
		"movw [DE],AX\n\t"	// task_SP[current_task] = task's SP
		// now we set the pointer to the context stack, so we can save current context
		"mov A,#24\n\t"		// A = 24
		"mov X,%[ct]\n\t"	// X = current_task
		"inc X\n\t"			// X = current_task+1 (so we to the top of the context stack)
		"mulu X\n\t"		// AX = 24 * (current_task+1)
		"addw AX,%[tc]\n\t"	// AX = address of task_context[current_task]
		"movw SP,AX\n\t"	// set the SP to the top of task's context stack
		:
		:[ct]"m"(ulwos_current_task),[tc]"i"(ulwos_task_context),[tsp]"i"(ulwos_taskSP)
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
		"mov X,%[ct]\n\t"	// X = current_task
		"mulu X\n\t"		// AX = 24 * (current_task)
		"addw AX,%[tc]\n\t"	// AX = address of task_context[current_task]
		"movw SP,AX\n\t"	// set the SP to the top of task's context stack
		:
		:[ct]"m"(ulwos_current_task),[tc]"i"(ulwos_task_context)
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
		"mov X,%[ct]\n\t"	// X = current_task
		"clrb A\n\t"		// A = 0 (AX = current_task)
		"shlw AX,1\n\t"		// AX = current_task*2
		"movw BC,%[tsp]\n\t"// BC = taskSP
		"addw AX,BC\n\t"	// AX = address of taskSP+(current_task*2) => taskSP[current_task]
		"movw DE,AX\n\t"	// DE = address of taskSP[current_task]
		"movw AX,[DE]\n\t"	// AX = taskSP[current_task]
		"movw SP,AX\n\t"	// set the SP for the current task
		"reti\n\t"			// return from this interrupt (change the context)
		:
		:[ct]"m"(ulwos_current_task),[tsp]"i"(ulwos_taskSP)
	);
}

ULWOS_TASKHANDLER ulwos_create_task(void * task_address){
	if (ulwos_num_tasks>=ULWOS_NUM_TASKS) return -1;
	ulwos_tempSP = (int) task_address;
	ulwos_current_task = ulwos_num_tasks;
	ulwos_task_status[ulwos_current_task]=ULWOS_TASK_RUNNING;
	ulwos_num_tasks++;
	asm volatile (
		"sel RB3\n\t"
		"movw HL,SP\n\t"	// HL has the old SP
		"clrw AX\n\t"		// AX = 0
		"movw DE,%[tsz]\n\t"// DE = ULWOS_TASK_STACK_SIZE
		"clrb B\n\t"
		"mov C,%[ct]\n\t"	// BC = current_task
		"inc C\n\t"
		// this is a simple 16-bit multiply
		"LOOP:"
		"addw AX,DE\n\t"	// AX = AX + ULWOS_TASK_STACK_SIZE
		"decw BC\n\t"		// BC = BC-1
		"cmp0 B\n\t"		// compares B with 0
		"bnz $LOOP\n\t"		// branch to loop if not zero
		"cmp0 C\n\t"		// compares C with 0
		"bnz $LOOP\n\t"		// branch to loop if not zero
		// AX = ULWOS_TASK_STACK_SIZE * (current_task+1)
		"movw BC,AX\n\t"
		"movw AX,%[ts]\n\t"
		"addw AX,BC\n\t"	// AX = pointer to task_stack[current_stack]
		"movw SP,AX\n\t"	// set the SP to the top of the new task stack
		"movw DE,AX\n\t"	// DE has the TOS for this task
		"movw AX,#0x8600\n\t"
		"push AX\n\t"		// save PSW and PCH (all 0)
		"movw AX,%[tsp]\n\t"// AX = task function address (stored in tempSP)
		"push AX\n\t"		// save the function address onto stack
		:
		:[tsz]"i"(ULWOS_TASK_STACK_SIZE),[ct]"m"(ulwos_current_task),[ts]"i"(ulwos_task_stack),[tsp]"m"(ulwos_tempSP)
	);
	asm volatile (
		"mov X,%[ct]\n\t"	// X = current_task
		"clrb A\n\t"		// A = 0 (AX = current_task)
		"shlw AX,1\n\t"		// AX = current_task*2
		"movw BC,%[tsp]\n\t"// BC = taskSP
		"addw AX,BC\n\t"	// AX = taskSP+(current_task*2) => taskSP[current_task]
		"movw DE,AX\n\t"	// DE = taskSP[current_task]
		"movw AX,SP\n\t"	// AX = task's SP
		"movw [DE],AX\n\t"	// task_SP[current_task] = task's SP
		"movw AX,HL\n\t"
		"movw SP,AX\n\t"	// restore the old SP
		"sel RB0\n\t"
		:
		:[tsp]"i"(ulwos_taskSP),[ct]"m"(ulwos_current_task)
	);
	return ulwos_current_task;
}

void inline ulwos_start(void){
	ulwos_current_task = 0;
	OSMC = bWUTMMCK0;		// set LOCO (15kHz) as the IT/RTC clock source
	RTCEN = 1;				// enables the RTC and IT
	ITMC = bRINTE | 14;		// IT enabled, interval = 1ms
	ITMK = 0;				// IT interrupt enabled
	ulwos_counter = ulwos_intcounter = ulwos_idlecounter = 0;
	asm volatile (
		"sel RB3\n\t"		// select register bank 3
		"movw AX,SP\n\t"	// AX = current SP
		"movw %[isp],AX\n\t"// ulwos_idleSP = current SP
		"movw AX,%[tsp]\n\t"// AX = taskSP[0]
		"movw SP,AX\n\t"	// SP = AX = taskSP[0]
		// this is a fake return from interrupt, it restores the PSW and PC from the stack
		"reti\n\t"
		"nop\n\t"
		:
		:[tsp]"m"(ulwos_taskSP),[isp]"m"(ulwos_idleSP)
	);
}

void ulwos_idle_task(){
	ulwos_counter++;
	while(1){
		ulwos_halt();
	}
}

void inline ulwos_run_idle_task(){
	ulwos_current_task = ULWOS_IDLETASK_ID;
	asm volatile (
		"sel RB3\n\t"		// select register bank 3
		"movw AX,%[isp]\n\t"// AX = ulwos_idleSP
		"movw SP,AX\n\t"	// SP = ulwos_idleSP
		"movw AX,#0x8600\n\t"
		"push AX\n\t"		// save PSW and PCH (all 0)
		"movw AX,%[idl]\n\t"// AX = idle task function address
		"push AX\n\t"		// save the function address onto stack
		"reti\n\t"			// return from interrupt (go to idle task function)
		:
		:[isp]"m"(ulwos_idleSP),[idl]"i"(ulwos_idle_task)
	);
}

/*
 * ulwos_delayms - put task in delay mode for time ms
 */
void ulwos_delayms(unsigned int time){
	ulwos_critical_start();
	ulwos_task_delay[ulwos_current_task] = time;
	ulwos_task_status[ulwos_current_task] = ULWOS_TASK_DELAY;
	ulwos_critical_end();
	ulwos_halt();
}

void ulwos_semaphore_init(ULWOS_SEMAPHORE *semaphore){
	ulwos_critical_start();
	*semaphore = 0;
	ulwos_critical_end();
}

void ulwos_semaphore_acquire(ULWOS_SEMAPHORE *semaphore){
	ULWOS_SEMAPHORE intsem;
	do{
		ulwos_critical_start();
		intsem = *semaphore;
		ulwos_critical_end();
	} while (intsem);	// wait while the semaphore is busy
	*semaphore = 1;	// acquire the semaphore
	ulwos_critical_end();	// end of critical section
}

void ulwos_semaphore_release(ULWOS_SEMAPHORE *semaphore){
	ulwos_critical_start();
	*semaphore = 0;			// release semaphore
	ulwos_critical_end();	// end of critical section
}

void __attribute__ ((naked)) INT_IT(void) {
	if (ulwos_current_task==ULWOS_IDLETASK_ID){
		ulwos_current_task = ulwos_old_task;	// replace the task id with the task one
	} else save_context();			// if current task is not IDLE, save its context
	ulwos_current_task++;
	if (ulwos_current_task>=ulwos_num_tasks) {	// if we reached the last task, restart from the first one
		ulwos_current_task=0;
	}
	unsigned char aux;
	for (aux=0;aux<ulwos_num_tasks;aux++){
		if (ulwos_task_delay[aux]) ulwos_task_delay[aux]--;
	}
	ulwos_intcounter++;							// the internal tick counter
	if (ulwos_intcounter==1000){				// every 1000ms
		ulwos_idlecounter = ulwos_counter;		// get the current idle time
		ulwos_counter = ulwos_intcounter = 0;	// and reset counter for another measurement
	}
	if (ulwos_task_status[ulwos_current_task]==ULWOS_TASK_DELAY){	// if the task is in delay mode
		if (ulwos_task_delay[ulwos_current_task]){	// if the timer counting is not completed
			ulwos_old_task = ulwos_current_task;
			ulwos_run_idle_task();					// and run idle task again
		} else ulwos_task_status[ulwos_current_task]=ULWOS_TASK_RUNNING;	// else, put task in running mode again
	}
	restore_context();			// restore task's context
}


