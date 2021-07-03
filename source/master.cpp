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
vector<struct sockaddr_in> my_connect;
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
    char recv_data[256];
    int n, rc;
    socklen_t addr_len;
    addr_len = sizeof(struct sockaddr);
    while (true)
    {
        bzero(recv_data, 256);
        n = recvfrom(sock, recv_data, 256, 0, (struct sockaddr *)&client_addr, &addr_len);
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
        if (recv_data[0] == 'c')
        {
            string structure(recv_data, 256);

            my_connect.push_back(client_addr);

            cout << "recibido: " << structure << endl;
            int size_nodo = string_int(structure.substr(1, 3));
            structure = structure.substr(4, 256);
            int number_attributes = string_int(structure.substr(0, 2));
            structure = structure.substr(2, structure.size());
            int number_relations = string_int(structure.substr(0, 3));
            structure = structure.substr(3, structure.size());
            string name_node = structure.substr(0, size_nodo);
            structure = structure.substr(size_nodo, structure.size());

            int res = name_node[0] % (repositories.size());
            cout << "ressss " << res << endl;
            cout << "size " << repositories.size() << endl;
            n = sendto(sock, recv_data, 256, 0, (struct sockaddr *)&repositories[res], sizeof(struct sockaddr));
        }
        if (recv_data[0] == 'r')
        {
            string structure(recv_data, 256);
            cout << "recibido: " << structure << endl;

            int res = structure[0] % (repositories.size());
            cout << "res " << res << endl;
            cout << "size " << repositories.size() << endl;
            n = sendto(sock, recv_data, 256, 0, (struct sockaddr *)&repositories[res], sizeof(struct sockaddr));
        }
         if (recv_data[0] == 'd')
        {
            string structure(recv_data, 256);
            cout << "recibido->repositorio: " << structure << endl;
            n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&my_connect[0], sizeof(struct sockaddr));
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
    reading(sock);
    return 0;
}