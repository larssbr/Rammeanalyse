

#include "lesInn.h"
#include "skalering.h" // får og ha med "skaleringsfaktor"

extern int selectedState;
extern int viewForces;
extern float maxHolder[4];
extern float skaleringMultiplikator;


void tegnMaxverdi(void); // blir kalt fra oglprogram.c

void tegnFargeForMaxVerdi(void);

void tegnFargeSpekter(void);

void finnMaxVerdier(void);

void tegnSystem(void); // blir kalt fra oglprogram.c



//metoder inne i tegnDiagram.c som blir brukt til å gjør tegnSystem:
//float skalerFarge(float tall, float skaleringsFaktor);  // blir brukt av drawElement 
//void tegn3punktsDiagram(float x1, float y1, float x2, float y2, int i, float faktorKP1, float faktorMidten, float faktorKP2); // blir kalt dra drawElement
//void tegnSkjaerDiagram(float x1, float y1, float x2, float y2, float faktorKP1, float faktorKP2);

//void velgDiagram(float x1, float y1, float x2, float y2, int i);
