/*Compile : 
 * "make" fra terminalen i mappen "rammeanalyse"
 * 
 * to run: ./oglprogram
*/


/*
 * Losningen av matrise metoden for en vilkaarlig konstruksjon presenteres grafisk vha OpenGL.
 *
 * Lars Brusletto November 2013
 * 
 * Kun resultater for baeyemoment kreves presentert. 
 * Bjelkeendene er ment aa kunne roterere, men ikke bevege seg i horisontal eller vetikal retning. 
 *  
 * Laesningen skal presenteres grafisk vha OpenGL.
 * Belastninger kan visualiseres vha farger.
 * 
 * Tradisjonelt skjaerkraft og boyemoment-diagram kreves ikke. 
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GL/glut.h"

#include "oglProgram.h"


static GLuint TexObj[2];
static GLfloat Angle = 0.0f;
static GLboolean UseObj = GL_FALSE;


//static GLuint Window = 0;
GLuint Window = 0;

/*
drawSystem = Metode som  blir kalt fra main som kaller alle metodene som skal tegne
*/
void draw(void){
   glClear( GL_COLOR_BUFFER_BIT );
   glColor3f( 1.0, 1.0, 1.0 );
   
   glPushMatrix();
	   // tegn konstruksjonen og hvis hvordan den skal vises metode 
	   tegnSystem(); 
	   
	   // tegn info
	   tegnMeny();
	   tegnTversnittsData();
	   tegnMaxverdi();
	   tegnFargeForMaxVerdi();
	   tegnFargeSpekter();
	 
		if (viewForces==1){ // 1 = I = vise hvordan kreftene er satt opp.
			tegnPunktLast();
			tegnFordeltLast();
			tegnYtreMoment();
		}
   glPopMatrix();
   // Faa endringene til aa vises paa skjermen
   glutSwapBuffers();
}

/*
 * idle = blir kalt fra main med "glutIdleFunc( idle )"
 * 
 * funksjon =
 */
static void idle( void ){ // trenger ikke endre{
  static double t0 = -1.;
  double dt, t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  if (t0 < 0.0)
    t0 = t;
  dt = t - t0;
  t0 = t;
  Angle += 120.0*dt;
  glutPostRedisplay();
}

/* change view Angle, exit upon ESC 
 * 
 * Blir kalt fra main
 * 
 * key = metode som lytter til tastaturer
 * endrer verdier om "bestemte" knapper trykkes
 */
static void key(unsigned char k, int x, int y){
	(void) x;
	(void) y;
	switch (k) {
	case 27:
		glutDestroyWindow(Window);
		exit(0);
	case 'v':
		selectedState = 0;
		break;
	case 'm':
		selectedState = 1;
		break;
	case 'b':
		selectedState = 2;
		break;
	case 'r':
		selectedState = 3;
		break;
	case 'i':
		viewForces =1;
		break;
	case 'o':
		viewForces =0;
		break;
	case 'z': // Hever skaleringen
		skaleringMultiplikator = skaleringMultiplikator + 0.4;
		break;
	case 'x': // Minker skaleringen
		skaleringMultiplikator = skaleringMultiplikator - 0.4;
		break;
   }
}

/* new window size or exposure */

/*
reshape = metode som blir kalt fra main.
* 
* funskjon =
* 
* fordi =
* 
*/
static void reshape( int width, int height ){
   glViewport(0, 0, (GLint)width, (GLint)height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // glOrtho( -3.0, 3.0, -3.0, 3.0, -10.0, 10.0 );
   //glFrustum( -2.0, 2.0, -2.0, 6.0, 2.0, 20.0 );
   glFrustum( -2.0, 2.0, -2.0, 2.0, 2.0, 20.0 );
   
   //glPerspective( -2.0, 2.0, -2.0, 2.0, 6.0, 20.0 );
   glMatrixMode(GL_MODELVIEW);
   
   glLoadIdentity();
   
   //glTranslatef( 0.0, 0.0, -8.0 ); 
   glTranslatef(-konstruksjonensC_X,-konstruksjonensC_Y,-3.0);
  
}

/*
main = main metode som kjaerer programmet
* 
* readFile =
* scaleKonstruksjon =
*/
int main( int argc, char *argv[] ){
	//  lesInn input
	lesInputFile();
	lesResultFile();
	
	//skalering:
	skalerKonstruksjon();
	
	// finner max verdiene slik at de kan brukes til skalering og de vises paa skjermen
	finnMaxVerdier(); 
		
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 1000);
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );

	Window = glutCreateWindow("Brusletticus 2D beam");
	if (!Window) {
	  exit(1);
	}

	glutReshapeFunc( reshape );
	glutKeyboardFunc( key );
	glutIdleFunc( idle );
	glutDisplayFunc( draw );
	glutMainLoop();
	return 0;
}
