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
#include <io.h>

#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "sys/alt_stdio.h"
#include "alt_types.h"


#include "altera_up_avalon_character_lcd_regs.h"
#include "stdlib.h"
#include <time.h>
#include <unistd.h>
#include "string.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE/2];
OS_STK    task2_stk[TASK_STACKSIZE/2];
OS_STK    bulletTsk_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define BULLETTASK_PRIORITY	3


volatile int edge_capture; // For KEYS
volatile int time_capture;
volatile char jtag_capture;


void handle_key_interrupts(void*);
void key1_isr();
void key2_isr();
void key3_isr();


int out_LED = 0;
int mask_LED = 0x20000;
int push_val;
int currTime = 0;
int offset = 0;
int time_sec = 0;
int sample = 0;


/***********************************************************
				      Text Strings
***********************************************************/
char text_hello[] 	 = "hello";

/***********************************************************
						 Flags
***********************************************************/
int flag = 0;
int flag_timer = 0;
/***********************************************************
						 Timer Variables
***********************************************************/
alt_u32 clock_period;
alt_u32 clock_period_l = 0xb280;
alt_u32 clock_period_h = 0x0EE6;
int second = 2000;
time_t t;


/***********************************************************
						 Hex display values
***********************************************************/

int HEX_DISPLAY [] ={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
					 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
struct Alpha
{
	int value_hex;
	char value_alpha;
};


int Pattern1 = 0x2aaaa;
int Pattern2 [] = {0x0, 0x30003, 0x3c00f, 0x3f03f, 0x3fcff, 0x3ffff};
int pattern3 = 0x30000;
int customPattern [5] = {};

struct Alpha sevseg_list[26] =
{
		{0x88, 'a'},
		{0x83, 'b'},
		{0xA7, 'c'},
		{0xA1, 'd'},
		{0x86, 'e'},
		{0x8E, 'f'},
		{0x90, 'g'},
		{0x89, 'h'},
		{0xCF, 'i'},
		{0xF1, 'j'},
		{0x8A, 'k'},
		{0xC7, 'l'},
		{0x00, 'm'},
		{0xC8, 'n'},
		{0xC0, 'o'},
		{0x8C, 'p'},
		{0x98, 'q'},
		{0xAF, 'r'},
		{0x92, 's'},
		{0x87, 't'},
		{0xC1, 'u'},
		{0xC1, 'v'},
		{0x00, 'w'},
		{0x00, 'x'},
		{0x91, 'y'},
		{0xA4, 'z'}
	};


OS_EVENT* ledMutex;
OS_EVENT* lcdMutex;

void display_init();
static void timer_init(float n);

void printword(char *word);





char character = '$';

int customChar[8] = {
	0b00100,
	0b01110,
	0b00100,
	0b00100,
	0b00100,
	0b01110,
	0b10101,
	0b01010
};

int player_still[] = {
  0b00111,
  0b00111,
  0b11010,
  0b01110,
  0b00010,
  0b00010,
  0b00010,
  0b00110
};

int player_walk[] = {
  0b00111,
  0b00111,
  0b11010,
  0b01110,
  0b00010,
  0b00110,
  0b01001,
  0b11011
};

int zombie_still[] = {
  0b11100,
  0b11100,
  0b01000,
  0b01110,
  0b01010,
  0b01000,
  0b01000,
  0b01100
};

int zombie_walk[] = {
  0b11100,
  0b11100,
  0b01000,
  0b01110,
  0b01010,
  0b01000,
  0b10100,
  0b11110
};

int bullet_move[] = {
		  0b00000,
		  0b00100,
		  0b00000,
		  0b00000,
		  0b00000,
		  0b00000,
		  0b00000,
		  0b00000
		};


int tombstone[] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b01110
};

/*
int bullet[] = {
  B00000,
  B00000,
  B10000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
*/


// Global variables for bullet position and status
int bulletPosition = -1;  // Initialize to an invalid position
int bulletActive = 0;     // Flag to indicate if a bullet is currently active
int playerPosition = 0;  // Initial position of the player character

char bullet = '-';

void shootBullet() {
	INT8U err;
    // Check if a bullet is already active
    if (bulletActive) {
        return;  // Skip shooting if a bullet is still active
    }

    // Set the bullet position to the player's position
    bulletPosition = playerPosition;
    bulletActive = 1;  // Set the flag to indicate an active bullet
    OSMutexPend(lcdMutex, 0, &err);
    // Display the bullet at the initial position
    IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x80 + bulletPosition);
    IOWR_8DIRECT(LCD_BASE, ALT_UP_CHARACTER_LCD_DATA_REG, bullet);

    // Introduce a delay (you may need to adjust the duration)
    //usleep(100000);  // Sleep for 100ms (adjust as needed)
    OSTimeDlyHMSM(0, 0, 0, 100);
    OSMutexPost(lcdMutex);
}

