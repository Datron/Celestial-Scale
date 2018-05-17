#include<iostream>
#include<string>
#include<GL/glut.h>
#include "Planet.h"
using namespace std;
Planet::Planet(){
    
}
Planet::Planet(string name,double radius,string units,string facts,string description,string texLoc){
    this->name = name; this->radius = radius; this->units = units;
    this->facts = facts; this->description = description; this->texLoc = texLoc;
}
void Planet::render(double x,double y,double z){
    glTranslated(x,y,z);
    // glTranslated(0.0,this->radius,0.0);
    glutSolidSphere(this->radius,80,80);
}
double Planet::getRadius(){
    return this->radius;
}
string Planet::getUnit(){
    return this->units;
}
string Planet::getTexLoc(){
    return "bmp/"+this->texLoc;
}
Planet::~Planet(){
    cout << this->name << "\t\t" << this-> radius << "\t\t" << this->units << "\t\t" << this->facts << "\t\t" << this->texLoc <<endl;
}