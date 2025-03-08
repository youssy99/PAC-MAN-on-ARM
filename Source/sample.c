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
#include "button_EXINT/button.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "adc/adc.h"
#include "led/led.h"
#include "CAN/CAN.h"
#include <stdio.h>
#include <time.h>
#include <string.h>


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
int score=0;
char value[255];
char scoreStr[]="Score:";
int changeDirection=0;
int xPacMan,yPacMan,vite=1;
int xPacManPrecedente=-1,yPacManPrecedente=-1,xGhostPrecedente=16,yGhostPrecedente=12;
int xGhost,yGhost;
int ghostEaten=0;
int matrice[Nrighe][Ncolonne]= {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //Inizio
																	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //Inizio
																	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //Inizio
																	{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2}, //Riga 1
																	{-2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -2},	//Riga 2
																	{-2, 1, -2, -2, -2, -2, -2, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, -2, 1, -2, -2, -2, 1, -2}, //Riga 3
																	{-2, 1, -2, -2, -2, -2, -2, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, -2, 1, -2, -2, -2, 1, -2},	//Riga 4
																	{-2, 1, -2, -2, -2, -2, -2, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, -2, 1, 1, 1, 1, 1, -2}, //Riga 5
																	{-2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -2, -2, -2, -2, 1, -2, -2, -2, -2, -2, -2, 1, -2}, //Riga 6
																	{-2, 1, -2, 1, -2, -2, -2, -2, -2, -2, 1, 1, 1, 1, 1, 1, -2, -2, -2, -2, -2, -2, 1, -2}, //Riga 7
																	{-2, 1, -2, 1, -2, -2, -2, -2, -2, -2, 1, -2, -2, -2, -2, 1, 1, 1, 1, 1, 1, 1, 1, -2},	//Riga 8
																	{-2, 1, -2, 1, -2, -2, -2, -2, -2, -2, 1, -2, -2, -2, -2, -2, -2, 1, -2, 1, -2, -2, 1, -2},	//Riga 9
																	{-2, 1, -2, 1, 1, 1, 1, 1, 1, 1, 1, -2, -2, -2, -2, -2, -2, 1, -2, 1, -2, -2, 1, -2},	//Riga 10
																	{-2, 1, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, -2, -2, -2, -2, -2, 1, -2, 1, -2, -2, 1, -2}, //Riga 11
																	{-2, 1, -2, -2, -2, 1, -2, -2, -2, 0, 0, 0, 0, 0, 0, 0, -2, 1, -2, 1, 1, 1, 1, -2},	//Riga 12
																	{-2, 1, -2, -2, -2, 1, -2, -2, -2, 0, -2, -2, -3, -2, -2, 0, -2, 1, -2, -2, -2, -2, 1, -2},	//Riga 13
																	{3, 1, 1, 1, 1, 1, 1, 1, 1, 0, -2, 0, 7, 0, -2, 0, 1, 1, -2, -2, -2, -2, 1, 3},	//Riga 14
																	{-2, 1, -2, -2, -2, -2, -2, -2, -2, 0, -2, -2, -2, -2, -2, 0, -2, -2, -2, -2, -2, -2, 1, -2},	//Riga 15
																	{-2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 4, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, -2}, //Riga 16
																	{-2, 1, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, -2, -2, 1, -2, -2, 1, -2, -2, 1, -2, 1, -2}, //Riga 17
																	{-2, 1, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, -2, -2, 1, -2, -2, 1, -2, -2, 1, -2, 1, -2},	//Riga 18
																	{-2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -2, -2, 1, -2, -2, 1, -2, 1, -2}, //Riga 19
																	{-2, 1, -2, -2, -2, -2, -2, -2, -2, -2, 1, -2, 1, -2, -2, -2, -2, 1, -2, -2, 1, -2, 1, -2}, //Riga 20
																	{-2, 1, -2, -2, -2, -2, -2, -2, -2, -2, 1, -2, 1, 1, 1, 1, 1, 1, 1, 1, 1, -2, 1, -2},	//Riga 21
																	{-2, 1, 1, 1, 1, 1, -2, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, 1, -2, -2, 1, -2, 1, -2}, //Riga 22
																	{-2, 1, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, 1, 1, 1, 1, -2, 1, -2}, //Riga 23
																	{-2, 1, -2, -2, -2, 1, -2, -2, -2, -2, 1, 1, 1, 1, 1, 1, -2, -2, -2, -2, 1, -2, 1, -2}, //Riga 24
																	{-2, 1, -2, -2, -2, 1, -2, -2, -2, -2, 1, -2, -2, -2, -2, -2, -2, -2, -2, -2, 1, -2, 1, -2}, //Riga 25
																	{-2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -2}, //Riga 26
																	{-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2},	//Riga 27
																	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},	//Fine
																	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}	//Fine
    };

