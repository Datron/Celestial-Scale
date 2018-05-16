#include<iostream>
#include<string>
#include<GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Planet.h"
using namespace std;
Planet::Planet(){
    
}
Planet::Planet(string name,double radius,string units,string facts,string description,string texLoc){
    this->name = name; this->radius = radius; this->units = units;
    this->facts = facts; this->description = description; this->texLoc = texLoc;
}
void Planet::render(double x,double y,double z,GLuint id){
    int w;
    int h;
    int comp;
   unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    unsigned char *data = stbi_load("bmp/earth.png", &w, &h, &comp, STBI_rgb_alpha);
    if (data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        cout << "Failed to load texture" << endl;
    stbi_image_free(data);
    glTranslated(x,y,z);
    // glTranslated(0.0,this->radius,0.0);
    glutSolidSphere(this->radius,80,80);
    glTexCoord2f(x,y);
    glDisable(GL_TEXTURE_2D);
}
double Planet::getRadius(){
    return this->radius;
}
Planet::~Planet(){
    cout << this->name << "\t\t" << this-> radius << "\t\t" << this->units << "\t\t" << this->facts << "\t\t" << this->texLoc <<endl;
}