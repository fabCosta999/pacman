/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "button/button.h"
#include "Game/Game.h"



#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif




																
int main(void)
{
	/* inits */
	SystemInit();  												
	BUTTON_init();
	joystick_init();											
	LCD_Initialization();
	
	#ifndef SIMULATOR
	CAN_Init();
	#endif
	
	/*  at the start timer 0 is used to
			obtain the seed for generating random numbers
			later in the application it will be used for other purposes
			and will cause interrupts so in the init function MCR is set to 3
			but now it is needed just to count up
			so that when the user starts the game the initial seed can be 
			taken from the count and be different every time.
			Therefore I manually change che MCR value to 2.*/
	init_timer(0,0xFFFFFFFF);	
	LPC_TIM0->MCR = 2;
	enable_timer(0);
	
	setup_map();
	init_RIT(0x004C4B40);									// 50 ms
	enable_RIT();			

	#ifndef SIMULATOR
	ADC_init();
	#endif
	
	LPC_SC->PCON |= 0x1;									// power-down mode
	LPC_SC->PCON &= ~(0x2);		
	
	/* for music */
	#ifndef SIMULATOR
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	#endif
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