void drawHeart(int x, int y){
	int i,j,nx, ny,rigSpecial,colSpecial,partenza;
	uint16_t colore=Red;
	x=x*scalamento+1+5;
	y=y*scalamento+1;
	nx=x+scalamento;
	ny=y+scalamento-1;
	//Parte superiore
	rigSpecial=x+1;
	colSpecial=ny-1;
	partenza=y;
	i=x;
		for(j=partenza;j<colSpecial;j++){
			if(j!=y+3 && j!=y+4){
				LCD_SetPoint(j,i,colore);
			}
		}
		partenza-=1;
		
		for(i=x+1;i<nx-4;i++){
			for(j=partenza;j<ny;j++){
				LCD_SetPoint(j,i,colore);
			}
		}
		for(i=nx-4;i<nx;i++){
			for(j=partenza+1;j<ny-1;j++){
				LCD_SetPoint(j,i,colore);
			}
			partenza=partenza+1;
			ny=ny-1;
		}
		
	return;
	}



void clearPacMan(int x,int y){
	int i,j,nx,ny;
	x=x*scalamento;
	y=y*scalamento;
	nx=x+scalamento;
	ny=y+scalamento;
	for(i=x;i<nx;i++){
		for(j=y;j<ny;j++){
			LCD_SetPoint(j,i,Black);
		}
	}
}

int eaten(){
	int happen=-1;
	if((xGhost==xPacMan && yGhost==yPacMan) || (xGhost==xPacManPrecedente && yGhost==yPacManPrecedente )){
	if(blueGhost==-1){
		startWaiting=1;
		score+=100;
		ghostEaten++;
		clearPacMan(xGhost,yGhost);
		xGhost=16;	//16
		yGhost=12;	//12
		timerFantasma=0;
		mosse=0;
		contatore=0;
		blueGhost=1;
	}else{
		if(vite==3){
		clearPacMan(30,5);
		clearPacMan(31,5);
		clearPacMan(xGhost,yGhost);
			xGhost=16;	//16
			yGhost=12;	//12
			xPacMan=18;
			yPacMan=12;
			mosse=0;
			direzione=-1;
			contatore=0;
	}else if(vite==2){
		clearPacMan(30,3);
		clearPacMan(31,3);
		clearPacMan(xGhost,yGhost);
			xGhost=16;	//16
			yGhost=12;	//12
			xPacMan=18;
			yPacMan=12;
			mosse=0;
			direzione=-1;
			contatore=0;
	}else{
		clearPacMan(30,1);
		clearPacMan(31,1);
		clearPacMan(xGhost,yGhost);
		//disable_timer(0);
		disable_timer(1);
		//disable_timer(2);
		disable_RIT();
		//reset_timer(0);
		gameOver=1;
		cambiaStato(-1,Red);
	}
	vite--;
	happen=1;
	}
	}
	return happen;
}

void drawPacMan(int x, int y){
	static int ticks=0,sirenaPlayed=0,firstTime=0;
	int check=eaten();
	if(matrice[x][y]==1){
	if(!isNotePlaying()){
		++ticks;
		if(ticks == 1)
		{
			ticks = 0;
			playNote(sirena[sirenaPlayed]);
			sirenaPlayed++;
			if(sirenaPlayed>=2){
				sirenaPlayed=0;
			}
		}
	}
	}else if(matrice[x][y]==5){
		sirenaPlayed=2;
		if(!isNotePlaying()){
		++ticks;
		if(ticks == 1)
		{
			ticks = 0;
			playNote(sirena[sirenaPlayed]);
			sirenaPlayed++;
			if(sirenaPlayed>=4){
				sirenaPlayed=0;
			}
		}
	}
	}
	if(check==1){
		x=xPacMan;
		y=yPacMan;
	}
		if(vite>0){
		if(score>=1000 && score<2000){
		if(firstTime==0){
			drawHeart(30,3);
			vite=2;
			firstTime++;
		}
	}else if(score>=2000){
		if(firstTime==1){
			drawHeart(30,5);
			vite=3;
		}
	}
	int i,j,nx, ny,rigSpecial,colSpecial,partenza;
	if(matrice[x][y]==1 || matrice[x][y]==5){
		score+=matrice[x][y]*10;
	};
	uint16_t colore=Yellow;
	x=x*scalamento+1;
	y=y*scalamento+1;
	nx=x+scalamento-1;
	ny=y+scalamento-1;
	//Parte superiore
	rigSpecial=x+2;
	colSpecial=ny-2;
	partenza=y+2;
	for(i=x;i<rigSpecial;i++){
		if(i==x+1){
			colSpecial++;
		}
		for(j=partenza;j<colSpecial;j++){
			LCD_SetPoint(j,i,colore);
		}
		partenza--;
	}
	//Parte centrale
	for(i=x+2;i<nx-2;i++){
		for(j=y;j<ny;j++){
			LCD_SetPoint(j,i,colore);
		}
	}
	//Parte inferiore
	colSpecial=ny-1;
	partenza=y+1;
	for(i=nx-2;i<nx;i++){
		if(i==nx-1){
			colSpecial=colSpecial-1;
		}
		for(j=partenza;j<colSpecial;j++){
			LCD_SetPoint(j,i,colore);
		}
		partenza++;
	}
	sprintf(value,"%d",score);
	GUI_Text(60,10,(uint8_t *)value,White,Black);
	if(check==1){
		drawGhost(1);
	}
	}
	return;
}


