#include "music.h"

#include "../timer/timer.h"


extern unsigned char state;

void playNote(NOTE note)
{
	if (state){
		if(note.freq != pause)
		{
			reset_timer(1);
			init_timer(1, note.freq);
			enable_timer(1);
		}
	}
	reset_timer(3);
	init_timer(3, note.duration);
	enable_timer(3);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM1->TCR != 0) || (LPC_TIM3->TCR != 0));
}
