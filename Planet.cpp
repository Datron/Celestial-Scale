#include<iostream>
#include<string>
using namespace std;
class Planet {
    public:
        Planet(){
            cout<<"initialized"<<endl;
        }
        void init(string name){
            this->name = name;
            cout << name << endl;
        }
        void render(double x,double y,double z);
        ~Planet(){
            cout<< "Planet deleted"<<endl;
        }
    private:
        string name,facts,description;
        double radius;
};