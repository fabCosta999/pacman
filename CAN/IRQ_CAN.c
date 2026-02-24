/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"
#include "../Game/Game.h"
#include <stdio.h>
extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */                                



void can_write_score(int score){	
	char msg[5];
	sprintf(msg, "%4d", score);
	GUI_Text(PADDINGL + WIDTH / 2 - 15, PADDINGT/3, (uint8_t*) msg, White, Black);
}

void can_write_lives(int lives){	
	char msg[8];
	sprintf(msg, "vite: %d", lives);
	GUI_Text(PADDINGL, PADDINGT/3, (uint8_t*) msg, White, Black);
}

void can_write_time(int time){	
	char msg[3];
	sprintf(msg, "%2d", time);
	GUI_Text(WIDTH - PADDINGL, PADDINGT/3, (uint8_t*) msg, White, Black);
}


/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {
	uint32_t message;
	
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

	 if (icr & (1 << 0)) {                          	/* CAN Controller #2 meassage is received */
		CAN_rdMsg (2, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN2->CMR = (1 << 2);                    		/* Release receive buffer */
		
		message = ((uint32_t) CAN_RxMsg.data[0]) << 24;
		message |= ((uint32_t) CAN_RxMsg.data[1]) << 16;
		message |= ((uint32_t) CAN_RxMsg.data[2]) << 8;
		message |= ((uint32_t) CAN_RxMsg.data[3]);
		
		can_write_time((message & 0xFF000000) >> 24);
		can_write_lives((message & 0x00FF0000) >> 16);
		can_write_score(message & 0x0000FFFF);
	}
	 
	/* check CAN controller 1 */
	icr = 0;
	icr = (LPC_CAN1->ICR | icr) & 0xFF;             /* clear interrupts */
}
