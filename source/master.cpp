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

using namespace std;

struct sockaddr_in server_addr, client_addr;
vector<struct sockaddr_in> clientes;
vector<struct sockaddr_in> repositories;


int string_int(string s)
{
    stringstream geek(s);
    int x = 0;
    geek >> x;
    return x;
}

void reading(int sock)
{
    char recv_data[1000];
    int n, rc;
    socklen_t addr_len;
    addr_len = sizeof(struct sockaddr);
    while (true)
    {
        bzero(recv_data, 1000);
        n = recvfrom(sock, recv_data, 1000, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0)
        {
            perror("ERROR reading from socket");
        }
        if (recv_data[0] == 'R')
        {
            for (int i = 0; i < repositories.size(); i++)
            {
                if (repositories[i].sin_port != client_addr.sin_port)
                    repositories.push_back(client_addr);
            }
            if (repositories.size() == 0)
                repositories.push_back(client_addr);
            cout << "repositorio registrado " << endl;
            printf("[%i] [%s : %hd]\n", repositories.size(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        }
        int contador = 0;
        if (recv_data[0] == 'c')
        {
            string structure(recv_data, 1000);
            string structure_copy(recv_data, 1000);
            //inser cliente
            
            for (int i=0; i<clientes.size(); i++)
            {
                if (inet_ntoa(client_addr.sin_addr)== inet_ntoa(clientes[i].sin_addr))
                {
                    contador++;
                }
            }
            if(contador==0){
                clientes.push_back(client_addr);
            }

            for (int i=0; i<clientes.size(); i++)
            {
              cout<<"Nustros Clientes:"<<inet_ntoa(clientes[i].sin_addr)<<endl;
            }

            //insert repositorio
            //clientes.push_back(client_addr);

            cout << "recibido: " << structure << endl;
            int size_nodo = string_int(structure.substr(1, 3));
            structure = structure.substr(4, 1000);
            int number_attributes = string_int(structure.substr(0, 2));
            structure = structure.substr(2, structure.size());
            int number_relations = string_int(structure.substr(0, 3));
            structure = structure.substr(3, structure.size());
            string name_node = structure.substr(0, size_nodo);
            structure = structure.substr(size_nodo, structure.size());
            structure.clear();
            int res = name_node[0] % (repositories.size());
            //cout << "ressss " << res << endl;
            cout << "size_repo " << repositories.size() << endl;// 0 1 2 
            //structure_copy = "0"+structure_copy;

            n = sendto(sock, structure_copy.c_str(), 1000, 0, (struct sockaddr *)&repositories[res], sizeof(struct sockaddr));
        }
        //client-->master
        if (recv_data[0] == 'r')
        {
            string structure(recv_data, 1000);
            cout << "recibido: " << structure << endl;

            int res = structure[0] % (repositories.size());
            cout << "res " << res << endl;
            cout << "size " << repositories.size() << endl;
            n = sendto(sock, recv_data, 1000, 0, (struct sockaddr *)&repositories[res], sizeof(struct sockaddr));
        }
        //master-->cllient
        if (recv_data[0] == 'd')
        {
            string structure(recv_data, 1000);
            cout << "recibido->repositorio: " << structure << endl;
            n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&clientes[0], sizeof(struct sockaddr));
            //n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
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