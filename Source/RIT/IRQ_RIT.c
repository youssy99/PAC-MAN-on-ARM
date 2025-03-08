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
//#include "../adc/adc.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h" 
#include "adc/adc.h"
#include <string.h>
#include <stdio.h> /*for sprintf*/
#include <math.h>
volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2	=	0;
volatile int areaPausa[5][7];
int direzione=-1,prossimaMossa=-1,statoPausa=0,momentoTimer;
#define UPTICKS 1
NOTE gameOverSong[]={
		{c3, time_semicroma},
    {g2, time_semicroma},
    {e2, time_semicroma},
    {pause,     time_semicroma},
    {c2, time_semicroma},
    {c3, time_semicroma},
    {g2, time_semicroma},
    {e2, time_semicroma},
    {pause,     time_semicroma},
    {c2, time_semicroma},
    {pause,     time_semicroma},
};
int firstTime=0;
void cambiaStato(int tipo,uint16_t colore){
	int i,j;
	static int canzoneGameOver=0,canzoneVittoria=0,ticks=0;
	if(firstTime==0){
		for(i=0;i<50;i++){
		for(j=0;j<70;j++){
			LCD_SetPoint(j+90,i+140,Black);
		}
	 }
	}
	if(tipo==-1){
		if(firstTime==0){
			GUI_Text(105,145,(uint8_t *)"Game",colore,Black);
			GUI_Text(105,160,(uint8_t *)"Over!",colore,Black);
			firstTime=1;
		}
		if(canzoneGameOver<11){
				if(!isNotePlaying()){
					++ticks;
					if(ticks == UPTICKS){
						ticks = 0;
						playNote(gameOverSong[canzoneGameOver++]);
					}
				}
			}else{
				disable_timer(0);
			}
	}else if(tipo==1){
		if(firstTime==0){
			GUI_Text(95,155,(uint8_t *)"Victory!",colore,Black);
			firstTime=1;
		}
		if(canzoneVittoria<35){
				if(!isNotePlaying()){
					++ticks;
					if(ticks == UPTICKS){
						ticks = 0;
						playNote(song[canzoneVittoria++]);
					}
				}
			}else{
				disable_timer(0);
			}
	}else{
		GUI_Text(105,155,(uint8_t *)"Pause",colore,Black);
	}
	return;
}

void recuperaStato(){
	int i,j;
	if(xGhost!=16 && yGhost!=12){
		matrice[16][12]=0;
		matrice[1][12]=0;
	}
	for(i=0;i<5;i++){                            
		for(j=0;j<7;j++){
			if(matrice[i+14][j+9]==4){
				drawPacMan(i+14,j+9);
			}else{
				backgroundSetup(i+14,j+9,matrice[i+14][j+9]);
			}
		}
	}
	return;
}

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{		
	static int pausa=0;
	/*GESTIONE ADC*/
	//ADC_start_conversion();
	
	static int j_select=0, j_left=0, j_right=0, j_down=0, j_up=0;
	static int pressioni=0;
	volatile int i,j;
	init_timer(0, 0x002DC6C0); 						/* 12ms * 25MHz = 1.25*10^6 = 0x1312D0  velocità di pacman*/
	//init_timer(0,0x004C4B40);
	//init_timer(0, 0x001312D0); 						/* 12ms * 25MHz = 1.25*10^6 = 0x1312D0  velocità di pacman*/
	enable_timer(0);
	/*Gestione Joystick Polling*/
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){ //joystick select
		j_select++;
		switch(j_select){
			case 1:
				break;
			//per controllare pressioni prolungate: es. 5[sec]/50[msec](RIT timer) = 100
			//case 100:
			//	break;
			default:
				break;
		}
	}else
		j_select=0;
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){ //joystick down
		j_down++;
		switch(j_down){
			case 1:
				prossimaMossa=3;
				break;			
			default:
				break;
		}
	}else
		j_down=0;
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){ //joystick left
		j_left++;
		switch(j_left){
			case 1:
				prossimaMossa=0;
				break;			
			default:
				break;
		}
	}else
		j_left=0;
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){ //joystick right
		j_right++;
		switch(j_right){
			case 1:
			prossimaMossa=1;
				break;			
			default:
				break;
		}
	}else
		j_right=0;
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){ //joystick up
		j_up++;
		switch(j_up){
			case 1:
				prossimaMossa=2;
				break;			
			default:
				break;
		}
	}else
		j_up=0;
	
	/*Gestione BUTTONS*/
	if(down_0!=0){  
			down_0 ++;  
			//controllo il piedino fisico 10->INT0, 11->KEY1, 12->KEY2
			if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
				switch(down_0){
				case 2:
					pressioni++;
					if(pressioni==2){
						statoPausa=0;
						pressioni=0;
					}else{
						statoPausa=1;
					}
					 if(pausa==0){
						pausa=1;
						 cambiaStato(2,White);
						 disable_timer(1);
						 }else if(pausa==1){
								pausa=0;
								recuperaStato();
								enable_timer(1);
							}
					//codice da eseguire in caso di pressione del button
					break;
				//per controllare pressioni prolungate: es. 5[sec]/50[msec](RIT timer) = 100
				//case 100:
				//	break;
				default:
					//se voglio gestire il button on X click
					//if(down_0 % X == 0){
					//}					
					break;
			}
		}
		else {	/* button released */
			down_0=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
