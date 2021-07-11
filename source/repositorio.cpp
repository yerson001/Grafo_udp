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

int BUFFER = 100;

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
//obtiene el valor del nodo y pa profundidad
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

void inserciones(string nodo, int a, int r, string name_db)
{
    SQlite db(name_db);
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
void read(string &structure, string name_db)
{
    string nombre_nodo("");
    string atributos("");
    string relaciones("");
    string name_atributos("");

    int size_nodo = string_int(structure.substr(1, 3));
    structure = structure.substr(4, BUFFER);
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
    inserciones(nombre_nodo, number_attributes, number_relations, name_db);
}
void writing(int sock, string name_db)
{
    SQlite db(name_db);
    int n, write_sise;

    char send_data[BUFFER];
    socklen_t addr_len;
    string structure = "R";
    //cout<<"MSG: ";
    //cin>>structure;
    n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    while (true)
    {
        //printf("msg server-----repo");
        n = recvfrom(sock, send_data, BUFFER, 0, (struct sockaddr *)&server_addr, &addr_len);

        // Store the id_client
        string id_client(send_data, 1);
        string s(send_data, BUFFER);
        cout << "server-->client[completo]: " << s << endl;
        cout << "ID_CLIENTE: " << id_client << endl;
        //eliminar id_cliente pa el identificador
        s.erase(0, 1);
        cout << "server-->client[elinando]: " << s << endl;

        //eliminamos el dolar
        s = cortar(s);

        cout << "server-->client[procesado]: " << s << endl;
        //elimanos contador
        if (n < 0)
        {
            cout << "connect" << endl;
        }
        if (s[0] == 'c')
        {
            read(s, name_db);
        }
        if (s[0] == 'r')
        {
            db.init();
            s.erase(0, 1);
            cout << "S->R: " << s << endl;
            string nodo = s;
            int dep = get_value_dep(nodo);
            //separamos el nombre y la prufundidad
            cout << "nodo: " << nodo << "-->" << dep << endl;
            // todas las relacioens retorna ---> nodo = 66  66|12|66|77|66|88|
            string todos_relaciones = db.select_dep("relaciones", nodo);
            //Eliminar la columna nodo_inicial y guardar en un vector los valores
            vector<string> los_niveles = get_value_dep(todos_relaciones, nodo);
            //mostramos los niveles
            for (int i = 0; i < los_niveles.size(); i++)
            {
                cout << "into niveles: " << los_niveles[i] << endl;
            }

            string respuesta = db.select_dep("relaciones", nodo, dep);
            respuesta = "d" + id_client + respuesta;
            cout << "SELECT->DEP: (R->M) " << respuesta << endl;
            if (respuesta.size() != 0)
            {
                n = sendto(sock, respuesta.c_str(), respuesta.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
            }

            respuesta.clear();
            id_client.clear();
        }

        //n = sendto(sock,structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

        bzero(send_data, BUFFER);
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
    string name_db = argv[1];
    name_db = name_db + ".db";
    cout << "NAME:" << name_db << endl;
    writing(sock, name_db);
}