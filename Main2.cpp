// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "Client.h"
using namespace std;

void func(int sockfd)
{
    char buff[1024];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Controller : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main(int argc, char const* argv[]) {
    int valread;
    const char* hello = "Hello from client";
    char buffer[1024] = { 0 };

    string file_path = argv[1];
    cout << "File Path: " << file_path << endl;
    
    // Create new client
    Client client(file_path);

    // Creat new client socket
    int sock = client.createSocket();

    // Print file info
    client.printFileInfo();

    // // Request connection with client socket
    // client.requestConnection();
    // // Chat with controller
    // send(sock, hello, strlen(hello), 0);
    // printf("Hello message sent\n");
    // valread = read(sock, buffer, 1024);
    // printf("%s\n", buffer);
    // // Chat
    // func(sock);
    // // Close connection
    // client.closeConnection();
    return 0;
}