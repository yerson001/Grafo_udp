
#include<iostream>
#include<vector>
#include <sqlite3.h>
#include "SQlite.h"

using namespace std;
SQlite sql("mydata.db");
vector<string> nombre_atributo;
int main(){
    string str("'D','E','S',");
    //sql.init();
    //cout<<sql.select_db("nodos")<<endl;
    for(int i=0; i<3;i++){
        int found = str.find(",");
        //cout<<found<<endl;
        std::string str2 = str.substr (0,found);
        //cout<<str2<<endl;
        nombre_atributo.push_back(str2);
        str.erase (0,found+1); 
    }

    for(int i=0; i<nombre_atributo.size(); i++){cout<<nombre_atributo[i]<<endl;}
    

    return 0;
}
