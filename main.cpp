#include<iostream>
#include<string>
#include<GL/glut.h>

void universe_init(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0,1.0,1.0,1.0);
}

void display(){
    glMatrixMode(GL_MODELVIEW);
    universe_init();
    glLoadIdentity();
    glOrtho(-1000,1000,-1000,1000,-1000,1000);
    glColor3f(1.0,0.0,0.0);
    glTranslatef(-500,0,-1);
    glutSolidSphere(200, 60, 60);
    glTranslatef(500,0,-200);
    glColor3f(0.0,0.6,0.7);
    glutSolidSphere(200, 60, 60);
    glutSwapBuffers();
    glFlush();
}
void reshape(int x, int y)
{
    if (y == 0 || x == 0) return;   
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity(); 
    gluPerspective(39.0,(GLdouble)x/(GLdouble)y,0.6,21.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);  //Use the whole window for rendering
}
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("Scale of the Universe");
    // glutFullScreen();
	glutDisplayFunc(display);
    // glutReshapeFunc(reshape);
	// glutIdleFunc(display);
	glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}