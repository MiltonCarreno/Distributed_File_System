#include "Controller.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <thread>
using namespace std;

// Number of connection requests waiting to be accepted
const int MAX_CONN_REQS = 1;
// Number of total connections to be accepted
const int MAX_CONN = 3;

void chat(int connection){
    char buff[1024];
    bool continueChat = true;
    int n;

    // Continue chat until Controller quits
    while (continueChat) {
        bzero(buff, sizeof(buff));
        read(connection, buff, sizeof(buff));
        printf("Client: %s\nController: ", buff);
        bzero(buff, sizeof(buff));
        n = 0;
        
        // cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    vector<thread> threads;
    for (int i = 0; i<MAX_CONN; i++) {
        printf("In while loop!");
        server.listenConnection(MAX_CONN_REQS);
        int conn = server.acceptConnection();

        // Delegate chatting/connections to individual threads
        threads.emplace_back(thread(chat, conn));
    }

    // Wait for all the threads to finish
    for (int i = 0; i<threads.size(); i++) {
        threads[i].join();
    }
  
    // Close listening socket
    server.shutdownSocket();
    return 0;
}