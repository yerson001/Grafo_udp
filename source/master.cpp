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
#include <sqlite3.h>
#include <map>
using namespace std;

struct sockaddr_in server_addr, client_addr;
vector<struct sockaddr_in> my_connect;

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
        else
        {
            std::system("clear");
            cout << "************connections**************" << endl;
            my_connect.push_back(client_addr);
            cout<<"socket size:: "<<my_connect.size()<<endl;
            for(int i=0; i<my_connect.size(); i++){
                    printf("[%i] [%s : %hd]\n",i, inet_ntoa(my_connect[i].sin_addr), ntohs(my_connect[i].sin_port));
            }
            cout << "************************************" << endl;
        }

        char action = recv_data[0];
        printf("menssage action: %s\n", recv_data);

        string structure = "**********succefull connect**********";
        n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

        
        switch (action) {
            case 'r': {
                string res = "***client--->master->slave***";
                n = sendto(sock, res.c_str(), res.size(), 0, (struct sockaddr *)&my_connect[0], sizeof(struct sockaddr));
            }
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
    printf("\nUDPServer Waiting for client on port 5000 \n");
    reading(sock);
    return 0;
}
