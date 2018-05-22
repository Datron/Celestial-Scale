#include<iostream>
#include<string>
#include<math.h>
#include<GL/glut.h>
#include<sqlite3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Planet.h"
using namespace std;
// background colors
double bgr = 0.0,bgg = 0.0,bgb = 0.0;
static int window,menu_id,go_to_submenu_id,music_submenu_id,rotate_submenu_id,background_submenu_id,translate_submenu_id;
// menu choice variable
int choice=-1;
// variable that manages the current planet being viewed
int cview = 0;
// lighting parameters
GLfloat lightpos[] = {0.0,0.0,1.0,0.0};
GLfloat lightam[] = {1.0,1.0,1.0,1.0};
GLfloat lightdif[] = {1.0,1.0,1.0,1.0};
// how much to move by when 'z' or 'x' key is pressed using glOrtho
double nmov = -32,
        fmov = 32,
     topmov = -32,
     botmov =  32,
     leftmov= -32,
     rightmov= 32;
// a variable to decide distance between planets
double pos=0.0,posy = 0.0;
// how much to zoom out to display the new object
int zoom;
// db variables
sqlite3 *db;
char *error = nullptr;
int rc;
Planet* celestial[33];
GLuint texture[33];
int celes_count=0;

// program starts here
void strokeString(float x,float y,float sx,float sy,string string,int width){
    const char *c = string.c_str();
    int i=0;
    glLineWidth(width);
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(sx,sy,0);
    for (i=0; c[i] != '\0'; i++)
         glutStrokeCharacter(GLUT_STROKE_ROMAN, c[i]);
    glPopMatrix();
}
void front(){
    glPushMatrix();
    gluOrtho2D(0,1000,-100,1000);
    glClearColor(1.0,1.0,1.0,1.0);
    glColor3f(0.6,0.6,0);
    strokeString(270,600,0.18,0.18,"BANGALORE INSTITUTE OF TECHNOLOGY",2);
    glColor3f(1,0,0);
    strokeString(130,550,0.3,0.3,"DEPARTMENT OF COMPUTER SCIENCE",4);
    strokeString(330,500,0.3,0.3,"AND ENGINEERING",4);
    strokeString(200,450,0.3,0.3,"AN OPENGL MINI PROJECT ON",2);
    glColor3f(0,0.5,1);
    strokeString(200,400,0.43,0.4,"Scale of The Universe",6);
    glColor3f(0,0.5,0);
    strokeString(350,240,0.15,0.15,"Select start to continue",2);
    glColor3f(0,0.5,1);
    strokeString(50,120,0.18,0.18,"Kartik G",2);
    strokeString(50,80,0.18,0.18,"1BI15CS068",2);
    strokeString(750,120,0.2,0.2,"Prof K.J.Banushree",2);
    strokeString(750,80,0.2,0.2,"Dr M.Kempanna",2);
    strokeString(750,40,0.2,0.2,"Prof Prathima.M.G",2);
    glColor3f(1,0,0);
    strokeString(50,160,0.15,0.15,"By:",2);
    strokeString(750,160,0.15,0.15,"Under the guidance of ",2);
    glPopMatrix();
}
static int callback(void *data,int argc,char** argv,char** azColName){
    int i;
    for(i=0;i<6;i+=6){
            // cout << argv[i+6] << endl;
            celestial[celes_count++] =new Planet(argv[i],atof(argv[i+1])/100,argv[i+2],"|","|",argv[i+6]);
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
    rotate_submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Enable",11);
    glutAddMenuEntry("Disable",12);
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Start",0);
    glutAddSubMenu("Go to...",go_to_submenu_id);
    glutAddMenuEntry("Background",9);
    glutAddMenuEntry("Lighting",10);
    glutAddSubMenu("Rotation",rotate_submenu_id);
    glutAddMenuEntry("Quit",13);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void universe_init(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(bgr,bgg,bgb,1.0);
    glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightam);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightdif);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
}
void display(){
    universe_init();
    if(choice == -1)
        front();
    int i;
    int w;
    int h;
    int comp;
    switch(choice){
        case 0:
            // glColor3f(1.0,0.0,0.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            printf("cview = %d pos = %f %f %f %f %f %f %f\n\n",cview,pos,nmov,fmov,topmov,botmov,leftmov,rightmov);
            glOrtho(nmov,fmov,topmov,botmov,leftmov,rightmov);
            strokeString(pos-(cview+1)*5,botmov/3,fmov/1600,fmov/1600,celestial[cview]->getName(),2);
            strokeString(pos-(cview+1)*5,topmov/2,fmov/1600,fmov/1600,celestial[cview]->getFacts(),2);
            for(i=0;i<celes_count;i++){
                if(i!=0) 
                    pos = celestial[i]->getRadius()*2.1;
                    // celestial[i]->getRadius()/(100*(cview+1))
                
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
                glBindTexture(GL_TEXTURE_2D, texture[i]);
                glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // set the texture wrapping/filtering options (on the currently bound texture object)
                // load and generate the texture
                string texMap = celestial[i]->getTexLoc();
                unsigned char *data = stbi_load(texMap.c_str(), &w, &h, &comp, STBI_rgb_alpha);
                if (data)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                else
                    cout << "Failed to load texture" << endl;
                stbi_image_free(data);
                // gluLookAt(pos,0.0,-1.0,nmov,0.0,0.0,1.0,0.0,0.0);
                // gluLookAt(pos,0.0,-1.0,pos,0.0,0.0,0.0,0.0,0.0);
                celestial[i]->render(pos,0.0,0.0);
            }
            glDisable(GL_TEXTURE_2D);
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
            bgr = bgb = bgg = 1.0;
            break;
        case 10:
            for(i=0;i<4;i++)
                lightam[i] = 0.1;
            glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
            glLightfv(GL_LIGHT0,GL_AMBIENT,lightam);
            glLightfv(GL_LIGHT0,GL_DIFFUSE,lightdif);
            glEnable(GL_LIGHT0);
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
            if(cview < celes_count-1)
                cview++;
            cout << celestial[cview]->getRadius() <<endl;
            if(nmov < celestial[cview]->getRadius()){
                nmov *= 2;
                fmov *= 2;
                topmov *= 2;
                botmov *= 2;
                leftmov *= 2;
                rightmov *= 2;
            }
            glTranslated(celestial[cview]->getRadius(),0.0,0.0);
            break;
        case 'x':
            if(cview != 0)
                cview--;
            cout << celestial[cview]->getRadius() <<endl;
            nmov /= 2;
            fmov /= 2;
            topmov /= 2;
            botmov /= 2;
            leftmov /= 2;
            rightmov /= 2;
            //glTranslated(rightmov,0.0,0.0);
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
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("Scale of the Universe");
    createMenu();
    //glutFullScreen(); // don't enable this.
    glGenTextures(33,texture);
	glutDisplayFunc(display);
    glEnable(GL_LIGHTING);
    glutKeyboardFunc(myKeyboard);
    // glutIdleFunc(display);
	glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
