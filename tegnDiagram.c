
#include "tegnDiagram.h"


int selectedState = 1; // settes med hvilken tilstand vinduet har når det opnes. % = i som viser kreftene som påvirker konstuksjonen.
int viewForces = 1; // 1 = true , betyr at vi vil se kreftene
float maxHolder[4]; // til 3 fordi maxHolder inneholder [0], [1], [2]
float skaleringMultiplikator=1.9;



/*
 * Fiks bytt variabelnavn.
 */
void tegnMaxverdi(void) {
	char setning[40];
	
	glColor3f(1.0,1.0,1.0); // hvit farge
	glRasterPos2f(-2.2,2); // seter posisjonen der "setning" skal printes på skjermen.
        
	switch(selectedState) {
	case 0:
		//sprintf() skriver til character string "setning" 
		sprintf(setning,"%s %d %s","Max-skjaerKraft:", (int)maxHolder[selectedState], "KN");
		break;
	case 1:
		sprintf(setning,"%s %d %s","Max-Moment:", (int)maxHolder[selectedState], "KNm");
		break;
	case 2:
		sprintf(setning,"%s %d %s","Max-Boyespenning:", (int)maxHolder[selectedState], "MPa");
		break;
	case 3:
		sprintf(setning,"%s %d %s","Max-Boyespenning prosentvis:", (int)maxHolder[selectedState], "%");
		break;  
	}
	
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);// skriv "setning" til skjermen
}


/*
 skalerFarge = får inn en verdi og en "skaleringsfaktor" bruker dette til 
 * å regne ut en verdi som blir brukt til å gi en farge. Denne fargen er relativ i forhold
 * til de samme verdiene for den typen som blir visualisert. 
 * 
 * skaleringMultiplikator kan forandres ved å trykke Z(minker verdien) eller X(hever verdien).
 * skaleringMultiplikator kan andvendes slik at en kan se paa hvordan konstruksjonen blir paavirket ved aa se 
 * paa hvordan fargene forandrer seg ved aa trykke Z og X.
 * 
 */
float skalerFarge(float tall, float skaleringsFaktor){
	if(tall != 0.0){ // for å ikke "divide by zero"
		return (skaleringMultiplikator*tall)/(skaleringsFaktor);	
	} else{ 
		return tall;
	}
}

// bruk "MomentUtsattKP" til å tegne momentdiagramene fra midten også
/*
 * drawMomentDiagram = Metode som tegner to linjer mellom punktene (x1,y1) og (xMidt,yMidt),
 *  (xMidt,yMidt) og (x2,y2). 
 * 	 Bestemmer så farge for å beskrive størelsen av momentet.
 */
void tegn3punktsDiagram(float x1, float y1, float x2, float y2, int i, float faktorKP1, float faktorMidten, float faktorKP2) {
	
	float xMidt, yMidt, avstandKP1, skalerDiagram;
	
	// bestem nye kordinatene til yMidt og xMidt
	avstandKP1 = MomentUtsattKP[i][0];
	xMidt = x1;
	yMidt = y1;
	if(x1 == x2){
		yMidt = y1 + avstandKP1;
	}
	if(y1 == y2){
		xMidt = x1 + avstandKP1;
	}
	
	// Tegne linjen
	glLineWidth(5);
	glBegin(GL_LINES);	
		//KP1
		glColor3f(faktorKP1, 0.0, 1 -faktorKP1); 
		glVertex2f(x1,y1);
		
		//kpMidten1
		glColor3f(faktorMidten, 0.0, 1 -faktorMidten); 
		glVertex2f(xMidt,yMidt);
	glEnd();
		
	glBegin(GL_LINES);	
		//kpMidten2
		glColor3f(faktorMidten, 0.0, 1 -faktorMidten); 
		glVertex2f(xMidt,yMidt);
		
		// KP2
		glColor3f(faktorKP2, 0.0, 1 - faktorKP2);
		glVertex2f(x2,y2);
	glEnd();
}


/*
 * tegnSkjaerDiagram = Metode som tegner en linje mellom punktet (x1,y1) og (x2,y2)
 * 
 */
void tegnSkjaerDiagram(float x1, float y1, float x2, float y2, float faktorKP1, float faktorKP2){
	
	glLineWidth(5);
	glBegin(GL_LINES);
		//KP1
		glColor3f(faktorKP1, 0.0, 1 -faktorKP1); 
		glVertex2f(x1,y1);
		// KP2
		glColor3f(faktorKP2, 0.0, 1 - faktorKP2);
		glVertex2f(x2,y2);	
	glEnd();	
}


