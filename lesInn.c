#include "lesInn.h"

int nKnutePunkt, nElement, nPunktLast, nFordelteLaster, nYmoment, nPipeProfile, nIPE_beam, nMateriale;

// matriser som leses inn fra inputfilen
float **knutePunkt;
int **element;
float **punktLast;
float **fordeltLast;
float **Ymoment;

float **PipeProfile;
float **IPE_beam;
float **materiale; 

// matriser som leses inn fra resultatfilen
float **Endemomenter;
float **MomentUtsattKP;
float **Skjaerkrefter;
float **Boyespenninger;
float **RelativBoyespenninger;

void allocerInputfilInfo(void){
	int i;
	
	// allocerer knutePunkt
	knutePunkt = (float**)malloc(nKnutePunkt * sizeof(float*)); //Allocate matrix of size nKnutePunkt*3. 
	for(i=0;i<nKnutePunkt; i++) {
		knutePunkt[i] = (float*)malloc(3 * sizeof(float));
	}
	// allocerer element
	element = (int**)malloc(nElement * sizeof(int*)); //Allocate matrix of size nElement*5.
	for(i=0;i<nElement; i++) {
		element[i] = (int*)malloc(5 * sizeof(int));
	}
	// allocerer punktLast
	punktLast = (float**)malloc(nPunktLast * sizeof(float*)); //Allocate matrix of size nPunktLast*3. 
	for(i=0;i<nPunktLast; i++) {
		punktLast[i] = (float*)malloc(3 * sizeof(float));
	}
	// allocerer fordeltLast
	fordeltLast = (float**)malloc(nFordelteLaster * sizeof(float*)); //Allocate matrix of size nFordelteLaster*3. 
	for(i=0;i<nFordelteLaster; i++) {
		fordeltLast[i] = (float*)malloc(3 * sizeof(float));
	}
	// allocerer Ymoment
	Ymoment = (float**)malloc(nYmoment * sizeof(float*)); //Allocate matrix of size nYmoment*2. 
	for(i=0;i<nYmoment; i++) {
		Ymoment[i] = (float*)malloc(2 * sizeof(float));
	}
	//allocerer PipeProfile
	PipeProfile = (float**)malloc(nPipeProfile * sizeof(float*)); //Allocate matrix of size nPipeProfile*2. 
	for(i=0;i<nPipeProfile; i++) {
		PipeProfile[i] = (float*)malloc(2 * sizeof(float));
	}
	//allocerer IPE_beam
	IPE_beam = (float**)malloc(nIPE_beam * sizeof(float*)); //Allocate matrix of size nIPE_beam*2. 
	for(i=0;i<nIPE_beam; i++) {
		IPE_beam[i] = (float*)malloc(4 * sizeof(float));
	}
	//allocerer materiale 
	materiale = (float**)malloc(nMateriale * sizeof(float*)); //Allocate matrix of size nIPE_beam*2. 
	for(i=0;i<nMateriale; i++) {
		materiale[i] = (float*)malloc(2 * sizeof(float));
	}
}

/*
 * readInputFile = metdode som blir kalt fra main
 * 
 * funksjon = lesse inn input, allocere minne til matrisser, fylle disse matrisene med data
 */ 
