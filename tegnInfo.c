
#include "tegnInfo.h"


/*
 * 
 */
void tegnTversnittsData(void) {
	char setning[40];
	glColor3f(1.0,1.0,1.0); // hvit farge
	
	if (PipeProfile!=NULL){
		glRasterPos2f(-2.2,1.6);
		sprintf(setning,"%s %d %s","Rorets Ytre Diameter:", (int)PipeProfile[0][0], "mm");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		glRasterPos2f(-2.2,1.4);
		sprintf(setning,"%s %d %s","Rorets Tykkelse:        ", (int)PipeProfile[0][1], "mm");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning); 
	}
	
	if (IPE_beam!=NULL){
		glRasterPos2f(-2.2,1.2);
		sprintf(setning,"%s %d %s","Lengde Flens:            ", (int)IPE_beam[0][0], "mm");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		glRasterPos2f(-2.2,1.0);
		sprintf(setning,"%s %d %s","Tykkelse Flens:          ", (int)IPE_beam[0][1], "mm");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		glRasterPos2f(-2.2,0.8);
		sprintf(setning,"%s %d %s","Lengde Stag:             ", (int)IPE_beam[0][2], "mm");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		glRasterPos2f(-2.2,0.6);
		sprintf(setning,"%s %d %s","Tykkelse Stag:           ", (int)IPE_beam[0][3], "mm");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);  
	}
	
	
	glRasterPos2f(-2.2,0.2);
	sprintf(setning,"%s %d %s","E-Modul: ", (int)materiale[0][0], "GPa");
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning); 
	
	glRasterPos2f(-2.2,0.0);
	sprintf(setning,"%s %d %s","Tilatt flytespenning:", (int)materiale[0][1], "GPa");
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);	
}

/*
 * 
 */
void tegnMeny(void){	
	char setning[200];
	glColor3f(1.0,1.0,1.0); // hvit farge
	
	glPushAttrib(GL_ENABLE_BIT); 
			
		glRasterPos2f(-0.8,-0.6);
		sprintf(setning,"%s","Meny: ");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		glRasterPos2f(-0.8,-0.8);
		sprintf(setning,"%s","Visualiser Krefter: 'I'    |     Boyemoment : 'B'      |      MomentDiagram: 'M'");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		glRasterPos2f(-0.8,-1.0);
		sprintf(setning,"%s"," Skru av Visualiser Krefter: 'O' ");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		glRasterPos2f(-0.8,-1.2);
		sprintf(setning,"%s","Skjaerkraft: 'V'    |     Relativt Boyemoment: 'R'    |  Lukk vinduet : 'Esc'");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
		
		glRasterPos2f(-0.8,-1.4);
		sprintf(setning,"%s","Minsk skaleringen : 'Z'    |     Heve skaleringen: 'X' ");
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, setning);
	
	glPopAttrib();
}


