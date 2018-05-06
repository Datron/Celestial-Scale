#include<iostream>
#include<string>
#include<GL/glut.h>
#include "Planet.h"
using namespace std;
Planet::Planet(){
    cout << "Array is here" <<endl;
}
Planet::Planet(string name,double radius,string units,string facts,string description,string texLoc){
    this->name = name; this->radius = radius; this->units = units;
    this->facts = facts; this->description = description; this->texLoc = texLoc;
}
void Planet::render(double x,double y,double z){
    glColor3f(1.0,0.0,0.0);
    glTranslated(x,y,z);
    glutSolidSphere(this->radius,80,80);
}
double Planet::getRadius(){
    return this->radius;
}
Planet::~Planet(){
    cout << this->name << "\t\t" << this-> radius << "\t\t" << this->units << "\t\t" << this->facts << "\t\t" << this->description <<endl;
}