#include<iostream>
#include<string>
#include<sstream>
#include<GL/glut.h>
#include "Planet.h"
using namespace std;
Planet::Planet(){
    cout << "Planet created" <<endl;
}
Planet::Planet(string name,double radius,string units,string facts,string description,string texLoc){
    this->name = name; this->radius = radius; this->units = units;
    this->facts = facts; this->description = description; this->texLoc = texLoc;
}
void Planet::render(double x,double y,double z){
    glPushMatrix();
            glBegin(GL_LINES);
            glVertex2f(0,0);
            glVertex2f(x+2,0);
            glEnd();
    glPopMatrix();
    glTranslated(x+2,this->radius,z);
    //glTranslated(0.0,this->position,0.0);
    glutSolidSphere(this->radius,80,80);
}
void Planet::renderRing(double pos,double radius){
    int i;
    glRotated(90.0,0.0,1.0,0.0);
    glRotated(-130.0,0.0,1.0,1.0);
    for(i=0;i<10;i++)
        glutSolidTorus(30,radius+i*100,80,100);
}
double Planet::getRadius(){
    return this->radius;
}
string Planet::getUnit(){
    return this->units;
}
string Planet::getName(){
    return this->name;
}
string Planet::getFacts(){
    ostringstream s;
    s << (long int)this->radius*100;
    string str = s.str();
    return "Radius:"+str+ this->units;
}
string Planet::getTexLoc(){
    return "bmp/"+this->texLoc;
}
void Planet::setPosition(double pos){
    this->position = pos;
}
double Planet::getPosition(){
    cout << "My position " << this->position <<endl;
    return this->position;
}
Planet::~Planet(){
    cout << this->name << "\t\t" << this-> radius << "\t\t" << this->units << "\t\t" << this->facts << "\t\t" << this->texLoc <<endl;
}