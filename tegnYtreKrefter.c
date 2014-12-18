#include "tegnYtreKrefter.h"

/*
 *tegnHorisontalPil
 */
void tegnHorisontalPil(float xtot,float ytot, float forceValue){
	float  pilTuppLengde;
	
	pilTuppLengde = 0.06;
	glColor3f(0.5, 0.7, 0.3);
	
	glLineWidth(5);
	// tegner linjen av pilen = --
	glBegin(GL_LINES);
		glVertex2d(xtot - forceValue, ytot );
		glVertex2d(xtot, ytot);
	glEnd();
	// tegner spisen(trekantdelen) av pilen = >
	glBegin(GL_TRIANGLES);
		glVertex2d(xtot ,ytot); // setter tuppen av pilen ved enden av pilen : xtot + forceValue
		glVertex2d(xtot-pilTuppLengde,ytot+pilTuppLengde);
		glVertex2d(xtot-pilTuppLengde,ytot-pilTuppLengde);	
	glEnd();
}
/*
 * tegnVerticalPil
 */
void tegnVerticalPil(float xtot,float ytot, float forceValue){
	float  pilTuppLengde;
	
	pilTuppLengde= 0.06;
	glColor3f(0.5, 0.7, 0.3);
	glLineWidth(5);
	// --> horisontal bjelke pil skal gå langs y-aksen _|_
	
	// tegner linjen av pilen = --
	glBegin(GL_LINES);
		glVertex2d(xtot,ytot+forceValue);
		glVertex2d(xtot,ytot);
	glEnd();	
	// tegner spisen(trekantdelen) av pilen = _|_
	glBegin(GL_TRIANGLES);
		glVertex2d(xtot,ytot);
		glVertex2d(xtot-pilTuppLengde,ytot+pilTuppLengde);
		glVertex2d(xtot+pilTuppLengde,ytot+pilTuppLengde);
	glEnd();
}
/*
*/
void velgPilensKordinatOgRetning(int ElementNrBjelke, float forceValue, float avstandKP1){
	
	int i,kp1, kp2;
	float x1, y1, x2, y2;
	float xtot, ytot;
	
	// henter ut knutepunktene kp1 gir tilgang på x1, y1 og kp2 gir x2, y2	
	kp1 = element[ElementNrBjelke][0] -1; //trekker fra 1 siden  innput til fortan er 1-->16 mens c er 0-->15
	kp2 = element[ElementNrBjelke][1] -1;
	
	// hente ut x1,y1 og x2, y2	
	x1 =  knutePunkt[kp1][0];
	y1 =  knutePunkt[kp1][1];
	
	x2 =  knutePunkt[kp2][0];
	y2 =  knutePunkt[kp2][1];
	
	// se på sammenhengen mellom x1,y1 g x2, y2 for å finne retningen	
	if (x1 == x2){ // vertikal bjelke |
		xtot = x1; // leger til avstanden i horisontal x-retning der pilen skal treffe
		ytot = y1 + avstandKP1;
		tegnHorisontalPil(xtot, ytot, forceValue); // er en pil i x retning
	}
	else if (y1 == y2) { // (y1 == y2) --> horisontal bjelke -
		xtot = x1 + avstandKP1;
		ytot = y1; // leger til avstanden i vertikal  y-retning der pilen skal treffe
		tegnVerticalPil(xtot, ytot, forceValue);  // er en pil i y retning
	}
	// tar ikke hensyn til krefter som står på skrå bjelker (enda | |)
}

/*
 * tegnPunktLast = kalles fra draw, kaller velgPilensKordinatOgRetning
 * 
 * 
 */
void tegnPunktLast(void){
	int ElementNrBjelke, i,kp1, kp2;
	float forceValue,avstandKP1, x1, y1, x2, y2, xtot, ytot;
	
	for(i=0;i<nPunktLast;i++){ 
		//trekker fra 1 siden  innput til fortan er 1-->3 mens c er 0-->2
		ElementNrBjelke = (int)punktLast[i][0] -1; // caster floaten til en int og 
		forceValue = punktLast[i][1];
		avstandKP1 = punktLast[i][2];
		velgPilensKordinatOgRetning(ElementNrBjelke, forceValue, avstandKP1);
	}	
}

/*
 * drawFordeltLast = tegner inn fordelte laster.
 * 
 * bruker tegnVerticalPil til å tegne pilene
 * 
 * bruker dobbel for løkke slik at den kan itterere over elemenetet og legge inn piler. 
 * Den tar også høyde for å tegne linære laster ved å bruke qtot. En sum som blir regnet ut som følger:
 * qtot = qtot + ((q2-q1)/5);
 * dvs at hvis q2<q1 så vil pilene synke i verdi fra kp1 til kp2. Omvendt om q2>q1.
 */
