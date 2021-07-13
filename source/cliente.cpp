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
int BUFFER = 100;
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
    cout << "  - u: Update\n";
    cout << "  - b: Delete\n";
    cout << "  - s: Select\n";
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
    char recv_data[BUFFER];
    socklen_t addr_len;

    while (1)
    {
        n = recvfrom(sock, recv_data, BUFFER, 0, (struct sockaddr *)&server_addr, &addr_len);
        if (recv_data[0] == 'd')
        {
            string structure(recv_data, BUFFER);
            structure.erase(0, 1);
            //--------------------formato-------------------
            cout << "RECIBIDO (R->M->C): " << structure << endl;
            //get_values(structure);
            structure.clear();
        }
    }
}

void writing(int sock)
{
    int n, write_sise;
    char send_data[BUFFER];
    socklen_t addr_len;
    while (true)
    {
        print_menu();
        char action;
        cout << "action: ";
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
                int_to_string(number_relations, 3) + name_node + attributes_structure + relations_structure + "$";
            cout << "send to server: " << structure << endl;
            string str;
            str.assign(BUFFER - structure.size() - 1, '0');
            structure += str;

            n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
            //n=recvfrom(sock, send_data,BUFFER, 0, (struct sockaddr *)&server_addr, &addr_len);
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
            cout << "dep: ";
            cin >> dep;

            name = "r" + name + "*" + dep + "$";
            str.assign(BUFFER - name.size() - 1, '0');
            name += str;
            //cout<<"this-> "<<name<<endl;
            n = sendto(sock, name.c_str(), name.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
            break;
        }
        case 'u':
        {
            int type;
            string name_node1, name_node2, attribute, value;

            cout << "What do you want to uptade?\n";
            cout << "Press 0: Node,  1: Attributes" << endl;
            cin >> type;
            if (type == 0) {
                cout << "Enter the name of the node" << endl;
                cin >> name_node1;
                structure = structure + int_to_string(name_node1.size(), 3) + name_node1;
                cout << "Enter the name of the new node" << endl;
                cin >> name_node2;
                structure = structure + int_to_string(name_node2.size(), 3) + name_node2;
            }
            else {
                cout << "Opp! There is nothing here yet." << endl; break;
                /*cout << "Enter the name of the first node" << endl;
                cin >> name_node1;
                structure = int_to_string(name_node1.size(), 3) + name_node1 + structure;
                cout << "Enter the name of the attribute" << endl;
                cin >> attribute;
                structure = int_to_string(attribute.size(), 3) + attribute + structure;
                cout << "Enter the name of the new value" << endl;
                cin >> value;
                structure = int_to_string(value.size(), 3) + value + structure;*/
            }
            structure = string(1, action) + int_to_string(type,1) + structure + "$"; //
            cout << structure << endl;
            string str;
            str.assign(BUFFER - structure.size() - 1, '0');
            structure += str;
            n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
            break;
        }
        case 'b':
        {
            int type;
            string name_node1, name_node2, attribute;

            cout << "What do you want to delete?\n";
            cout << "Press 1: Node,  2: Attributes, 3: Relations" << endl;
            cin >> type;
            if (type == 1)
            {
                cout << "Enter the name of the node" << endl;
                cin >> name_node1;
                structure = int_to_string(name_node1.size(), 3) + name_node1 + structure;
            }
            else if (type == 2)
            {
                cout << "Enter the name of the node" << endl;
                cin >> name_node1;
                cout << "Enter the name of the attribute" << endl;
                cin >> attribute;
                structure = int_to_string(name_node1.size(), 3) + name_node1 +
                            int_to_string(attribute.size(), 3) + attribute + structure;
            }
            else
            {
                cout << "Enter the name of the first node" << endl;
                cin >> name_node1;
                cout << "Enter the name of the second node" << endl;
                cin >> name_node2;
                structure = int_to_string(name_node1.size(), 3) + name_node1 +
                            int_to_string(name_node2.size(), 3) + name_node2 + structure;
            }
            structure = string(1, action) + int_to_string(type, 1) + structure + "$"; //
            cout << structure << endl;
            string str;
            str.assign(BUFFER - structure.size() - 1, '0');
            structure += str;
            n = sendto(sock, structure.c_str(), structure.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
            break;
        }
        case 'q':
        {
            break;
        }
        case 's':
        {
            string dep;
            string str;
            cout << "Enter the name of node\n";
            string name; //3 bits
            cout << "tabla: ";
            cin >> name;
            cout << "repositorio: ";
            cin >> dep;

            name = "s" + name + "*" + dep + "$";
            str.assign(BUFFER - name.size() - 1, '0');
            name += str;
            //cout<<"this-> "<<name<<endl;
            n = sendto(sock, name.c_str(), name.size(), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
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
    thread(writing, sock).detach();
    thread(reading, sock).detach();
    while (1)
    {
    }
}