void lesInputFile(void){
	FILE *file;
	int i;
	char buffer[1000];
	char *filNavn;
	
	filNavn ="inputFil.dat"; 
	file = fopen(filNavn, "r");
	
	 if (file != NULL){	
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på første linje
		
		// Leser inn antallet av den forskjellige informasjonen
		int telle0 = fscanf(file, "%i, %i, %i, %i, %i, %i, %i, %i \n", &nKnutePunkt, &nElement, &nPunktLast, &nFordelteLaster, 
		&nYmoment, &nPipeProfile, &nIPE_beam, &nMateriale);
		
		fgets(buffer, sizeof(buffer), file);// gjør ikke noe på tredje linje
		

//***************************(allocerer)*******
		allocerInputfilInfo();		
//***********************(Leser inn)***********			
		// les in elementenes knutepunkt kordinater
		for (i = 0; i<nKnutePunkt; i++) {
			fscanf(file, "%f, %f, %f \n", &knutePunkt[i][0], &knutePunkt[i][1], &knutePunkt[i][2]);		
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
	
		// les in elementenes konektivitet i element
		for (i = 0; i<nElement; i++) {
			fscanf(file, "%i, %i, %i, %i, %i \n", &element[i][0], &element[i][1]  ,&element[i][2], &element[i][3], &element[i][4]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in punktLast
		for (i = 0; i<nPunktLast; i++) {
			fscanf(file, "%f, %f, %f \n", &punktLast[i][0], &punktLast[i][1]  ,&punktLast[i][2]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in fordeltLast
		for (i = 0; i<nFordelteLaster; i++) {
			fscanf(file, "%f, %f, %f \n", &fordeltLast[i][0], &fordeltLast[i][1]  ,&fordeltLast[i][2]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
			
		// leser in Ymoment
		for (i = 0; i<nYmoment; i++) {
			fscanf(file, "%f, %f \n", &Ymoment[i][0], &Ymoment[i][1]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in PipeProfile
		for (i = 0; i<nPipeProfile; i++) {
			fscanf(file, "%f, %f \n", &PipeProfile[i][0], &PipeProfile[i][1]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in IPE_beam
		for (i = 0; i<nIPE_beam; i++) {
			fscanf(file, "%f, %f, %f, %f \n", &IPE_beam[i][0], &IPE_beam[i][1], &IPE_beam[i][2], &IPE_beam[i][3]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in materiale
		for (i = 0; i<nMateriale; i++) {
			fscanf(file, "%f, %f \n", &materiale[i][0], &materiale[i][1]); 
		}	
	fclose(file);
	
	}
    else {
        printf("Klarte ikke å åpne filen : %s: \n", filNavn);
        printf("Sjekk at du har stavet filnavnet riktig, og se om filen ligger i samme mappe som programmet.\n");
        exit(-1);
    }
}
/* ----------------------------------Resultatfilen-------------------------------------------------------------- */
	
void allocerResulatfilInfo(void){		
	int i;
		
	// allocerer Endemomenter
	Endemomenter = (float**)malloc(nElement * sizeof(float*)); //Allocate matrix of size nElement*2. 
	for(i=0;i<nElement; i++) {
		Endemomenter[i] = (float*)malloc(2 * sizeof(float));
	}
	// allocerer MomentUtsattKP
	MomentUtsattKP = (float**)malloc(nElement * sizeof(float*)); //Allocate matrix of size nElement*2. 
	for(i=0;i<nElement; i++) {
		MomentUtsattKP[i] = (float*)malloc(2 * sizeof(float));
	}
	// allocerer Skjaerkrefter
	Skjaerkrefter = (float**)malloc(nElement * sizeof(float*)); //Allocate matrix of size nElement*2. 
	for(i=0;i<nElement; i++) {
		Skjaerkrefter[i] = (float*)malloc(2 * sizeof(float));
	}
	// allocerer Boyespenninger
	Boyespenninger = (float**)malloc(nElement * sizeof(float*)); //Allocate matrix of size nElement*3. 
	for(i=0;i<nElement; i++) {
		Boyespenninger[i] = (float*)malloc(3 * sizeof(float));
	}
	// allocerer RelativBoyespenninger
	RelativBoyespenninger = (float**)malloc(nElement * sizeof(float*)); //Allocate matrix of size nElement*3. 
	for(i=0;i<nElement; i++) {
		RelativBoyespenninger[i] = (float*)malloc(3 * sizeof(float));
	}	
}


/*
 * lesInputFile = metdode som blir kalt fra main
 * 
 * funksjon = lesse inn input, allocere minne til matrisser, fylle disse matrisene med data
 */	
void lesResultFile(void){	
	
	FILE *file;
	int i;
	char buffer[1000];
	char *filNavn;
	
//***************************(allocerer)************
	allocerResulatfilInfo();
//***********************(Leser inn)****************		
	
	filNavn = "resultatFil.dat";
	file = fopen(filNavn, "r");	
	
	if (file != NULL){	
		// gjor ikke noe på 5 forste linjene
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe med rotasjonene
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// les in elementenes konektivitet i Endemomenter
		for (i = 0; i<nElement; i++) {
			fscanf(file, "%f %f \n", &Endemomenter[i][0], &Endemomenter[i][1]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in MomentUtsattKP
		for (i = 0; i<nElement; i++) {
			fscanf(file, "%f %f \n", &MomentUtsattKP[i][0], &MomentUtsattKP[i][1]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in Skjaerkrefter
		for (i = 0; i<nElement; i++) {
			fscanf(file, "%f %f \n", &Skjaerkrefter[i][0], &Skjaerkrefter[i][1]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in Boyespenninger
		for (i = 0; i<nElement; i++) {
			fscanf(file, "%f %f %f \n", &Boyespenninger[i][0], &Boyespenninger[i][1], &Boyespenninger[i][2]); 
		}
		
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		fgets(buffer, sizeof(buffer), file); // gjør ikke noe på den beskrivende linjen
		
		// leser in RelativBoyespenninger
		for (i = 0; i<nElement; i++) {
			fscanf(file, "%f %f %f \n", &RelativBoyespenninger[i][0], &RelativBoyespenninger[i][1], &RelativBoyespenninger[i][2]); 
		}
	fclose(file);
	}
	else {
		printf("Klarte ikke å åpne filen : %s: \n", filNavn);
		printf("Sjekk at du har stavet filnavnet riktig, og se om filen ligger i samme mappe som programmet.\n");
		exit(-1);
	}
}