void tegnFordeltLast(void){
	int i, j, ElementNrBjelke;
	float q1, q2, qtot;
	
	int kp1, kp2;
	float x1, y1, x2, y2;
	float xtot, ytot;
	
	for (i=0; i<nFordelteLaster;i++){ 
		//trekker fra 1 siden  innput til fortan er 1-->3 mens c er 0-->2
		ElementNrBjelke = (int)fordeltLast[i][0] -1;// caster floaten til en int og 
		
		q1 = fordeltLast[i][1];
		q2 = fordeltLast[i][2];
		qtot = q1;
		
		/*--------------------------------------------*/
		// henter ut knutepunktene kp1 gir tilgang på x1, y1 og kp2 gir x2, y2	
		kp1 = element[ElementNrBjelke][0] -1; //trekker fra 1 siden  innput til fortan er 1-->16 mens c er 0-->15
		kp2 = element[ElementNrBjelke][1] -1;
		
		// henter ut x1,y1 og x2, y2	
		x1 =  knutePunkt[kp1][0];
		y1 =  knutePunkt[kp1][1];
		
		x2 =  knutePunkt[kp2][0];
		y2 =  knutePunkt[kp2][1];
		
		/*-----------------------------------------------------------------------------*/
				// se på sammenhengen mellom x1,y1 g x2, y2 for å finne retningen	
		if (x1 == x2){ // vertikal bjelke |
			
			xtot = x1; // leger til avstanden i horisontal x-retning der pilen skal treffe
			ytot = y1;
			tegnHorisontalPil(xtot, ytot, qtot); // Må tegne første pilen i x retning
			for (j=0; j<5; j++){
				qtot = qtot + ((q2-q1)/5);
				ytot = ytot + ((y2-y1)/5);
				tegnHorisontalPil(xtot, ytot, qtot); // er en pil i x retning
			}
		}
		else if (y1 == y2) { // (y1 == y2) --> horisontal bjelke -
			ytot = y1; // leger til avstanden i vertikal  y-retning der pilen skal treffe
			xtot = x1;
			tegnVerticalPil(xtot, ytot, qtot);  // Må tegne første pilen i y retning
			for (j=0; j<5; j++){
				qtot = qtot + ((q2-q1)/5);
				xtot = xtot + ((x2-x1)/5);
				tegnVerticalPil(xtot, ytot, qtot);  // er en pil i y retning
			}
		}
	}
}

/* ikkke ferdig fiks den!! */
void tegnSirkel(float cx, float cy, float r) { 
	int i;
	float theta ,tangentFaktor, radialFaktor, x, y, tx, ty;
	
	// Bestemer fargen til sirkelen  i forhold til retningen til momentet.
	if (r >0) { // momentet er posetivt. Rød farge for med klokka.
		glColor3f( 1.0, 0.0, 0.0);
	}
	else{ // momentet er negativt. blå farge for med klokka.
		glColor3f( 0.0, 0.0, 1.0); 
	}
	theta = 2 * 3.1415926 / 10.0;
	tangentFaktor = tanf(theta);//calculate the tangential factor 
	radialFaktor = cosf(theta);//calculate the radial factor 
	x = r;//we start at angle = 0 
	y = 0; 
    
    glLineWidth(5);
    // tegner sirkelen
	glBegin(GL_LINE_LOOP); 
		for(i = 0; i < 10; i++) { // til 10 siden det er antall vinkel endringer på sirkelen jeg vil tegne. 
			glVertex2f(cx + x, cy + y);//output vertex 
			
			//calculate the tangential vector 
			//remember, the radial vector is (x, y) 
			//to get the tangential vector we flip those coordinates and negate one of them 
			tx = -y; 
			ty = x; 
			
			//add the tangential vector 
			x += tx * tangentFaktor; 
			y += ty * tangentFaktor; 
			
			//correct using the radial factor
			x *= radialFaktor; 
			y *= radialFaktor; 
		} 
	glEnd(); 
}

/* tegner inn hvor Ymoment ligger*/
void tegnYtreMoment(void){
	int i, ElementNrBjelke, kp1;
	float ytreMoment, x1, y1;
	
	for (i=0; i<nYmoment;i++){
		ElementNrBjelke = Ymoment[i][0] -1; //trekker fra 1 siden  innput til fortan er 1-->3 mens c er 0-->2
		ytreMoment = Ymoment[i][1];
		
		// henter ut knutepunktene kp1 gir tilgang på x1, y1 
		kp1 = element[ElementNrBjelke][0] -1; //trekker fra 1 siden  innput til fortan er 1-->16 mens c er 0-->15
	
		// hente ut x1,y1 
		x1 =  knutePunkt[kp1][0];
		y1 =  knutePunkt[kp1][1];
	
		//tegnSirkel(x1, y1, ytreMoment, 10); 
		tegnSirkel(x1, y1, ytreMoment);
	}
}