/*
 * Skal flyttes lenger opp, er ikke ferdig. 
 * Bytt navn på variabler
 * 
 * Brukes til å skalerer fargene elementene får når en skal visualisere de forskjellige tilstandene
 * 
 * Finner hvilken av knutepunktene som gir størst max verdi.
 * 
 * blir kalt fra oglprogram.c, er viktig for skalerKonstruksjonen
 * 
 */
void finnMaxVerdier(void){
  int i, j;  
  
  // setter maxHolder til 0
  maxHolder[0]= 0;
  maxHolder[1]= 0;
  maxHolder[2]= 0;
  maxHolder[3]= 0;
  
// til 3 fordi maxHolder inneholder [0], [1], [2]
	for(i = 0; i<nElement; i++) {
		
		// v = 0. Skjaerkrefter
		if (fabs(Skjaerkrefter[i][0])>maxHolder[0]){ 
			maxHolder[0] = Skjaerkrefter[i][0];
		}
		if (fabs(Skjaerkrefter[i][1])>maxHolder[0]){
			maxHolder[0] = Skjaerkrefter[i][1];
		}
		// m = 1. Endemomenter
		if (fabs(Endemomenter[i][0])>maxHolder[1]){
			maxHolder[1] = Endemomenter[i][0];
		}
		if (fabs(MomentUtsattKP[i][1])>maxHolder[1]){ // moment på på utsatt punkt.
			maxHolder[1] = MomentUtsattKP[i][1];
		}
		if (fabs(Endemomenter[i][1])>maxHolder[1]){
			maxHolder[1] = Endemomenter[i][1];
		}
		// b = 2. Boyespenninger
		if (fabs(Boyespenninger[i][0])>maxHolder[2]){
			maxHolder[2] = Boyespenninger[i][0];
		}
		if (fabs(Boyespenninger[i][1])>maxHolder[2]){ // boyespenninger på utsatt punkt.
			maxHolder[2] = Boyespenninger[i][1];
		}
		if (fabs(Boyespenninger[i][2])>maxHolder[2]){ // [i][2] --> 2 side n det er KP2
			maxHolder[2] = Boyespenninger[i][2];
		}
		
		// r = 3. Relative Boyespenninger
		if (fabs(RelativBoyespenninger[i][0])>maxHolder[3]){
			maxHolder[3] = RelativBoyespenninger[i][0];
		}
		if (fabs(RelativBoyespenninger[i][1])>maxHolder[3]){ // Relative boyespenninger på midten.
			maxHolder[3] = RelativBoyespenninger[i][1];
		}
		if (fabs(RelativBoyespenninger[i][2])>maxHolder[3]){ // [i][2] --> 2 side n det er KP2
			maxHolder[3] = RelativBoyespenninger[i][2];
		}
	}
	
	//fabsMaxHolder[0] // fabsmax av max og min
}


/*
 * tegnFargeForMaxVerdi = tegner en firkant med fargen til den hoyeste verdien som er i diagrammet
 */
void tegnFargeForMaxVerdi(void){
	float faktorFarge, skalerDiagramFaktor;
	
	skalerDiagramFaktor = maxHolder[selectedState];
	
	if (selectedState == 3){ // != RelativBoyespenninger
		faktorFarge = skalerDiagramFaktor/100;		
	}
	else{ // skaler ved å bruke skalerfarge 
		faktorFarge = skalerFarge(fabs(maxHolder[selectedState]),skalerDiagramFaktor);
	}
	
	glLineWidth(15);
	glBegin(GL_LINES);
		glColor3f(faktorFarge, 0.0, 1 -faktorFarge); 
		glVertex2f(-2.3,2.07);
		glVertex2f(-2.3,2.0);	
	glEnd();

}

