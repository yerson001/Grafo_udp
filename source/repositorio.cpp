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
#include "SQlite.h"
using namespace std;
struct sockaddr_in server_addr;
struct hostent *host;
vector<string> nombre_atributos;
vector<string> valor_atributos;
vector<string> valor_relaciones;
SQlite db("mydata.db");

int string_int(string s)
{
    stringstream geek(s);
    int x = 0;
    geek >> x;
    return x;
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

string cortar(string m)
{
    string str(m);
    int dolar = str.find("$");
    str.erase(dolar, str.size());
    //cout << str << endl;
    return str;
}

void get_values(vector<string> &arr, string data, int nn)
{
    string str(data);
    //sql.init();
    //cout<<sql.select_db("nodos")<<endl;
    for (int i = 0; i < nn; i++)
    {
        int found = str.find(",");
        //cout<<found<<endl;
        std::string str2 = str.substr(0, found);
        //cout<<str2<<endl;
        arr.push_back(str2);
        str.erase(0, found + 1);
    }

    //for(int i=0; i<arr.size(); i++){cout<<arr[i]<<endl;}
}

void inserciones(string nodo, int a, int r)
{
    db.init();
    db.insert_db("'nodos'", nodo);
    //atributos
    for (int i = 0; i < a; i++)
    {
        db.insert_db("atributos", nodo + "," + nombre_atributos[i] + "," + valor_atributos[i]);
    }
    //relaciones
    for (int i = 0; i < r; i++)
    {
        db.insert_db("relaciones", nodo + "," + valor_relaciones[i]);
    }
    nombre_atributos.clear();
    valor_atributos.clear();
    valor_relaciones.clear();
}

void read(string &structure)
{
    string nombre_nodo("");
    string atributos("");
    string relaciones("");
    string name_atributos("");

    int size_nodo = string_int(structure.substr(1, 3));
    structure = structure.substr(4, 1000);
    int number_attributes = string_int(structure.substr(0, 2));
    structure = structure.substr(2, structure.size());
    int number_relations = string_int(structure.substr(0, 3));
    structure = structure.substr(3, structure.size());
    string name_node = structure.substr(0, size_nodo);
    structure = structure.substr(size_nodo, structure.size());
    string campos = "";

    for (int i = 0; i < number_attributes; i++)
    {
        int size_atr = string_int(structure.substr(0, 3));
        structure = structure.substr(3, structure.size());
        int size_valor = string_int(structure.substr(0, 3));
        structure = structure.substr(3, structure.size());
        string name_valor = structure.substr(0, size_atr);
        structure = structure.substr(size_atr, structure.size());
        string valor = structure.substr(0, size_valor);
        structure = structure.substr(size_valor, structure.size());
        campos += "'" + valor + "',";
        name_atributos += "'" + name_valor + "',";
    }
    name_node = "'" + name_node + "'";

    nombre_nodo = name_node;
    atributos = campos;

    if (number_attributes)
    {
        campos = campos.substr(0, campos.size() - 1);
        campos = "";
    }
    for (int i = 0; i < number_relations; i++)
    {
        int size_rel = string_int(structure.substr(0, 3));
        structure = structure.substr(3, structure.size());
        string n_node = structure.substr(0, size_rel);
        structure = structure.substr(size_rel, structure.size());
        campos += "'" + n_node + "',";
    }
    relaciones = campos;
    get_values(nombre_atributos, name_atributos, number_attributes);
    get_values(valor_atributos, atributos, number_attributes);
    get_values(valor_relaciones, relaciones, number_relations);

    for (int i = 0; i < nombre_atributos.size(); i++)
    {
        cout << nombre_atributos[i] << endl;
    }
    for (int i = 0; i < valor_atributos.size(); i++)
    {
        cout << valor_atributos[i] << endl;
    }
    for (int i = 0; i < valor_relaciones.size(); i++)
    {
        cout << valor_relaciones[i] << endl;
    }

    if (number_relations)
    {
        campos = campos.substr(0, campos.size() - 1);
        campos = "";
    }
    inserciones(nombre_nodo, number_attributes, number_relations);
}
void writing(int sock)
{
    int n, write_sise;
     
    char send_data[1000];
    socklen_t addr_len;
    string structure = "R";
    //cout<<"MSG: ";
    //cin>>structure;
    n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    while (true)
    {
        //printf("msg server-----repo");
        n = recvfrom(sock, send_data, 1000, 0, (struct sockaddr *)&server_addr, &addr_len);

        string id_client(send_data,1);
        string s(send_data, 1000);
         cout<<"ID_CLIENTE: "<<id_client<<endl;
         //eliminar id_cliente
        //s.erase(0,1);

        //eliminamos el dolar
        s = cortar(s);

        cout<<"server-->client: "<<s<<endl;
        //elimanos contador
        if (n < 0)
        {
            cout << "connect" << endl;
        }
        if (send_data[0] == 'c')
        {
            read(s);
        }
        if (send_data[0] == 'r')
        {
            db.init();
            s.erase(0,1);
            string respuesta = db.select_db(s);
            //cout << "SS:" << s << endl;
            cout << "SELECT: " << respuesta << endl;
            respuesta = "d" + respuesta;

            if (respuesta.size() != 0)
            {
                n = sendto(sock, respuesta.c_str(), respuesta.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
            }
            respuesta.clear();
        }

        //n = sendto(sock,structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

        bzero(send_data, 1000);
    }
}

int main(int argc, char *argv[])
{
    int sock;
    host = (struct hostent *)gethostbyname((char *)"127.0.0.1");
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero), 8);
    string name_db;
    //cout<<"name database"<<endl;
    //cin>>name_db;
    //name_db=name_db+".db";
    //SQlite db("mydata.db");
    //db.init();
    writing(sock);
}