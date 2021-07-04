
#include<iostream>
#include<vector>
#include <sqlite3.h>
#include "SQlite.h"
#include <map>
using namespace std;
SQlite sql("mydata.db");
vector<string> nombre_atributo;

int main(){
    sql.init();
    cout<<"-----"<<endl;
    cout<<sql.select_db("nodos")<<endl;

    return 0;
}
