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
    GLfloat diffusecoef[] = {1.0,1.0,1.0,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffusecoef);
    glTranslated(x+2,y,z);
    //glTranslated(0.0,this->position,0.0);
    glutSolidSphere(this->radius,80,80);
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
    this->position = position;
}
double Planet::getPosition(){
    cout << "My position " << this->position <<endl;
    return this->position;
}
Planet::~Planet(){
    cout << this->name << "\t\t" << this-> radius << "\t\t" << this->units << "\t\t" << this->facts << "\t\t" << this->texLoc <<endl;
}