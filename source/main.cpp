#include <iostream>
#include <vector>
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
#include <algorithm>

using namespace std;

vector<string> nombre_atributo;

string no_id_client(string &name, int &client)
{
    string primera = name.substr(0, 1);
    string segunda = name.substr(2, name.size());
    //cout<<primera<<"   "<<segunda<<"  "<<name.substr(1,1)<<endl;
    client = stoi(name.substr(1, 1));
    return primera + segunda;
}

string get_value(string &name)
{
    int d = name.find("*");
    name.erase(d, name.size());
    name.erase(0, 1);
    return name;
}
int string_int(string s)
{
    stringstream geek(s);
    int x = 0;
    geek >> x;
    return x;
}
int get_value_dep(string &name)
{
    string to_dep = name;
    int d = name.find("*");
    int dep = 0;
    name.erase(d, name.size());
    dep = string_int(to_dep.erase(0, d + 1));
    //cout<<"---> "<<dep<<endl;
    return dep;
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
//funcion para contar en numero de colunnas de una consulta sqlite
// equivalente a COUNT(*)
int num_colum(string s)
{
    return (count(s.begin(), s.end(), '|')) / 2;
}

//funcion para sacar las relacioens de este nodo y guardarlos en un vector
vector<string> get_value_dep(string query, string value)
{
    vector<string> niveles;
    //cout<<"num_ieracion: "<<num_colum(query)<<endl;
    int it = num_colum(query);
    it = it * 2;
    for (int i = 0; i < it; i++)
    {
        int d = query.find("|");
        //cout << "str: " << query.substr(0, d) << endl;
        if (value == query.substr(0, d))
        {
            query.erase(0, d + 1);
            //cout<<"quwr"<<query<<endl;
        }
        else
        {
            niveles.push_back(query.substr(0, d));
            query.erase(0, d + 1);
        }
    }
    return niveles;
}

vector<string> get_value_dep_filter(string &query, string value)
{
    vector<string> niveles;
    //cout<<"num_ieracion: "<<num_colum(query)<<endl;
    int it = num_colum(query);
    it = it * 2;
    for (int i = 0; i < it; i++)
    {
        int d = query.find("|");
        //cout << "str: " << query.substr(0, d) << endl;
        if (value == query.substr(0, d))
        {
            query.erase(0, d + 1);
            //cout<<"quwr"<<query<<endl;
        }
        else
        {
            niveles.push_back(query.substr(0, d));
            query.erase(0, d + 1);
        }
    }
    return niveles;
}


void start(string name)
{
    SQlite sql(name);
    sql.init();
    string todos_relaciones = sql.select_dep("relaciones", "66");
    cout << "query: " << todos_relaciones << endl;
    cout << "columnas: " << num_colum(todos_relaciones) << endl;

    vector<string> los_niveles = get_value_dep(todos_relaciones, "66");
    for (int i = 0; i < los_niveles.size(); i++)
    {
        cout << "into niveles: " << los_niveles[i] << endl;
    }
}

//funcion para saber el numero de repositorio a travez
// de su nombre   repo0.db
// retorna 0 en int 
int num_repo(string name)
{
    string to_dep = name;
    int d = name.find(".");
    int dep = 0;
    name.erase(d-2,d);
    dep = string_int(name);
    //cout<<"---> "<<dep<<endl;
    return dep;
}

map<int,vector<int>> nivel_2;
vector<int> actual_nivel;
string cortar(string m)
{cout<<"entrando"<<endl;
    string str(m);
    int dolar = str.find("$");
    str.erase(dolar, str.size());
    //cout << str << endl;
    return str;
}

int main()
{
    start("repo0.db");

    string test = "d066|12|66|77|66|88|";
    string it = "77|66|$0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    string dd;
    
   
    if(it.find("$")<100){
        dd= cortar(it);
    }
     
    cout<<"it -- "<<dd<<endl;


    nivel_2.insert(pair<int,vector<int>>(1,actual_nivel));



    vector<string> op;
    string text = "12|22|12|10|12|11|77|66|88|66|";

    op =  get_value_dep_filter(text, "12");
    for(int i=0; i<op.size(); i++){
        cout<<"_-->"<<op[i];
    }cout<<endl;
    
    op =  get_value_dep_filter(text, "77");
        for(int i=0; i<op.size(); i++){
        cout<<"_-->"<<op[i];
    }cout<<endl;
    
    op =  get_value_dep_filter(text, "88");
    for(int i=0; i<op.size(); i++){
        cout<<"_-->"<<op[i];
    }cout<<endl;

    //string value="656*2";
    //int dep = get_value_dep(value);
    //cout<<value<<" "<<dep<<endl;

    //cout<<"-----"<<endl;
    //cout<<sql.select_db("nodos")<<endl;
    //string name = "d5|A|B|C";
    //int c;
    //cout<<no_id_client(name,c)<<"----"<<c+2<<endl;
    //int cc = 4;
    //cout<<int_to_string(cc,2)<<endl;

    return 0;
}