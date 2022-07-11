#include "Controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Number of connection requests waiting to be accepted
const int MAX_CONN_REQS = 2;
// Number of total connections to be accepted
const int MAX_CONN = 2;

void chat(int connection){
    char buff[1024];
    bool continueChat = true;
    int n;

    // Continue chat until Controller quits
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