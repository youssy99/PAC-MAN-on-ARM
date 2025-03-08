/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include <stdio.h> /*for sprintf*/
#include "RIT/RIT.h"

#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1
int tempo=105;
int wholeNote=2286;
//SHORTENING UNDERTALE: TOO MANY REPETITIONS
NOTE song[] = 
{
// 1
{c2, time_semicroma},
{c3, time_semicroma},
{g2, time_semicroma},
{e2, time_semicroma},
{c3, time_semicroma},
{g2, time_semicroma},
{e2, time_semicroma},
{pause,time_biscroma},
{db2, time_semicroma},
{db3, time_semicroma},
{ab2, time_semicroma},
{f2, time_semicroma},
{db3, time_semicroma},
{ab2, time_semicroma},
{f2, time_semicroma},
{pause,time_biscroma},
{c2, time_semicroma},
{c3, time_semicroma},
{g2, time_semicroma},
{e2, time_semicroma},
{c3, time_semicroma},
{g2, time_semicroma},
{e2, time_semicroma},
{pause,time_biscroma},
{e2, time_biscroma},
{f2, time_biscroma},
{gb2, time_biscroma},
{gb2, time_biscroma},
{g2, time_biscroma},
{ab2, time_biscroma},
{ab2, time_biscroma},
{a2, time_biscroma},
{bb2, time_biscroma},
{b2, time_biscroma},
{c3, time_biscroma}
};
NOTE sirena[]={{c2,time_semicroma},{pause,time_semicroma},{d2,time_semicroma},{pause,time_semicroma}};
NOTE pausa[]={pause,time_minima};
int direzioneGhost=-1,direzioneTemporanea=-1,contatore=0,mosse=0,blueGhost=1,eated=0,timerFantasma=0,startWaiting=0,blueGhostSpeed=0,blueGhostTime=0;
int sirenIsOver=0,started=0;
static int currentNote = 0;
int gameOver=0,victory=0, NPalline = 240;
uint16_t SinTable[45] =                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1){ //MR0
	static int currentNote = 0;
	static int ticks = 0;
	if(gameOver==0 && victory==0){
		if(direzione==-1){
		if(currentNote<35 && started==1){
		if(!isNotePlaying())
	{
		++ticks;
		if(ticks == UPTICKS)
		{
			ticks = 0;
			playNote(song[currentNote++]);
		}
	}
	}
	}
		if(statoPausa==0){
			if(prossimaMossa!=-1){
			switch (prossimaMossa){
				case 0:
					if(matrice[xPacMan][yPacMan-1]!=-2){
						direzione=prossimaMossa;
						prossimaMossa=-1;
					}
					break;
				case 1:
					if(matrice[xPacMan][yPacMan+1]!=-2){
						direzione=prossimaMossa;
						prossimaMossa=-1;
					}
					break;
				case 2:
					if(matrice[xPacMan-1][yPacMan]!=-2){
						direzione=prossimaMossa;
						prossimaMossa=-1;
					}
					break;
				case 3:
					if(matrice[xPacMan+1][yPacMan]!=-2){
						direzione=prossimaMossa;
						prossimaMossa=-1;
					}
					break;
			}
		}
			switch(direzione){
			case 0: //direzione sinistra
				if(matrice[xPacMan][yPacMan-1]!=-2){
					if(yPacMan==23){
						clearPacMan(xPacMan,0);
					}
					if(matrice[xPacMan][yPacMan-1]==1){
						NPalline--;
					}
					if(matrice[xPacMan][yPacMan-1]!=3){
						clearPacMan(xPacMan,yPacMan);
						if(matrice[xPacMan][yPacMan]!=3){
							matrice[xPacMan][yPacMan]=0;
						}
						if(matrice[xPacMan][yPacMan-1]==5){
							blueGhost=-1;
							eated=-1;
						}
						yPacManPrecedente=yPacMan;
						yPacMan--;
						drawPacMan(xPacMan,yPacMan);
						matrice[xPacMan][yPacMan]=4;
					}else if(matrice[xPacMan][yPacMan-1]==3){
						clearPacMan(xPacMan,yPacMan);
						matrice[xPacMan][yPacMan]=0;
						drawPacMan(xPacMan,yPacMan);
						yPacMan=23;
					}
					if(startWaiting==0){
						mosse++;
					}
				}
				break;
			case 1: //direzione destra
				if(matrice[xPacMan][yPacMan+1]!=-2){
					if(yPacMan==0){
						clearPacMan(xPacMan,23);
					}
					if(matrice[xPacMan][yPacMan+1]==1){
						NPalline--;
					}
					if(matrice[xPacMan][yPacMan+1]!=3){
						clearPacMan(xPacMan,yPacMan);
						if(matrice[xPacMan][yPacMan]!=3){
							matrice[xPacMan][yPacMan]=0;
						}
						if(matrice[xPacMan][yPacMan+1]==5){
							blueGhost=-1;
							eated=-1;
						}
						yPacManPrecedente=yPacMan;
						yPacMan++;
						drawPacMan(xPacMan,yPacMan);
						matrice[xPacMan][yPacMan]=4;
					}else if(matrice[xPacMan][yPacMan+1]==3){
						clearPacMan(xPacMan,yPacMan);
						matrice[xPacMan][yPacMan]=0;
						drawPacMan(xPacMan,yPacMan);
						yPacMan=0;
					}
					if(startWaiting==0){
						mosse++;
					}
				}
				break;
			case 2: //direzione up
				if(matrice[xPacMan-1][yPacMan]!=-2){
						if(matrice[xPacMan-1][yPacMan]==1){
							NPalline--;
						}
						clearPacMan(xPacMan,yPacMan);
						matrice[xPacMan][yPacMan]=0;
						if(matrice[xPacMan-1][yPacMan]==5){
							blueGhost=-1;
							eated=-1;
						}
						xPacManPrecedente=xPacMan;
						xPacMan--;
						drawPacMan(xPacMan,yPacMan);
						matrice[xPacMan][yPacMan]=4;
						if(startWaiting==0){
							mosse++;
						}	
				}
				break;
			case 3: //direzione down
				if(matrice[xPacMan+1][yPacMan]!=-2){
					if(matrice[xPacMan+1][yPacMan]==1){
						NPalline--;
					}
					clearPacMan(xPacMan,yPacMan);
					matrice[xPacMan][yPacMan]=0;
					if(matrice[xPacMan+1][yPacMan]==5){
							blueGhost=-1;
							eated=-1;
					}
					xPacManPrecedente=xPacMan;
					xPacMan++;
					drawPacMan(xPacMan,yPacMan);
					matrice[xPacMan][yPacMan]=4;
					if(startWaiting==0){
						mosse++;
					}
				}
				break;
		}
		if(direzione!=-1){
			if(mosse<3){
			if(contatore<2 && startWaiting==0){
				matrice[xGhost-1][yGhost]=0;
				clearPacMan(xGhost,yGhost);
				xGhostPrecedente=xGhost;
				xGhost--;
				drawGhost(1);
				if(contatore==0){
					matrice[xGhost][yGhost]=-2;
				}
				contatore++;
			}
		}else{
			if(blueGhost==-1){
				if(eated==-1){
					blueGhostTime=1;
					//enable_timer(2);
					blueGhostSpeed=0;
					eated=0;
				}
					if(blueGhostSpeed==1){
						frightenedMode(blueGhost);
						blueGhostSpeed=0;
					}
			}else{
				if(startWaiting==0){
					calcolaPercorso(xGhost,yGhost,xPacMan,yPacMan,blueGhost);
				}
				
			}
		}
	}
			
			if(score==2640-ghostEaten*100){
				disable_timer(1);
				//disable_timer(0);
				disable_RIT();
				victory=1;
			}
		}
		
	}else if(gameOver==1){
		cambiaStato(-1,Red);
	}else if(victory==1){
		cambiaStato(1,Yellow);
	}
	LPC_TIM0->IR = 1;	
}
	/*else if(LPC_TIM0 -> IR & 2){
	if(isNotePlaying()){
		stopNote();
	}
	LPC_TIM0->IR = 2;			// clear interrupt flag 
	}*/
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	char tempo[255];
	static int contatore=60,i,j,ghostTime=0;
	volatile int numero,k,found,contatorePalline=0;
	if(LPC_TIM1->IR & 1){ //MR0
		if(contatore>0){
			if(contatore==momentoTimer){
				srand(xPacMan*yPacMan);
				for(k=0;k<6;k++){
				found=-1;
				numero=rand()%(NPalline+1);
				contatorePalline=0;
				for(i=0;i<Nrighe && found==-1;i++){
				for(j=0;j<Ncolonne && found==-1;j++){
					if(matrice[i][j]==1){
						contatorePalline++;
						if(contatorePalline==numero){
							found=1;
							backgroundSetup(i,j,5);
							matrice[i][j]=5;
						}
					}
				}
		}
	}
			}
			contatore--;
			if(started==0){
				started=1;
			}
			sprintf(tempo,"%d",contatore);
			if(contatore==9){
				for(i=0;i<16;i++){
					for(j=0;j<8;j++){
						LCD_SetPoint(j+218,i+10,Black);
					}
				}
			}
			GUI_Text(210,10,(uint8_t *)tempo,White,Black);
		}else{
			//disable_timer(0);
			disable_timer(1);
			disable_RIT();
			gameOver=1;
			//reset_timer(0);
		}
		if(startWaiting==1){
		timerFantasma++;
		if(timerFantasma==3){
				drawGhost(1);
				timerFantasma=0;
				startWaiting=0;
		}
		}
		
		blueGhostSpeed++;
		if(blueGhostTime==1){
			ghostTime++;
			if(ghostTime==10){
				blueGhostTime=0;
				ghostTime=0;
				blueGhost=1;
			}
		}	
	}
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1){
		static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= 1;
	currentValue += 410;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;
	}
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}
/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER3_IRQHandler (void)
{
	if(LPC_TIM3->IR & 1){
	disable_timer(2);
}
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/