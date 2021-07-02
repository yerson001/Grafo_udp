#include<iostream>
#include <sqlite3.h>
#include "SQlite.h"

using namespace std;
SQlite sql("mydata.db");
int main(){
    sql.init();
    sql.select_db("nodos");
    return 0;
}