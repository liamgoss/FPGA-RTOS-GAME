/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"
#include "system.h"
#include "io.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2

volatile int edge_capture; // For KEYS
static void handle_key_interrupts(void*);
static void key1_isr();
static void key2_isr();
static void key3_isr();

static void pio_init();

OS_EVENT* ledMutex;


/*
 *
 * OSMutexPend Function:
		Parameters:
			- OS_EVENT *pevent: Pointer to the mutex control block.
			- INT16U timeout: The maximum amount of time (in ticks) the task is willing to wait for the mutex. Use 0 for no timeout (wait indefinitely).
			- INT8U *perr: Pointer to an INT8U variable where the error code will be stored.
		Description:
			- OSMutexPend is used by a task to request ownership of a mutex.
			- If the mutex is available (not owned by another task), the task obtains ownership immediately.
			- If the mutex is already owned by another task, the calling task may wait for it to become available, depending on the specified timeout.
			- If timeout is 0, the task waits indefinitely until the mutex becomes available.
			- If timeout is non-zero, the task waits for the specified time, and if the mutex is not available within that time, it returns with an error.
 *
 */


void task1(void* pdata)
{
    INT8U err;
    while (1)
    {
        INT32U timestamp = OSTimeGet();
        printf("Task1 is running at timestamp: %lu\n", timestamp);
        // Request ownership of the mutex
        OSMutexPend(ledMutex, 0, &err);
        IOWR(LEDS_BASE, 0, 0x1);  // Turn on LED 1
        OSTimeDlyHMSM(0, 0, 3, 0);
        IOWR(LEDS_BASE, 0, 0x0);  // Turn off LED 1


        OSTimeDlyHMSM(0, 0, 3, 0);  // Sleep for 3 seconds before repeating
        // Release ownership of the mutex
        OSMutexPost(ledMutex);
    }
}

void task2(void* pdata)
{
    INT8U err;
    while (1)
    {
        INT32U timestamp = OSTimeGet();
        printf("Task2 is running at timestamp: %lu\n", timestamp);

        OSMutexPend(ledMutex, 0, &err);
        IOWR(LEDS_BASE, 0, 0x2);  // Turn on LED 2
        OSTimeDlyHMSM(0, 0, 3, 0);
        IOWR(LEDS_BASE, 0, 0x0);  // Turn off LED 2


        OSTimeDlyHMSM(0, 0, 3, 0);  // Sleep for 3 seconds before repeating
        OSMutexPost(ledMutex);
    }
}



/**********************
 **********************
 **** KEY ISR CODE ****
 **********************
 **********************/

static void pio_init() {
	// Initialize interrupt function
	void* edge_capture_ptr = (void*)&(edge_capture); // get edge capture pointer ready for use
	// Turn edge capture pointer in a void pointer (this is what the alt_ic_isr_register() takes in)
	// Key0 is system reset so only mask out Keys 3-1
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0X7); // Bit masks for Key 3,2,1
	// Clear pushbutton key's edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0X00); // clear edge capture register
	// register interrupt handler with values for keys, edge capture pointer, etc.
	alt_ic_isr_register(KEYS_IRQ_INTERRUPT_CONTROLLER_ID, KEYS_IRQ, handle_key_interrupts, edge_capture_ptr, 0);
}

static void handle_key_interrupts(void* context) {
	// Interrupt Handler
	// Cast context to edge_capture's type
	// Make it volatile so compiler doesn't try to optimize it
	volatile int* edge_capture_ptr = (volatile int*) context;
	// Read the edge capture register on the button PIO and store the value
	*edge_capture_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);
	switch (*edge_capture_ptr) {
	case 0x1:
		key1_isr();
		break;
	case 0x2:
		key2_isr();
		break;
	case 0x4:
		key3_isr();
		break;
	default:
		printf("UNKNOWN INTERRUPT..., received: %d\n", *edge_capture_ptr);
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE,0x00);
		break;
	}
	return;
}


static void key1_isr() {
	// Interrupt Service Routine - Key 1


	printf("\nKEY 1 ISR RAN!\n");

	// Write to the edge capture register to reset it
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE,0x00);
	// Read the PIO to delay the ISR's exit
	IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);


	return;
}

static void key2_isr() {
	// Interrupt Service Routine - Key 2


	printf("\nKEY 2 ISR RAN!\n");

	// Write to the edge capture register to reset it
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE,0x00);
	// Read the PIO to delay the ISR's exit
	IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);
	return;
}

static void key3_isr() {
	// Interrupt Service Routine - Key 3


	printf("\nKEY 3 ISR RAN!\n");

	// Write to the edge capture register to reset it
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE,0x00);
	// Read the PIO to delay the ISR's exit
	IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);
	return;
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{
  INT8U err;
  IOWR(LEDS_BASE, 0, 0);
  printf("Main init\n");
  ledMutex = OSMutexCreate(0, &err);  // Create a mutex for controlling access to the LED resource
  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
              
               
  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
