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
string int_to_string(int number, int digits) {
    string ans = "";
    while (number) {
        ans.push_back('0' + (number % 10));
        number /= 10;
    }
    reverse(ans.begin(), ans.end());
    return string(digits - ans.size(), '0') + ans;
}
void print_menu() {
    cout << "Actions: \n";
    cout << "  - c: Create\n";
}
void writing(int sock) {
    int n,write_sise;char send_data[256];
    socklen_t addr_len;
    while (true) {
       print_menu();
        char action;cout<<endl;
        cin >> action;
        cin.ignore();
        string structure;
        //cout<<"MSG: ";cin>>structure;
        switch (action) {
            case 'c':{
                cout << "Enter the name of the node you want to create\n";
               string name_node;//3 bits
               getline(cin, name_node);
               int number_attributes;
               cout << "Enter the number of atributes (<= 99)\n";
               cin >> number_attributes;
               cin.ignore();
               string attributes_structure = "";
                for (unsigned int i = 0; i < number_attributes; i++) {
                    cout << "Enter the name of the atributes " << i + 1 << "\n";
                    string name_attribute;
                    getline(cin, name_attribute);
                    cout << "Enter the value of the attributes " << i + 1 << "\n";
                    string value_attribute;
                    getline(cin, value_attribute);
                    attributes_structure +=
                        int_to_string(name_attribute.size(), 3) + int_to_string(value_attribute.size(), 3) + name_attribute + value_attribute;
                }
                cout << "Enter the number of relations (<= 999)\n";
                int number_relations;
                cin >> number_relations;
                cin.ignore();
                string relations_structure = "";
                for (unsigned int i = 0; i < number_relations; i++) {
                    cout << "Enter the name of the relation " << i + 1 << "\n";
                    string name_relation;
                    getline(cin, name_relation);
                    relations_structure += int_to_string(name_relation.size(), 3) + name_relation;
                }
               structure =
                  string(1, action) + int_to_string(name_node.size(), 3) + int_to_string(number_attributes, 2) +
                    int_to_string(number_relations, 3) + name_node + attributes_structure + relations_structure;
               cout<<"send to server: "<<structure<<endl;
                n = sendto(sock,structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
                //n=recvfrom(sock, send_data,256, 0, (struct sockaddr *)&server_addr, &addr_len);    
                
            }
            case 'q':{
                break;
            }
           
        }
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