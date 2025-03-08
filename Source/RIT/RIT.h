/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           RIT.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_RIT, funct_RIT, IRQ_RIT .c files
** Correlated files:    lib_RIT.c, funct_RIT.c, IRQ_RIT.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __RIT_H
#define __RIT_H
#include <stdint.h>
#include "music/music.h"
#include "timer/timer.h"
#include "LPC17xx.h"


/* init_RIT.c */
extern uint32_t init_RIT( uint32_t RITInterval );
extern void enable_RIT( void );
extern void disable_RIT( void );
extern void reset_RIT( void );
extern void cambiaStato(int tipo,uint16_t colore);
extern void recuperaStato(void );
/* IRQ_RIT.c */
extern void RIT_IRQHandler (void);
#define Nrighe 32
#define Ncolonne 24
#define scalamento 10
#define scalamentoPalline 6
extern int xPacMan,yPacMan;
extern int xGhost,yGhost;
extern int matrice[Nrighe][Ncolonne];
extern int changeDirection;
extern void drawPacMan(int x, int y);
extern void clearPacMan(int x, int y);
extern void drawGhost(int valore);
extern int direzione,prossimaMossa,statoPausa,partitaFinita,score,mosse,contatore,blueGhost,timerFantasma,startWaiting;
extern int xPacManPrecedente,yPacManPrecedente,xGhostPrecedente,yGhostPrecedente,gameOver,ghostEaten,momentoTimer;
extern NOTE song[],sirena[],pausa[];
#endif /* end __RIT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
