#include "Controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int MAX_CONN_REQS = 3;

void chat(int connection) {
    char buffer[1024] = { 0 };
    char* hello = "Hello from Controller";
    int valread = read(connection, buffer, 1024);
    printf("%s\n", buffer);
    send(connection, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    
    // Close connection socket
    close(connection);
}

int main(int argc, char* argv[]) {
    // Create Controller
    Controller server;

    // Create and bind new listening socket
    server.createSocket();
    server.bindSocket();

    // Accept connection request and create new connection socket
    bool newConnReq = server.listenConnection(MAX_CONN_REQS);
    int i = 0;
    while(newConnReq && i != 3) {
        i++;
        printf("In while loop!");
        int conn = server.acceptConnection();
        chat(conn);
        newConnReq = server.listenConnection(MAX_CONN_REQS);
    }
  
    // Close listening socket
    server.shutdownSocket();
    return 0;
}