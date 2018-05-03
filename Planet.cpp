#include<iostream>
#include<string>
#include "Planet.h"
using namespace std;

Planet::Planet(string name,double radius,string facts,string description){
    this->name = name; this->radius = radius;
    this->facts = facts; this->description = description;
}
void Planet::render(double x,double y,double z){
    cout << x << "\n" << y << "\n" << z << endl;
}
Planet::~Planet(){
    cout << this->name << "\t" << this-> radius << "\t" << this->facts << "\t" << this->description <<endl;
}