void moveBullet(void* pdata) {
	INT8U err;
    // Check if a bullet is active
    if (bulletActive) {
        while (bulletPosition >= 0) {
            // Clear the bullet at the current position

            // Update the bullet position
            bulletPosition--;
            OSMutexPend(lcdMutex, 0, &err);
            // Display the bullet at the new position
            IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, ALT_UP_CHARACTER_LCD_COMM_CLEAR_DISPLAY);
            IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x80 + playerPosition);
            IOWR_8DIRECT(LCD_BASE, ALT_UP_CHARACTER_LCD_DATA_REG, 0x1);
            IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x80 + bulletPosition);
            IOWR_8DIRECT(LCD_BASE, ALT_UP_CHARACTER_LCD_DATA_REG, bullet);

            // Introduce a delay (you may need to adjust the duration)
            //usleep(100000);  // Sleep for 100ms (adjust as needed)
            OSTimeDlyHMSM(0, 0, 0, 100);
            OSMutexPost(lcdMutex);
        }

        bulletActive = 0;  // Deactivate the bullet once it goes off the display
    }
}
void movePlayer(int direction) {
    // Direction: 0 for left, 1 for right
	INT8U err;
    OSMutexPend(lcdMutex, 0, &err);
    // Clear the entire display
    IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, ALT_UP_CHARACTER_LCD_COMM_CLEAR_DISPLAY);

    // Update the player position based on the direction
    if (direction == 0 && playerPosition > 0) {
        playerPosition--;
    } else if (direction == 1 && playerPosition < 15) {
        playerPosition++;
    }

    // Display the player character at the new position

    IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x80 + playerPosition);
    IOWR_8DIRECT(LCD_BASE, ALT_UP_CHARACTER_LCD_DATA_REG,0x1);
    OSTimeDlyHMSM(0, 0, 0, 100);
    IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, ALT_UP_CHARACTER_LCD_COMM_CLEAR_DISPLAY);
    IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x80 + playerPosition);
    IOWR_8DIRECT(LCD_BASE, ALT_UP_CHARACTER_LCD_DATA_REG,0x0);
    OSMutexPost(lcdMutex);

}

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
        OSTimeDlyHMSM(0, 0, 1, 0);
        IOWR(LEDS_BASE, 0, 0x0);  // Turn off LED 1


        OSTimeDlyHMSM(0, 0, 1, 0);  // Sleep for 3 seconds before repeating
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
        OSTimeDlyHMSM(0, 0, 1, 0);
        IOWR(LEDS_BASE, 0, 0x0);  // Turn off LED 2


        OSTimeDlyHMSM(0, 0, 1, 0);  // Sleep for 3 seconds before repeating
        OSMutexPost(ledMutex);
    }
}

void pio_init() {
	void* edge_capture_ptr = (void*)&(edge_capture);

	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0x7);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x00);

	alt_ic_isr_register(KEYS_IRQ_INTERRUPT_CONTROLLER_ID, KEYS_IRQ, handle_key_interrupts, edge_capture_ptr, 0);

}

void handle_key_interrupts(void* context) {
	// Call RTOS Interrupt Handling function
	OSIntEnter();
	volatile int* edge_capture_ptr = (volatile int*) context;

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
		printf("UNKNOWN INTERRUPT! Received: %d\n", *edge_capture_ptr);
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x00);
		break;
	}
	OSIntExit();
	//return;
}

void key1_isr() {
	//printf("\nISR - KEY 1 PRESSED\n");
	shootBullet();
	// Reset edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x00);
	// Read the PIO to delay the ISR's exit
	IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);

	return;
}

void key2_isr() {
	//printf("\nISR - KEY 2 PRESSED\n");
	movePlayer(1);
	// Reset edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x00);
	// Read the PIO to delay the ISR's exit
	IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);

	return;
}

void key3_isr() {
	//printf("\nISR - KEY 3 PRESSED\n");
	movePlayer(0);
	// Reset edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x00);
	// Read the PIO to delay the ISR's exit
	IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);

	return;
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{
  INT8U err;

  char hello[] = "Hello" ;
  display_init();
  //timer_init(second);
  pio_init();

  IOWR(LEDS_BASE, 0, 0);
  printf("Main init\n");

  //IOWR_8DIRECT(LCD_BASE, ALT_UP_CHARACTER_LCD_DATA_REG, character);
  IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x40);
  for (int i = 0; i < 8; ++i) {
	  IOWR_ALT_UP_CHARACTER_LCD_DATA(LCD_BASE, player_still[i]);
  }

  IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x48);
  for (int i = 0; i < 8; ++i) {
	  IOWR_ALT_UP_CHARACTER_LCD_DATA(LCD_BASE, player_walk[i]);
  }

  IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x56);
  for (int i = 0; i < 8; ++i){
	  IOWR_ALT_UP_CHARACTER_LCD_DATA(LCD_BASE, bullet_move[i]);
  }

  IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x64);
  for (int i = 0; i < 8; ++i)
  {
	  IOWR_ALT_UP_CHARACTER_LCD_DATA(LCD_BASE, bullet_move[i]);
  }


  // Set DDRAM address back to the start (cursor position)
  IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, ALT_UP_CHARACTER_LCD_COMM_RETURN_HOME);
  IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x0c);



  ledMutex = OSMutexCreate(0, &err);  // Create a mutex for controlling access to the LED resource
  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE/2,
                  NULL,
                  0);
              
               
  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE/2-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE/2,
                  NULL,
                  0);
  /*
  OSTaskCreateExt(moveBullet,
		  	  	  NULL,
		  	  	  (void *)&bulletTsk_stk[TASK_STACKSIZE-1],
		  	  	  BULLETTASK_PRIORITY,
		  	  	  BULLETTASK_PRIORITY,
		  	  	  bulletTsk_stk,
		  	  	  TASK_STACKSIZE,
		  	  	  NULL,
		  	  	  0);
   */

  OSStart();

  return 0;
}

