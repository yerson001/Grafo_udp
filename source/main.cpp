
#include<iostream>
#include<vector>
#include <sqlite3.h>
#include "SQlite.h"
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>
#include <sstream>
using namespace std;
SQlite sql("mydata.db");
vector<string> nombre_atributo;


string no_id_client(string &name,int &client){
    string primera = name.substr(0,1);
    string segunda=name.substr(2,name.size());
    int d = name.find("|");
    name.erase(0,1);
    name.erase(d-1,name.size());
    client = stoi(name);
    return primera+segunda;
}
string int_to_string(int number, int digits)
{
    string ans = "";
    while (number)
    {
        ans.push_back('0' + (number % 10));
        number /= 10;
    }
    reverse(ans.begin(), ans.end());
    return string(digits - ans.size(), '0') + ans;
}

int main(){
    sql.init();
    cout<<"-----"<<endl;
    //cout<<sql.select_db("nodos")<<endl;
    string name = "d5|A|B|C";
    int c;
    cout<<no_id_client(name,c)<<"----"<<c+2<<endl;
    int cc = 4;
    cout<<int_to_string(cc,2)<<endl;


    return 0;
}
