#include "music.h"
#include "RIT/RIT.h"

void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(2);
		init_timer(2, note.freq);
		enable_timer(2);
	}
	reset_timer(3);
	//con emulatore
	//init_timer(3, note.duration/3);
	//con scheda reale
	init_timer(3, note.duration);
	enable_timer(3);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM2->TCR != 0) || (LPC_TIM3->TCR != 0));
}

void stopNote(){
	disable_timer(2);
	disable_timer(3);
}
