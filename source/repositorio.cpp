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
using namespace std;
struct sockaddr_in server_addr;
struct hostent *host;


void writing(int sock) {
    int n,write_sise;char send_data[256];
    socklen_t addr_len;
    string structure;
    cout<<"MSG: ";
    cin>>structure;
    n = sendto(sock,structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    
    while (true) {
        n = recvfrom(sock, send_data,256, 0, (struct sockaddr *)&server_addr, &addr_len);
        printf("msg server: %s\n",send_data);
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
   writing(sock);
}
