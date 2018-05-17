#include<string>
using namespace std;
class Planet {
    public:
        Planet();
        Planet(string name,double radius,string units,string facts,string description,string texLoc);
        void render(double,double,double);
        double getRadius();
        string getUnit();
        string getTexLoc();
        void setSecondaryTextPrimitives(double,double,double);
        ~Planet();
    private:
        void loadTexture();
        string name,facts,description,units;
        string texLoc;
        double radius;
        double scaling_factor;
        GLuint texture[33];
};