#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>
#include <sstream>
//#include <sqlite3.h>
#include <map>

int BUFFER = 100;

using namespace std;

struct sockaddr_in server_addr, client_addr;
vector<struct sockaddr_in> clientsAddr;
vector<struct sockaddr_in> repositories;

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

// Store the client address into clientsAddr and verified there are no repetitions
// Return the id of the client
string storeClientAddress()
{
    string id_client;
    for (int i = 0; i < clientsAddr.size(); ++i)
    {
        // If the address exists, just return the id (position in the clientsAdrr)
        if (ntohs(client_addr.sin_port) == ntohs(clientsAddr[i].sin_port))
        {
            id_client = int_to_string(i, 1);
            return id_client;
        }
    }
    // The address doesn't exist, so it's store into clientsAddr and return
    // it's position
    clientsAddr.push_back(client_addr);
    id_client = int_to_string(clientsAddr.size() - 1, 1);
    return id_client;
}

int string_int(string s)
{
    stringstream geek(s);
    int x = 0;
    geek >> x;
    return x;
}
// fucnion para eliminar el indice del cliente 
// para saber que cliente es y no alterar la cadena
// d|cliente01|resconsulta -> d|resconsulta --> sola del [0..9]
string no_id_client(string &name, int &client)
{
    string primera = name.substr(0, 1);
    string segunda = name.substr(2, name.size());
    //cout<<primera<<"   "<<segunda<<"  "<<name.substr(1,1)<<endl;
    client = stoi(name.substr(1,1));
    return primera + segunda;
}

//*************hash*******************
int hashFunction(int x, int SIZE)
{
    return (x % SIZE);
}
string get_value(string name){
    int d = name.find("*");
    name.erase(d,name.size());
    name.erase(0,1);
    return name;
}

void reading(int sock)
{
    char recv_data[BUFFER];
    int n, rc;
    socklen_t addr_len;
    addr_len = sizeof(struct sockaddr);
    string id_client; // Store the position of the IP address in clientsAddr
    while (true)
    {
        bzero(recv_data, BUFFER);
        n = recvfrom(sock, recv_data, BUFFER, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0)
        {
            perror("ERROR reading from socket");
        }
        if (recv_data[0] == 'R')
        {/*
            for (int i = 0; i < repositories.size(); i++)
            {
                if (repositories[i].sin_port != client_addr.sin_port)
                    repositories.push_back(client_addr);
            }
            if (repositories.size() == 0)*/
                repositories.push_back(client_addr);
            cout << "repositorio registrado " << endl;
            //printf("[%i] [%s : %hd]\n", repositories.size(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        }
        if (recv_data[0] == 'c') // Client create a node
        {
            string structure(recv_data, BUFFER);
            string structure_copy(recv_data, BUFFER);

            // 'id_client' is the posicion of the client address in 'clientsAddr'
            id_client = storeClientAddress();

            for (int i = 0; i < clientsAddr.size(); i++)
            {
                cout << "Nustros Clientes:" << inet_ntoa(clientsAddr[i].sin_addr) << endl;
            }

            //insert repositorio
            //clientsAddr.push_back(client_addr);

            cout << "recibido->client: " << structure << endl;
            int size_nodo = string_int(structure.substr(1, 3));
            structure = structure.substr(4, BUFFER);
            int number_attributes = string_int(structure.substr(0, 2));
            structure = structure.substr(2, structure.size());
            int number_relations = string_int(structure.substr(0, 3));
            structure = structure.substr(3, structure.size());
            string name_node = structure.substr(0, size_nodo);
            structure = structure.substr(size_nodo, structure.size());
            structure.clear();
            //int res = name_node[0] % (repositories.size());
            int res = hashFunction(string_int(name_node), repositories.size());
            //cout << "ressss " << res << endl;
            //cout << "size_repo " << repositories.size() << endl;// 0 1 2

            // Add the id of the client into the message;
            structure_copy = id_client + structure_copy;

            n = sendto(sock, structure_copy.c_str(), BUFFER, 0, (struct sockaddr *)&repositories[res], sizeof(struct sockaddr));
            id_client.clear();
        }
        //client-->master
        if (recv_data[0] == 'r') // Client send a query
        {
            // 'id_client' is the posicion of the client address in 'clientsAddr'
            id_client = storeClientAddress();
             

            string structure(recv_data, BUFFER);
            //eliminar primer caracter 'r'
            //structure.erase(0,1);
            
            cout << "recibido->client: " << structure << endl;

            //int res = structure[0] % (repositories.size());}
            //para usar la funcion hash necesitamos el valor para saber
            //enque repositorioesta  
            string nodo_ = get_value(structure);
            int res = hashFunction(string_int(nodo_), repositories.size());
               cout << "repositorio: " << res << endl;
            cout << "Numero de repositorios: " << repositories.size() << endl;

            structure = id_client + structure;
            n = sendto(sock, structure.c_str(), BUFFER, 0, (struct sockaddr *)&repositories[res], sizeof(struct sockaddr));
            id_client.clear();
        }
        //master-->cllient
        if (recv_data[0] == 'd') // Repository send a reply
        {
            int clientIndex;
            string structure(recv_data, BUFFER);
            cout << "RECIBIDO (R->M): " << structure << endl;
           
            structure = no_id_client(structure, clientIndex);
            cout << "CLIENT->INDEX: "<<clientIndex<<endl;

            n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&clientsAddr[clientIndex], sizeof(struct sockaddr));
            //n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
            id_client.clear();
        }
    }
}

int main()
{
    int sock, bytes_read;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Bind");
        exit(1);
    }

    printf("[%s] [%s : %hd]\n", "Master: ", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
    //reading(sock);
    thread(reading, sock).detach();
    for (;;)
    {
    }
    return 0;
}