void timer_isr(void)
{
	OSIntEnter();
	printf("\ntimer isr called!\n");
	switch(flag_timer)
	{
	case 0: // wait state

		flag_timer = 1;
		break;
	case 1:
		//IOWR_8DIRECT(LCD_BASE, ALT_UP_CHARACTER_LCD_DATA_REG,0x1);
		//IOWR_ALT_UP_CHARACTER_LCD_COMMAND(LCD_BASE, 0x10);
		flag_timer = 0;
		break;
	}

	IOWR_ALTERA_AVALON_TIMER_CONTROL(SYSTEM_TIMER_BASE, 0x09); // Stop timer
	IOWR_ALTERA_AVALON_TIMER_STATUS(SYSTEM_TIMER_BASE, 0x0);	// Reset TO bit
	IOWR_ALTERA_AVALON_TIMER_CONTROL(SYSTEM_TIMER_BASE, 0x07); // Stop timer
	OSIntExit();
	//return;
}

static void timer_init(float n)
{ //timer initializer function
	void* time_capture_ptr = (void*) &time_capture;
	clock_period = 0xC350 * n; //calculates period in terms of ms
	clock_period_l = clock_period & 0x0000ffff;
	clock_period = clock_period >> 16;
	clock_period_h = clock_period & 0xffff;
	IOWR_ALTERA_AVALON_TIMER_CONTROL(SYSTEM_TIMER_BASE,0x9);
	IOWR_ALTERA_AVALON_TIMER_PERIODL(SYSTEM_TIMER_BASE,clock_period_l); //0xb280
	IOWR_ALTERA_AVALON_TIMER_PERIODH(SYSTEM_TIMER_BASE,clock_period_h); //0x0EE6
	IOWR_ALTERA_AVALON_TIMER_STATUS(SYSTEM_TIMER_BASE,0x0);
	alt_ic_isr_register(SYSTEM_TIMER_IRQ_INTERRUPT_CONTROLLER_ID, SYSTEM_TIMER_IRQ, timer_isr, time_capture_ptr, 0x0);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(SYSTEM_TIMER_BASE,0x5);
}


void display_init()
{//function to shut off seven segment display
	printf("\ndisplay_init\n");
	INT8U err;
	int hex_off = 0xff;
    OSMutexPend(lcdMutex, 0, &err);
	IOWR_8DIRECT(SEVEN_SEG_0_BASE,0,hex_off);
	IOWR_8DIRECT(SEVEN_SEG_1_BASE,0,hex_off);
	IOWR_8DIRECT(SEVEN_SEG_2_BASE,0,hex_off);
	IOWR_8DIRECT(SEVEN_SEG_3_BASE,0,hex_off);
	IOWR_8DIRECT(SEVEN_SEG_4_BASE,0,hex_off);
	IOWR_8DIRECT(SEVEN_SEG_5_BASE,0,hex_off);
	IOWR_8DIRECT(SEVEN_SEG_6_BASE,0,hex_off);
	IOWR_8DIRECT(SEVEN_SEG_7_BASE,0,hex_off);
    OSMutexPost(lcdMutex);
};

void printword(char string[]){//display text to the seven segment
	display_init();
	int len = strlen(string);
	char temp_1 = '!'; // holds char of string
	char temp_2 = '!'; // finds same char form alpha pattern array
	int hex_out = 0;
	int offset = 0;

	for(int i = 0; i<len;i++)
	{
		temp_1 = string[i]; // gets the current char of the string
		for(int j = 0;j<26;j++)
		{
			temp_2 = sevseg_list[j].value_alpha;
			if (temp_1 == temp_2)
			{
				hex_out = (sevseg_list[j].value_hex);
				break;
			}
		}

		IOWR_8DIRECT(SEVEN_SEG_7_BASE,offset,hex_out);
		offset = offset + 16;
	}

}

