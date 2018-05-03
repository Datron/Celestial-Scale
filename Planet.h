#include<string>
using namespace std;
class Planet {
    public:
        Planet(string name,double radius,string facts,string description);
        void render(double,double,double);
        ~Planet();
    private:
        string name,facts,description;
        double radius;
};