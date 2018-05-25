#include<iostream>
#include<string>
#include<math.h>
#include<GL/glut.h>
#include<sqlite3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Planet.h"
using namespace std;

struct camera_pos {
    double x,y,z,posx,posy,posz;
} camera;
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
double nmov = -64,
        fmov = 64,
     topmov = -64,
     botmov =  64,
     leftmov= -64,
     rightmov= 64;
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
// rotate parameter
double theta = 0.0;
int rotate = 0;
// function prototypes
void strokeString();
void front();
void rotation(int n);
void display();
void myKeyboard(unsigned char,int,int);
void animate(int n);
double findPlanetPos(int c);
void menu(int num);
void universe_init();
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
    cout << "Planets rendered " << celes_count <<endl;
    return 0;
}
void menu(int num){
    int i;
    if(num == 27){
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
    int i;
    go_to_submenu_id = glutCreateMenu(menu);
    for(i=0;i<celes_count;i++)
        glutAddMenuEntry((celestial[i]->getName()).c_str(),i+1);
    menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Start",0);
    glutAddSubMenu("Go to...",go_to_submenu_id);
    glutAddMenuEntry("Background",24);
    glutAddMenuEntry("Rotate",25);
    glutAddMenuEntry("Animate",26);
    glutAddMenuEntry("Quit",27);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
double findPlanetPos(int c){
    cout << "Finding Planet "+celestial[c]->getName() << celestial[c]->getPosition()<< endl;
    choice = 0; 
    return celestial[c]->getPosition();
}
void animate(int n){
    myKeyboard('z',0,0);
    myKeyboard('c',0,0);
    if(cview < 24)
        glutTimerFunc(1,animate,0);
}
void rotation(int n){
    if(choice != -1)
        myKeyboard('y',0,0);
    glutTimerFunc(1,rotation,0);
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
    glColor3f(0.0,0.0,0.0);
    if(choice == -1){
        front();
    }
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
            gluLookAt(1.0,0.0,0.0,0.0,0.0,-camera.posx,0.0,1.0,0.0);
            strokeString(camera.posx-(cview*50),botmov/2,fmov/1600,fmov/1600,celestial[cview]->getName(),2);
            strokeString(camera.posx-(cview*100),topmov/1.5,fmov/1600,fmov/1600,celestial[cview]->getFacts(),2);
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
                glPushMatrix();
                glRotated(theta,0.0,1.0,0.0);
                celestial[i]->setPosition(pos);
                celestial[i]->render(pos,0.0,0.0);
                glPopMatrix();
                glTranslated(pos+2,0.0,0.0);
            }
            glDisable(GL_TEXTURE_2D);
            
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            pos = camera.posy;
            camera.posx = findPlanetPos(choice-1);
            cout << "camera.posx value " << camera.posx <<endl;
            glutPostRedisplay();
            break;
        case 24:
            bgr = bgb = bgg = 1.0;
            break;
        case 25:
            if(rotate == 1){
                rotate = 0;
                glutTimerFunc(100,nullptr,0);
            }
            else {
                rotate = 1;
                glutTimerFunc(10,rotation,0);
            }
            choice = 0;
            break;
        case 26:
            glutTimerFunc(100,animate,0);
            break;
    }
    glutSwapBuffers();
    glFlush();
}
void reshape(int x, int y){
    if (y == 0 || x == 0) return;   
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,x);
    glTranslated(0,-x,0);  
}
void myKeyboard(unsigned char key,int x,int y){
    
    switch(key){
        case 'z':
            if(cview < celes_count-1)
                cview++;
            pos = 0;
            camera.posx = celestial[cview-1]->getRadius()+celestial[cview]->getRadius()*2;
            pos -= camera.posx;
            camera.posy = pos;
            break;
        case 'x':
            if(cview >= 1)
                cview--;
            pos = camera.posy;
            if(cview-1 != 0){
                camera.posx = celestial[cview]->getRadius()*2;
                pos += camera.posx;
            }
            camera.posy = pos;
            break;
        case 'c':
            pos = camera.posy;
            nmov *= 1.52;
            fmov *= 1.52;
            topmov *= 1.52;
            botmov *= 1.52;
            leftmov *= 1.52;
            rightmov *= 1.52;
            break;
        case 'y':
            pos = 0;
            nmov *= 1;
            fmov *= 1;
            topmov *= 1;
            botmov *= 1;
            leftmov *= 1;
            rightmov *= 1;
            if(rotate)
                theta += 2;
            if(theta > 360)
                theta = 0.0;
            cout << "in Timer:" << theta << endl;
            break;
        case 'v':
            pos = camera.posy;
            nmov /= 1.52;
            fmov /= 1.52;
            topmov /= 1.52;
            botmov /= 1.52;
            leftmov /= 1.52;
            rightmov /= 1.52;
            break;
    }
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
    glGenTextures(33,texture);
	glutDisplayFunc(display);
    //glutTimerFunc(10,rotation,0);
    glEnable(GL_LIGHTING);
    glutKeyboardFunc(myKeyboard);
    glutReshapeFunc(reshape);
	glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
