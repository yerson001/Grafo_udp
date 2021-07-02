
#include<iostream>
#include <sqlite3.h>
#include "SQlite.h"

using namespace std;
SQlite sql("mydata.db");
int main(){
    sql.init();
    cout<<sql.select_db("nodos")<<endl;
    return 0;
}