void drawGhost(int valore){
	int check=eaten();
	if(vite>0){
	int i,j,x,y,nx, ny,rigSpecial,colSpecial,partenza;
	x=xGhost*scalamento;
	y=yGhost*scalamento;
	uint16_t colore;
	if(valore==-1){
		colore=Cyan;
	}else{
		colore=Red;
	}
	nx=x+scalamento;
	ny=y+scalamento;
	//Parte superiore
	rigSpecial=x+3;
	colSpecial=ny-4;
	partenza=y+3;
	for(i=x;i<rigSpecial;i++){
		if(i<x+3){
			colSpecial++;
		}
		for(j=partenza;j<colSpecial;j++){
			LCD_SetPoint(j,i,colore);
		}
		partenza--;
	}
	//Parte centrale
	for(i=x+3;i<nx-1;i++){
		for(j=y;j<ny;j++){
			if(i==x+3 || i==x+4){
				if(j==y+2 || j==y+3 || j==y+6 || j==y+7){
					if((i==x+3 && j==y+3) || (i==x+3 && j==y+7) ){
						LCD_SetPoint(j,i,Black);
					}else{
						LCD_SetPoint(j,i,White);
					}
			}else{
				LCD_SetPoint(j,i,colore);
			}
			}else{
				LCD_SetPoint(j,i,colore);
			}
		}
	}
	//Parte inferiore
	colSpecial=ny;
	partenza=y;
	for(i=nx-1;i<nx;i++){
		for(j=partenza;j<colSpecial;j++){
			if(j!=partenza+2 && j!=colSpecial-3){
				LCD_SetPoint(j,i,colore);
			}
		}
	}
	if(check==1){
		drawPacMan(xPacMan,yPacMan);
	}
	}
	return;
}
void backgroundSetup(int x, int y, int valore){
	int i,j,nx,ny;
	uint16_t colore;
	x=x*scalamento;
	y=y*scalamento;
	if(valore==-2){
		nx=x+scalamento;
		ny=y+scalamento;
		colore=Blue;
	}else if(valore==1){
		nx=x+scalamentoPalline;
		ny=y+scalamentoPalline;
		x=x+4;
		y=y+4;
		colore=Green;
	}else if(valore==-3){
		nx=x+scalamento-7;
		ny=y+scalamento;
		colore=Magenta;
	}else if(valore==0){
		nx=x+scalamento;
		ny=y+scalamento;
		colore=Black;
	}else if(valore==5){
		nx=x+scalamento-3;
		ny=y+scalamento-3;
		x=x+3;
		y=y+3;
		colore=Red;
	}
	for(i=x;i<nx;i++){
		for(j=y;j<ny;j++){
				LCD_SetPoint(j,i,colore);
		}		
	}
	if(valore==-3){
		for(i=nx;i<nx+7;i++){
			for(j=y;j<ny;j++){
				LCD_SetPoint(j,i,Black);
			}
		}
	}
	return;
}


int main(void){
	int i,j;
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	CAN_Init();
	time_t t=time(NULL);
	srand((unsigned int)t);
	momentoTimer=40+rand()%(21);
  LCD_Initialization(); 
	joystick_init();
	BUTTON_init();
	init_RIT(0xF4240);								/*100.000.000[Mhz]*0.05[sec]=5.000.000 = 0x004C4B40 -> 100.000.000*/
	enable_RIT();
	ADC_init();
  //TP_Init();
	//TouchPanel_Calibrate();
	LCD_Clear(Black);
	GUI_Text(10,10,(uint8_t *)scoreStr,White,Black);
	sprintf(value,"%d",score);
	GUI_Text(60,10,(uint8_t *)value,White,Black);
	xGhost=16;	//16
	yGhost=12;	//12
	for(i=0;i<Nrighe;i++){ //Prime 3 righe vuote (y)
			for(j=0;j<Ncolonne;j++){// Relative colonne vuote (x)
					if(matrice[i][j]==4){
						xPacMan=i;
						yPacMan=j;
						drawPacMan(i,j);
					}else if(matrice[i][j]==7){
						drawGhost(1);
					}
					else{
							backgroundSetup(i,j,matrice[i][j]);		
						}
			}
	}
	drawHeart(30,1);
	init_timer(1,0x00802C80);								/* 0.34s * 25MHz= 25*10^6=017D7840 */
	//init_timer(1,0x00200B20);								
	enable_timer(1);
	//init_timer(2,0x0510FF40);								/* 3.4s * 25MHz= 25*10^6=017D7840 */
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);		
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);			//Speaker 
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
