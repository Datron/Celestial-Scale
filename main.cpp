#include<iostream>
#include<string>
#include<GL/glut.h>
#include<sqlite3.h>
#include "Planet.h"
using namespace std;
static int window,menu_id,go_to_submenu_id,music_submenu_id,rotate_submenu_id,background_submenu_id,translate_submenu_id;
// menu choice variable
int choice=-1;
// how much to move by when 'z' or 'x' key is pressed
int movement=0;
// a variable to decide distance between planets
double pos=-0.0;
// how much to zoom out to display the new object
int zoom;
// db variables
sqlite3 *db;
char *error = nullptr;
int rc;
Planet celestial[33];
int celes_count=0;
static int callback(void *data,int argc,char** argv,char** azColName){
    int i;
    cout << data <<endl;
    for(i=0;i<3;i+=3){
        celestial[celes_count++] = Planet(argv[i],atof(argv[i+1])/1000,argv[i+2],"|","|","|");
    }
    return 0;
}
void menu(int num){
    if(num == 13){
        glutDestroyWindow(window);
        exit(0);
    }
    else
        choice = num;
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-1000,1000,-1000,1000,-1000,1000);
    glColor3f(1.0,0.0,0.0);
    for(i=0;i<celes_count;i++){
        celestial[i].render(pos,0.0,0.0);
        pos += celestial[i].getRadius();
    }
    // glColor3f(1.0,0.0,0.0);
    // glTranslatef(movement,0,-1);
    // glutSolidSphere(200, 60, 60);
    // glTranslatef(movement+500,0,-200);
    // glColor3f(0.0,0.6,0.7);
    // glutSolidSphere(200, 60, 60);
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
void viewNext(int x,int y,int z){
    glTranslated(x,y,z);
    printf("%d %d %d\n",x,y,z);
}
void myKeyboard(unsigned char key,int x,int y){
    switch(key){
        case 'z':
            movement-=100;
            viewNext(movement,0.0,0.0);
            break;
        case 'x':
            movement+=100;
            viewNext(movement,0.0,0.0);
            break;
    }
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
    universe_init();
    // glutFullScreen();
    createMenu();
	glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboard);
	glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    sqlite3_close(db);
}