#include<iostream>
#include<sqlite3.h>
#include "Planet.h"
using namespace std;

static int callback(void *data,int argc,char** argv,char** azColName){
    int i;
    cout << data <<endl;
    for(i=0;i<3;i++)
        cout << azColName[i] << ":" << argv[i] << endl;
    return 0;
}

int main(int argc,char** argv){
    sqlite3 *db;
    char *error = nullptr;
    int rc;
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
    sqlite3_close(db);
}