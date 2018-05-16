#include<iostream>
#include<string>
#include<GL/glut.h>
#include<sqlite3.h>
#include "Planet.h"
using namespace std;

static int window,menu_id,go_to_submenu_id,music_submenu_id,rotate_submenu_id,background_submenu_id,translate_submenu_id;
// menu choice variable
int choice=-1;
// variable that manages the current planet being viewed
int cview = 0;
// how much to move by when 'z' or 'x' key is pressed using glOrtho
double nmov=-100,fmov=100,topmov=-100,botmov=100,leftmov=-100,rightmov=100;
// a variable to decide distance between planets
double pos=0.0;
// how much to zoom out to display the new object
int zoom;
// db variables
sqlite3 *db;
char *error = nullptr;
int rc;
// 
Planet* celestial[33];
int celes_count=0;
static int callback(void *data,int argc,char** argv,char** azColName){
    int i;
    cout << data <<endl;
    for(i=0;i<3;i+=3){
        celestial[celes_count++] =new Planet(argv[i],atof(argv[i+1])/100000,argv[i+2],"|","|","bmp/earth.jpg");
    }
    return 0;
}
void menu(int num){
    int i;
    if(num == 13){
        glutDestroyWindow(window);
        sqlite3_close(db);
        for(i=0;i<celes_count;i++)
            delete celestial[i];
        exit(0);
    }
    else
        choice = num;
    glutPostRedisplay();
}
void createMenu(){
    go_to_submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Ceres",1);
    glutAddMenuEntry("Moon",2);
    glutAddMenuEntry("Callisto",3);
    glutAddMenuEntry("Mercury",4);
    glutAddMenuEntry("Mars",5);
    music_submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Track 1",7);
    glutAddMenuEntry("Track 2",8);
    rotate_submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Enable",11);
    glutAddMenuEntry("Disable",12);
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Start",0);
    glutAddSubMenu("Go to...",go_to_submenu_id);
    glutAddMenuEntry("Animate",6);
    glutAddSubMenu("Music",music_submenu_id);
    glutAddMenuEntry("Background",9);
    glutAddMenuEntry("Shading",10);
    glutAddSubMenu("Rotation",rotate_submenu_id);
    glutAddMenuEntry("Quit",13);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void universe_init(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0,1.0,1.0,1.0);
}
void display(){
    int i;
    switch(choice){
        case 0:
            // glColor3f(1.0,0.0,0.0);
            universe_init();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            printf("cview = %d pos = %f %f %f %f %f %f %f\n\n",cview,pos,nmov,fmov,topmov,botmov,leftmov,rightmov);
            glOrtho(nmov,fmov,topmov,botmov,leftmov,rightmov);
            for(i=0;i<celes_count;i++){
                celestial[i]->render(pos,0.0,0.0,i);
                pos += celestial[i]->getRadius()+celestial[i]->getRadius()*2;
            }
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
    }
    glutSwapBuffers();
    glFlush();
}
void reshape(int x, int y){
    if (y == 0 || x == 0) return;   
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity(); 
    gluPerspective(39.0,(GLdouble)x/(GLdouble)y,0.6,21.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);  //Use the whole window for rendering
}
void updateScreen(){
    glutPostRedisplay();
}
void myKeyboard(unsigned char key,int x,int y){
    switch(key){
        case 'z':
            if(cview!=0)
                cview--;
            nmov *= 2;
            fmov *= 2;
            topmov *= 2;
            botmov *= 2;
            leftmov *= 2;
            rightmov *= 2;
            
            break;
        case 'x':
            if(cview < 33)
                cview++;
            nmov /= 2;
            fmov /= 2;
            topmov /= 2;
            botmov /= 2;
            leftmov /= 2;
            rightmov /= 2;
            // viewNext();
            break;
    }
    pos = 0;
    glutPostRedisplay();
}
int main(int argc,char** argv){
    rc = sqlite3_open("planets.db",&db);
    if(rc){
        fprintf(stderr,"Cannot open database: %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    rc = sqlite3_exec(db,"SELECT * FROM Planets",callback,0,&error);
    if(rc!=SQLITE_OK){
        fprintf(stderr,"Error:%s\n",error);
        sqlite3_free(error);
    }
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("Scale of the Universe");
    // glutFullScreen();
    createMenu();
	glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboard);
    // glutIdleFunc(display);
	glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}