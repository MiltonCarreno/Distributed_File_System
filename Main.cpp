#include "Controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int MAX_CONN_REQS = 2;
const int MAX_CONN = 2;

void chat(int connection){
    char buff[1024];
    bool continueChat = true;
    int n;

    // infinite loop for chat
    while (continueChat) {
        bzero(buff, sizeof(buff));
        read(connection, buff, sizeof(buff));
        printf("From client: %s\nTo client: ", buff);
        bzero(buff, sizeof(buff));
        n = 0;

        while ((buff[n++] = getchar()) != '\n');
        write(connection, buff, sizeof(buff));
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            continueChat = false;
        }
    }

    // Close connection socket
    close(connection);
}

// void chat(int connection) {
//     char buffer[1024] = { 0 };
//     char* hello = "Hello from Controller";
//     int valread = read(connection, buffer, 1024);
//     printf("%s\n", buffer);
//     send(connection, hello, strlen(hello), 0);
//     printf("Hello message sent\n");
    
//     // Close connection socket
//     close(connection);
// }

int main(int argc, char* argv[]) {
    // Create Controller
    Controller server;

    // Create and bind new listening socket
    server.createSocket();
    server.bindSocket();

    // Accept connection request and create new connection socket
    bool newConnReq = server.listenConnection(MAX_CONN_REQS);
    int i = 0;
    while(newConnReq && i != MAX_CONN) {
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