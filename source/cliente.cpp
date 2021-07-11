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
void print_menu()
{
    cout << "Actions: \n";
    cout << "  - c: Create\n";
    cout << "  - r: Read\n";
}

string get_values(string data)
{
    string str;
    for (int i = 0; i < data.size(); i++)
    {
        int found = data.find("|");
        //cout<<found<<endl;
        std::string str2 = data.substr(0, found);
        cout << str2 << endl;
        //arr.push_back(str2);
        data.erase(0, found + 1);
        str += str2;
    }
    return str;
}

void reading(int sock)
{
    int n, write_sise;
    char recv_data[1000];
    socklen_t addr_len;
   
    while (1)
    {
        n = recvfrom(sock, recv_data, 1000, 0, (struct sockaddr *)&server_addr, &addr_len);
        if (recv_data[0] == 'd')
        {
            string structure(recv_data, 1000);
            structure.erase(0,1);
            //--------------------formato-------------------
            cout << "RECIBIDO (R->M->C): " <<structure<< endl;
            //get_values(structure);
            structure.clear();
        }
    }
}


void writing(int sock)
{
    int n, write_sise;
    char send_data[1000];
    socklen_t addr_len;
    while (true)
    {
        print_menu();
        char action;
        cout <<"action: ";
        cin >> action;
        cin.ignore();
        string structure;
        //cout<<"MSG: ";cin>>structure;
        switch (action)
        {
        case 'c':
        {
            cout << "Enter the name of the node you want to create\n";
            string name_node; //3 bits
            getline(cin, name_node);
            int number_attributes;
            cout << "Enter the number of atributes (<= 99)\n";
            cin >> number_attributes;
            cin.ignore();
            string attributes_structure = "";
            for (unsigned int i = 0; i < number_attributes; i++)
            {
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
            for (unsigned int i = 0; i < number_relations; i++)
            {
                cout << "Enter the name of the relation " << i + 1 << "\n";
                string name_relation;
                getline(cin, name_relation);
                relations_structure += int_to_string(name_relation.size(), 3) + name_relation;
            }
            structure =
                string(1, action) + int_to_string(name_node.size(), 3) + int_to_string(number_attributes, 2) +
                int_to_string(number_relations, 3) + name_node + attributes_structure + relations_structure+"$";
            cout << "send to server: " << structure << endl;
            string str;
            str.assign(1000-structure.size()-1,'0');
            structure+=str;

            n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
            //n=recvfrom(sock, send_data,1000, 0, (struct sockaddr *)&server_addr, &addr_len);
            break;
        }
        case 'r':
        {
            string dep;
            string str;
            cout << "Enter the name of node\n";
            string name; //3 bits
            cout << "name: ";
            cin >> name;
            cout<<"dep: ";
            cin >> dep;



            name = "r" + name+"*"+dep+"$";
            str.assign(1000-name.size()-1,'0');
            name +=str;
            //cout<<"this-> "<<name<<endl;
            n = sendto(sock, name.c_str(), name.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
            break;
        }
        case 'q':
        {
            break;
        }
        }
    }
}

int main(int argc, char *argv[])
{
    int sock;
    host = (struct hostent *)gethostbyname((char *)"127.0.0.1");
    //host = (struct hostent *)gethostbyname((char *)"34.152.23.49");
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero), 8);
    //writing(sock);
    thread (writing, sock).detach();
    thread (reading, sock).detach();
    while (1)
    {

    }
}
