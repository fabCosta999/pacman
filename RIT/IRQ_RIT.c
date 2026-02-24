/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h" 
#include "../timer/timer.h"
#include <stdlib.h>
#include <stdio.h>

// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern uint8_t state;
uint8_t int0Pressed	= 0;		
Direction direction = UP;




//SHORTENING UNDERTALE: TOO MANY REPETITIONS
NOTE song[] = 
{
	// 1
	{d3, time_semicroma},
	{d3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 2
	{c3, time_semicroma},
	{c3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 3
	{c3b, time_semicroma},
	{c3b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 4
	{a2b, time_semicroma},
	{a2b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 5
	
};



void RIT_IRQHandler (void){		
	#ifndef SIMULATOR
	static int currentNote = 0;
	static int ticks = 0;
	#endif
	/* joystick management */
	// debouncing isn't necesearry for the joystick
	if (state){ // can't update the direction if the game is paused
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0) direction = DOWN;
		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0) direction = LEFT;
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0) direction = RIGHT;
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0) direction = UP;
	}
	
	/* button management */
	if(int0Pressed>=1){ 
		switch(int0Pressed){		
				#ifdef SIMULATOR
				case 1: pause_game(); break;
				#endif
				#ifndef SIMULATOR				
				case 2: pause_game(); break;
				#endif
				default: break;
			}
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0)	/* INT0 pressed */
			++int0Pressed;
		else{ 	/* button released */
			int0Pressed=0;		
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */			
		}
	}
	
	
	#ifndef SIMULATOR
	/* music */
	if (state){
		if(!isNotePlaying())
		{
			++ticks;
			if(ticks == UPTICKS)
			{
				ticks = 0;
				playNote(song[currentNote++]);
			}
		}
	
		if(currentNote == (sizeof(song) / sizeof(song[0])))
			currentNote = 0;
	}
	#endif
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}





/******************************************************************************
**                            End Of File
******************************************************************************/