/*
 * tegnFargeSpekter = viser 11 farge firkanter som viser fra 0% til 100%  hvilken verdi fargen representerer.
*/
void tegnFargeSpekter(void){
	int i, prosentInt;
	float faktorFarge[10];
	float faktorVerdi[10];
	float skalerDiagramFaktor, x1 , y1, y2, prosent;
	char setning[200];
	
	// seter variablene
	x1 = 2.4;
	y1 = 0.60;
	y2 = 0.53;
	prosent = 0.0;
	prosentInt = 0;
	
	skalerDiagramFaktor = maxHolder[selectedState];
	faktorVerdi[10] = maxHolder[selectedState]; // kansje slettes
	
	if (selectedState == 3){ // == RelativBoyespenninger
		faktorFarge[10] = 1; //skalerDiagramFaktor/skalerDiagramFaktor = 1 --> 100 % er rød		
	}
	else{ // skaler ved å bruke skalerfarge 
		faktorFarge[10] = skalerFarge(fabs(maxHolder[selectedState]),skalerDiagramFaktor);
	}
	
	// Regner ut prosentvise farger i forhold til max farge verdi
	glLineWidth(15);
	for(i=0;i<11;i++){
		faktorFarge[i]=faktorFarge[10]*prosent;
		
		faktorVerdi[i] = faktorVerdi[10]*prosent;
	
		glBegin(GL_LINES);
			glColor3f(faktorFarge[i], 0.0, 1 -faktorFarge[i]); 
			glVertex2f(x1, y1);
			glVertex2f(x1, y2);	
		glEnd();
		
		// printe ut hvilken verdi detprosentInt er 
		glColor3f(1.0,1.0,1.0); // hvit farge
		
		// skriver ut hvilken prosent av max fargen hører til
		glRasterPos2f(x1 + 0.1, y2);
		sprintf(setning,"%d %s", prosentInt, "%");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		// skriver ut hva prosent verdien represnterer i verdi
		if (selectedState != 3){ // != RelativBoyespenninger
			// skriver ut verdi 
			glRasterPos2f(x1 + 0.4, y2);
			sprintf(setning,"%s %d "," = " ,(int)faktorVerdi[i]);
			glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		}
		// ny posisjon
		y1= y1 + 0.14;
		y2 = y2 + 0.14;
		// ny prosent
		prosent = prosent +0.1;
		prosentInt = prosentInt + 10;
	}
}





/*prosentInt
drawElement =  Bruker den valgte "selectedState" fra key til å tegne et gitt diagram
* 
*/
void velgDiagram(float x1, float y1, float x2, float y2, int i) {
	float faktorKP1, faktorMidten, faktorKP2, skalerDiagramFaktor;
	
	skalerDiagramFaktor = maxHolder[selectedState]; // viktig
	
	if (selectedState == 0){ // 0= v --> skjærkraftdiagram
		
		faktorKP1 = skalerFarge(fabs(Skjaerkrefter[i][0]), skalerDiagramFaktor);
		faktorKP2 = skalerFarge(fabs(Skjaerkrefter[i][1]), skalerDiagramFaktor);
		
		tegnSkjaerDiagram(x1, y1, x2, y2, faktorKP1, faktorKP2);
	}
   else if(selectedState == 1){ // 1 = m --> momentdiagram
	   
	    faktorKP1 = skalerFarge(fabs(Endemomenter[i][0]), skalerDiagramFaktor); //  bruker skaleringsFaktor som = maxHolder[selectedState]
		faktorMidten = skalerFarge(fabs(MomentUtsattKP[i][1]), skalerDiagramFaktor);	
		faktorKP2 = skalerFarge(fabs(Endemomenter[i][1]), skalerDiagramFaktor);
		
		tegn3punktsDiagram(x1, y1, x2, y2, i, faktorKP1, faktorMidten, faktorKP2);
	}
   else if(selectedState == 2){ // 2 = b --> bøyemomentdiagram
		
		faktorKP1 = skalerFarge(fabs(Boyespenninger[i][0]), skalerDiagramFaktor);
		faktorMidten = skalerFarge(fabs(Boyespenninger[i][1]), skalerDiagramFaktor);
		faktorKP2 = skalerFarge(fabs(Boyespenninger[i][2]), skalerDiagramFaktor);
		
		tegn3punktsDiagram(x1, y1, x2, y2, i, faktorKP1, faktorMidten, faktorKP2);
	}
	else if(selectedState == 3){ // 3 = R --> RelativBoyespenninger
		
		faktorKP1 = RelativBoyespenninger[i][0]/100.0; // deler på 100 siden det er prosent
		faktorMidten = RelativBoyespenninger[i][1]/100.0;
		faktorKP2 = RelativBoyespenninger[i][2]/100.0;
		
		tegn3punktsDiagram(x1, y1, x2, y2, i, faktorKP1, faktorMidten, faktorKP2);
	}
}

/*
drawSystem = Metode som bruker konnektiviteten fra element 
til finne linjene som skal tegnes mellom knutepunktene (x1,y1) og (x2,y2) 
*/
void tegnSystem(void) {
  int i, j, kp1, kp2;
  float x1, y1, x2 ,y2;
 
	for(i=0;i<nElement;i++){

		kp1 =element[i][0] -1; // trekker fra 1 siden  innput til fortan er 1-->16 mens c er 0-->15
		kp2 =element[i][1] -1;
  			
		y1 = knutePunkt[kp1][1];
		x1 = knutePunkt[kp1][0];

		y2 = knutePunkt[kp2][1];
		x2 = knutePunkt[kp2][0];
		
		velgDiagram(x1, y1, x2, y2, i); // kaller velgDiagram som tegner linjen mellom punktene
	}
}

