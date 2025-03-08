#ifndef __ghost_H
#define __ghost_H
#include "RIT//RIT.h"
typedef struct{
	int x;
	int y;
	int xParent;
	int yParent;
	int f;
	int g;
	int h;
}Node;
Node closed[Nrighe*Ncolonne];
Node opened[Nrighe*Ncolonne];
Node t,nodoCorrente;
int openedSize=0;
int visitOpened=1,visitClosed=0;
void calcolaPercorso(int xPartenza,int yPartenza,int xDestinazione, int yDestinazione,int blueGhost);
void frightenedMode(int blueGhost);
int calcolaDistanza(int xGhost, int yGhost, int xPacMan, int yPacMan);
#endif