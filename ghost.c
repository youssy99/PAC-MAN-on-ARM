#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "RIT/RIT.h"
#include "ghost.h"


int calcolaDistanza(int xGhost, int yGhost, int xPacMan, int yPacMan){
	return abs(xGhost-xPacMan)+abs(yGhost-yPacMan);
}

int insertSorted(Node arr[], int n, Node key, int capacity)
{
    // Cannot insert more elements if n is already
    // more than or equal to capacity
    if (n >= capacity){
			 return n;
		}
    int i;
    for (i = n - 1; (i >= 0 && arr[i].g > key.g); i--){
			arr[i + 1] = arr[i];
		}
    arr[i + 1] = key;
    return (n + 1);
}

void calcolaOpened(Node padre,int xPartenza,int yPartenza,int xDestinazione,int yDestinazione){
	t.x=xPartenza;
	t.y=yPartenza;
	t.g=padre.g+1;
	t.h=calcolaDistanza(xPartenza,yPartenza,xDestinazione,yDestinazione);
	t.f=t.g+t.h;
	t.xParent=padre.x;
	t.yParent=padre.y;
	return;
}


void calcolaPercorso(int xPartenza,int yPartenza,int xDestinazione, int yDestinazione,int blueGhost){
		opened[0].x=xPartenza;
		opened[0].y=yPartenza;
		opened[0].h=calcolaDistanza(xPartenza,yPartenza,xDestinazione,yDestinazione);
		opened[0].g=0;
		opened[0].f=opened[0].h+opened[0].g;
		int direzione[]={-1,1};
		visitOpened=1;
		visitClosed=0;

		int allVisited=1,i,j,k,minF,indiceMinimo=0;
		//inizializzazione opened
		while(visitOpened>0){
			minF=1000;
			indiceMinimo=-1;
			for(k=0;k<visitOpened;k++){
				if(opened[k].f<minF){
					minF=opened[k].f;
					indiceMinimo=k;
				}
			}
			nodoCorrente=opened[indiceMinimo];
			opened[indiceMinimo]=opened[--visitOpened];
			//Destinazione trovata
			if(nodoCorrente.x==xDestinazione && nodoCorrente.y==yDestinazione){
            int tempX = nodoCorrente.x;
            int tempY = nodoCorrente.y;
            while (!(nodoCorrente.xParent == xPartenza && nodoCorrente.yParent == yPartenza)) {
                tempX = nodoCorrente.xParent;
                tempY = nodoCorrente.yParent;
								for(i=visitClosed-1;i>=0;i--){
									if(closed[i].x==tempX && closed[i].y==tempY){
										nodoCorrente = closed[i];
									}
								}
							}
						xGhostPrecedente=nodoCorrente.xParent;
						yGhostPrecedente=nodoCorrente.yParent;
					 clearPacMan(nodoCorrente.xParent,nodoCorrente.yParent);
					 xGhost=tempX;
					 yGhost=tempY;
					 if(mosse>2){
						if(contatore>1){
							backgroundSetup(nodoCorrente.xParent,nodoCorrente.yParent,matrice[nodoCorrente.xParent][nodoCorrente.yParent]);
						}
					 }
           drawGhost(blueGhost);
						return;
				 }
			closed[visitClosed++]=nodoCorrente;
			for(i=0;i<4;i++){
				if(i<2){
					if(matrice[nodoCorrente.x+direzione[i]][nodoCorrente.y]!=-2){
						calcolaOpened(nodoCorrente,nodoCorrente.x+direzione[i],nodoCorrente.y,xDestinazione,yDestinazione);
						int flag=0;
						for(j=0;j<visitClosed && flag==0;j++){
							if(closed[j].x==t.x && closed[j].y==t.y){
								flag=1;
							}
						}
						if(flag==0){
							opened[visitOpened++]=t;
						}
					}
				}else{
					if(matrice[nodoCorrente.x][nodoCorrente.y+direzione[i-2]]!=-2){
						calcolaOpened(nodoCorrente,nodoCorrente.x,nodoCorrente.y+direzione[i-2],xDestinazione,yDestinazione);
						int flag=0;
						for(j=0;j<visitClosed && flag==0;j++){
							if(closed[j].x==t.x && closed[j].y==t.y){
								flag=1;
							}
						}
						if(flag==0){
							opened[visitOpened++]=t;
						}
					}
				}
			}
		}			
		return ;
}

void frightenedMode(int blueGhost){
	int i,bestMove=-1,bestDistance=-1;
  int risultato[]={-1,-1,-1,-1};
	int direzione[]={-1,1};
	for(i=0;i<4;i++){
		if(i<2){
			if(matrice[xGhost+direzione[i]][yGhost]!=-2){
				risultato[i]=calcolaDistanza(xGhost+direzione[i],yGhost,xPacMan,yPacMan);
			}
		}else{
			if(matrice[xGhost][yGhost+direzione[i-2]]!=-2){
				risultato[i]=calcolaDistanza(xGhost,yGhost+direzione[i-2],xPacMan,yPacMan);
			}
		}
		
	}
	for(i=0;i<4;i++){
		if(risultato[i]>bestDistance){
			bestDistance=risultato[i];
			if(risultato[i]!=-1){
				bestMove=i;
			}
		}
	}
	clearPacMan(xGhost,yGhost);
	xGhostPrecedente=xGhost;
	yGhostPrecedente=yGhost;
	backgroundSetup(xGhost,yGhost,matrice[xGhost][yGhost]);
	switch(bestMove){
		case 0: //up
			xGhost=xGhost-1;
			break;
		case 1: //down
			xGhost=xGhost+1;
			break;
		case 2:	//left
			yGhost=yGhost-1;
			break;
		case 3:	//right
			yGhost=yGhost+1;
			break;
	}
	drawGhost(blueGhost);
	return;
}
