#include "skalering.h"

float konstruksjonensC_X, konstruksjonensC_Y, skaleringsFaktor;  // C_X = centrum sitt x kordinat

// finnSkaleringsFaktor = finner ut hvem som har størst verdi-rom velger så den til skaleringsFaktor
void finnSkaleringsFaktor(float max_x,float max_y,float min_x,float min_y){
	
	// finner ut hvem som har størst verdi-rom velger så den til skaleringsFaktor
	if((max_x - min_x) > (max_y - min_y))
		skaleringsFaktor = max_x - min_x;
	else{
		skaleringsFaktor = max_y - min_y;
	}
}

float finnMax( int pos){
	int i;
	float maxTall;
	maxTall=0;
	for(i=0;i<nKnutePunkt;i++) {
		if(knutePunkt[i][pos] > maxTall){
			maxTall = knutePunkt[i][pos];
		}
	}
	return maxTall;
}

float finnMin( int pos){
	int i;
	float minTall;
	minTall=100;
	for(i=0;i<nKnutePunkt;i++) {
		if(knutePunkt[i][pos] < minTall){
			minTall = knutePunkt[i][pos];
		}
	}
	return minTall;
}

/*
skalerKonstruksjon = Metode som  blir kalt fra main som scalerer systemet slik at konstruksjonen blir vist
* i en fin størelse i vinduet
*/
void skalerKonstruksjon(void) {
	int i;
	float max_x, max_y, min_x, min_y;
	
	max_x = finnMax(0); //0 for x posisjon
	max_y = finnMax(1); //1 for y posisjon
	min_x = finnMin(0); //0 for x posisjon
	min_y = finnMin(1); //1 for y posisjon
		
	finnSkaleringsFaktor(max_x, max_y, min_x, min_y);

	// setter center punkter slik at en får alt i en rute.
	konstruksjonensC_X = ((max_x + min_x) / 2) / skaleringsFaktor;
	konstruksjonensC_Y = ((max_y + min_y) / 2) / skaleringsFaktor;

	// skalerer knyttepunktskordinatene
	for(i=0;i<nKnutePunkt;i++) {
		knutePunkt[i][0] = (knutePunkt[i][0] / skaleringsFaktor)*2;
		knutePunkt[i][1] = (knutePunkt[i][1] / skaleringsFaktor)*2;
	}
	// skalerer punktLast verdiene
	for(i=0;i<nPunktLast;i++) {
		// tar ikke [0] siden den er elementBjelkenr
		punktLast[i][1] = (punktLast[i][1] / (skaleringsFaktor*5)); // skalerer størelsen til forcevalue--> altså lengden til pilen
		punktLast[i][2] = (punktLast[i][2] / skaleringsFaktor)*2;
	}
	// skalerer fordeltLast verdiene
	for(i=0;i<nFordelteLaster;i++) {
		// tar ikke [0] siden den er elementBjelkenr
		fordeltLast[i][1] = (fordeltLast[i][1] / (skaleringsFaktor)); // skalerer størelsen til forcevalue--> altså lengden til pilen
		fordeltLast[i][2] = (fordeltLast[i][2] / (skaleringsFaktor));
	}

	// skalerer Ymoment verdiene
	for(i=0;i<nYmoment;i++) {
		// tar ikke [0] siden den er elementBjelkenr
		Ymoment[i][1] = (Ymoment[i][1] / (skaleringsFaktor*50)); // skalerer størelsen til ytre moment
	}

	// skaler MomentUtsattKP 
	for(i=0;i<nElement;i++) {
		MomentUtsattKP[i][0] = (MomentUtsattKP[i][0] / (skaleringsFaktor)); // skalerer avstandKP1
		//MomentUtsattKP[i][1] = (MomentUtsattKP[i][1] / (skaleringsFaktor));  // skalerer momentet ved utsatt kp
	} 	
}




