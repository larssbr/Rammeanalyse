#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GL/glut.h"

// tall som lesses inn
extern int nKnutePunkt, nElement, nPunktLast, nFordelteLaster, nYmoment, nPipeProfile, nIPE_beam, nMateriale;

// matriser som leses inn fra inputfilen
extern float **knutePunkt;
extern float **knutePunkt;
extern int **element;
extern float **punktLast;
extern float **fordeltLast;
extern float **Ymoment;

extern float **PipeProfile;
extern float **IPE_beam;
extern float **materiale; 

// matriser som leses inn fra resultatfilen
extern float **Endemomenter;
extern float **MomentUtsattKP;
extern float **Skjaerkrefter;
extern float **Boyespenninger;
extern float **RelativBoyespenninger;

// Metodene som en trenger
void lesInputFile(void); // kalles fra oglprogram

void lesResultFile(void);// kalles fra oglprogram


